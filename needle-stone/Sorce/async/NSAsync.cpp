#include "NSAsync.h"
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
//#include <iostream>
//#include <future>
//#include <thread>
//
//void compute(std::promise<int>&& prom) {
//    // ģ���ʱ����
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    prom.set_value(42); // ���ý��
//}
//
//int main() {
//    std::promise<int> prom;
//    std::future<int> fut = prom.get_future();
//
//    std::thread t(compute, std::move(prom));
//
//    // ��������������...
//    std::cout << "Doing other work...\n";
//
//    // ��ȡ���
//    int value = fut.get();
//    std::cout << "Result: " << value << "\n";
//
//    t.join();
//    return 0;
//}
//----------------------------�߳�------------------

//----------------------------�߳�ͨ��------------------

//#include <iostream>
//#include <thread>
//#include <mutex>
//
////������ (Mutex) �������ͬ��ԭ����ڱ���������Դ��һ�����鲻���������� ����ڶ�����δ��ȡ�������ˣ���һ������һֱ�����������׻���ȴ�����
//std::mutex mtx;
//int shared_data = 0;
//
//void increment() {//����Դ �����Ƿ�����
//    for (int i = 0; i < 100000; ++i) {
//        mtx.lock();
//        ++shared_data;
//        mtx.unlock();
//
//        /* std::lock_guard (RAII ���) ����ʱ����������ʱ���� ��ֹ�쳣�׳�������
//        std::lock_guard<std::mutex> lock(mtx);
//        ++shared_data;*/
//    }
//}

//std::mutex mtx1;
//std::mutex mtx2;
//void transfer(int amount, int& from, int& to) {//����Դ
//    std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock);
//    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);
//    std::lock(lock1, lock2); // ����������ԭ�Ӳ��� Ҫôͬʱ������Ҫôͬʱ����
//
//    from -= amount;
//    to += amount;
//}

//int main() {
//    std::thread t1(increment);
//    std::thread t2(increment);
//
//    t1.join();
//    t2.join();
//
//    std::cout << "Final value: " << shared_data << "\n";
//    return 0;
//}

//�������� (Condition Variables) ��������ֻ�뵥�����������ʹ�ò�Ȼ std::lock������ ֻ�ͷ���һ��  ��������
//�����̼߳�֪ͨ�͵ȴ��ض������� ������-������ �����ߵȴ�������

//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <condition_variable>
//#include <queue>
//
//std::mutex mtx;
//std::condition_variable cv;
//std::queue<int> data_queue;
//bool finished = false;
//
//void producer() {
//    for (int i = 0; i < 10; ++i) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //����100ms
//        std::lock_guard<std::mutex> lock(mtx);
//        data_queue.push(i);
//        cv.notify_one();
//    }
//
//    {
//        std::lock_guard<std::mutex> lock(mtx);
//        finished = true;
//        cv.notify_all();
//    }
//}
// 

//std::lock ���������Ҫ��ȡ�������std::lockԭ�Ӳ���ͬʱ�޾�����������ס��������  ���������ϵ�����Ҫ����������
//void transfer_data() {
//    std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);
//    std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);
//
//    // ͬʱ��������unique_lock����Ļ�����
//    std::lock(lock1, lock2);
//
//    // �ٽ�������
//    // ...���������ܱ�������Դ...
//} // �Զ�����
// 
//ʾ�� - �̰߳�ȫ���˻�ת��
//class BankAccount {
//    std::mutex mtx;
//    double balance;
//public:
//    void transfer(BankAccount& to, double amount) {
//        // ͬʱ���������˻��Ļ�����
//        std::lock(mtx, to.mtx);//ֻ�����ȡ������������������  ������Ҫunique_lock����lock_guard���ʹ��
//
//        // ʹ��unique_lock����Ҳ����ʹ��lock_guard��//������ʱRAII ��������׳��쳣�������ᱻ�ͷ�
//        std::unique_lock<std::mutex> lock1(mtx, std::adopt_lock);
//        std::unique_lock<std::mutex> lock2(to.mtx, std::adopt_lock);
//
//        balance -= amount;
//        to.balance += amount;
//
//        // ��¼��־�Ȳ���...
//    }
//};

