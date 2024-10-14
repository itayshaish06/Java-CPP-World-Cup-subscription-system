#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

using namespace std;

class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(size_t thread_count);

    ~ThreadPool();


    void enqueue(function<void()> func);

private:
    std::vector<std::thread> threads;
    std::queue<Task> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};
