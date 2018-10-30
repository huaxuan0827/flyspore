// flyspore.cpp: 定义控制台应用程序的入口点。
//

#include "core/task_pool.h"
#include "Threads.h"

#include "core/Spore.h"
#include "core/Context.h"
#include "core/Session.h"
#include "core/DataPack.h"
#include "core/Path.h"
using namespace fs;

int main()
{
	class MyDataStruct
	{
		unsigned long long value;
		unsigned int level;
	};

	auto _startTime = std::chrono::high_resolution_clock::now();

	int ntestcount = 100000;
	for (int n = 0; n < ntestcount; n ++)
	{
		AnyValues av;
		auto &ii = av.ref<int>("VP");
		ii = 45;
		bool ok = av.is_a<int>("VP");
		//auto &ss = av.ref<std::string>("VP");
		//ss = "abcd";
		//auto tt = av.ref<std::string>("VP");

		//auto p1 = av.ref<std::string>("p1");
		//auto p2 = av.ref<std::string>("p2");
		//auto p3 = av.ref<std::string>("p3");
		//auto p4 = av.ref<std::string>("p4");
		//auto p5 = av.ref<std::string>("p5");
		//auto p6 = av.ref<std::string>("p6");
		//auto p7 = av.ref<std::string>("p7");
		//auto p8 = av.ref<std::string>("p8");

	}
	auto _currentTime = std::chrono::high_resolution_clock::now();
	auto elapsed_seconds = ((std::chrono::duration<double>)(_currentTime - _startTime)).count();
	std::cout << "  elapsed time: " << elapsed_seconds << "s | avg: " << ntestcount / elapsed_seconds << "\n";

	auto mainSpore = Spore::newSpore("Main Space");
	auto entryPin = mainSpore->addPin("entry", Pin_Type::IN_PIN);

	auto L1 = mainSpore->addChild("sporeL1");

	auto fun_LXX = [&](Context& cc, AnyValues &data) {

	};

	auto fun_L1 = [&](Context& cc, P_Data data) {
		static std::mutex _lock;

		auto &nLevel = data->ref<int>("Level");
		auto &maxLevel = data->ref<int>("MaxLevel");
		auto &expCount = data->ref<int>("expCount");

		auto &currentLevel = cc.session.ref<int>("currentLevel");
		auto &nCount = cc.session.ref<int>("Count");
		{
			std::lock_guard<std::mutex> lock(_lock);
			nCount = nCount++;
		}

		if (currentLevel < nLevel)
		{
			currentLevel = nLevel;
			std::lock_guard<std::mutex> lock(_lock);
			std::cout << "! new Level: " << nLevel;

			
			auto _currentTime = std::chrono::high_resolution_clock::now();
			auto elapsed_seconds = ((std::chrono::duration<double>)(_currentTime - _startTime)).count();
			std::cout <<  "  elapsed time: " << elapsed_seconds << "s\n";
		}
		if (nLevel >= maxLevel)
			return;

		for (int n = 0; n < expCount; n++)
		{
			auto outData = cc.newData();
			outData->ref<int>("Level") = nLevel + 1;
			outData->ref<int>("MaxLevel") = maxLevel;
			outData->ref<int>("expCount") = expCount;
			cc.push("out", outData);
		}
	};
	auto out_L1 = L1->addPin("out");
	auto in_L1 = L1->addPin("in", fun_L1);

	Path::connect(entryPin, in_L1);
	Path::connect(out_L1, in_L1);


	auto out_L2 = L1->addPin<Schema>("in", 
		[&](Context& cc, MyDataStruct &data) {

	});

	auto session = Session::newSession(entryPin);
	AnyValues originData;
	originData.ref<int>("Level") = 0;
	originData.ref<int>("MaxLevel") = 6;
	originData.ref<int>("expCount") = 10;

	session->submit(originData);
	session->join();
	
	auto& finishCount = session->values().ref<int>("Count");

	std::cout << "Session finish, the result:" << finishCount << "\n";

	_currentTime = std::chrono::high_resolution_clock::now();
	elapsed_seconds = ((std::chrono::duration<double>)(_currentTime - _startTime)).count();
	std::cout << "  elapsed time: " << elapsed_seconds << "s | avg: " << finishCount / elapsed_seconds << "\n";

	task_pool &_tp = task_pool::get_instance();
	//std::string strresult = "abcd";
	//auto future_1 = _tp.submit(g_do_some_work6, 4, 5, std::ref(strresult));
	//std::string ret_1 = future_1.get();
	//ret_1 = _tp.submit([](int n, int m) {return to_string(n) + to_string(m);  }, 4, 5).get();
	//std::cout << ret_1;
	std::cout << "All task finish,ths  count is :" << _tp.get_finish_count() << "\n";
	char _c;
	std::cin >> _c;
	return 0;

}

