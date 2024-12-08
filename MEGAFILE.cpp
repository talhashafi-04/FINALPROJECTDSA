/*
    NAME TALHA SHAFI
    ROLL 23i0563
    NAME HANZALA 
    ROLL 23i0799
    NAME HUZAIFA TAHIR
    ROLL 23i0731
*/

#include <iostream>

#include <fstream>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <windows.h>
#include <wincrypt.h>
using namespace std;
using namespace filesystem;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////STRING/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class String
{
public:
    char* data;
    int length;

    int calculateLength(const char* str) const
    {
        int len = 0;
        while (str[len] != '\0')
            ++len;
        return len;
    }

    void copyData(const char* str) 
    {
        length = calculateLength(str);
        data = new char[length + 1];
        for (int i = 0; i < length; i++)
        {
            data[i] = str[i];
        }
        data[length] = '\0';
    }
public:
    String():data(new char(0)),length(0)
    {}

    String(const char* str)
    {
        copyData(str);
    }
    String(int len, char ch = 0):length(len)
    {
        data = new char[length] {};
    }

    String(const String& other)
    {
        copyData(other.data);
    }
    String(String&& other): data(other.data), length(other.length) 
    {
        other.data = nullptr;
        other.length = 0;
    }
    ~String() {
        delete[] data;
    }

    //////////////////////////////UTILITY////////////////////////////////////////

    bool empty() const
    {
        return !length;
    }

    static String toString(int i)
    {
        String STR;
        while (i)
        {
            STR = (char)('0' + (i % 10)) + STR;
            i /= 10;
        }
        String ret = STR.c_str();
        return ret;
    }
    static int custom_strcmp(const char* str1, const char* str2)
    {
        while (*str1 && *str2)
        {
            if (*str1 != *str2)
            {
                return (*str1 < *str2) ? -1 : 1; 
            }
            str1++;
            str2++;
        }

        if (*str1) 
            return 1;  
        if (*str2)
            return -1;  
        return 0;              
    }

    int compare(const String& other) const
    {
        return custom_strcmp(this->c_str(), other.c_str());
    }

    const char* c_str() const
    {
        return data ? data : "";
    }
    bool operator<(const String& other) const {
        return compare(other) < 0;
    }
    bool operator>(const String& other) const {
        return compare(other) > 0;
    }

    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            copyData(other.data);
        }
        return *this;
    }
    String& operator=(String&& other) 
    {
        if (this != &other) {
            delete[] data;
            data = other.data;
            length = other.length;
            other.data = nullptr;
            other.length = 0;
        }
        return *this;
    }
    
    // Concatenation
    String operator+(const String& other) const 
    {
        char* newData = new char[length + other.length + 1];
        for (int i = 0; i < length; i++) 
        {
            newData[i] = data[i];
        }
        for (int i = 0; i < other.length; i++) 
        {
            newData[length + i] = other.data[i];
        }
        newData[length + other.length] = '\0';
        String result(newData);
        delete[] newData;
        return result;
    }

    String& operator+=(const String& other)
    {
        *this = *this + other;
        return *this;
    }

    bool operator==(const String& other) const 
    {
        if (length != other.length)
            return false;
        for (int i = 0; i < length; i++) 
        {
            if (data[i] != other.data[i]) 
                return false;
        }
        return true;
    }

    bool operator!=(const String& other) const
    {
        return !(*this == other);
    }

    char& operator[](int index)
    {
        if (index < length) 
        return data[index];
    }

    const char& operator[](int index) const
    {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }

    int size() const
    {
        return length;
    }

   
    friend std::ostream& operator<<(std::ostream& output, const String& str) 
{
        output << str.data;
        return output;
    }

    friend std::istream& operator>>(std::istream& input, String& str) 
    {
        char buffer[1000];
        input.getline(buffer, 1000);
        str = String(buffer);
        return input;
    }

    friend String operator+(const char* left, const String& right) 
    {
        int leftLength = 0;
        while (left[leftLength] != '\0')
            leftLength++;

        char* newData = new char[leftLength + right.length + 1];
        for (int i = 0; i < leftLength; i++)
        {
            newData[i] = left[i];
        }
        for (int i = 0; i < right.length; i++) 
        {
            newData[leftLength + i] = right.data[i];
        }
        newData[leftLength + right.length] = '\0';

        String result(newData);
        delete[] newData;
        return result;
    }

    friend String operator+(char left, const String& right) 
    {
        char* newData = new char[right.length + 2];

        newData[0] = left;

        for (int i = 0; i < right.length; i++) 
        {
            newData[i + 1] = right.data[i];
        }

        newData[right.length + 1] = '\0';
        String result(newData);
        delete[] newData;

        return result;
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////VECTOR/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
class Vector
{
private:
    T* data;
    int vSize;
    int vCapacity;

    void resize(int cap)
    {
        T* newData = new T[cap];
        for (int i = 0; i < vSize; i++)
        {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        vCapacity = cap;
    }

public:
    Vector() :data(nullptr), vSize(0), vCapacity(0)
    {}

    ~Vector()
    {
        delete[] data;
    }

    void push_back(const T& value)
    {
        if (vSize == vCapacity)
        {
            int cap = (vCapacity == 0) ? 1 : vCapacity * 2;
            resize(cap);
        }
        data[vSize++] = value;
    }

    void pop_back()
    {
        if (vSize > 0)
        {
            vSize--;
        }
    }

    int size() const
    {
        return vSize;
    }

    int capacity() const
    {
        return vCapacity;
    }

    void clear()
    {
        vSize = 0;
    }

    T& operator[](int index)
    {
        if (index < vSize)
            return data[index];
    }

    const T& operator[](int index) const
    {
        if (index < vSize)
            return data[index];
    }

    void insert(int index, const T& value)
    {
        if (index > vSize) {
            cout << "Index out of range";
        }

        if (vSize == vCapacity) {
            int cap = (vCapacity == 0) ? 1 : vCapacity * 2;
            resize(cap);
        }

        for (int i = vSize; i > index; --i) {
            data[i] = data[i - 1];
        }

        data[index] = value;
        vSize++;
    }

    void erase(int index)
    {
        if (index >= vSize)
        {
            cout << "Index out of range";
        }

        for (int i = index; i < vSize - 1; i++)
        {
            data[i] = data[i + 1];
        }
        vSize--;
    }
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////AVL//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AVLNode
{

public:
    int height;
    String key;
    String left;
    String right;
    String hash;
    String csvRow;

    AVLNode(const String& data, const String& keyValue, int h = 1):csvRow(data), key(keyValue), height(h), left(""), right(""), hash("")
    {}
    ~AVLNode() 
    {}

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

class AVL
{

public:
    String rootFile;
    String pathToroot;

    AVL(String pathToroot):pathToroot(pathToroot)
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
            cout << "Could not open file " << fileName << " for reading" << endl;
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
        rootFile = insert(rootFile, data, key, fileName);
    }
    String insert(const String& rootFile, const String& data, const String& key, const String fileName)
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
            returned = insert(left, data, key, fileName);
        }
        else 
        {
            String right = node->right;
            leftCall = false;
            delete node;
            returned = insert(right, data, key, fileName);
        }
        
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
    void deleteNode(String& key)
    {
        rootFile = deleteNode(rootFile, key);
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
        if (balance > 1 && getHeight(node->left) >= getHeight(node->right)) 
        {
            return rotateRight(rootFile);
        }

        //RR
        if (balance > 1 && getHeight(node->left) < getHeight(node->right)) 
        {
            node->left = rotateLeft(node->left);
            return rotateRight(rootFile);
        }

        //LR
        if (balance < -1 && getHeight(node->right) >= getHeight(node->left))
        {
            return rotateLeft(rootFile);
        }

        //RL
        if (balance < -1 && getHeight(node->right) < getHeight(node->left))
        {
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
        if (!node) 
        {
            return; 
        }
        String left = node->left;
        String right = node->right;
        String csv = node->csvRow;
        delete node;
        getBlocks(left, blocks);
        blocks.push_back(csv);
        getBlocks(right, blocks);
    }
    void save(const String& fileName)
    {
        ofstream outFile(fileName.c_str());
        if (!outFile.is_open())
        {
            cout << "Unable to open file: " << fileName << " for writing." << endl;
            return;
        }

        savePreOrder(rootFile, outFile);
        outFile.close();
    }

    void savePreOrder(const String& rootFile, ofstream& outFile)
    {
        if (rootFile.empty())
        {
            return;
        }

        AVLNode* node = loadFromFile(rootFile);
        if (!node)
        {
            return;
        }

        outFile << node->csvRow << endl;

        savePreOrder(node->left, outFile);

        savePreOrder(node->right, outFile);

        delete node;
    }


};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////RBTREE////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RBNode {
public:
    int color; // 0 for black, 1 for red
    String key;
    String left;
    String right;
    String csvRow;

    RBNode(const String& data, const String& keyValue, int c = 1)
        : csvRow(data), key(keyValue), color(c), left(""), right("") {}

    void saveToFile(const String& fileName) {
        ofstream file(("RB/" + (fileName + ".node")).c_str(), ios::binary);
        if (!file) {
            cout << "Could not open file " << fileName << " for writing" << endl;
            return;
        }

        file.write((char*)&color, sizeof(color));

        int keyLen = key.size(), leftLen = left.size(), rightLen = right.size(), dataLen = csvRow.size();

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

class RBTree {
    String rootFile;
    String pathToRoot;

    void getBlocks(const String& rootFile, Vector<String>& blocks)
    {
        if (rootFile.empty())
        {
            return;
        }

        RBNode* node = loadFromFile(rootFile); // Load the current node from file
        if (!node)
        {
            return;
        }

        // Recursively traverse the left subtree
        getBlocks(node->left, blocks);

        // Add the current node's CSV row to the blocks
        blocks.push_back(node->csvRow);

        // Recursively traverse the right subtree
        getBlocks(node->right, blocks);

        delete node; // Clean up to prevent memory leaks
    }


    void update(const String& rootFile, const String& key, const String& newRow)
    {
        RBNode* node = nullptr;
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
            // Key matches; update the node's csvRow
            node->csvRow = newRow;
            node->saveToFile(rootFile);

            // Update further nodes with the same key (if duplicate keys are allowed)
            String right = node->right;
            delete node;
            update(right, key, newRow);
        }
    }



public:
    RBTree() {
        ifstream file("RB/root.txt", ios::binary);
        if (!file) {
            ofstream outFile("RB/root.txt", ios::binary | ios::trunc);
            rootFile = "";
        }
        else {
            int len = 0;
            file.read((char*)&len, sizeof(len));
            String str(len + 1, 0);
            file.read((char*)str.data, len);
            rootFile = str;
            file.close();
        }
    }

    RBTree(const String& pathToRoot) : pathToRoot(pathToRoot)
    {
        ifstream file(pathToRoot.data, ios::binary);
        if (!file.is_open())
        {
            cout << "Unable to open Red-Black Tree root file for input" << endl;
            ofstream outFile(pathToRoot.data, ios::binary | ios::trunc); // Create an empty file if it doesn't exist
            rootFile = "";
            outFile.close();
            return;
        }

        int len = 0;
        file.read((char*)&len, sizeof(len));
        String str(len + 1, 0);
        file.read((char*)str.data, len);
        rootFile = str;
        file.close();
    }


    ~RBTree() {
        ofstream file("RB/root.txt", ios::binary);
        if (!file) {
            cout << "Unable to open RB root file for output";
        }
        int len = rootFile.size();
        file.write((char*)&len, sizeof(len));
        file.write((char*)rootFile.data, len);
        file.close();
    }

    void update(const String& key, const String& newRow)
    {
        update(rootFile, key, newRow);
    }

    RBNode* loadFromFile(const String& fileName) {
        ifstream file(("RB/" + (fileName + ".node")).c_str(), ios::binary);
        if (!file) {
            cerr << "Could not open file " << fileName << " for reading" << endl;
            return nullptr;
        }

        int c, keyLen, leftLen, rightLen, dataLen;
        file.read((char*)&c, sizeof(c));

        file.read((char*)&keyLen, sizeof(keyLen));
        String keyValue(keyLen + 1, 0);
        file.read((char*)keyValue.data, keyLen);

        file.read((char*)&leftLen, sizeof(leftLen));
        String left(leftLen + 1, '\0');
        if (leftLen) file.read(left.data, leftLen);

        file.read((char*)&rightLen, sizeof(rightLen));
        String right(rightLen + 1, '\0');
        if (rightLen) file.read(right.data, rightLen);

        file.read((char*)&dataLen, sizeof(dataLen));
        String csvRow(dataLen + 1, '\0');
        if (dataLen) file.read(csvRow.data, dataLen);

        RBNode* node = new RBNode(csvRow, keyValue, c);
        node->left = left;
        node->right = right;

        file.close();
        return node;
    }




    String rotateRight(const String& rootFile) {
        RBNode* y = loadFromFile(rootFile);
        if (!y) return rootFile;
        String st = y->left;

        RBNode* x = loadFromFile(st);
        if (!x) return rootFile;

        String T2 = x->right;
        x->right = rootFile;
        y->left = T2;

        y->saveToFile(rootFile);
        x->saveToFile(st);

        delete x;
        delete y;

        return st;
    }

    String rotateLeft(const String& rootFile) {
        RBNode* x = loadFromFile(rootFile);
        if (!x) return rootFile;
        String st = x->right;

        RBNode* y = loadFromFile(x->right);
        if (!y) return rootFile;

        String T2 = y->left;
        y->left = rootFile;
        x->right = T2;

        x->saveToFile(rootFile);
        y->saveToFile(st);

        delete x;
        delete y;

        return st;
    }

    String balanceInsert(const String& rootFile) {
        RBNode* node = loadFromFile(rootFile);
        if (!node) return rootFile;

        RBNode* leftNode = node->left.empty() ? nullptr : loadFromFile(node->left);
        RBNode* rightNode = node->right.empty() ? nullptr : loadFromFile(node->right);

        // Case 1: Recoloring
        if (leftNode && leftNode->color == 1 && rightNode && rightNode->color == 1) {
            node->color = 1;
            leftNode->color = 0;
            rightNode->color = 0;

            node->saveToFile(rootFile);
            leftNode->saveToFile(node->left);
            rightNode->saveToFile(node->right);

            delete node;
            delete leftNode;
            delete rightNode;
            return rootFile;
        }

        // Case 2: Left-Left (LL)
        if (leftNode && leftNode->color == 1) {
            RBNode* leftLeftNode = leftNode->left.empty() ? nullptr : loadFromFile(leftNode->left);
            if (leftLeftNode && leftLeftNode->color == 1) {
                String newRoot = rotateRight(rootFile);
                RBNode* newNode = loadFromFile(newRoot);
                RBNode* newRightNode = newNode->right.empty() ? nullptr : loadFromFile(newNode->right);

                newNode->color = 0;
                if (newRightNode) newRightNode->color = 1;

                newNode->saveToFile(newRoot);
                if (newRightNode) newRightNode->saveToFile(newNode->right);

                delete newNode;
                delete newRightNode;
                delete node;
                delete leftNode;
                delete leftLeftNode;
                return newRoot;
            }
            delete leftLeftNode;
        }

        // Additional cases for LR, RR, RL omitted for brevity
        return rootFile;
    }

    void insert(const String& data, const String& key, const String& fileName) {
        rootFile = insertHelper(rootFile, data, key, fileName);

        // Ensure the root node is always black
        RBNode* rootNode = loadFromFile(rootFile);
        if (rootNode) {
            rootNode->color = 0; // Black
            rootNode->saveToFile(rootFile);
            delete rootNode;
        }
    }

    String insertHelper(const String& rootFile, const String& data, const String& key, const String& fileName) {
        RBNode* node = nullptr;

        // Load the root node if it exists
        if (!rootFile.empty()) {
            node = loadFromFile(rootFile);
        }

        // Base case: Create a new node if the tree is empty
        if (!node) {
            RBNode* newNode = new RBNode(data, key, 1); // New nodes are Red by default
            newNode->saveToFile(fileName);
            delete newNode;
            return fileName;
        }

        String returnedFile = "";

        // Perform BST insertion
        if (key < node->key) {
            String left = node->left;
            delete node;
            returnedFile = insertHelper(left, data, key, fileName);
            node = loadFromFile(rootFile);
            node->left = returnedFile;
        }
        else if (key > node->key) {
            String right = node->right;
            delete node;
            returnedFile = insertHelper(right, data, key, fileName);
            node = loadFromFile(rootFile);
            node->right = returnedFile;
        }
        else {
            // Duplicate keys are not allowed
            return rootFile;
        }

        // Fix Red-Black Tree properties after insertion
        String fixedRoot = fixViolations(rootFile, node);
        delete node;
        return fixedRoot;
    }

    void printTree()
    {
        printTree(rootFile);
    }
    
    void printTree(const String& rootFile) {
        if (rootFile.empty()) {
            return;
        }

        RBNode* node = loadFromFile(rootFile);
        if (!node) {
            return;
        }

        // Recursively print the left subtree
        printTree(node->left);

        // Print the current node
        cout << "Key: " << node->key
            << ", Data: " << node->csvRow
            << ", Color: " << (node->color == 0 ? "Black" : "Red")
            << ", Left: " << node->left
            << ", Right: " << node->right
            << endl;

        // Recursively print the right subtree
        printTree(node->right);

        delete node;
    }



    String fixViolations(const String& rootFile, RBNode* node) {
        RBNode* parent = node;
        RBNode* leftChild = (!node->left.empty()) ? loadFromFile(node->left) : nullptr;
        RBNode* rightChild = (!node->right.empty()) ? loadFromFile(node->right) : nullptr;

        // Case 1: Both children of a Red node are Red
        if (node->color == 1 &&
            ((leftChild && leftChild->color == 1) || (rightChild && rightChild->color == 1))) {
            node->color = 0; // Recolor node to Black
            if (leftChild) leftChild->color = 1; // Recolor left child to Red
            if (rightChild) rightChild->color = 1; // Recolor right child to Red
        }

        // Case 2: Left-Heavy Rotations (LL and LR)
        if (leftChild && leftChild->color == 1) {
            if (!rightChild || rightChild->color == 0) {
                if (!leftChild->left.empty() && loadFromFile(leftChild->left)->color == 1) {
                    return rotateRight(rootFile); // LL Case
                }
                if (!leftChild->right.empty() && loadFromFile(leftChild->right)->color == 1) {
                    node->left = rotateLeft(node->left);
                    node->saveToFile(rootFile);
                    return rotateRight(rootFile); // LR Case
                }
            }
        }

        // Case 3: Right-Heavy Rotations (RR and RL)
        if (rightChild && rightChild->color == 1) {
            if (!leftChild || leftChild->color == 0) {
                if (!rightChild->right.empty() && loadFromFile(rightChild->right)->color == 1) {
                    return rotateLeft(rootFile); // RR Case
                }
                if (!rightChild->left.empty() && loadFromFile(rightChild->left)->color == 1) {
                    node->right = rotateRight(node->right);
                    node->saveToFile(rootFile);
                    return rotateLeft(rootFile); // RL Case
                }
            }
        }

        node->saveToFile(rootFile);
        delete leftChild;
        delete rightChild;
        return rootFile;
    }

    String balanceAfterDelete(const String& rootFile, const String& key) {
        if (rootFile.empty()) return rootFile;

        RBNode* node = loadFromFile(rootFile);
        if (!node) return rootFile;

        // Case 1: If the node to delete is black, and its sibling is black, we need to fix the double-black node.
        if (node->color == 0) {
            String siblingFile = node->left;
            RBNode* sibling = loadFromFile(siblingFile);

            // Case 1.1: Sibling is red, rotate to fix the double-black node.
            if (sibling && sibling->color == 1) {
                node->color = 1; // Promote the current node to red
                sibling->color = 0; // Demote sibling to black
                String newRoot = rotateLeft(siblingFile);
                return newRoot;
            }
            // Case 1.2: Sibling is black and its children are black, recolor the sibling to red.
            else if (sibling && sibling->color == 0) {
                sibling->color = 1;
                node->color = 0; // Ensure the node is black
                return rootFile; // Return after recoloring.
            }
        }

        delete node;
        return rootFile;  // Return root (might change if rotated)
    }

    String findMin(const String& rootFile) {
        RBNode* node = loadFromFile(rootFile);
        while (node && !node->left.empty()) {
            String left = node->left;
            delete node;
            node = loadFromFile(left);
        }
        String minKey = node ? node->key : "";
        delete node;
        return minKey;
    }

    // Deletion logic
    String deleteHelper(const String& rootFile, const String& key) {
        RBNode* node = loadFromFile(rootFile);
        if (!node) return rootFile;

        if (key < node->key) {
            node->left = deleteHelper(node->left, key);
        }
        else if (key > node->key) {
            node->right = deleteHelper(node->right, key);
        }
        else {
            // Node found: handle deletion cases
            if (node->left.empty() && node->right.empty()) {
                delete node;
                return ""; // Leaf node
            }
            else if (node->left.empty() || node->right.empty()) {
                String child = node->left.empty() ? node->right : node->left;
                delete node;
                return child; // Single child
            }
            else {
                // Two children: Replace with in-order successor
                String minKey = findMin(node->right);
                RBNode* successor = loadFromFile(minKey);
                node->key = successor->key;
                node->csvRow = successor->csvRow;
                node->right = deleteHelper(node->right, successor->key);
                delete successor;
            }
        }

        String newRoot = balanceAfterDelete(rootFile, key);
        node->saveToFile(rootFile);
        delete node;
        return newRoot;
    }

    void deleteNode(const String& key) {
        rootFile = deleteHelper(rootFile, key);

        // Ensure root is always black
        RBNode* root = loadFromFile(rootFile);
        if (root) {
            root->color = 0; // Black
            root->saveToFile(rootFile);
            delete root;
        }
    }

    void getBlocks(Vector<String>& blocks)
    {
        getBlocks(rootFile, blocks);
    }




    void testInsert() {
        insert("Data1", "50", "node50");
        insert("Data2", "21", "node21");
        insert("Data3", "54", "node54");
        insert("Data4", "12", "node12");
        insert("Data5", "43", "node43");
        insert("Data6", "57", "node57");
        insert("Data7", "16", "node16");
        insert("Data7", "53", "node53");
        insert("Data7", "15", "node15");



        deleteNode("57");
        deleteNode("53");
        printTree(rootFile);

    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////B-TREE/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class BTreeNode 
{

public:
    int degree;
    int numKeys;
    bool isLeaf;
    String* keys;
    String* children; 
    String fileName;  
    String csvRow;
    

    BTreeNode(int deg, bool leaf) : degree(deg), isLeaf(leaf), numKeys(0), csvRow("")
    {
        keys = new String[2 * degree - 1];
        children = new String[2 * degree];
    }

    ~BTreeNode() 
    {
        delete[] keys;
        delete[] children;
    }

    void saveToFile(const String& fileName)
    {
        this->fileName = fileName;
        std::ofstream file(("BTree/" + fileName + ".node").c_str(), std::ios::binary);
        if (!file) {
            std::cerr << "Error: Could not open file " << fileName << " for writing." << std::endl;
            return;
        }

        file.write((char*)&degree, sizeof(degree));
        file.write((char*)&numKeys, sizeof(numKeys));
        file.write((char*)&isLeaf, sizeof(isLeaf));

        int fileNameLen = fileName.size();
        file.write((char*)&fileNameLen, sizeof(fileNameLen));
        file.write(fileName.c_str(), fileNameLen);

        int csvLen = csvRow.size();
        file.write((char*)&csvLen, sizeof(csvLen));
        file.write(csvRow.c_str(), csvLen);

        for (int i = 0; i < numKeys; ++i) {
            int keyLen = keys[i].size();
            file.write((char*)&keyLen, sizeof(keyLen));
            file.write(keys[i].c_str(), keyLen);
        }

        if (!isLeaf) {
            for (int i = 0; i <= numKeys; ++i) {
                int childLen = children[i].size();
                file.write((char*)&childLen, sizeof(childLen));
                file.write(children[i].c_str(), childLen);
            }
        }

        file.close();
    }

    static BTreeNode* loadFromFile(const String& fileName) {
        std::ifstream file(("BTree/" + fileName + ".node").c_str(), std::ios::binary);
        if (!file) {
            std::cerr << "Error: Could not open file " << fileName << " for reading." << std::endl;
            return nullptr;
        }

        int degree, numKeys;
        bool isLeaf;

        file.read((char*)&degree, sizeof(degree));
        file.read((char*)&numKeys, sizeof(numKeys));
        file.read((char*)&isLeaf, sizeof(isLeaf));

        int fileNameLen;
        file.read((char*)&fileNameLen, sizeof(fileNameLen));
        char* fileNameBuffer = new char[fileNameLen + 1];
        file.read(fileNameBuffer, fileNameLen);
        fileNameBuffer[fileNameLen] = '\0';
        String loadedFileName = fileNameBuffer;
        delete[] fileNameBuffer;

        int csvLen;
        file.read((char*)&csvLen, sizeof(csvLen));
        char* csvBuffer = new char[csvLen + 1];
        file.read(csvBuffer, csvLen);
        csvBuffer[csvLen] = '\0';
        String loadedCsvRow = csvBuffer;
        delete[] csvBuffer;

        BTreeNode* node = new BTreeNode(degree, isLeaf);
        node->fileName = loadedFileName;
        node->numKeys = numKeys;

        for (int i = 0; i < numKeys; ++i) {
            int keyLen;
            file.read((char*)&keyLen, sizeof(keyLen));
            char* keyBuffer = new char[keyLen + 1];
            file.read(keyBuffer, keyLen);
            keyBuffer[keyLen] = '\0';
            node->keys[i] = keyBuffer;
            delete[] keyBuffer;
        }

        if (!isLeaf) {
            for (int i = 0; i <= numKeys; ++i) {
                int childLen;
                file.read((char*)&childLen, sizeof(childLen));
                char* childBuffer = new char[childLen + 1];
                file.read(childBuffer, childLen);
                childBuffer[childLen] = '\0';
                node->children[i] = childBuffer;
                delete[] childBuffer;
            }
        }

        file.close();
        return node;
    }
};

class BTree {
private:
    String rootFile;
    String pathToRoot;
    int degree;

public:
    BTree(int deg, const String& path) : degree(deg), pathToRoot(path)
    {
        // Check if root file exists
        ifstream file(pathToRoot.data, ios::binary);
        if (!file.is_open())
        {
            cout << "Unable to open B-Tree root file. Creating new." << endl;
            ofstream outFile(pathToRoot.data, ios::binary);
            rootFile = "";
        }
        else
        {
            // Read root file name
            int len = 0;
            file.read((char*)&len, sizeof(len));
            String str(len + 1, 0);
            file.read((char*)str.data, len);
            rootFile = str;
        }
        file.close();
    }

    void insert(const String& data, const String& key, const String& fileName) {
        if (rootFile.empty()) {
            rootFile = "root";
            BTreeNode* root = new BTreeNode(degree, true);
            root->keys[0] = key;
            root->numKeys = 1;
            root->saveToFile(rootFile);
            delete root;
        }
        else {
            BTreeNode* root = BTreeNode::loadFromFile(rootFile);
            if (!root) {
                std::cerr << "Error: Could not load root node from file." << std::endl;
                return;
            }

            if (root->numKeys == 2 * degree - 1) {
                String newRootFile = root->keys[root->numKeys / 2];
                BTreeNode* newRoot = new BTreeNode(degree, false);
                newRoot->children[0] = rootFile;
                newRoot->saveToFile(newRootFile);
                splitChild(rootFile, newRootFile, 0);

                int i = 0;
                if (newRoot->keys[0] < key) {
                    i++;
                }
                insertNonFull(newRootFile, data, key);

                newRoot->saveToFile(newRootFile);
                rootFile = newRootFile;
                delete newRoot;
            }
            else {
                insertNonFull(rootFile, data, key);
            }
            delete root;
        }

        BTreeNode* targetNode = new BTreeNode(degree, true);
        targetNode->keys[0] = data;
        targetNode->numKeys = 1;
        //targetNode->saveToFile(fileName);
        delete targetNode;
    }

    void traverse() {
        if (!rootFile.empty()) {
            traverse(rootFile);
        }
    }

    void splitChild(const String& root, const String& s, int index) {
        BTreeNode* parent = BTreeNode::loadFromFile(root);
        if (!parent) {
            std::cerr << "Error: Could not load parent node from file." << std::endl;
            return;
        }

        BTreeNode* fullChild = BTreeNode::loadFromFile(s);
        if (!fullChild) {
            std::cerr << "Error: Could not load full child node from file." << std::endl;
            delete parent;
            return;
        }

        String newChildFile = "Node3"/* + String::toString(rand())*/;
        BTreeNode* newChild = new BTreeNode(degree, parent->isLeaf);
        newChild->numKeys = degree - 1;

        for (int j = 0; j < degree - 1; j++) {
            newChild->keys[j] = parent->keys[j + degree];
        }

        if (!parent->isLeaf) {
            for (int j = 0; j < degree; j++) {
                newChild->children[j] = parent->children[j + degree];
            }
        }

        parent->numKeys = degree - 1;
        newChild->saveToFile(newChildFile);

        for (int j = parent->numKeys; j >= index + 1; j--) {
            fullChild->children[j + 1] = fullChild->children[j];
        }
        fullChild->children[index + 1] = newChildFile;

        for (int j = parent->numKeys - 1; j >= index; j--) {
            fullChild->keys[j + 1] = fullChild->keys[j];
        }
        fullChild->keys[index] = parent->keys[degree - 1];

        fullChild->numKeys++;
        parent->saveToFile(root);
        fullChild->saveToFile(s);

        delete parent;
        delete fullChild;
        delete newChild;
    }
    void saveTree(const String& csvFileName) 
    {
        if (rootFile.empty()) {
            std::cerr << "Error: B-Tree is empty, nothing to save.\n";
            return;
        }

        std::ofstream treeFile(("BTree/" + rootFile + ".tree").c_str(), std::ios::binary);
        std::ofstream csvFile(("BTree/" + csvFileName + ".csv").c_str());
        if (!treeFile || !csvFile) {
            std::cerr << "Error: Could not open files for writing.\n";
            return;
        }

        saveTreeRecursive(rootFile, treeFile, csvFile);

        treeFile.close();
        csvFile.close();
        std::cout << "B-Tree structure saved to " << rootFile << ".tree\n";
        std::cout << "CSV data saved to " << csvFileName << ".csv\n";
    }

    void saveTreeRecursive(const String& nodeFile, std::ofstream& treeFile, std::ofstream& csvFile) {
        BTreeNode* node = BTreeNode::loadFromFile(nodeFile);
        if (!node) {
            std::cerr << "Error: Could not load node " << nodeFile << " from file.\n";
            return;
        }

        // Save the node's metadata (structure) to the tree file
        treeFile.write((char*)&node->degree, sizeof(node->degree));
        treeFile.write((char*)&node->numKeys, sizeof(node->numKeys));
        treeFile.write((char*)&node->isLeaf, sizeof(node->isLeaf));

        int fileNameLen = node->fileName.size();
        treeFile.write((char*)&fileNameLen, sizeof(fileNameLen));
        treeFile.write(node->fileName.c_str(), fileNameLen);

        // Save the keys to the tree file
        for (int i = 0; i < node->numKeys; ++i) {
            int keyLen = node->keys[i].size();
            treeFile.write((char*)&keyLen, sizeof(keyLen));
            treeFile.write(node->keys[i].c_str(), keyLen);

            // Save corresponding CSV row
            csvFile << node->keys[i] << "," << node->csvRow << "\n";
        }

        // Save the child file names if the node is not a leaf
        if (!node->isLeaf) {
            for (int i = 0; i <= node->numKeys; ++i) {
                int childLen = node->children[i].size();
                treeFile.write((char*)&childLen, sizeof(childLen));
                treeFile.write(node->children[i].c_str(), childLen);
            }

            // Recursively save all children
            for (int i = 0; i <= node->numKeys; ++i) {
                saveTreeRecursive(node->children[i], treeFile, csvFile);
            }
        }

        delete node;
    }
    void insertNonFull(const String& nodeFile, const String& data, const String& key) {
        BTreeNode* node = BTreeNode::loadFromFile(nodeFile);
        if (!node) {
            std::cerr << "Error: Could not load node from file." << std::endl;
            return;
        }

        int i = node->numKeys - 1;

        if (node->isLeaf) {
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }

            node->keys[i + 1] = key;
            node->numKeys++;
            node->saveToFile(nodeFile);
        }
        else {
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;

            if (BTreeNode::loadFromFile(node->children[i])->numKeys == 2 * degree - 1) {
                splitChild(rootFile, nodeFile, i);
                if (key > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], data, key);
        }
        delete node;
    }

    void traverse(const String& nodeFile) {
        BTreeNode* node = BTreeNode::loadFromFile(nodeFile);
        if (!node) {
            return;
        }

        for (int i = 0; i < node->numKeys; i++) {
            if (!node->isLeaf) {
                traverse(node->children[i]);
            }
            std::cout << node->keys[i] << " ";
        }

        if (!node->isLeaf) {
            traverse(node->children[node->numKeys]);
        }
        delete node;
    }

    void getBlocks(Vector<String>& blocks) {
        getBlocks(rootFile, blocks);
    }

    void getBlocks(const String& nodeFile, Vector<String>& blocks) {
        if (nodeFile.empty()) {
            return;
        }

        BTreeNode* node = BTreeNode::loadFromFile(nodeFile);
        if (!node) {
            return;
        }

        blocks.push_back(node->csvRow);

        for (int i = 0; i < node->numKeys; i++) {
            if (!node->isLeaf) {
                getBlocks(node->children[i], blocks);
            }
            blocks.push_back(node->keys[i]);
        }

        if (!node->isLeaf) {
            getBlocks(node->children[node->numKeys], blocks);
        }

        delete node;
    }



    void update(const String& key, const String& newRow)
    {
        update(rootFile, key, newRow);
    }

    void update(const String& rootFile, const String& key, const String& newRow)
    {
        BTreeNode* node = nullptr;
        if (!rootFile.empty()) {
            node = BTreeNode::loadFromFile(rootFile);
        }

        if (!node) {
            return;
        }

        int i = 0;

        // Find the index of the key or the child to traverse
        while (i < node->numKeys && key > node->keys[i]) {
            i++;
        }

        if (i < node->numKeys && node->keys[i] == key) {
            // Key found; update the corresponding CSV data
            node->csvRow = newRow;
            node->saveToFile(rootFile);
        }
        else if (!node->isLeaf) {
            // Key not found; recurse on the appropriate child
            update(node->children[i], key, newRow);
        }

        delete node;
    }


};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// ///MERKLE TREE//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ComputeSHA256(const String& input, String& outputHash)
{
    HCRYPTPROV hCryptProv = NULL;
    HCRYPTHASH hHash = NULL;
    unsigned char hash[32];
    DWORD hashSize = sizeof(hash);

    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) 
    {
        cout << "Error: Unable to acquire cryptographic context." << std::endl;
        return false;
    }

    if (!CryptCreateHash(hCryptProv, CALG_SHA_256, 0, 0, &hHash)) {
        cout << "Error: Unable to create hash object." << std::endl;
        CryptReleaseContext(hCryptProv, 0);
        return false;
    }

    if (!CryptHashData(hHash, reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), 0)) {
        cout << "Error: Unable to hash data." << std::endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hCryptProv, 0);
        return false;
    }

    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashSize, 0)) {
        cout << "Error: Unable to retrieve hash value." << std::endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hCryptProv, 0);
        return false;
    }

    char hexHash[65];
    for (DWORD i = 0; i < hashSize; i++) {
        sprintf_s(hexHash + (i * 2), 3, "%02x", hash[i]);
    }
    outputHash = hexHash;

    // Cleanup
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    return true;
}

