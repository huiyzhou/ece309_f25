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
            valueMutex.lock();
            int localCopy = sharedValue;
            valueMutex.unlock();
            
            std::cout << "[Reader] sees: " << localCopy << std::endl;

            //valueMutex.unlock();
            
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

// ----- Functor that writes to the global variable -----
class Writer {
public:
    void operator()() const {
        for (int i = 0; i < 10; ++i) {
            valueMutex.lock();
            sharedValue++;
            int newValue = sharedValue;
            valueMutex.unlock();
            
            std::cout << "    [Writer] increments to: " << newValue << std::endl;

            //valueMutex.unlock();
          
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
