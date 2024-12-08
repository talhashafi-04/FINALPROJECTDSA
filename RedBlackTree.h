#pragma once
#include <iostream>
#include <fstream>
#include "String.h"
#include <queue>

using namespace std;

class RedBlackNode {
public:
    bool isRed;  // true = red, false = black
    String key;
    String left;
    String right;
    String csvRow;

    RedBlackNode(const String& data, const String& keyValue, bool color = true)
        : csvRow(data), key(keyValue), isRed(color), left(""), right("") {}

    void saveToFile(const String& fileName) {
        ofstream file(("RB/" + (fileName + ".node")).c_str(), ios::binary);
        if (!file) {
            cout << "Could not open file " << fileName << " for writing" << endl;
            return;
        }

        int keyLen = key.size(), leftLen = left.size(), rightLen = right.size(), dataLen = csvRow.size();

        file.write((char*)&isRed, sizeof(isRed));
        file.write((char*)&keyLen, sizeof(keyLen));
        file.write(key.c_str(), keyLen);
        file.write((char*)&leftLen, sizeof(leftLen));
        file.write(left.c_str(), leftLen);
        file.write((char*)&rightLen, sizeof(rightLen));
        file.write(right.c_str(), rightLen);
        file.write((char*)&dataLen, sizeof(dataLen));
        file.write(csvRow.c_str(), dataLen);

        file.close();
    }
};

class RedBlackTree 
{
public:
    static RedBlackNode* loadFromFile(const String& fileName) {
        if (fileName.empty()) return nullptr;

        ifstream file(("RB/" + (fileName + ".node")).c_str(), ios::binary);
        if (!file) {
            cerr << "Could not open file " << fileName << " for reading" << endl;
            return nullptr;
        }

        bool isRed;
        int keyLen, leftLen, rightLen, dataLen;

        file.read((char*)&isRed, sizeof(isRed));
        file.read((char*)&keyLen, sizeof(keyLen));
        String keyValue(keyLen + 1, 0);
        file.read((char*)keyValue.data, keyLen);

        file.read((char*)&leftLen, sizeof(leftLen));
        String left(leftLen + 1, 0);
        file.read((char*)left.data, leftLen);

        file.read((char*)&rightLen, sizeof(rightLen));
        String right(rightLen + 1, 0);
        file.read((char*)right.data, rightLen);

        file.read((char*)&dataLen, sizeof(dataLen));
        String csvRow(dataLen + 1, 0);
        file.read((char*)csvRow.data, dataLen);

        RedBlackNode* node = new RedBlackNode(csvRow, keyValue, isRed);
        node->left = left;
        node->right = right;

        file.close();
        return node;
    }

    static void rotateLeft(const String& rootFile) {
        RedBlackNode* x = loadFromFile(rootFile);
        if (!x || x->right.empty()) return;

        RedBlackNode* y = loadFromFile(x->right);

        // Perform rotation
        String T2File = y->left;
        y->left = rootFile;
        x->right = T2File;

        // Save updated nodes
        x->saveToFile(rootFile);
        y->saveToFile(y->left);
    }

    static void rotateRight(const String& rootFile) {
        RedBlackNode* y = loadFromFile(rootFile);
        if (!y || y->left.empty()) return;

        RedBlackNode* x = loadFromFile(y->left);

        // Perform rotation
        String T2File = x->right;
        x->right = rootFile;
        y->left = T2File;

        // Save updated nodes
        y->saveToFile(rootFile);
        x->saveToFile(x->right);
    }

    static RedBlackNode* insert(const String& rootFile, const String& data, const String& key) {
        RedBlackNode* node = nullptr;
        if (!rootFile.empty()) {
            node = loadFromFile(rootFile);
        }

        if (!node) {
            RedBlackNode* newNode = new RedBlackNode(data, key, true);  // New nodes are red by default
            newNode->saveToFile(key);
            return newNode;
        }

        // Insert into left or right subtree based on the key
        if (key < node->key) {
            if (node->left.empty()) {
                node->left = key + ".node";
                RedBlackNode* leftChild = new RedBlackNode(data, key, true);
                leftChild->saveToFile(key);
            }
            else {
                node->left = insert(node->left, data, key)->key + ".node";
            }
        }
        else if (key > node->key) {
            if (node->right.empty()) {
                node->right = key + ".node";
                RedBlackNode* rightChild = new RedBlackNode(data, key, true);
                rightChild->saveToFile(key);
            }
            else {
                node->right = insert(node->right, data, key)->key + ".node";
            }
        }
        else {
            return node;  // Duplicate keys are not allowed
        }

        // Adjust the tree to maintain RBT properties
        if (isRed(loadFromFile(node->right)) && !isRed(loadFromFile(node->left))) rotateLeft(rootFile);
        if (isRed(loadFromFile(node->left)) && isRed(loadFromFile(loadFromFile(node->left)->left))) rotateRight(rootFile);
        if (isRed(loadFromFile(node->left)) && isRed(loadFromFile(node->right))) flipColors(node);

        node->saveToFile(rootFile);
        return node;
    }