void myHash(String input, String& output)
{
    int product = 1;
    for (int i = 0; i < input.size() && input[i] != 0; i++)
    {
        product *= input[i];
        product %= 29;
    }
    output = String::toString(product);
}


struct Node
{
    String hash;
    Node* left = nullptr;
    Node* right = nullptr;

    Node(const String& h) : hash(h) {}
    ~Node() {
        delete left;
        delete right;
    }
};

class MerkleTree
{
public:

    Node* root;
private:

    Node* buildTree(const Vector<String>& hashes, int start, int end)
    {
        if (start == end) {
            return new Node(hashes[start]);
        }
        int mid = (start + end) / 2;
        Node* leftChild = buildTree(hashes, start, mid);
        Node* rightChild = buildTree(hashes, mid + 1, end);
        String combinedHash;
        if (hashType)
        {
            combinedHash = String(65, 0);
            ComputeSHA256(leftChild->hash + rightChild->hash, combinedHash);
        }
        else
        {
            combinedHash = String(3, 0);
            myHash(leftChild->hash + rightChild->hash, combinedHash);
        }

        Node* node = new Node(combinedHash);
        node->left = leftChild;
        node->right = rightChild;
        return node;
    }
    bool hashType;
public:
    MerkleTree(const Vector<String>& blocks, bool hashType) :hashType(hashType)
    {
        Vector<String> hashes;
        for (int i = 0; i < blocks.size(); i++)
        {
            if (hashType)
            {
                String hash(65, 0);
                ComputeSHA256(blocks[i], hash);
                hashes.push_back(hash);
            }
            else
            {
                String hash(3, 0);
                myHash(blocks[i], hash);
                hashes.push_back(hash);
            }
        }
        root = buildTree(hashes, 0, hashes.size() - 1);
    }

