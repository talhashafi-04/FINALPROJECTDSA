#include <iostream>
#include <fstream>
#include "String.h"
#include <queue>

using namespace std;


class AVLNode 
{
public :
    int height;

    String key;   
    String left;       
    String right;      
    String hash;
    String csvRow;



    AVLNode(const String& data , const String &keyValue, int h = 1): csvRow (data) ,key(keyValue), height(h), left(""), right(""), hash("") {}
      
    void saveToFile(const String& fileName)
    {
        ofstream file(("AVL/" + (fileName + ".node")).c_str(), ios::binary);
        if (!file)
        {
            cout << "Could not open file " << fileName << " for writing" << endl;
            return;
        }

        // Write key and height
        file.write((char*)&height, sizeof(height));

        // Write lengths and contents for left, right, hash, and csvRow
        int keyLen = key.size(), leftLen = left.size(), rightLen = right.size(), hashLen = hash.size(), dataLen = csvRow.size();

        file.write((char*)&keyLen, sizeof(keyLen));
        file.write(key.c_str(), keyLen);

        file.write((char*)&leftLen, sizeof(leftLen));
        file.write(left.c_str(), leftLen);

        file.write((char*)&rightLen, sizeof(rightLen));
        file.write(right.c_str(), rightLen);

        file.write((char*)&hashLen, sizeof(hashLen));
        file.write(hash.c_str(), hashLen);

        file.write((char*)&dataLen, sizeof(dataLen));
        file.write(csvRow.c_str(), dataLen);

        file.close();
    }


    

};

class AVL
{
    
public:

    
    static AVLNode* loadFromFile(const String& fileName)
    {
        ifstream file(("AVL/" + (fileName + ".node")).c_str(), ios::binary);
        if (!file)
        {
            cerr << "Could not open file " << fileName << " for reading" << endl;
            return nullptr;
        }

        // Read key and height
        int h;
        file.read((char*)&h, sizeof(h));

        // Read lengths and contents for left, right, hash, and csvRow
        int keyLen, leftLen, rightLen, hashLen, dataLen;
        
        file.read((char*)&keyLen, sizeof(keyLen));
        String keyValue(keyLen+1, 0);
        file.read((char*)keyValue.data, keyLen);

        file.read((char*)&leftLen, sizeof(leftLen));
        String left(leftLen+1, '\0');
        file.read(left.data, leftLen);

        file.read((char*)&rightLen, sizeof(rightLen));
        String right(rightLen+1, '\0');
        if(rightLen)
        file.read(right.data, rightLen);

        file.read((char*)&hashLen, sizeof(hashLen));
        String hash(hashLen+1, '\0');
        if(hashLen)
        file.read(hash.data, hashLen);

        file.read((char*)&dataLen, sizeof(dataLen));
        String csvRow(dataLen+1, '\0');
        if(dataLen)
        file.read(csvRow.data, dataLen);

        // Create the node and assign values
        AVLNode* node = new AVLNode(csvRow, keyValue, h);
        node->left = left;
        node->right = right;
        node->hash = hash;

        file.close();
        return node;
    }


    static int getHeight(const String& rootFile) {
        AVLNode* node = loadFromFile(rootFile);
        if (!node) return 0;
        return node->height;
    }


    static AVLNode* findMin(const String& rootFile) {
        AVLNode* current = loadFromFile(rootFile);
        while (current && !current->left.empty()) {
            current = loadFromFile(current->left);
        }
        return current;
    }


    static AVLNode* rotateRight(const String& rootFile) {
        // Load the node from its file
        AVLNode* y = loadFromFile(rootFile);
        if (!y) return nullptr;

        // Load left child (x)
        AVLNode* x = loadFromFile(y->left);
        if (!x) return y; // If left child doesn't exist, no rotation is possible

        // Perform rotation
        String T2File = x->right;
        x->right = rootFile;
        y->left = T2File;

        // Update heights
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        // Save updated nodes
        y->saveToFile(rootFile);
        x->saveToFile(x->right);

        // Return new root (x)
        return x;
    }