//void consumer() {
//    while (true) {
//        std::unique_lock<std::mutex> lock(mtx); //������ ����ִ�У���Ҫunique_lock��������lock_guard����Ϊlock_guardͬ���� RAII�ģ���û���ֶ�lock��unlock������ʱ��ȡ��������ʱ�����������������Ҫ��cv.wait�жϲ���
//        cv.wait(lock, [] { return !data_queue.empty() || finished; });//�����Ĳ�����ԭ�ӵģ������������������true ����������ִ�У� false �ͷ��� �����ȴ�
//
//        if (finished && data_queue.empty()) break;
//
//        while (!data_queue.empty()) {
//            int data = data_queue.front();
//            data_queue.pop();
//            std::cout << "Got data: " << data << "\n";
//        }
//    }
//}
//
//int main() {
//    std::thread p(producer);
//    std::thread c(consumer);
//
//    p.join();
//    c.join();
//
//    return 0;
//}

//ԭ�Ӳ���(Atomic)
//���������̰߳�ȫ������
//#include <iostream>
//#include <thread>
//#include <atomic>
//
//std::atomic<int> counter(0);
//
//void increment_atomic() {
//    for (int i = 0; i < 100000; ++i) {
//        ++counter;
//    }
//}
//
//int main() {
//    std::thread t1(increment_atomic);
//    std::thread t2(increment_atomic);
//
//    t1.join();
//    t2.join();
//
//    std::cout << "Counter: " << counter << "\n";
//    return 0;
//}

//�������� (Condition Variables)
//�����̼߳�֪ͨ�͵ȴ��ض������� ������-������ �����������߻���ȴ�

//#include <iostream>
//#include <queue>
//#include <thread>
//#include <mutex>
//#include <condition_variable>
//
//const int BUFFER_SIZE = 10; // ����������
//std::queue<int> data_queue;
//std::mutex mtx;
//std::condition_variable cv_producer; // ��������������
//std::condition_variable cv_consumer; // ��������������
//bool production_complete = false;    // ������ɱ�־
//
//void producer(int total_items) {
//    for (int i = 1; i <= total_items; ++i) {
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//
//        std::unique_lock<std::mutex> lock(mtx);
//
//        // ��������������ȴ�����������
//        cv_producer.wait(lock, [] {
//            return data_queue.size() < BUFFER_SIZE;
//            });
//
//        data_queue.push(i);
//        std::cout << "Produced: " << i << " (Buffer size: "
//            << data_queue.size() << ")\n";
//
//        // ֪ͨ��������������
//        cv_consumer.notify_one();
//    }
//
//    // �������
//    {
//        std::lock_guard<std::mutex> lock(mtx);
//        production_complete = true;
//    }
//    cv_consumer.notify_all(); // ֪ͨ����������
//}
//
//void consumer(int id) {
//    while (true) {
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//
//        std::unique_lock<std::mutex> lock(mtx);
//
//        // �ȴ������ݻ��������
//        cv_consumer.wait(lock, [] {
//            return !data_queue.empty() || production_complete;//����Ϊ�棬��ȡ��������ִ�У��٣��ͷ�������������
//            });
//
//        // ������п���������ɣ����˳�
//        if (data_queue.empty() && production_complete) {
//            break;
//        }
//
//        // ��������
//        int data = data_queue.front();
//        data_queue.pop();
//        std::cout << "Consumer " << id << " consumed: " << data
//            << " (Buffer size: " << data_queue.size() << ")\n";
//
//        // ֪ͨ�������п�λ
//        cv_producer.notify_one();
//    }
//}
//
//int main() {
//    const int NUM_PRODUCERS = 1;
//    const int NUM_CONSUMERS = 2;
//    const int TOTAL_ITEMS = 20;
//
//    std::thread producers[NUM_PRODUCERS];
//    std::thread consumers[NUM_CONSUMERS];
//
//    // ����������
//    for (int i = 0; i < NUM_PRODUCERS; ++i) {
//        producers[i] = std::thread(producer, TOTAL_ITEMS);
//    }
//
//    // ����������
//    for (int i = 0; i < NUM_CONSUMERS; ++i) {
//        consumers[i] = std::thread(consumer, i + 1);
//    }
//
//    // �ȴ������߳����
//    for (auto& t : producers) t.join();
//    for (auto& t : consumers) t.join();
//
//    std::cout << "All done!\n";
//    return 0;
//}

