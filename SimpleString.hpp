#ifndef SIMPLE_STRING_SIMPLESTRING_HPP
#define SIMPLE_STRING_SIMPLESTRING_HPP

#include <stdexcept> // you can use std::out_of_range
#include <cstring>
// other headers are not allowed

class MyString {
private:
    /**
     * @brief To implement Small String Optimization (SSO), two storage methods are needed.
     *
     * This code demonstrates a possible approach:
     * - A union is used to switch between a small buffer and a heap pointer.
     * - `heap_ptr` is used for larger strings that exceed the buffer capacity.
     * - `small_buffer` is an inline buffer for small strings, enhancing performance by avoiding heap allocation.
     */
    union {
        char* heap_ptr;        // Pointer to heap-allocated memory
        char small_buffer[16]; // Buffer for small string optimization (SSO)
    };

    size_t length;      // Current length of the string (excluding null terminator)
    size_t cap;         // Capacity (excluding null terminator)
    bool is_sso;        // True if using SSO, false if using heap

    // Helper function to check if we should use SSO
    bool should_use_sso(size_t len) const {
        return len <= 15;
    }

public:
    MyString() : length(0), cap(15), is_sso(true) {
        small_buffer[0] = '\0';
    }

    /**
     * @brief Constructor from a C-string.
     * @param s Null-terminated character array.
     * @todo Initialize `MyString` with the contents of `s`. Implement SSO if length is below 16.
     */
    MyString(const char* s) {
        if (s == nullptr) {
            length = 0;
            cap = 15;
            is_sso = true;
            small_buffer[0] = '\0';
            return;
        }

        length = strlen(s);

        if (should_use_sso(length)) {
            is_sso = true;
            cap = 15;
            memcpy(small_buffer, s, length + 1);
        } else {
            is_sso = false;
            // Allocate with some extra capacity (2x growth strategy)
            cap = length * 2;
            heap_ptr = new char[cap + 1];
            memcpy(heap_ptr, s, length + 1);
        }
    }

    MyString(const MyString& other) {
        length = other.length;
        is_sso = other.is_sso;

        if (is_sso) {
            cap = 15;
            memcpy(small_buffer, other.small_buffer, length + 1);
        } else {
            cap = other.cap;
            heap_ptr = new char[cap + 1];
            memcpy(heap_ptr, other.heap_ptr, length + 1);
        }
    }

    MyString(MyString&& other) noexcept {
        length = other.length;
        cap = other.cap;
        is_sso = other.is_sso;

        if (is_sso) {
            memcpy(small_buffer, other.small_buffer, length + 1);
        } else {
            heap_ptr = other.heap_ptr;
            other.heap_ptr = nullptr;
        }

        // Reset other to empty state
        other.length = 0;
        other.cap = 15;
        other.is_sso = true;
        other.small_buffer[0] = '\0';
    }

    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            // Clean up current resources
            if (!is_sso) {
                delete[] heap_ptr;
            }

            length = other.length;
            cap = other.cap;
            is_sso = other.is_sso;

            if (is_sso) {
                memcpy(small_buffer, other.small_buffer, length + 1);
            } else {
                heap_ptr = other.heap_ptr;
                other.heap_ptr = nullptr;
            }

