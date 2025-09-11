#include <iostream>

using namespace std;

template <typename T>
class SinglyLinkedList {
private:
    // ====================== Node (nested) ======================
    struct Node {
        T data;
        Node* next;
        Node(const T& value, Node* n = nullptr)
            : data(value), next(n) {}
    };

    Node* head;
    Node* tail;

    void clear() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
    }

    void copyFrom(const SinglyLinkedList& other) {
        if (!other.head) {
            head = tail = nullptr;
            return;
        }
        head = new Node(other.head->data);
        Node* curr = head;
        Node* otherCurr = other.head->next;
        while (otherCurr) {
            curr->next = new Node(otherCurr->data);
            curr = curr->next;
            otherCurr = otherCurr->next;
        }
        tail = curr;
    }

public:
    // ====================== Iterator ======================
    class Iterator {
    private:
        Node* node;
    public:
        Iterator(Node* n) : node(n) {}
        T& operator*() { return node->data; }
        Iterator& operator++() { node = node->next; return *this; }
        bool operator!=(const Iterator& other) const { return node != other.node; }
    };

    // Rule of Five
    SinglyLinkedList() : head(nullptr), tail(nullptr) {}
    SinglyLinkedList(const SinglyLinkedList& other) : head(nullptr), tail(nullptr) { copyFrom(other); }
    SinglyLinkedList(SinglyLinkedList&& other) noexcept : head(other.head), tail(other.tail) {
        other.head = other.tail = nullptr;
    }
    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            other.head = other.tail = nullptr;
        }
        return *this;
    }
    ~SinglyLinkedList() { clear(); }

    // Member functions
    void push_front(const T& value) {
        Node* newNode = new Node(value, head);
        head = newNode;
        if (!tail) tail = head;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value, nullptr);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void pop_front() {
        if (!head) return;
        Node* tmp = head;
        head = head->next;
        delete tmp;
        if (!head) tail = nullptr;
    }

    bool empty() const { return head == nullptr; }

    size_t size() const {
        size_t count = 0;
        Node* curr = head;
        while (curr) {
            ++count;
            curr = curr->next;
        }
        return count;
    }

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

// ====================== Example Usage ======================
int main() {
    SinglyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_front(0);

    cout << "List: ";
    for (auto it = list.begin(); it != list.end(); ++it)
        cout << *it << " ";
    cout << "\n";

    cout << "Size = " << list.size() << "\n";

    list.pop_front();
    cout << "After pop_front, size = " << list.size() << "\n";
    
    SinglyLinkedList<int> list2(std::move(list));
    cout << "List: ";
    for (auto it = list.begin(); it != list.end(); ++it)
        cout << *it << " ";
    cout << "\n";
    cout << "List2: ";
    for (auto it = list2.begin(); it != list2.end(); ++it)
        cout << *it << " ";
    cout << "\n";
}
