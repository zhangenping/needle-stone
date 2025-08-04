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
//#include <iostream>
//#include <future>
//#include <thread>
//
//void compute(std::promise<int>&& prom) {
//    // 模拟耗时计算
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    prom.set_value(42); // 设置结果
//}
//
//int main() {
//    std::promise<int> prom;
//    std::future<int> fut = prom.get_future();
//
//    std::thread t(compute, std::move(prom));
//
//    // 可以做其他事情...
//    std::cout << "Doing other work...\n";
//
//    // 获取结果
//    int value = fut.get();
//    std::cout << "Result: " << value << "\n";
//
//    t.join();
//    return 0;
//}


//----------------------------线程通信------------------

//#include <iostream>
//#include <thread>
//#include <mutex>
//
////互斥锁 (Mutex) 最基本的同步原语，用于保护共享资源。
//std::mutex mtx;
//int shared_data = 0;
//
//void increment() {//单资源 考虑是否死锁
//    for (int i = 0; i < 100000; ++i) {
//        mtx.lock();
//        ++shared_data;
//        mtx.unlock();
//
//        /* std::lock_guard (RAII 风格)
//        std::lock_guard<std::mutex> lock(mtx);
//        ++shared_data;*/
//    }
//}

//std::mutex mtx1;
//std::mutex mtx2;
//void transfer(int amount, int& from, int& to) {//多资源
//    std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock);
//    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);
//    std::lock(lock1, lock2); // 避免死锁
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

//条件变量 (Condition Variables)
//用于线程间通知和等待特定条件。 生产者-消费者 消费者等待生产者

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

//原子操作(Atomic)
//无需锁的线程安全操作。
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

//条件变量 (Condition Variables)
//用于线程间通知和等待特定条件。 生产者-消费者 消费者生产者互相等待

//#include <iostream>
//#include <queue>
//#include <thread>
//#include <mutex>
//#include <condition_variable>
//
//const int BUFFER_SIZE = 10; // 缓冲区容量
//std::queue<int> data_queue;
//std::mutex mtx;
//std::condition_variable cv_producer; // 生产者条件变量
//std::condition_variable cv_consumer; // 消费者条件变量
//bool production_complete = false;    // 生产完成标志
//
//void producer(int total_items) {
//    for (int i = 1; i <= total_items; ++i) {
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//
//        std::unique_lock<std::mutex> lock(mtx);
//
//        // 如果缓冲区满，等待消费者消费
//        cv_producer.wait(lock, [] {
//            return data_queue.size() < BUFFER_SIZE;
//            });
//
//        data_queue.push(i);
//        std::cout << "Produced: " << i << " (Buffer size: "
//            << data_queue.size() << ")\n";
//
//        // 通知消费者有新数据
//        cv_consumer.notify_one();
//    }
//
//    // 生产完成
//    {
//        std::lock_guard<std::mutex> lock(mtx);
//        production_complete = true;
//    }
//    cv_consumer.notify_all(); // 通知所有消费者
//}
//
//void consumer(int id) {
//    while (true) {
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//
//        std::unique_lock<std::mutex> lock(mtx);
//
//        // 等待有数据或生产完成
//        cv_consumer.wait(lock, [] {
//            return !data_queue.empty() || production_complete;
//            });
//
//        // 如果队列空且生产完成，则退出
//        if (data_queue.empty() && production_complete) {
//            break;
//        }
//
//        // 消费数据
//        int data = data_queue.front();
//        data_queue.pop();
//        std::cout << "Consumer " << id << " consumed: " << data
//            << " (Buffer size: " << data_queue.size() << ")\n";
//
//        // 通知生产者有空位
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
//    // 启动生产者
//    for (int i = 0; i < NUM_PRODUCERS; ++i) {
//        producers[i] = std::thread(producer, TOTAL_ITEMS);
//    }
//
//    // 启动消费者
//    for (int i = 0; i < NUM_CONSUMERS; ++i) {
//        consumers[i] = std::thread(consumer, i + 1);
//    }
//
//    // 等待所有线程完成
//    for (auto& t : producers) t.join();
//    for (auto& t : consumers) t.join();
//
//    std::cout << "All done!\n";
//    return 0;
//}

//信号量 (C++20)  std::counting_semaphore<5> sem(0); // 最大5，初始0
//屏障(Barrier - C++20) std::barrier sync_point(3); // 等待3个线程