    static bool isRed(RedBlackNode* node) {
        return node && node->isRed;
    }

    static void flipColors(RedBlackNode* node) {
        if (!node) return;

        node->isRed = !node->isRed;

        if (!node->left.empty()) {
            RedBlackNode* leftNode = loadFromFile(node->left);
            if (leftNode) {
                leftNode->isRed = !leftNode->isRed;
                leftNode->saveToFile(node->left);
            }
        }

        if (!node->right.empty()) {
            RedBlackNode* rightNode = loadFromFile(node->right);
            if (rightNode) {
                rightNode->isRed = !rightNode->isRed;
                rightNode->saveToFile(node->right);
            }
        }
    }


public:
    // Function to delete a node
    static RedBlackNode* deleteNode(const String& rootFile, const String& key) {
        RedBlackNode* root = loadFromFile(rootFile);
        if (!root) return nullptr;

        root = deleteNodeHelper(root, key);

        // Fix the Red-Black Tree properties after deletion
        fixDelete(root);
        root->saveToFile(rootFile);
        return root;
    }

private:
    static RedBlackNode* deleteNodeHelper(RedBlackNode* root, const String& key) {
        if (!root) return nullptr;

        if (key < root->key) {
            RedBlackNode* newLeftChild = deleteNodeHelper(loadFromFile(root->left), key);
            root->left = newLeftChild ? newLeftChild->key + ".node" : "";
        }
        else if (key > root->key) {
            RedBlackNode* newRightChild = deleteNodeHelper(loadFromFile(root->right), key);
            root->right = newRightChild ? newRightChild->key + ".node" : "";
        }
        else {
            // Node with one child or no child
            if (root->left.empty() || root->right.empty()) {
                RedBlackNode* temp = nullptr;
                if (!root->left.empty()) {
                    temp = loadFromFile(root->left);
                    root->left = "";
                }
                else if (!root->right.empty()) {
                    temp = loadFromFile(root->right);
                    root->right = "";
                }

                // If the node is a leaf or has only one child, replace it with its child
                if (!temp) {
                    delete root;  // Deleting a leaf node
                    return nullptr;
                }
                *root = *temp;  // Copy the child into the root node
                delete temp;
            }
            else {
                // Node with two children: Get the inorder successor
                RedBlackNode* successor = findMin(loadFromFile(root->right));  // Load the right child and find the min
                root->key = successor->key;
                root->csvRow = successor->csvRow;
                RedBlackNode* newRightChild = deleteNodeHelper(loadFromFile(root->right), successor->key);
                root->right = newRightChild ? newRightChild->key + ".node" : "";
            }
        }


        return root;
    }

    static RedBlackNode* findMin(RedBlackNode* node) {
        while (!node->left.empty()) {
            node = loadFromFile(node->left);
        }
        return node;
    }

    // Fix the tree after deletion
    static void fixDelete(RedBlackNode* root) {
        if (!root) return;

        if (root->isRed) {
            root->isRed = false;
            return;
        }

        // If both children are black, double black situation occurs.
        if (!isRed(loadFromFile(root->left)) && !isRed(loadFromFile(root->right))) {
            fixDoubleBlack(root);
        }
    }

    // Handle double black situations (fixing violations)
    static void fixDoubleBlack(RedBlackNode* node) {
        if (!node) return;

        RedBlackNode* sibling = nullptr;

        // If node's left child is double black, rotate right
        if (!node->left.empty()) {
            sibling = loadFromFile(node->right);
            if (sibling) {
                sibling->isRed = true;
                sibling->saveToFile(node->right);
            }
        }
    }


    static void printTree(const String& rootFile) {
        if (rootFile.empty()) {
            cout << "Tree is empty." << endl;
            return;
        }

        queue<String> nodesQueue;
        nodesQueue.push(rootFile);

        while (!nodesQueue.empty()) {
            String currentFile = nodesQueue.front();
            nodesQueue.pop();

            RedBlackNode* currentNode = loadFromFile(currentFile);
            if (!currentNode) continue;

            // Print the current node's data
            cout << "Key: " << currentNode->key << ", Data: " << currentNode->csvRow
                << ", Color: " << (currentNode->isRed ? "Red" : "Black") << endl;

            // Enqueue left and right children
            if (!currentNode->left.empty()) {
                nodesQueue.push(currentNode->left);
            }
            if (!currentNode->right.empty()) {
                nodesQueue.push(currentNode->right);
            }

            // Free the memory of the current node
            delete currentNode;
        }
    }

};
