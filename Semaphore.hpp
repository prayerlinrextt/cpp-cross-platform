#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>

class Semaphore {
public:
    explicit Semaphore(int initialCount = 0, int maximumCount = 1) : count_(initialCount), maximumCount_(maximumCount) {}

    void notify() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (count_ < maximumCount_) {
            ++count_;
            cv_.notify_one();
        }
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return count_ > 0; });
        --count_;
    }

    bool wait_with_timeout(int milliseconds) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!cv_.wait_for(lock, std::chrono::milliseconds(milliseconds), [this] { return count_ > 0; })) {
            return false; // Timeout
        }
        --count_;
        return true;
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
    int maximumCount_;
};
