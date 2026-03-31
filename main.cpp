#include "SimpleString.hpp"
#include <iostream>
#include <cassert>

int main() {
    // Test 1: Default constructor
    MyString s1;
    assert(s1.size() == 0);
    assert(s1.capacity() == 15);

    // Test 2: Constructor from C-string (SSO)
    MyString s2("Hello");
    assert(s2.size() == 5);
    assert(s2.capacity() == 15);

    // Test 3: Constructor from C-string (heap)
    MyString s3("This is a longer string that exceeds SSO");
    assert(s3.size() == 40);
    assert(s3.capacity() >= 40);

    // Test 4: Copy constructor
    MyString s4(s2);
    assert(s4.size() == 5);

    // Test 5: Move constructor
    MyString s5(std::move(s4));
    assert(s5.size() == 5);
    assert(s4.size() == 0);

    // Test 6: operator[]
    MyString s6("test");
    assert(s6[0] == 't');
    assert(s6[1] == 'e');

    // Test 7: at() with valid index
    assert(s6.at(0) == 't');

    // Test 8: at() with invalid index
    try {
        s6.at(100);
        assert(false);  // Should not reach here
    } catch (const std::out_of_range&) {
        // Expected
    }

    // Test 9: operator+
    MyString s7 = s2 + MyString(" World");
    assert(s7.size() == 11);

    // Test 10: append
    MyString s8("Hello");
    s8.append(" World");
    assert(s8.size() == 11);

    // Test 11: reserve
    MyString s9;
    s9.reserve(100);
    assert(s9.capacity() >= 100);

    // Test 12: resize (grow)
    MyString s10("test");
    s10.resize(10);
    assert(s10.size() == 10);

    // Test 13: resize (shrink)
    MyString s11("test string");
    s11.resize(4);
    assert(s11.size() == 4);

    // Test 14: iterators
    MyString s12("abc");
    auto it = s12.begin();
    assert(*it == 'a');
    ++it;
    assert(*it == 'b');
    ++it;
    assert(*it == 'c');
    ++it;
    assert(it == s12.end());

    // Test 15: const iterators
    const MyString s13("xyz");
    auto cit = s13.cbegin();
    assert(*cit == 'x');

    // Test 16: Copy assignment
    MyString s14("source");
    MyString s15;
    s15 = s14;
    assert(s15.size() == 6);

    // Test 17: Move assignment
    MyString s16("source");
    MyString s17;
    s17 = std::move(s16);
    assert(s17.size() == 6);
    assert(s16.size() == 0);

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
