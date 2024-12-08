#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <fstream>
#include "String.h"
#include "Vector.h"
#include<iomanip>
#include <queue>

using namespace std;


class AVLNode
{
public:
    int height;
    static int i;
    String key;
    String left;
    String right;
    String hash;
    String csvRow;



    AVLNode(const String& data, const String& keyValue, int h = 1) : csvRow(data), key(keyValue), height(h), left(""), right(""), hash("") { i++; }
    ~AVLNode() { i--; }
    void saveToFile(const String& fileName)
    {
        ofstream file(((fileName + ".node")).c_str(), ios::binary);
        if (!file)
        {
            cout << "Could not open file " << fileName << " for writing" << endl;
            return;
        }

        file.write((char*)&height, sizeof(height));

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
int AVLNode::i = 0;

class AVL
{
public:
    String rootFile;
    String pathToroot;
    AVL(String pathToroot) :pathToroot(pathToroot)
    {
        ifstream file(pathToroot.data, ios::binary);
        if (!file.is_open())
        {
            cout << "Unable to open AVL root file for input";
            ofstream outFile(pathToroot.data, ios::binary);
            rootFile = "";
        }
        int len = 0;

        file.read((char*)&len, sizeof(len));
        String str(len + 1, 0);
        file.read((char*)str.data, len);
        rootFile = str;
        file.close();
    }

    ~AVL()
    {
        ofstream file(pathToroot.data, ios::binary);
        if (!file.is_open())
        {
            cout << "Unable to open AVL root file for ouptut";
        }
        int len = rootFile.size();
        file.write((char*)&len, sizeof(len));
        file.write((char*)rootFile.data, len);
        file.close();
    }
    AVLNode* loadFromFile(const String& fileName)
    {
        if (fileName.empty())
        {
            return nullptr;
        }
        ifstream file(((fileName + ".node")).c_str(), ios::binary);
        if (!file.is_open())
        {
            cerr << "Could not open file " << fileName << " for reading" << endl;
            return nullptr;
        }

        int h;
        file.read((char*)&h, sizeof(h));

        int keyLen, leftLen, rightLen, hashLen, dataLen;

        file.read((char*)&keyLen, sizeof(keyLen));
        String keyValue(keyLen + 1, 0);
        file.read((char*)keyValue.data, keyLen);

        file.read((char*)&leftLen, sizeof(leftLen));
        String left(leftLen + 1, '\0');
        file.read(left.data, leftLen);

        file.read((char*)&rightLen, sizeof(rightLen));
        String right(rightLen + 1, '\0');
        if (rightLen)
            file.read(right.data, rightLen);

        file.read((char*)&hashLen, sizeof(hashLen));
        String hash(hashLen + 1, '\0');
        if (hashLen)
            file.read(hash.data, hashLen);

        file.read((char*)&dataLen, sizeof(dataLen));
        String csvRow(dataLen + 1, '\0');
        if (dataLen)
            file.read(csvRow.data, dataLen);

        AVLNode* node = new AVLNode(csvRow, keyValue, h);
        node->left = left;
        node->right = right;
        node->hash = hash;

        file.close();
        return node;
    }


    int getHeight(const String& rootFile)
    {
        AVLNode* node = loadFromFile(rootFile);
        if (!node) return 0;
        int i = node->height;
        delete node;
        return i;
    }


    AVLNode* findMin(const String& rootFile)
    {
        AVLNode* current = loadFromFile(rootFile);
        while (current && !current->left.empty())
        {
            delete current;
            current = loadFromFile(current->left);
        }
        return current;
    }

    void update(const String& key, const String& newRow)
    {
        update(rootFile, key, newRow);
    }
    void update(const String& rootFile, const String& key, const String& newRow)
    {
        AVLNode* node = nullptr;
        if (!rootFile.empty())
        {
            node = loadFromFile(rootFile);
        }

        if (!node)
        {
            return;
        }

        if (key < node->key)
        {
            String left = node->left;
            delete node;
            update(left, key, newRow);
        }
        else if (key > node->key)
        {
            String right = node->right;
            delete node;
            update(right, key, newRow);
        }
        else
        {
            node->csvRow = newRow;
            node->saveToFile(rootFile);
            String right = node->right;
            delete node;
            update(right, key, newRow);

        }

    }

    String rotateRight(const String& rootFile)
    {
        AVLNode* y = loadFromFile(rootFile);
        if (!y)
            return "";
        String st = y->left;

        AVLNode* x = loadFromFile(y->left);
        if (!x)
            return rootFile;

        String T2File = x->right;
        x->right = rootFile;
        y->left = T2File;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        y->saveToFile(rootFile);

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        x->saveToFile(st);

        delete x;
        delete y;

        return st;
    }

    String rotateLeft(const String& rootFile)
    {
        AVLNode* x = loadFromFile(rootFile);
        if (!x)
            return "";
        String st = x->right;

        AVLNode* y = loadFromFile(x->right);
        if (!y)
        {
            delete x;
            return rootFile;
        }

        String T2File = y->left;
        y->left = rootFile;
        x->right = T2File;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        x->saveToFile(rootFile);

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        y->saveToFile(st);

        delete x, y;

        return st;
    }


    void insert(const String& data, const String& key, const String fileName)
    {
        rootFile = insertHelper(rootFile, data, key, fileName);
    }
    String insertHelper(const String& rootFile, const String& data, const String& key, const String fileName)
    {
        AVLNode* node = nullptr;
        if (!rootFile.empty())
        {
            node = loadFromFile(rootFile);
        }

        if (!node)
        {
            AVLNode* newNode = new AVLNode(data, key);
            newNode->saveToFile(fileName);
            delete newNode;
            return fileName;
        }
        String returned = "";
        bool leftCall = true;

        if (key < node->key)
        {
            String left = node->left;
            leftCall = true;
            delete node;
            returned = insertHelper(left, data, key, fileName);
        }
        else //if (key >= node->key) 
        {
            String right = node->right;
            leftCall = false;
            delete node;
            returned = insertHelper(right, data, key, fileName);
        }
        /* else
         {
             delete node;
             return rootFile;
         }*/

        node = loadFromFile(rootFile);
        leftCall ? (node->left = returned) : (node->right = returned);
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getHeight(node->left) - getHeight(node->right);

        AVLNode* del = nullptr;
        // LL
        if (balance > 1 && key < (del = loadFromFile(node->left))->key)
        {
            delete del;
            delete node;
            return rotateRight(rootFile);
        }
        if (del)
        {
            delete del;
            del = nullptr;
        }
        // RR
        if (balance < -1 && key >(del = loadFromFile(node->right))->key)
        {
            delete node;
            delete del;
            return rotateLeft(rootFile);
        }
        if (del)
        {
            delete del;
            del = nullptr;
        }
        // LR
        if (balance > 1 && key > (del = loadFromFile(node->left))->key)
        {
            node->left = rotateLeft(node->left);
            node->saveToFile(rootFile);
            delete node;
            delete del;
            return rotateRight(rootFile);
        }
        if (del)
        {
            delete del;
            del = nullptr;
        }
        // RL
        if (balance < -1 && key < (del = loadFromFile(node->right))->key)
        {
            node->right = rotateRight(node->right);
            node->saveToFile(rootFile);
            delete node;
            delete del;
            return rotateLeft(rootFile);
        }
        if (del)
        {
            delete del;
            del = nullptr;
        }
        node->saveToFile(rootFile);
        delete node;
        return rootFile;
    }

    String deleteNode(const String& rootFile, String key)
    {
        AVLNode* node = nullptr;
        if (!rootFile.empty())
        {
            node = loadFromFile(rootFile);
        }

        if (!node)
            return "";

        if (key < node->key)
        {
            node->left = deleteNode(node->left, key);
        }
        else if (key > node->key)
        {
            node->right = deleteNode(node->right, key);
        }
        else
        {
            if (node->left.empty())//right or no child
            {
                if (!remove(rootFile.c_str()))
                {
                    cout << "Unable to delete file " << rootFile;
                }
                return node->right;
            }
            else if (node->right.empty())   //left child
            {
                if (!remove(rootFile.c_str()))
                {
                    cout << "Unable to delete file " << rootFile;
                }
                return node->left;
            }
            else
            {
                AVLNode* temp = findMin(node->right);

                node->key = temp->key;
                node->csvRow = temp->csvRow;
                node->hash = temp->hash;

                node->right = deleteNode(node->right, temp->key);

            }
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getHeight(node->left) - getHeight(node->right);

        //LL
        if (balance > 1 && getHeight(node->left) >= getHeight(node->right)) {
            return rotateRight(rootFile);
        }

        //RR
        if (balance > 1 && getHeight(node->left) < getHeight(node->right)) {
            node->left = rotateLeft(node->left);
            return rotateRight(rootFile);
        }

        //LR
        if (balance < -1 && getHeight(node->right) >= getHeight(node->left)) {
            return rotateLeft(rootFile);
        }

        //RL
        if (balance < -1 && getHeight(node->right) < getHeight(node->left)) {
            node->right = rotateRight(node->right);
            return rotateLeft(rootFile);
        }

        node->saveToFile(rootFile);
        delete node;
        return rootFile;
    }
    void printTree()
    {
        printTree(rootFile);
    }
    void printTree(const String& rootFile)
    {
        if (rootFile.empty()) {
            return;
        }

        AVLNode* node = loadFromFile(rootFile);
        if (!node) {
            return;
        }
        String left = node->left, right = node->right;
        delete node;
        printTree(left);

        node = loadFromFile(rootFile);
        cout << "Key: " << setw(10) << node->key << ", Data: " << setw(30) << node->csvRow << ", Height: " << setw(3) << node->height
            << ", Hash: " << node->hash << ", Left: " << node->left << ", Right: " << node->right << endl;
        delete node;
        printTree(right);
    }
    void getBlocks(Vector<String>& blocks)
    {
        getBlocks(rootFile, blocks);
    }

    void getBlocks(const String& rootFile, Vector<String>& blocks)
    {
        if (rootFile.empty())
        {
            return;
        }
        AVLNode* node = loadFromFile(rootFile);
        if (!node) {
            return; // If the node is empty, return
        }
        String left = node->left;
        String right = node->right;
        String csv = node->csvRow;
        delete node;
        getBlocks(left, blocks);
        blocks.push_back(csv); // Store the current csvRow as a string in the blocks
        getBlocks(right, blocks);
    }

};


#endif