            // Reset other to empty state
            other.length = 0;
            other.cap = 15;
            other.is_sso = true;
            other.small_buffer[0] = '\0';
        }
        return *this;
    }

    MyString& operator=(const MyString& other) {
        if (this != &other) {
            // Clean up current resources
            if (!is_sso) {
                delete[] heap_ptr;
            }

            length = other.length;
            is_sso = other.is_sso;

            if (is_sso) {
                cap = 15;
                memcpy(small_buffer, other.small_buffer, length + 1);
            } else {
                cap = other.cap;
                heap_ptr = new char[cap + 1];
                memcpy(heap_ptr, other.heap_ptr, length + 1);
            }
        }
        return *this;
    }

    ~MyString() {
        if (!is_sso) {
            delete[] heap_ptr;
        }
    }

    const char* c_str() const {
        return is_sso ? small_buffer : heap_ptr;
    }

    size_t size() const {
        return length;
    }

    size_t capacity() const {
        return cap;
    }

    void reserve(size_t new_capacity) {
        // If already have enough capacity, do nothing
        if (new_capacity <= cap) {
            return;
        }

        // If currently using SSO and new capacity requires heap
        if (is_sso && new_capacity > 15) {
            char* new_ptr = new char[new_capacity + 1];
            memcpy(new_ptr, small_buffer, length + 1);
            heap_ptr = new_ptr;
            cap = new_capacity;
            is_sso = false;
        } else if (!is_sso) {
            // Already on heap, reallocate
            char* new_ptr = new char[new_capacity + 1];
            memcpy(new_ptr, heap_ptr, length + 1);
            delete[] heap_ptr;
            heap_ptr = new_ptr;
            cap = new_capacity;
        }
        // If SSO and new_capacity <= 15, no action needed
    }

    void resize(size_t new_size) {
        if (new_size > cap) {
            // Need to grow capacity
            reserve(new_size);
        }

        char* data = is_sso ? small_buffer : heap_ptr;

        if (new_size > length) {
            // Fill new characters with '\0'
            memset(data + length, '\0', new_size - length);
        }

        length = new_size;
        data[length] = '\0';

        // Check if we can shrink back to SSO
        if (!is_sso && should_use_sso(length)) {
            char temp[16];
            memcpy(temp, heap_ptr, length + 1);
            delete[] heap_ptr;
            memcpy(small_buffer, temp, length + 1);
            is_sso = true;
            cap = 15;
        }
    }

    /**
     * @brief Accesses a character by index.
     * @param index The position of the character.
     * @return Reference to the character at `index`.
     * @throw std::out_of_range if `index` is out of bounds.
     */
    char& operator[](size_t index) {
        return is_sso ? small_buffer[index] : heap_ptr[index];
    }

    /**
     * @brief Concatenates two `MyString` instances.
     * @param rhs The `MyString` to concatenate.
     * @return A new `MyString` containing the concatenated result.
     */
    MyString operator+(const MyString& rhs) const {
        MyString result;
        size_t new_len = length + rhs.length;

        if (new_len > 15) {
            result.is_sso = false;
            result.cap = new_len * 2;
            result.heap_ptr = new char[result.cap + 1];
            memcpy(result.heap_ptr, c_str(), length);
            memcpy(result.heap_ptr + length, rhs.c_str(), rhs.length);
            result.heap_ptr[new_len] = '\0';
        } else {
            result.is_sso = true;
            result.cap = 15;
            memcpy(result.small_buffer, c_str(), length);
            memcpy(result.small_buffer + length, rhs.c_str(), rhs.length);
            result.small_buffer[new_len] = '\0';
        }

        result.length = new_len;
        return result;
    }

    /**
     * @brief Appends a C-string to the current `MyString`.
     * @param str Null-terminated character array.
     */
    void append(const char* str) {
        if (str == nullptr) return;

        size_t str_len = strlen(str);
        size_t new_len = length + str_len;

        if (new_len > cap) {
            // Need to grow capacity
            size_t new_cap = new_len * 2;
            reserve(new_cap);
        }

        char* data = is_sso ? small_buffer : heap_ptr;
        memcpy(data + length, str, str_len);
        length = new_len;
        data[length] = '\0';

        // Check if we need to move from SSO to heap
        if (is_sso && new_len > 15) {
            char temp[256];
            memcpy(temp, small_buffer, new_len + 1);
            cap = new_len * 2;
            heap_ptr = new char[cap + 1];
            memcpy(heap_ptr, temp, new_len + 1);
            is_sso = false;
        }
    }

    /**
     * @brief Accesses a character by index safely.
     * @param pos The position of the character.
     * @throw std::out_of_range if `pos` is out of bounds.
     */
    const char& at(size_t pos) const {
        if (pos >= length) {
            throw std::out_of_range("Index out of range");
        }
        return is_sso ? small_buffer[pos] : heap_ptr[pos];
    }

    class const_iterator;

    class iterator {
    private:
        char* ptr;

    public:
        iterator(char* p = nullptr) : ptr(p) {}

        /**
         * @brief Pre-increment iterator.
         * @return Reference to the incremented iterator.
         */
        iterator& operator++() {
            ++ptr;
            return *this;
        }

        /**
         * @brief Post-increment iterator.
         * @return Copy of iterator before increment.
         */
        iterator operator++(int) {
            iterator temp = *this;
            ++ptr;
            return temp;
        }

        /**
         * @brief Pre-decrement iterator.
         * @return Reference to the decremented iterator.
         */
        iterator& operator--() {
            --ptr;
            return *this;
        }

        /**
         * @brief Post-decrement iterator.
         * @return Copy of iterator before decrement.
         */
        iterator operator--(int) {
            iterator temp = *this;
            --ptr;
            return temp;
        }

        char& operator*() const {
            return *ptr;
        }

        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }

        bool operator==(const const_iterator& other) const {
            return ptr == other.get_ptr();
        }

        bool operator!=(const const_iterator& other) const {
            return ptr != other.get_ptr();
        }

        char* get_ptr() const { return ptr; }
    };

    class const_iterator {
    private:
        const char* ptr;

    public:
        const_iterator(const char* p = nullptr) : ptr(p) {}

        const_iterator& operator++() {
            ++ptr;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++ptr;
            return temp;
        }

        const_iterator& operator--() {
            --ptr;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator temp = *this;
            --ptr;
            return temp;
        }

        const char& operator*() const {
            return *ptr;
        }

        bool operator==(const const_iterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const const_iterator& other) const {
            return ptr != other.ptr;
        }

        bool operator==(const iterator& other) const {
            return ptr == other.get_ptr();
        }

        bool operator!=(const iterator& other) const {
            return ptr != other.get_ptr();
        }

        const char* get_ptr() const { return ptr; }
    };

public:
    iterator begin() {
        return iterator(is_sso ? small_buffer : heap_ptr);
    }

    iterator end() {
        char* data = is_sso ? small_buffer : heap_ptr;
        return iterator(data + length);
    }

    const_iterator cbegin() const {
        return const_iterator(is_sso ? small_buffer : heap_ptr);
    }

    const_iterator cend() const {
        const char* data = is_sso ? small_buffer : heap_ptr;
        return const_iterator(data + length);
    }
};

#endif