    ~MerkleTree() {
        delete root;
    }

    String getRootHash() const {
        return root ? root->hash : "";
    }
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////REPOSITORY///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Repository
{

private:
    String repo;
    String currentBranch;
    Vector<String> branches;
    String treeType;
    bool useSHA256;
    String currentTree;
    int degree;

    void saveBranches()
    {
        ofstream outFile((repo + "/branches.txt").c_str());
        if (!outFile)
        {
            cout << "Unable to save branches";
            exit(0);
        }
        for (int i = 0; i < branches.size(); i++)
        {
            outFile << branches[i] << '\n';
        }
        outFile.close();
    }

    void loadBranches()
    {
        ifstream inFile((repo + "/branches.txt").c_str());
        if (!inFile)
            return;

        branches.clear();
        String branchName(200, 0);
        while (inFile.getline(branchName.data, 200))
        {
            branches.push_back(branchName.data);
        }
        inFile.close();
    }

    void saveCurrentBranch()
    {
        ofstream branchFile((repo + "/current_branch.txt").c_str());
        if (!branchFile)
        {
            cout << "Unable to save the current branch";
            exit(0);
        }
        branchFile << currentBranch;
        branchFile.close();
    }

    void loadCurrentBranch()
    {
        ifstream branchFile((repo + "/current_branch.txt").c_str());
        if (branchFile)
        {
            String temp(100, 0);
            branchFile.getline(temp.data, 100);
            currentBranch = temp.data;
            branchFile.close();
        }
        else
        {
            currentBranch = "main";
        }
    }

    void initializeCurrentTree()
    {
        currentTree = repo + "/branches/" + currentBranch;
        if (!exists(currentTree.c_str()))
        {
            create_directories(currentTree.c_str());
        }
    }

    void saveSettings()
    {
        ofstream settingsFile((repo + "/settings.txt").c_str());
        if (!settingsFile)
        {
            cout << "Unable to save settings";
            exit(0);
        }
        settingsFile << treeType << '\n';
        settingsFile << useSHA256 << '\n';
        settingsFile.close();
    }

    void loadSettings()
    {
        ifstream settingsFile((repo + "/settings.txt").c_str());
        if (settingsFile)
        {
            String temp(100, 0);
            settingsFile.getline(temp.data, 100);
            treeType = temp.data;

            settingsFile.getline(temp.data, 100);
            useSHA256 = (temp == "1");
            settingsFile.close();
        }
        else
        {
            treeType = "AVL";
            useSHA256 = true;
        }
    }

public:
    Repository(const String directory = "", String tree = "AVL", bool useSHA256 = true, int degree = 0)
        : repo(directory), treeType(tree), useSHA256(useSHA256), degree(degree)
    {
        if (!exists(directory.c_str()))
        {
            create_directories((repo).c_str());
            saveSettings();
        }
        else
        {
            loadSettings();
            loadBranches();
            loadCurrentBranch();
            initializeCurrentTree();
        }
    }

    void displayBranches()
    {
        loadBranches();
        for (int i = 0; i < branches.size(); i++)
        {
            cout << "BRANCH " << i + 1 << " : " << branches[i] << endl;
        }
    }


    void deleteBranch(const String& branchName)
    {

        String path = repo + "/branches/" + branchName;

        if (!exists(path.data))
        {
            cout << "Branch does not exist.\n";
            return;
        }

        if (branchName == currentBranch)
        {
            cout << "Cannot delete the current branch\n";
            return;
        }

        if (branchName == "main")
        {
            cout << "Cannot delete the main branch\n";
            return;
        }

        if (remove_all(path.data))
        {
            cout << branchName << " deleted successfully\n";

            for (int i = 0; i < branches.size(); i++)
            {
                if (branches[i] == branchName)
                {
                    branches.erase(i);
                    break;
                }
            }

            saveBranches();
        }
        else {
            cout << "Failed to delete the branch.\n";
        }
    }
    void initializeRepository(String csvPath)
    {
        if (!exists(repo.c_str()))
        {
            create_directories((repo + "/branches").c_str());
        }

        String main = repo + "/branches/main";
        if (!exists(main.c_str()))
        {
            create_directories(main.c_str());
            create_directories((main + "/tree").c_str()); // Create tree directory
            ofstream rootFile((main + "/tree/root.txt").c_str()); // Create root.txt
            rootFile.close();

            ofstream logs((main + "/main_log.txt").c_str());
            logs.close();
        }

        currentBranch = "main";
        branches.push_back("main");
        saveBranches();
        saveCurrentBranch();
        initializeCurrentTree();


        load(csvPath);
    }

    void createBranch(const String& branchName)
    {
        String baseBranch = repo + "/branches/" + currentBranch;
        String newBranch = repo + "/branches/" + branchName;

        if (exists(newBranch.data))
        {
            cout << "Branch already exists";
            return;
        }

        create_directories(newBranch.data);
        create_directories((newBranch + "/tree").c_str()); // Create tree directory
        ofstream rootFile((newBranch + "/tree/root.txt").c_str()); // Create root.txt
        rootFile << "null"; // Initially, no root node
        rootFile.close();

        // Copy log file
        copy((baseBranch + "/" + currentBranch + "_log.txt").c_str(),
            (newBranch + "/" + branchName + "_log.txt").c_str());

        branches.push_back(branchName);
        saveBranches();
    }

    void displayLog(const String& branchName)
    {
        String branchLogPath = repo + "/branches/" + branchName + "/" + branchName + "_log.txt";

        if (!exists(branchLogPath.c_str()))
        {
            cout << "No commit history available for branch: " << branchName << '\n';
            return;
        }

        ifstream logFile(branchLogPath.c_str());
        if (!logFile)
        {
            cout << "Unable to read the log file for branch: " << branchName << '\n';
            return;
        }

        String logLine(500, 0); 
        bool hasCommits = false;

        cout << "Commit History for Branch: " << branchName << '\n';
        cout << "----------------------------------------\n";

        while (logFile.getline(logLine.data, 500))
        {
            cout << logLine.data << '\n';
            hasCommits = true;
        }

        logFile.close();

        if (!hasCommits)
        {
            cout << "No commit history available for branch: " << branchName << '\n';
        }
    }

    

    void checkout(const String& branchName)
    {
        bool branchExists = false;
        for (int i = 0; i < branches.size(); i++)
        {
            if (branches[i] == branchName)
            {
                branchExists = true;
                break;
            }
        }

        if (!branchExists)
        {
            cout << "Branch does not exist";
            return;
        }

        currentBranch = branchName;
        saveCurrentBranch();
        initializeCurrentTree();
    }

    String getCurrentBranch() const
    {
        return currentBranch;
    }

    Vector<String> listBranches() const
    {
        return branches;
    }

    void status() const
    {
        cout << "Repository Directory: " << repo << '\n';
        cout << "Current Branch: " << currentBranch << '\n';
        cout << "Tree Type: " << treeType << '\n';
        cout << "Using SHA256: " << (useSHA256 ? "Yes" : "No") << '\n';
        cout << "Branches:\n";
        for (int i = 0; i < branches.size(); i++)
        {
            cout << "- " << branches[i] << '\n';
        }
    }
    void displayCurrentBranch()
    {
        loadCurrentBranch();
        cout << currentBranch;
    }

    void insert(const String& key, const String& csvRow, int csvRowNumber)
    {
        String treeFile = currentTree + "/" + currentBranch + "_tree.dat";
        ofstream outFile(treeFile.c_str(), ios::app);
        if (!outFile)
        {
            cout << "Unable to open tree file";
            return;
        }
        outFile << key << ',' << csvRow << ',' << csvRowNumber << '\n';
        outFile.close();
    }

    void commit(const String& commitMessage)
    {
        String branchPath = repo + "/branches/" + currentBranch;
        String treePath = branchPath + "/tree";
        String commitFolderPath = branchPath + "/commits";

        if (!exists(commitFolderPath.c_str()))
        {
            create_directories(commitFolderPath.c_str());
        }

        Vector<String> blocks;
        AVL avl(treePath + "/root.txt");
        avl.getBlocks(blocks);
        MerkleTree sourceTree(blocks, useSHA256);
        String sourceMerkleRoot = sourceTree.getRootHash();

        String commitPath = commitFolderPath + "/commit_" + sourceMerkleRoot;
        if (!exists(commitPath.c_str()))
        {
            create_directories(commitPath.c_str());

            for (const auto& entry : directory_iterator(treePath.c_str()))
            {
                String destPath = (commitPath + "/" + entry.path().filename().string().c_str());
                copy(entry.path().c_str(), destPath.c_str());
            }
        }
        else
        {
            cout << "Commit folder already exists for Merkle root: " << sourceMerkleRoot << '\n';
            return;
        }

        Vector<String> commitBlocks;
        AVL destAvl(commitPath + "/root.txt");
        destAvl.getBlocks(commitBlocks);
        MerkleTree destTree(commitBlocks, useSHA256);
        String destMerkleRoot = destTree.getRootHash();

        if (sourceMerkleRoot != destMerkleRoot)
        {
            cout << "Corruption detected. Resolving discrepancies...\n";

        }

        // Step 6: Append the commit details to the log file
        String branchLogPath = branchPath + "/" + currentBranch + "_log.txt";
        ofstream logFile(branchLogPath.c_str(), ios::app);
        if (!logFile)
        {
            cout << "Failed to update the branch log. Commit aborted.\n";
            return;
        }
        time_t now = time(0);
        logFile << "Commit: " << now << ", Message: " << commitMessage
            << ", Merkle Root: " << sourceMerkleRoot
            << ", Branch: " << currentBranch << '\n';
        logFile.close();

        cout << "Commit successful. Merkle root saved: " << sourceMerkleRoot << '\n';
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////



    void visualizeTree(const String& branchName)
    {
        if (treeType == "AVL" || treeType == "avl")
        {
            String path = repo + "/branches/" + branchName + "/tree/root.txt";
            AVL avl(path);
            avl.printTree();
        }
        if (treeType == "btree" || treeType == "BTREE")
        {
            String path = repo + "/branches/" + branchName + "/tree/root.txt";
            BTree btree(degree, path);
            btree.traverse();
        }
        else if (treeType == "redblack" || treeType == "RedBlack")
        {
            String path = repo + "/branches/" + branchName + "/tree/root.txt";
            RBTree rbTree(path);
            rbTree.printTree(); // Assuming printTree is the method for visualization
        }

    }
    void load(const String& csvPath)
    {

        String treePath = repo + "/branches/" + currentBranch + "/tree";
        for (const auto& entry : directory_iterator(treePath.c_str()))
        {
            remove_all(entry.path());
        }

        ofstream rootFile((treePath + "/root.txt").c_str());
        rootFile.close();

        ifstream csvFile(csvPath.c_str());
        if (!csvFile)
        {
            cout << "Unable to open CSV file";
            return;
        }

        String temp(100, 0);
        csvFile.getline(temp.data, 100);
        String line = temp.data;

        Vector<String> columns;
        temp = String(100, 0);
        int index = 0;
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == ',')
            {
                columns.push_back(temp.data);
                index = 0;
                temp = String(100, 0);
                continue;
            }

            temp[index++] = line[i];
        }
        columns.push_back(temp.data);

        cout << "\nSelect a column \n";
        for (int i = 0; i < columns.size(); i++)
        {
            cout << i << ": " << columns[i] << '\n';
        }

        int selectedCol;
        cin >> selectedCol;

        if (selectedCol < 0 || selectedCol >= columns.size())
        {
            cout << "Invalid column selected";
            return;
        }

        cout << "You selected column: " << columns[selectedCol] << '\n';

        if (treeType == "AVL" || treeType == "avl")
        {
            AVL avl((treePath + "/root.txt").c_str());
            temp = String(100, 0);
            int csvRowNum = 0;
            while (csvFile.getline(temp.data, 100))
            {
                csvRowNum++;
                String line = temp.data;
                temp = String(100, 0);


                int index = 0;
                int count = 0;
                for (int i = 0; i < line.size(); i++)
                {
                    if (count == selectedCol)
                    {
                        if (line[i] == ',')
                        {
                            temp[index] = 0;
                            break;
                        }
                        temp[index++] = line[i];
                    }
                    if (line[i] == ',')
                    {
                        count++;
                    }
                }

                avl.insert(line, temp.data, generateUniquePath(treePath + "/file"));
            }

        }
        if (treeType == "BTREE" || treeType == "btree")
        {
            BTree btree(degree, (treePath + "/root.txt").c_str());
            temp = String(100, 0);
            int csvRowNum = 0;
            while (csvFile.getline(temp.data, 100))
            {
                csvRowNum++;
                String line = temp.data;
                temp = String(100, 0);


                int index = 0;
                int count = 0;
                for (int i = 0; i < line.size(); i++)
                {
                    if (count == selectedCol)
                    {
                        if (line[i] == ',')
                        {
                            temp[index] = 0;
                            break;
                        }
                        temp[index++] = line[i];
                    }
                    if (line[i] == ',')
                    {
                        count++;
                    }
                }

                btree.insert(line, temp.data, generateUniquePath(treePath + "/file"));
            }
        }
        else if (treeType == "RedBlack" || treeType == "redblack")
        {
            RBTree rbTree((treePath + "/root.txt").c_str());
            temp = String(100, 0);
            int csvRowNum = 0;
            while (csvFile.getline(temp.data, 100))
            {
                csvRowNum++;
                String line = temp.data;
                temp = String(100, 0);

                int index = 0;
                int count = 0;
                for (int i = 0; i < line.size(); i++)
                {
                    if (count == selectedCol)
                    {
                        if (line[i] == ',')
                        {
                            temp[index] = 0;
                            break;
                        }
                        temp[index++] = line[i];
                    }
                    if (line[i] == ',')
                    {
                        count++;
                    }
                }

                rbTree.insert(line, temp.data, generateUniquePath(treePath + "/file"));
            }
        }


    }

