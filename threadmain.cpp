#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>
using namespace std;
int func(int a) {
	std::this_thread::sleep_for(std::chrono::seconds(5));
	cout << "hello world" << endl;
	return 3 + a;
}

void func2(int a,std::promise<int> &p) {
	std::this_thread::sleep_for(std::chrono::seconds(5));
	cout << "hello world" << endl;
	p.set_value(a + 3);
//	return 3 + a;
}

int main() {

	std::promise<int> promise;
	std::future<int> future = promise.get_future();
	std::thread work_thread(func2,3,std::move(promise));
	cout << "hello world" << endl;
	std::cout << future.get();
	work_thread.join();
	system("pause");
	return 0;
}