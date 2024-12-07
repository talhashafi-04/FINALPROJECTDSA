#pragma once
#include <iostream>


class String {
public:

    char* data;
    long long unsigned int length;

    long long unsigned int calculateLength(const char* str) const 
    {
        long long unsigned int len = 0;
        while (str[len] != '\0') ++len;
        return len;
    }

    void copyData(const char* str) {
        length = calculateLength(str);
        data = new char[length + 1];
        for (long long unsigned int i = 0; i < length; ++i) {
            data[i] = str[i];
        }
        data[length] = '\0';
    }

    bool isSpace(char c) const {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
    }

    char toLower(char c) const {
        return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
    }

    char toUpper(char c) const {
        return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
    }

public:
    // Default Constructor
    String() : data(new char[1] {'\0'}), length(0) {}

    
    // Parameterized Constructor
    String(const char* str) {
        copyData(str);
    }
    String(int len, char fillChar) : length(len) {
        data = new char[length];
        memset(data, 0, length);
    }

    // Copy Constructor
    String(const String& other) {
        copyData(other.data);
    }

    // Move Constructor
    String(String&& other) noexcept : data(other.data), length(other.length) {
        other.data = nullptr;
        other.length = 0;
    }

    // Destructor
    ~String() {
        delete[] data;
    }

    bool empty() const {
        return length == 0;
    }

    static String toString(int i)
    {
        String STR;
        while (i)
        {
            STR = (char)('0'+ (i % 10)) + STR;
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
                return (*str1 < *str2) ? -1 : 1; // Return based on comparison
            }
            ++str1;
            ++str2;
        }

        // If one string ends before the other
        if (*str1) return 1;   // `str1` is longer
        if (*str2) return -1;  // `str2` is longer
        return 0;              // Strings are equal
    }


    // Custom strcmp-like function
    int compare(const String& other) const 
    {
        return custom_strcmp (this->c_str(), other.c_str());
    }

    // Getter for raw data
    const char* c_str() const 
    {
        return data ? data : "";
    }

    // Operator <
    bool operator<(const String& other) const {
        return compare(other) < 0;
    }

    // Operator >
    bool operator>(const String& other) const {
        return compare(other) > 0;
    }

    // Assignment Operator
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            copyData(other.data);
        }
        return *this;
    }

    // Move Assignment Operator
    String& operator=(String&& other) noexcept {
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
    String operator+(const String& other) const {
        char* newData = new char[length + other.length + 1];
        for (long long unsigned int i = 0; i < length; ++i) {
            newData[i] = data[i];
        }
        for (long long unsigned int i = 0; i < other.length; ++i) {
            newData[length + i] = other.data[i];
        }
        newData[length + other.length] = '\0';
        String result(newData);
        delete[] newData;
        return result;
    }

    String& operator+=(const String& other) {
        *this = *this + other;
        return *this;
    }

    // Equality
    bool operator==(const String& other) const {
        if (length != other.length) return false;
        for (long long unsigned int i = 0; i < length; ++i) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    // Indexing
    char& operator[](long long unsigned int index) {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }

    const char& operator[](long long unsigned int index) const {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }

    // Get Length
    long long unsigned int size() const {
        return length;
    }

    // Substring
    String substring(long long unsigned int start, long long unsigned int len) const {
        if (start >= length) throw std::out_of_range("Start index out of range");
        len = (start + len > length) ? length - start : len;
        char* sub = new char[len + 1];
        for (long long unsigned int i = 0; i < len; ++i) {
            sub[i] = data[start + i];
        }
        sub[len] = '\0';
        String result(sub);
        delete[] sub;
        return result;
    }

    // Find
    long long unsigned int find(const String& substr) const {
        for (long long unsigned int i = 0; i <= length - substr.length; ++i) {
            bool match = true;
            for (long long unsigned int j = 0; j < substr.length; ++j) {
                if (data[i + j] != substr.data[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return i;
        }
        return -1; // Not found
    }

    // Replace
    String replace(const String& oldStr, const String& newStr) const {
        String result = *this;
        long long unsigned int pos = result.find(oldStr);
        while (pos != (long long unsigned int)-1) {
            result = result.substring(0, pos) + newStr + result.substring(pos + oldStr.size(), result.size());
            pos = result.find(oldStr);
        }
        return result;
    }

    // To Upper Case
    String toUpperCase() const {
        String result = *this;
        for (long long unsigned int i = 0; i < result.length; ++i) {
            result.data[i] = toUpper(result.data[i]);
        }
        return result;
    }

    // To Lower Case
    String toLowerCase() const {
        String result = *this;
        for (long long unsigned int i = 0; i < result.length; ++i) {
            result.data[i] = toLower(result.data[i]);
        }
        return result;
    }

    // Trim
    String trim() const {
        long long unsigned int start = 0;
        while (start < length && isSpace(data[start])) ++start;

        long long unsigned int end = length;
        while (end > start && isSpace(data[end - 1])) --end;

        return substring(start, end - start);
    }

    // Output Stream
    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        os << str.data;
        return os;
    }

    // Input Stream
    friend std::istream& operator>>(std::istream& is, String& str) {
        char buffer[1000];
        is.getline(buffer, 1000);
        str = String(buffer);
        return is;
    }
    friend String operator+(const char* lhs, const String& rhs) {
        size_t lhsLength = 0;
        while (lhs[lhsLength] != '\0') ++lhsLength;

        char* newData = new char[lhsLength + rhs.length + 1];
        for (size_t i = 0; i < lhsLength; ++i) {
            newData[i] = lhs[i];
        }
        for (size_t i = 0; i < rhs.length; ++i) {
            newData[lhsLength + i] = rhs.data[i];
        }
        newData[lhsLength + rhs.length] = '\0';

        String result(newData);
        delete[] newData;
        return result;
    }

    friend String operator+(char lhs, const String& rhs) {
        // Allocate memory for the new string
        char* newData = new char[rhs.length + 2]; // +2: 1 for char + 1 for null terminator

        // Add the single character to the start of the new string
        newData[0] = lhs;

        // Copy the contents of the existing string
        for (size_t i = 0; i < rhs.length; ++i) {
            newData[i + 1] = rhs.data[i];
        }

        // Null-terminate the new string
        newData[rhs.length + 1] = '\0';

        // Create the resulting String object
        String result(newData);

        // Free the allocated memory
        delete[] newData;

        return result;
    }
    bool contains(const String& substring) const {
        if (substring.length > this->length) return false;
        for (long long unsigned int i = 0; i <= this->length - substring.length; ++i) {
            bool match = true;
            for (long long unsigned int j = 0; j < substring.length; ++j) {
                if (this->data[i + j] != substring.data[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
        return false;
    }
    bool startsWith(const String& prefix) const {
        if (prefix.length > this->length) return false;
        for (long long unsigned int i = 0; i < prefix.length; ++i) {
            if (this->data[i] != prefix.data[i]) return false;
        }
        return true;
    }

};
