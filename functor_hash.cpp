#include <iostream>
#include <string>
using namespace std;

// ------------------ Hash Functor ------------------
struct SimpleHash {
    size_t operator()(const string& s) const {
        size_t h = 0;
        for (char c : s)
            h = h * 131 + c;
        return h;
    }
};

// ------------------ Hash Table ------------------
template<typename HashFunc>
class HashTable {
private:
    static const int SIZE = 10;

    string table[SIZE];
    bool used[SIZE] = { false };     // true = slot has ever been used
    bool deleted[SIZE] = { false };  // true = slot was used then deleted

    HashFunc hash;

public:
    // ------------------ INSERT ------------------
    void insert(const string& key) {
        size_t h = hash(key) % SIZE;

        for (int i = 0; i < SIZE; i++) {
            int idx = (h + i) % SIZE;

            if (!used[idx] || deleted[idx]) {
                table[idx] = key;
                used[idx] = true;
                deleted[idx] = false;
                cout << "Inserted \"" << key << "\" at index " << idx << endl;
                return;
            }
        }

        cout << "Table full! Cannot insert \"" << key << "\"\n";
    }

    // ------------------ FIND ------------------
    void find(const string& key) {
        size_t h = hash(key) % SIZE;

        for (int i = 0; i < SIZE; i++) {
            int idx = (h + i) % SIZE;

            if (!used[idx] && !deleted[idx]) {
                // Slot has never been used → cannot be further in probe chain
                cout << key << " not found\n";
                return;
            }

            if (!deleted[idx] && table[idx] == key) {
                cout << key << " found at index " << idx << endl;
                return;
            }
        }

        cout << key << " not found\n";
    }

    // ------------------ REMOVE ------------------
    void remove(const string& key) {
        size_t h = hash(key) % SIZE;

        for (int i = 0; i < SIZE; i++) {
            int idx = (h + i) % SIZE;

            if (!used[idx] && !deleted[idx]) {
                // Never used -> cannot be further
                cout << "Cannot remove \"" << key << "\" (not found)\n";
                return;
            }

            if (!deleted[idx] && table[idx] == key) {
                // Mark as deleted (tombstone)
                deleted[idx] = true;
                cout << "Removed \"" << key << "\" from index " << idx << endl;
                return;
            }
        }

        cout << "Cannot remove \"" << key << "\" (not found)\n";
    }
};
int main() {
    HashTable<SimpleHash> ht;

    ht.insert("apple");
    ht.insert("banana");
    ht.insert("orange");
    ht.insert("grape");

    cout << "\nFinding ...\n";
    ht.find("banana");
    ht.find("grape");

    cout << "\nRemoving ...\n";
    ht.remove("banana");
    ht.remove("grape");

    cout << "\nFinding after removal...\n";
    ht.find("banana");
    ht.find("grape");

    return 0;
}
