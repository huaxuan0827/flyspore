// flyspore.cpp: 定义控制台应用程序的入口点。
//

#include "core/task_pool.h"
#include "Threads.h"

#include "core/Spore.h"
#include "core/Context.h"
#include "core/Session.h"
using namespace fs;

int main()
{
	auto pSpace = Spore::newSpore("测试中的Space");
	auto entryPin = pSpace->addInPIn("entry");

	auto spore1 = pSpace->addChild("spore1");
	auto sp1InPin = spore1->addInPIn("in", [](Context& cc, C_Data data) {
		auto outdata = cc.newData();
		int n = data->ref<int>("value");
		if (n < 1000)
		{
			outdata->ref<int>("value") = n + 1;
			cc.push("out", outdata);
		}
	});
	spore1->addOutPIn("out");

	entryPin->connect(sp1InPin);

	auto session = pSpace->newSession("entry");
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

