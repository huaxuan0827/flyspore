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
	fs::AnyValues av;
	int &ii = av.ref<int>("aabcd");
	ii = 12;
	int &ic = av.ref<int>("aabcd");

	auto mainSpore = Spore::newSpore("测试中的Space");
	auto entryPin = mainSpore->addPin("entry", Pin_Type::IN_PIN);

	auto spore1 = mainSpore->addChild("spore1");

	auto fun = [](Context& cc, P_Data data) {
		auto &inValue = data->ref<int>("value");

		auto outdata = cc.newData();
		auto &outVlue = outdata->ref<int>("value");

		if (inValue < 1000)
		{
			outVlue = inValue + 1;
			cc.push("out", outdata);
		}
	};
	spore1->addPin("out");
	auto sp1InPin = spore1->addPin("in", fun);

	Path::connect(entryPin, sp1InPin);

	auto session = Session::newSession(entryPin);
	session->submit();
	session->join();

	task_pool &_tp = task_pool::get_instance();

	std::string strresult = "abcd";

	auto future_1 = _tp.submit(g_do_some_work6, 4, 5, std::ref(strresult));
	std::string ret_1 = future_1.get();

	ret_1 = _tp.submit([](int n, int m) {return to_string(n) + to_string(m);  }, 4, 5).get();

	std::cout << ret_1;
	std::cout << "\nFinish Count:" << _tp.get_finish_count() << std::endl;
	char _c;
	std::cin >> _c;
	return 0;

}

