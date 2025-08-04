//#include <iostream>
//#include <future>
//#include <thread>
//
//int compute() {
//    // ģ���ʱ����
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    return 42;
//}
//
//int main() {
//    // �첽��������
//    std::future<int> result = std::async(std::launch::async, compute);
//    
//    // ��������������...
//    std::cout << "Doing other work...\n";
//    
//    // ��ȡ�����������ֱ�����������
//    int value = result.get();
//    std::cout << "Result: " << value << "\n";
//    
//    return 0;
//}

//���ڸ���ϸ���Ƶ��첽������ݣ�
#include <iostream>
#include <future>
#include <thread>

void compute(std::promise<int>&& prom) {
    // ģ���ʱ����
    std::this_thread::sleep_for(std::chrono::seconds(2));
    prom.set_value(42); // ���ý��
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(compute, std::move(prom));

    // ��������������...
    std::cout << "Doing other work...\n";

    // ��ȡ���
    int value = fut.get();
    std::cout << "Result: " << value << "\n";

    t.join();
    return 0;
}