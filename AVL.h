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

class AVL
{
    String rootFile;
public:
    AVL()
    {
        ifstream file("AVL/root.txt", ios::binary);
        if (file)
        {
            cout << "Unable to open AVL root file for input";
            ofstream outFile("AVL/root.txt", ios::binary,ios::trunc);
            rootFile = "";
        }
        int len = 0 ;

        file.read((char*)&len, sizeof(len));
        String str(len + 1, 0);
        file.read((char*)str.data, len);
        rootFile = str;
        file.close();
    }

    ~AVL()
    {
        ofstream file("AVL/root.txt", ios::binary);
        if (file)
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
        ifstream file(("AVL/" + (fileName + ".node")).c_str(), ios::binary);
        if (!file)
        {
            cerr << "Could not open file " << fileName << " for reading" << endl;
            return nullptr;
        }

        int h;
        file.read((char*)&h, sizeof(h));

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

        AVLNode* node = new AVLNode(csvRow, keyValue, h);
        node->left = left;
        node->right = right;
        node->hash = hash;

        file.close();
        return node;
    }


    int getHeight(const String& rootFile) {
        AVLNode* node = loadFromFile(rootFile);
        if (!node) return 0;
        return node->height;
    }


    AVLNode* findMin(const String& rootFile) {
        AVLNode* current = loadFromFile(rootFile);
        while (current && !current->left.empty()) {
            current = loadFromFile(current->left);
        }
        return current;
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

        delete x, y;

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
            return rootFile; 


        String T2File = y->left;
        y->left = rootFile;
        x->right = T2File;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        x->saveToFile(rootFile);

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        y->saveToFile(st);

        delete x, y;
        // Return new root (y)
        return st;
    }


    void insert(const String& data, const String& key, const String fileName)
    {
        insertHelper(rootFile, data, key, fileName);
    }
    String insertHelper(const String& rootFile,const String & data,const String & key , const String fileName)
    {
        AVLNode* node = nullptr;
        if (!rootFile.empty()) 
        {
            node = loadFromFile(rootFile);
        }

        if (!node)
        {
            AVLNode* newNode = new AVLNode(data , key);
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
            returned = insertHelper(left,data, key , fileName);
        }
        else if (key > node->key) 
        {
            String right = node->right;
            leftCall = false;
            delete node;
            returned = insertHelper(right,data, key,fileName);
        }
        else 
        {
            return rootFile;
        }

        node = loadFromFile(rootFile);
        leftCall ? (node->left = returned) : (node->right = returned);
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
            node->left = rotateLeft(node->left) ;
            node->saveToFile(rootFile);
            return rotateRight(rootFile);
        }

        // RL
        if (balance < -1 && key < loadFromFile(node->right)->key) {
            node->right = rotateRight(node->right) ;
            node->saveToFile(rootFile);
            return rotateLeft(rootFile);
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
            node->left =  deleteNode(node->left, key);
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
                    cout << "Unable to delete file " << rootFile ;
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
            node->left = rotateLeft(node->left) ;
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
    void printTree(const String& rootFile) {
        if (rootFile.empty()) {
            return;
        }

        AVLNode* node = loadFromFile(rootFile);
        if (!node) {
            return;
        }

        // Recursively print the left subtree
        printTree(node->left);

        // Print the current node
        cout << "Key: " << node->key << ", Data: " << node->csvRow << ", Height: " << node->height
            << ", Hash: " << node->hash << ", Left: " << node->left << ", Right: " << node->right << endl;

        // Recursively print the right subtree
        printTree(node->right);
    }
};
