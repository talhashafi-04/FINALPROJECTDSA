#pragma once


#include <iostream>

template <typename T>
class Vector {
private:
    T* data;              // Pointer to the dynamically allocated array
    size_t size_;         // Number of elements in the vector
    size_t capacity_;     // Current capacity of the vector

    // Resizes the underlying array to a new capacity
    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < size_; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity_ = newCapacity;
    }

public:
    // Default Constructor
    Vector() : data(nullptr), size_(0), capacity_(0) {}

    // Destructor
    ~Vector() {
        delete[] data;
    }

    // Adds an element at the end
    void push_back(const T& value) {
        if (size_ == capacity_) {
            size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            resize(newCapacity);
        }
        data[size_++] = value;
    }

    // Removes the last element
    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    // Returns the number of elements
    size_t size() const {
        return size_;
    }

    // Returns the current capacity
    size_t capacity() const {
        return capacity_;
    }

    // Clears all elements
    void clear() {
        size_ = 0;
    }

    // Access element at a specific index (read/write)
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Access element at a specific index (read-only)
    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Insert an element at a specific position
    void insert(size_t index, const T& value) {
        if (index > size_) {
            throw std::out_of_range("Index out of range");
        }

        if (size_ == capacity_) {
            size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            resize(newCapacity);
        }

        for (size_t i = size_; i > index; --i) {
            data[i] = data[i - 1];
        }

        data[index] = value;
        ++size_;
    }

    // Erase an element at a specific position
    void erase(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        for (size_t i = index; i < size_ - 1; ++i) {
            data[i] = data[i + 1];
        }

        --size_;
    }

    // Output Stream
    friend std::ostream& operator<<(std::ostream& os, const Vector& vec) {
        os << "[";
        for (size_t i = 0; i < vec.size_; ++i) {
            os << vec.data[i];
            if (i < vec.size_ - 1) os << ", ";
        }
        os << "]";
        return os;
    }
};
