#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

// ----- Global shared variable -----
int sharedValue = 0;

// ----- Mutex to protect access to sharedValue -----
std::mutex valueMutex;

// ----- Functor that reads the global variable -----
class Reader {
public:
    void operator()() const {
        for (int i = 0; i < 10; ++i) {
            int localCopy;
            {
                std::lock_guard<std::mutex> lock(valueMutex);   //std::unique_lock<std::mutex> lock(valueMutex);
                localCopy = sharedValue;
                //lock.unlock();
            }
            std::cout << "[Reader] sees: " << localCopy << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

// ----- Functor that writes to the global variable -----
class Writer {
public:
    void operator()() const {
        for (int i = 0; i < 10; ++i) {
            int newValue;
            {
                std::lock_guard<std::mutex> lock(valueMutex);  //std::unique_lock<std::mutex> lock(valueMutex);
                sharedValue++;
                //lock.unlock();
                newValue = sharedValue;
            }
            std::cout << "    [Writer] increments to: " << newValue << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

int main() {
    Reader reader;
    Writer writer;

    std::thread t1(reader);
    std::thread t2(writer);

    t1.join();
    t2.join();

    return 0;
}