//����(Barrier - C++20) std::barrier sync_point(3); // �ȴ�3���߳�
//�ź��� (C++20)  std::counting_semaphore<5> sem(0); // ���5����ʼ0  һ��ͻ���������
// C++11/C++14/C++17 ��֧�ֱ�׼�ź�����������ͨ����ͬ��ʽʵ��
//Ӧ��ʾ��  ��д�����⣨����д�����ȣ����1д ���������ڼ䣺������д����  д���󵽴����ֹ�¶���������  �ȴ����ж�������ɣ��ſ�ʼд���� д������ɣ��������¶��������룩

//����������ʵ��
//#include <string>
//#include <iostream>
//#include <mutex>
//#include <thread>
//#include <vector>
//#include <condition_variable>
//
//class WritePriorityRWLock {
//    std::mutex mtx;
//    std::condition_variable read_cv, write_cv;
//    int active_readers = 0;
//    bool write_pending = false;
//    bool is_writing = false;
//    std::string shared_data = "Initial";
//    std::mutex cout_mtx;
//
//    void safe_print(const std::string& msg) {
//        std::lock_guard<std::mutex> lock(cout_mtx);
//        std::cout << msg << std::endl;
//    }
//
//public:
//    void read(int id) {
//        {
//            std::unique_lock<std::mutex> lock(mtx);
//
//            // �������״̬��ʾ
//            if (write_pending || is_writing) {
//                safe_print("Reader " + std::to_string(id) + " BLOCKED (write pending)");
//            }
//
//            read_cv.wait(lock, [this] {
//                return !write_pending && !is_writing;  //����Ϊ�棬����� ִ�У�Ϊ�� �ͷ��� ����
//                });
//            ++active_readers;
//
//            // ��ӻָ�ִ����ʾ
//            safe_print("Reader " + std::to_string(id) + " RESUMED");
//        }
//
//        std::string local_copy = shared_data;
//        safe_print("Reader " + std::to_string(id) + " read: " + local_copy);
//
//        {
//            std::lock_guard<std::mutex> lock(mtx);
//            if (--active_readers == 0) {
//                write_cv.notify_one();
//            }
//        }
//    }
//
//    void write(int id) {
//        {
//            std::unique_lock<std::mutex> lock(mtx);
//            write_pending = true;
//
//            // ���д���ߵȴ���ʾ
//            if (active_readers > 0 || is_writing) {
//                safe_print("Writer " + std::to_string(id) + " WAITING for readers");
//            }
//
//            write_cv.wait(lock, [this] {
//                return active_readers == 0 && !is_writing;
//                });
//            write_pending = false;
//            is_writing = true;
//
//            safe_print("Writer " + std::to_string(id) + " ACQUIRED lock");
//        }
//
//        shared_data = "Writer_" + std::to_string(id);
//        safe_print("Writer " + std::to_string(id) + " wrote: " + shared_data);
//
//        {
//            std::lock_guard<std::mutex> lock(mtx);
//            is_writing = false;
//        }
//        read_cv.notify_all();
//        write_cv.notify_one();
//
//        safe_print("Writer " + std::to_string(id) + " RELEASED lock");
//    }
//};
//
//int main() {
//    WritePriorityRWLock rwlock;
//    std::vector<std::thread> threads;
//
//    // ����3����ȡ��
//    for (int i = 0; i < 3; ++i) {
//        threads.emplace_back(&WritePriorityRWLock::read, &rwlock, i);
//        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // ȷ������˳��
//    }
//
//    // ����2��д����
//    threads.emplace_back(&WritePriorityRWLock::write, &rwlock, 1000);
//    for (int i = 3; i < 5; ++i) {
//        threads.emplace_back(&WritePriorityRWLock::read, &rwlock, i);
//    }
//    threads.emplace_back(&WritePriorityRWLock::write, &rwlock, 1001);
//
//    // ������2����ȡ��
//    for (int i = 6; i < 10; ++i) {
//        threads.emplace_back(&WritePriorityRWLock::read, &rwlock, i);
//    }
//
//    for (auto& t : threads) t.join();
//}