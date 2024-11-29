#include <iostream>
using namespace std;

// Node structure
struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int value) : key(value), height(1), left(nullptr), right(nullptr) {}
};

// AVL Tree Class
class AVLTree {
private:
    // Helper function to get the height of a node
    int height(Node* n) {
        return n == nullptr ? 0 : n->height;
    }

    // Helper function to get the balance factor of a node
    int getBalance(Node* n) {
        return n == nullptr ? 0 : height(n->left) - height(n->right);
    }

    // Right rotation
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    // Left rotation
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    // Recursive function to insert a key
    Node* insert(Node* node, int key) {
        // Step 1: Perform normal BST insert
        if (node == nullptr)
            return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else // Equal keys are not allowed in BST
            return node;

        // Step 2: Update height of the current node
        node->height = 1 + max(height(node->left), height(node->right));

        // Step 3: Get the balance factor and balance the tree
        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rotateRight(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return rotateLeft(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // Recursive function to perform in-order traversal
    void inOrder(Node* root) {
        if (root != nullptr) {
            inOrder(root->left);
            cout << root->key << " ";
            inOrder(root->right);
        }
    }

    // Recursive function to find the node with the smallest key
    Node* minValueNode(Node* node) {
        Node* current = node;

        // Loop to find the leftmost leaf
        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    // Recursive function to delete a key
    Node* deleteNode(Node* root, int key) {
        // Step 1: Perform normal BST delete
        if (root == nullptr)
            return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            // Node with only one child or no child
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;

                // No child case
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else // One child case
                    *root = *temp;

                delete temp;
            }
            else {
                // Node with two children: Get the inorder successor
                Node* temp = minValueNode(root->right);

                // Copy the inorder successor's data to this node
                root->key = temp->key;

                // Delete the inorder successor
                root->right = deleteNode(root->right, temp->key);
            }
        }

        // If the tree had only one node then return
        if (root == nullptr)
            return root;

        // Step 2: Update height of the current node
        root->height = 1 + max(height(root->left), height(root->right));

        // Step 3: Get the balance factor and balance the tree
        int balance = getBalance(root);

        // Left Left Case
        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);

        // Left Right Case
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);

        // Right Left Case
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

public:
    Node* root;

    // Constructor
    AVLTree() : root(nullptr) {}

    // Insert function
    void insert(int key) {
        root = insert(root, key);
    }

    // Delete function
    void deleteKey(int key) {
        root = deleteNode(root, key);
    }

    // In-order traversal
    void inOrder() {
        inOrder(root);
        cout << endl;
    }
};
