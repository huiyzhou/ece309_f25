#include <iostream>
#include <thread>
#include <chrono>

// ----- Global shared variable -----
int sharedValue = 0;

// ----- Functor that reads the global variable -----
class Reader {
public:
    void operator()() const {
        for (int i = 0; i < 10; ++i) {
            std::cout << "[Reader] sees: " << sharedValue << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

// ----- Functor that writes to the global variable -----
class Writer {
public:
    void operator()() const {
        for (int i = 0; i < 10; ++i) {
            sharedValue++;   // <-- data race (no synchronization)
            std::cout << "    [Writer] increments to: " << sharedValue << std::endl;
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
