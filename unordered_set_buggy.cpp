#include <iostream>
#include <string>
#include <cstring>  
using namespace std;

template <typename T>
class Set {
public:
    virtual bool insert(T) = 0;
    virtual bool find(T) const = 0;
    virtual void remove(T) = 0;
    virtual ~Set() = default; // base virtual destructor
};

// A simple hash-based Set implementation using open addressing (linear probing)
template <typename T>
class HashSet : public Set<T> {
private:
    enum SlotState { EMPTY, OCCUPIED, DELETED };

    struct Slot {
        T value;
        SlotState state;
        Slot() : state(EMPTY) {}
    };

    Slot* table;
    size_t capacity;
    size_t size_;
    static constexpr double load_factor_threshold = 0.6;

    // Simple hash function for integers and strings
    size_t hash(const T& key) const {
        if constexpr (is_integral_v<T>) {
            return static_cast<size_t>(key) % capacity;
        } else if constexpr (is_same_v<T, string>) {
            size_t h = 0;
            for (char c : key) {
                h = h * 131 + static_cast<unsigned char>(c);
            }
            return h % capacity;
        } else {
            static_assert(sizeof(T) == 0, "Hash not implemented for this type");
        }
    }

    void rehash() {
        size_t old_cap = capacity;
        Slot* old_table = table;

        capacity *= 2;
        table = new Slot[capacity];
        size_ = 0;

        for (size_t i = 0; i < old_cap; ++i) {
            if (old_table[i].state == OCCUPIED) {
                insert(old_table[i].value);
            }
        }

        delete[] old_table;
    }

public:
    // --- Rule of 5 ---
    HashSet(size_t cap = 8) : capacity(cap), size_(0) {
        table = new Slot[capacity];
    }

    ~HashSet() {
        delete[] table;
    }

    HashSet(const HashSet& other) : capacity(other.capacity), size_(other.size_) {
        table = new Slot[capacity];
        for (size_t i = 0; i < capacity; ++i)
            table[i] = other.table[i];
    }

    HashSet& operator=(const HashSet& other) {
        if (this != &other) {
            delete[] table;
            capacity = other.capacity;
            size_ = other.size_;
            table = new Slot[capacity];
            for (size_t i = 0; i < capacity; ++i)
                table[i] = other.table[i];
        }
        return *this;
    }

    HashSet(HashSet&& other) noexcept
        : table(other.table), capacity(other.capacity), size_(other.size_) {
        other.table = nullptr;
        other.capacity = 0;
        other.size_ = 0;
    }

    HashSet& operator=(HashSet&& other) noexcept {
        if (this != &other) {
            delete[] table;
            table = other.table;
            capacity = other.capacity;
            size_ = other.size_;
            other.table = nullptr;
            other.capacity = 0;
            other.size_ = 0;
        }
        return *this;
    }

    // --- Required virtual methods ---
    bool insert(T key) override {
        if ((double)size_ / capacity > load_factor_threshold) {
            rehash();
        }

        size_t idx = hash(key);
        size_t start = idx;
        do {
            if (table[idx].state == EMPTY || table[idx].state == DELETED) {
                table[idx].value = key;
                table[idx].state = OCCUPIED;
                ++size_;
                return true;
            }
            if (table[idx].state == OCCUPIED && table[idx].value == key)
                return false; // duplicate

            idx = (idx + 1) % capacity;
        } while (idx != start);

        return false; // full (should not happen after rehash)
    }

    bool find(T key) const override {
        size_t idx = hash(key);
        size_t start = idx;
        do {
            if (table[idx].state == EMPTY)
                return false;
            if (table[idx].state == OCCUPIED && table[idx].value == key)
                return true;
            idx = (idx + 1) % capacity;
        } while (idx != start);
        return false;
    }

    void remove(T key) override {
        size_t idx = hash(key);
        size_t start = idx;
        do {
            if (table[idx].state == EMPTY)
                return;
            if (table[idx].state == OCCUPIED && table[idx].value == key) {
                table[idx].state = DELETED;
                --size_;
                return;
            }
            idx = (idx + 1) % capacity;
        } while (idx != start);
    }

    void print() const {
        cout << "{ ";
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].state == OCCUPIED)
                cout << table[i].value << " ";
        }
        cout << "}" << endl;
    }
};

int main() {
    HashSet<int> s;
    s.insert(10);
    s.insert(20);
    s.insert(30);
    s.insert(20); // duplicate ignored

    s.print();

    cout << "Find 10: " << s.find(10) << endl;
    cout << "Find 25: " << s.find(25) << endl;

    s.remove(10);
    s.print();

    // Test copy constructor
    HashSet<int> copy = s;
    copy.insert(40);
    cout << "Copy: ";
    copy.print();

    // Test move constructor
    HashSet<int> moved = std::move(copy);
    cout << "Moved: ";
    moved.print();

    return 0;
}
