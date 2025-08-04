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


//----------------------------�߳�ͨ��------------------

//#include <iostream>
//#include <thread>
//#include <mutex>
//
////������ (Mutex) �������ͬ��ԭ����ڱ���������Դ��
//std::mutex mtx;
//int shared_data = 0;
//
//void increment() {//����Դ �����Ƿ�����
//    for (int i = 0; i < 100000; ++i) {
//        mtx.lock();
//        ++shared_data;
//        mtx.unlock();
//
//        /* std::lock_guard (RAII ���)
//        std::lock_guard<std::mutex> lock(mtx);
//        ++shared_data;*/
//    }
//}

//std::mutex mtx1;
//std::mutex mtx2;
//void transfer(int amount, int& from, int& to) {//����Դ
//    std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock);
//    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);
//    std::lock(lock1, lock2); // ��������
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

//�������� (Condition Variables)
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
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
//void consumer() {
//    while (true) {
//        std::unique_lock<std::mutex> lock(mtx);
//        cv.wait(lock, [] { return !data_queue.empty() || finished; });
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
//            return !data_queue.empty() || production_complete;
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

//�ź��� (C++20)  std::counting_semaphore<5> sem(0); // ���5����ʼ0
//����(Barrier - C++20) std::barrier sync_point(3); // �ȴ�3���߳