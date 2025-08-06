#include "NSAsync.h"
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
//----------------------------线程------------------

//----------------------------线程通信------------------

//#include <iostream>
//#include <thread>
//#include <mutex>
//
////互斥锁 (Mutex) 最基本的同步原语，用于保护共享资源。一个语句块不能有两个锁 假设第二个锁未获取到阻塞了，第一个锁就一直被锁定，容易互相等待死锁
//std::mutex mtx;
//int shared_data = 0;
//
//void increment() {//单资源 考虑是否死锁
//    for (int i = 0; i < 100000; ++i) {
//        mtx.lock();
//        ++shared_data;
//        mtx.unlock();
//
//        /* std::lock_guard (RAII 风格) 构造时加锁，析构时解锁 防止异常抛出不解锁
//        std::lock_guard<std::mutex> lock(mtx);
//        ++shared_data;*/
//    }
//}

//std::mutex mtx1;
//std::mutex mtx2;
//void transfer(int amount, int& from, int& to) {//多资源
//    std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock);
//    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);
//    std::lock(lock1, lock2); // 避免死锁，原子操作 要么同时加锁，要么同时解锁
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

//条件变量 (Condition Variables) 条件变量只与单个互斥量配合使用不然 std::lock了两个 只释放了一个  容易死锁
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
//        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //挂起100ms
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

//std::lock 假设操作需要获取多个锁，std::lock原子操作同时无竞争死锁的锁住了两个锁  两个或以上的锁不要用条件变量
//void transfer_data() {
//    std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);
//    std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);
//
//    // 同时锁定两个unique_lock管理的互斥量
//    std::lock(lock1, lock2);
//
//    // 临界区代码
//    // ...操作两个受保护的资源...
//} // 自动解锁
// 
//示例 - 线程安全的账户转账
//class BankAccount {
//    std::mutex mtx;
//    double balance;
//public:
//    void transfer(BankAccount& to, double amount) {
//        // 同时锁定两个账户的互斥量
//        std::lock(mtx, to.mtx);//只负责获取锁，不管理生命周期  所以需要unique_lock或者lock_guard配合使用
//
//        // 使用unique_lock管理（也可以使用lock_guard）//但必须时RAII 如果这里抛出异常，锁不会被释放
//        std::unique_lock<std::mutex> lock1(mtx, std::adopt_lock);
//        std::unique_lock<std::mutex> lock2(to.mtx, std::adopt_lock);
//
//        balance -= amount;
//        to.balance += amount;
//
//        // 记录日志等操作...
//    }
//};

//void consumer() {
//    while (true) {
//        std::unique_lock<std::mutex> lock(mtx); //持有锁 往下执行，需要unique_lock而不能是lock_guard，因为lock_guard同样是 RAII的，但没有手动lock，unlock，构造时获取锁，析构时解锁，这个锁后续需要在cv.wait判断操作
//        cv.wait(lock, [] { return !data_queue.empty() || finished; });//对锁的操作是原子的，不会产生竞争，条件true 持有锁继续执行， false 释放锁 阻塞等待
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
//            return !data_queue.empty() || production_complete;//条件为真，获取锁，继续执行，假，释放锁，阻塞进程
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

//屏障(Barrier - C++20) std::barrier sync_point(3); // 等待3个线程
//信号量 (C++20)  std::counting_semaphore<5> sem(0); // 最大5，初始0  一般和互斥锁共用
// C++11/C++14/C++17 不支持标准信号量，但可以通过不同方式实现
//应用示例  读写器问题（带有写入优先）多读1写 （读操作期间：不允许写操作  写请求到达后：阻止新读操作进入  等待现有读操作完成：才开始写操作 写操作完成：才允许新读操作进入）

//用条件变量实现
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
//            // 添加阻塞状态提示
//            if (write_pending || is_writing) {
//                safe_print("Reader " + std::to_string(id) + " BLOCKED (write pending)");
//            }
//
//            read_cv.wait(lock, [this] {
//                return !write_pending && !is_writing;  //条件为真，获得锁 执行，为假 释放锁 阻塞
//                });
//            ++active_readers;
//
//            // 添加恢复执行提示
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
//            // 添加写入者等待提示
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
//    // 启动3个读取者
//    for (int i = 0; i < 3; ++i) {
//        threads.emplace_back(&WritePriorityRWLock::read, &rwlock, i);
//        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 确保启动顺序
//    }
//
//    // 启动2个写入者
//    threads.emplace_back(&WritePriorityRWLock::write, &rwlock, 1000);
//    for (int i = 3; i < 5; ++i) {
//        threads.emplace_back(&WritePriorityRWLock::read, &rwlock, i);
//    }
//    threads.emplace_back(&WritePriorityRWLock::write, &rwlock, 1001);
//
//    // 再启动2个读取者
//    for (int i = 6; i < 10; ++i) {
//        threads.emplace_back(&WritePriorityRWLock::read, &rwlock, i);
//    }
//
//    for (auto& t : threads) t.join();
//}