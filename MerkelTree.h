#pragma once
#include <windows.h>
#include <wincrypt.h>
#include "String.h"
#include "Vector.h"
#include <iostream>

bool ComputeSHA256(const String& input, String& outputHash) {
    HCRYPTPROV hCryptProv = NULL;
    HCRYPTHASH hHash = NULL;
    unsigned char hash[32]; 
    DWORD hashSize = sizeof(hash);

    // Acquire a cryptographic provider context
    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        std::cerr << "Error: Unable to acquire cryptographic context." << std::endl;
        return false;
    }

    // Create a hash object
    if (!CryptCreateHash(hCryptProv, CALG_SHA_256, 0, 0, &hHash)) {
        std::cerr << "Error: Unable to create hash object." << std::endl;
        CryptReleaseContext(hCryptProv, 0);
        return false;
    }

     //Hash the input data
    if (!CryptHashData(hHash, reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), 0)) {
        std::cerr << "Error: Unable to hash data." << std::endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hCryptProv, 0);
        return false;
    }

    // Retrieve the hash value
    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashSize, 0)) {
        std::cerr << "Error: Unable to retrieve hash value." << std::endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hCryptProv, 0);
        return false;
    }

    // Convert the hash to a hexadecimal String
    char hexHash[65]; // 64 characters for hash + 1 for null terminator
    for (DWORD i = 0; i < hashSize; i++) {
        sprintf_s(hexHash + (i * 2), 3, "%02x", hash[i]);
    }
    outputHash = hexHash;

    // Cleanup
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    return true;
}

void myHash (String input, String& output)
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
    MerkleTree(const Vector<String>& blocks, bool hashType):hashType(hashType)
    {
        Vector<String> hashes;
        for (int i = 0; i < blocks.size();i++)
        {
            if (hashType)
            {
                String hash(65,0);
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




