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

};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////RBTREE////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////B-TREE/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
    Repository(const String directory = "", String tree = "AVL", bool useSHA256 = true)
        : repo(directory), treeType(tree), useSHA256(useSHA256)
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
        //check if commit exist, is this first commit?


        //do this

        //make merkel tree of last commit and current tree , wheather there are changes or not?

        String root;
        String path = repo + "/branches/" + currentBranch + "/tree/";
        {
            AVL avl(path + "root.txt");
            root = avl.rootFile;
        }
        if (treeType == "avl" || treeType == "AVL")
        {
            String path = repo + "/branches/" + currentBranch + "/tree/";
            AVL avl(path + "root.txt");
            Vector<String> blocks;
            avl.getBlocks(blocks);
            MerkleTree tree(blocks, useSHA256);
            String rootHash = tree.getRootHash();
        }




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

    }
    void update(String key, String newRow)
    {
        if (treeType == "avl" || treeType == "AVL")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            AVL avl(Path);
            avl.update(key, newRow);

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

    Repository repo(path, tree, useSHA256);
    bool init = false;

    String command;
    while (true) {
        cout << "> ";
        cin >> command;


        if (command == "commands")
        {
            cout << "init\nbranch\ncheckout\ncommit\nbranches\ndelete-branch\nmerge-tree\nvisualize-tree\n\log\current-branch\nload\nsave\nexit\n";
            cout << "\nupdate\nadd,delete\n";
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

            //repo.del(key, row);
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
           // repo.commit(message);
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
            //save();
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
