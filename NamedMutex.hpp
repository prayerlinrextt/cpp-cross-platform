#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <string>

class NamedMutex {
private:
    std::string mutexName;
    boost::interprocess::named_mutex mutex;

public:
    NamedMutex(const std::string& name)
        : mutexName(name), mutex(boost::interprocess::open_or_create, mutexName.c_str()) {}

    ~NamedMutex() {
        boost::interprocess::named_mutex::remove(mutexName.c_str());
    }

    void lock() {
        boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(mutex);
    }

    bool try_lock() {
        return mutex.try_lock();
    }

    void unlock() {
        mutex.unlock();
    }
};
