#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

// ----- Global shared variable (now atomic) -----
std::atomic<int> sharedValue(0); //Changed from int to std::atomic<int>, which provides lock-free thread-safe operations

// ----- Functor that reads the global variable -----
class Reader {
public:
    void operator()() const {
        for (int i = 0; i < 10; ++i) {
            int localCopy = sharedValue.load(); //Uses sharedValue.load() to atomically read the value
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
            int newValue = ++sharedValue; //Uses ++sharedValue which performs an atomic increment and returns the new value
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
