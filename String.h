#pragma once
#include <iostream>

class String {
private:
    char* data;
    size_t length;

    size_t calculateLength(const char* str) const {
        size_t len = 0;
        while (str[len] != '\0') ++len;
        return len;
    }

    void copyData(const char* str) {
        length = calculateLength(str);
        data = new char[length + 1];
        for (size_t i = 0; i < length; ++i) {
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
        for (size_t i = 0; i < length; ++i) {
            newData[i] = data[i];
        }
        for (size_t i = 0; i < other.length; ++i) {
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
        for (size_t i = 0; i < length; ++i) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    // Indexing
    char& operator[](size_t index) {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }

    const char& operator[](size_t index) const {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }

    // Get Length
    size_t size() const {
        return length;
    }

    // Substring
    String substring(size_t start, size_t len) const {
        if (start >= length) throw std::out_of_range("Start index out of range");
        len = (start + len > length) ? length - start : len;
        char* sub = new char[len + 1];
        for (size_t i = 0; i < len; ++i) {
            sub[i] = data[start + i];
        }
        sub[len] = '\0';
        String result(sub);
        delete[] sub;
        return result;
    }

    // Find
    size_t find(const String& substr) const {
        for (size_t i = 0; i <= length - substr.length; ++i) {
            bool match = true;
            for (size_t j = 0; j < substr.length; ++j) {
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
        size_t pos = result.find(oldStr);
        while (pos != (size_t)-1) {
            result = result.substring(0, pos) + newStr + result.substring(pos + oldStr.size(), result.size());
            pos = result.find(oldStr);
        }
        return result;
    }

    // To Upper Case
    String toUpperCase() const {
        String result = *this;
        for (size_t i = 0; i < result.length; ++i) {
            result.data[i] = toUpper(result.data[i]);
        }
        return result;
    }

    // To Lower Case
    String toLowerCase() const {
        String result = *this;
        for (size_t i = 0; i < result.length; ++i) {
            result.data[i] = toLower(result.data[i]);
        }
        return result;
    }

    // Trim
    String trim() const {
        size_t start = 0;
        while (start < length && isSpace(data[start])) ++start;

        size_t end = length;
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
};
