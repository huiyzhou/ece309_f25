#include <iostream>
#include <algorithm>

template <typename T>
class AVLTree {
private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        int height;

        Node(const T& k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node) const {
        return node ? node->height : 0;
    }

    int getBalance(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(Node* node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* insertNode(Node* node, const T& key) {
        if (!node) return new Node(key);

        if (key < node->key)
            node->left = insertNode(node->left, key);
        else if (key > node->key)
            node->right = insertNode(node->right, key);
        else
            return node; // ignore duplicates

        updateHeight(node);

        int balance = getBalance(node);

        // LL
        if (balance > 1 && key < node->left->key)
            return rotateRight(node);

        // RR
        if (balance < -1 && key > node->right->key)
            return rotateLeft(node);

        // LR
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // RL
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }

    Node* removeNode(Node* node, const T& key) {
        if (!node) return nullptr;

        if (key < node->key)
            node->left = removeNode(node->left, key);
        else if (key > node->key)
            node->right = removeNode(node->right, key);
        else {
            // node with one or no child
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            } else {
                // node with two children
                Node* temp = minValueNode(node->right);
                node->key = temp->key;
                node->right = removeNode(node->right, temp->key);
            }
        }

        updateHeight(node);
        int balance = getBalance(node);

        // Balance rotations
        if (balance > 1 && getBalance(node->left) >= 0)
            return rotateRight(node);

        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && getBalance(node->right) <= 0)
            return rotateLeft(node);

        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    bool searchNode(Node* node, const T& key) const {
        if (!node) return false;
        if (key == node->key) return true;
        if (key < node->key)
            return searchNode(node->left, key);
        else
            return searchNode(node->right, key);
    }

    void inorder(Node* node) const {
        if (!node) return;
        inorder(node->left);
        std::cout << node->key << " ";
        inorder(node->right);
    }

    void preorder(Node* node) const {
        if (!node) return;
        std::cout << node->key << " ";
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(Node* node) const {
        if (!node) return;
        postorder(node->left);
        postorder(node->right);
        std::cout << node->key << " ";
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    Node* clone(Node* node) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->key);
        newNode->left = clone(node->left);
        newNode->right = clone(node->right);
        newNode->height = node->height;
        return newNode;
    }

public:
    // Constructors & Rule of 5
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        destroy(root);
    }

    AVLTree(const AVLTree& other) {
        root = clone(other.root);
    }

    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            destroy(root);
            root = clone(other.root);
        }
        return *this;
    }

    AVLTree(AVLTree&& other) noexcept : root(other.root) {
        other.root = nullptr;
    }

    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this != &other) {
            destroy(root);
            root = other.root;
            other.root = nullptr;
        }
        return *this;
    }

    // Public API
    void insert(const T& key) {
        root = insertNode(root, key);
    }

    void remove(const T& key) {
        root = removeNode(root, key);
    }

    bool search(const T& key) const {
        return searchNode(root, key);
    }

    void inorder() const {
        inorder(root);
        std::cout << "\n";
    }

    void preorder() const {
        preorder(root);
        std::cout << "\n";
    }

    void postorder() const {
        postorder(root);
        std::cout << "\n";
    }
};


int main() {
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);
    tree.insert(22);    
    tree.insert(26);
    
    std::cout << "Inorder traversal: ";
    tree.inorder();

    std::cout << "Preorder traversal: ";
    tree.preorder();

    std::cout << "Postorder traversal: ";
    tree.postorder();

    std::cout << "Removing 10...\n";
    tree.remove(10);

    std::cout << "Inorder after removal: ";
    tree.inorder();

    std::cout << "preorder after removal: ";
    tree.preorder();


    std::cout << "Search 25: " << (tree.search(25) ? "Found" : "Not Found") << "\n";
}

