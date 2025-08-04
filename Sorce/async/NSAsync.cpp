//#include <iostream>
//#include <future>
//#include <thread>
//
//int compute() {
//    // 模拟耗时计算
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    return 42;
//}
//
//int main() {
//    // 异步启动任务
//    std::future<int> result = std::async(std::launch::async, compute);
//    
//    // 可以做其他事情...
//    std::cout << "Doing other work...\n";
//    
//    // 获取结果（会阻塞直到结果就绪）
//    int value = result.get();
//    std::cout << "Result: " << value << "\n";
//    
//    return 0;
//}

//用于更精细控制的异步结果传递：
#include <iostream>
#include <future>
#include <thread>

void compute(std::promise<int>&& prom) {
    // 模拟耗时计算
    std::this_thread::sleep_for(std::chrono::seconds(2));
    prom.set_value(42); // 设置结果
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(compute, std::move(prom));

    // 可以做其他事情...
    std::cout << "Doing other work...\n";

    // 获取结果
    int value = fut.get();
    std::cout << "Result: " << value << "\n";

    t.join();
    return 0;
}