    void save(const String& FileName)
    {
        if (treeType == "avl" || treeType == "AVL")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            AVL avl(Path);
            avl.save(FileName);

        }
        if (treeType == "btree" || treeType == "BTREE")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            BTree btree(degree, Path);
            btree.saveTree(FileName);



        }
    }

    void update(String key, String newRow)
    {
        if (treeType == "avl" || treeType == "AVL")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            AVL avl(Path);
            avl.update(key, newRow);

        }

        if (treeType == "btree" || treeType == "BTREE")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            BTree btree(degree, Path);
            btree.update(key, newRow);

        }
        else if (treeType == "redblack" || treeType == "RedBlack")
        {
            // Handle Red-Black tree update
            String path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            RBTree rbTree(path);
            rbTree.update(key, newRow);
        }
    }
    void add(String key, String newRow)
    {
        if (treeType == "avl" || treeType == "AVL")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            AVL avl(Path);
            avl.insert(newRow, key, generateUniquePath(repo + "/branches/" + currentBranch + "/tree/file"));

        }
        if (treeType == "btree" || treeType == "BTREE")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            BTree btree(degree, Path);
            btree.insert(newRow, key, generateUniquePath(repo + "/branches/" + currentBranch + "/tree/file"));

        }
        else if (treeType == "redblack" || treeType == "RED_BLACK" || treeType == "RedBlack")
        {
            String path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            RBTree rbTree(path);
            rbTree.insert(newRow, key, generateUniquePath(repo + "/branches/" + currentBranch + "/tree/file"));
        }
    }
    void del(String key)
    {
        if (treeType == "avl" || treeType == "AVL")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            AVL avl(Path);
            avl.deleteNode(key);
        }
    }

    String generateUniquePath(const String& path)
    {
        srand(time(0));
        while (true)
        {
            String newP = path + String::toString(rand() % 100000);
            ifstream file((newP + ".node").data);
            if (!file)
            {
                return newP;
            }
            file.close();
        }
    }
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////DRIVER///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    cout << "ENTER THE PATH OF REPOSITORY : ";
    String path;
    cin >> path;
    cout << "ENTER TREE TYPE (AVL,BTREE,RBTREE) : ";
    String tree;
    cin >> tree;
    cout << "USE SHA-256 hash function ? (1/0) : ";
    bool useSHA256;
    cin >> useSHA256;

    int degree = 0;
    if (tree == "BTREE" || tree == "btree")
    {
        cout << "Enter the minimum degree of BTree: ";
        cin >> degree;
    }

    Repository repo(path, tree, useSHA256);
    bool init = false;

    String command;
    while (true) {
        cout << "> ";
        cin >> command;


        if (command == "commands")
        {
            cout << "init\nbranch\ncheckout\ncommit\nbranches\ndelete-branch\nmerge-tree\nvisualize-tree\nlog\ncurrent-branch\nload\nsave\nexit\n";
            cout << "\nupdate\nadd\ndelete\n";
        }
        else if (command == "update")
        {
            cout << "Enter Key for updation : ";
            String key;
            cin >> key;

            String newRow;
            cout << "Enter updated data(CSV ROW)";
            cin >> newRow;

            repo.update(key, newRow);
        }
        else if (command == "add")
        {
            String key;
            cout << "Enter key : ";
            cin >> key;
            String row;
            cout << "Enter row : ";
            cin >> row;
            repo.add(key, row);
        }
        else if (command == "del")
        {
            String key;
            cout << "Enter key : ";
            cin >> key;

            repo.del(key);
        }
        else if (command == "init")
        {
            String temp;
            cout << "Enter CSV File Path : ";
            cin >> temp;
            repo.initializeRepository(temp);
            init = true;
        }
        else if (command == "branch")
        {
            String branchName;
            cin >> branchName;
            repo.createBranch(branchName);
        }
        else if (command == "checkout") {
            String branchName;
            cout << "Enter Branch Name : ";
            cin >> branchName;
            repo.checkout(branchName);
        }
        else if (command == "commit")
        {
            String message;
            cout << "Enter commit message : ";
            cin >> message;
            repo.commit(message);
        }
        else if (command == "branches")
        {
            repo.displayBranches();
        }
        else if (command == "delete-branch") {
            String branchName;
            cin >> branchName;
            repo.deleteBranch(branchName);
        }
        else if (command == "merge")
        {
            String source, target;
            cout << "Source : ";
            cin >> source;
            cout << "Target : ";
            cin >> target;
            // merge(source, target);
        }
        else if (command == "visualize-tree")
        {
            String branchName;
            cout << "Enter branch name : ";
            cin >> branchName;
            repo.visualizeTree(branchName);
        }
        else if (command == "log")
        {
            //displayLog();
        }
        else if (command == "current-branch")
        {
            repo.displayCurrentBranch();
            cout << endl;
        }
        else if (command == "save")
        {
            cout << "Enter path to save the file : ";
            String path;
            cin >> path;
            repo.save(path);
        }
        else if (command == "load")
        {
            String fileName;
            cin >> fileName;
            repo.load(fileName);
        }
        else if (command == "exit")
            break;
        else if (command != "")
            cout << "INVALID command.\n";

    }


    //if (input.startsWith("UPDATE"))
    //{
    //    // Handle UPDATE commands
    //    String temp(100, 0);
    //    cin.getline(temp.data, 100); // Read the rest of the SQL command
    //    String command = temp.data;

    //    if (command.contains("BETWEEN"))
    //    {
    //        // Example: UPDATE Students SET Status = 'Promoted' WHERE RollNumber BETWEEN '21i-0066' AND '21i-0226';
    //        String tableName, columnName, value;
    //        String rangeStart, rangeEnd;
    //        sscanf_s(command.c_str(), "%s SET %[^ ] = '%[^']' WHERE %[^ ] BETWEEN '%[^']' AND '%[^']'",
    //            tableName.data, columnName.data, value.data, columnName.data, rangeStart.data, rangeEnd.data);

    //        cout << "Updating " << columnName << " to " << value << " in " << tableName << " for range " << rangeStart << " to " << rangeEnd << '\n';

    //        // Call the tree's update function
    //        // e.g., repo.updateRange(tableName, columnName, value, rangeStart, rangeEnd);
    //    }
    //}
    //else if (input.startsWith("DELETE"))
    //{

    //    String temp(100, 0);
    //    cin.getline(temp.data, 100); 
    //    String command = temp.data;

    //    if (command.contains("BETWEEN"))
    //    {
    //        // Example: DELETE FROM Students WHERE RollNumber BETWEEN '21i-0066' AND '21i-0226';
    //        String tableName, columnName, rangeStart, rangeEnd;
    //        sscanf_s(command.c_str(), "FROM %s WHERE %[^ ] BETWEEN '%[^']' AND '%[^']'",
    //            tableName.data, columnName.data, rangeStart.data, rangeEnd.data);

    //        cout << "Deleting records from " << tableName << " where " << columnName << " is between " << rangeStart << " and " << rangeEnd << '\n';

    //        // Call the tree's delete range function
    //        // e.g., repo.deleteRange(tableName, columnName, rangeStart, rangeEnd);
    //    }
    //    else if (command.contains("LIMIT"))
    //    {
    //        // Example: DELETE FROM Students WHERE Name = 'Owais' LIMIT 1;
    //        String tableName, columnName, value;
    //        sscanf_s(command.c_str(), "FROM %s WHERE %[^ ] = '%[^']' LIMIT 1",
    //            tableName.data, columnName.data, value.data);

    //        cout << "Deleting one record from " << tableName << " where " << columnName << " = " << value << '\n';

    //        // Call the tree's delete single record function
    //        // e.g., repo.deleteSingle(tableName, columnName, value);
    //    }
    //}
    //else
    //{
    //    cout << "Unrecognized command: " << input << '\n';
    //}

    return 0;
}