    static AVLNode* rotateLeft(const String& rootFile) {
        // Load the node from its file
        AVLNode* x = loadFromFile(rootFile);
        if (!x) return nullptr;

        // Load right child (y)
        AVLNode* y = loadFromFile(x->right);
        if (!y) return x; // If right child doesn't exist, no rotation is possible

        // Perform rotation
        String T2File = y->left;
        y->left = rootFile;
        x->right = T2File;

        // Update heights
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        // Save updated nodes
        x->saveToFile(rootFile);
        y->saveToFile(y->left);

        // Return new root (y)
        return y;
    }

   
    static AVLNode* insert(const String& rootFile,const String & data,const String & key)
    {
        AVLNode* node = nullptr;
        if (!rootFile.empty()) 
        {
            node = loadFromFile(rootFile);
        }

        if (!node)
        {
            AVLNode* newNode = new AVLNode(data , key);
            newNode->saveToFile(key);
            return newNode;
        }

        if (key < node->key)
        {
            node->left = insert(node->left,data, key)->key ;
        }
        else if (key > node->key) 
        {
            node->right = insert(node->right,data, key)->key;
        }
        else 
        {
            return node;
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getHeight(node->left) - getHeight(node->right);


        // LL
        if (balance > 1 && key < loadFromFile(node->left)->key) {
            return rotateRight(rootFile);
        }

        // RR
        if (balance < -1 && key > loadFromFile(node->right)->key) {
            return rotateLeft(rootFile);
        }

        // LR
        if (balance > 1 && key > loadFromFile(node->left)->key) {
            node->left = rotateLeft(node->left)->key + ".node";
            return rotateRight(rootFile);
        }

        // RL
        if (balance < -1 && key < loadFromFile(node->right)->key) {
            node->right = rotateRight(node->right)->key + ".node";
            return rotateLeft(rootFile);
        }

        node->saveToFile(rootFile);
        return node;
    }

    static AVLNode* deleteNode(const String& rootFile, String key) {
        // Step 1: Load the current node
        AVLNode* node = nullptr;
        if (!rootFile.empty()) {
            node = loadFromFile(rootFile);
        }

        // Base case: Node not found
        if (!node) return nullptr;

        // Step 2: Perform standard BSString deletion
        if (key < node->key) {
            // Key is in the left subtree
            node->left = deleteNode(node->left, key) ? node->left : "";
        }
        else if (key > node->key) {
            // Key is in the right subtree
            node->right = deleteNode(node->right, key) ? node->right : "";
        }
        else {
            // Key matches the current node
            // Case 1: Node with one or no child
            if (!node->left.empty() && !node->right.empty()) {
                // Case 2: Node with two children
                // Get the inorder successor (smallest in the right subtree)
                AVLNode* successor = findMin(node->right);
                node->key = successor->key;

                // Replace hash if necessary
                node->hash = successor->hash;

                // Delete the inorder successor
                node->right = deleteNode(node->right, successor->key) ? node->right : "";
            }
            else {
                // Case 1: Node with one or no child
                AVLNode* temp = nullptr;
                if (!node->left.empty()) {
                    temp = loadFromFile(node->left);
                }
                else if (!node->right.empty()) {
                    temp = loadFromFile(node->right);
                }

                if (!temp) {
                    // No child (leaf node)
                    delete node;
                    remove(rootFile.c_str());
                    return nullptr;
                }
                else {
                    // One child
                    *node = *temp; // Copy child data into current node
                    delete temp;
                    remove(temp->left.c_str());
                }
            }
        }

        // Step 3: Update the height of the current node
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        // Step 4: Balance the node
        int balance = getHeight(node->left) - getHeight(node->right);

        // Left Left Case
        if (balance > 1 && getHeight(node->left) >= getHeight(node->right)) {
            return rotateRight(rootFile);
        }

        // Left Right Case
        if (balance > 1 && getHeight(node->left) < getHeight(node->right)) {
            node->left = rotateLeft(node->left)->key + ".node";
            return rotateRight(rootFile);
        }

        // Right Right Case
        if (balance < -1 && getHeight(node->right) >= getHeight(node->left)) {
            return rotateLeft(rootFile);
        }

        // Right Left Case
        if (balance < -1 && getHeight(node->right) < getHeight(node->left)) {
            node->right = rotateRight(node->right)->key + ".node";
            return rotateLeft(rootFile);
        }

        // Save changes to the file
        node->saveToFile(rootFile);
        return node;
    }
    
};
