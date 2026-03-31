#include "SimpleString.hpp"
#include <iostream>
#include <cassert>

void test_sso_boundary() {
    std::cout << "Testing SSO boundary..." << std::endl;

    // String with 15 characters (should use SSO)
    MyString s1("123456789012345");
    assert(s1.size() == 15);
    assert(s1.capacity() == 15);

    // String with 16 characters (should use heap)
    MyString s2("1234567890123456");
    assert(s2.size() == 16);
    assert(s2.capacity() >= 16);

    std::cout << "SSO boundary tests passed!" << std::endl;
}

void test_resize_sso_to_heap() {
    std::cout << "Testing resize from SSO to heap..." << std::endl;

    MyString s("hello");
    assert(s.size() == 5);
    assert(s.capacity() == 15);

    s.resize(20);
    assert(s.size() == 20);
    assert(s.capacity() >= 20);

    std::cout << "Resize SSO to heap tests passed!" << std::endl;
}

void test_append_sso_to_heap() {
    std::cout << "Testing append from SSO to heap..." << std::endl;

    MyString s("hello");
    s.append(" world, this is a longer string");
    assert(s.size() > 15);

    std::cout << "Append SSO to heap tests passed!" << std::endl;
}

void test_iterators_detailed() {
    std::cout << "Testing iterators..." << std::endl;

    MyString s("test");

    // Test pre-increment
    auto it = s.begin();
    assert(*it == 't');
    ++it;
    assert(*it == 'e');

    // Test post-increment
    auto it2 = it++;
    assert(*it2 == 'e');
    assert(*it == 's');

    // Test pre-decrement
    --it;
    assert(*it == 'e');

    // Test post-decrement
    auto it3 = it--;
    assert(*it3 == 'e');
    assert(*it == 't');

    // Test iterator comparisons
    assert(s.begin() != s.end());
    assert(s.begin() == s.begin());

    std::cout << "Iterator tests passed!" << std::endl;
}

void test_capacity_management() {
    std::cout << "Testing capacity management..." << std::endl;

    MyString s;
    s.reserve(100);
    assert(s.capacity() >= 100);
    assert(s.size() == 0);

    s.append("test");
    assert(s.size() == 4);
    assert(s.capacity() >= 100);

    std::cout << "Capacity management tests passed!" << std::endl;
}

void test_move_semantics() {
    std::cout << "Testing move semantics..." << std::endl;

    // Test move constructor with heap string
    MyString s1("This is a very long string that uses heap allocation");
    size_t original_cap = s1.capacity();
    MyString s2(std::move(s1));

    assert(s2.size() > 15);
    assert(s1.size() == 0);
    assert(s1.capacity() == 15);

    // Test move assignment with heap string
    MyString s3("Another very long string that uses heap allocation");
    MyString s4;
    s4 = std::move(s3);

    assert(s4.size() > 15);
    assert(s3.size() == 0);
    assert(s3.capacity() == 15);

    std::cout << "Move semantics tests passed!" << std::endl;
}

void test_operator_plus() {
    std::cout << "Testing operator+..." << std::endl;

    MyString s1("Hello");
    MyString s2(" World");
    MyString s3 = s1 + s2;

    assert(s3.size() == 11);
    assert(std::string(s3.c_str()) == "Hello World");

    // Test concatenation resulting in heap allocation
    MyString s4("This is a");
    MyString s5(" very long string");
    MyString s6 = s4 + s5;

    assert(s6.size() == s4.size() + s5.size());

    std::cout << "Operator+ tests passed!" << std::endl;
}

int main() {
    test_sso_boundary();
    test_resize_sso_to_heap();
    test_append_sso_to_heap();
    test_iterators_detailed();
    test_capacity_management();
    test_move_semantics();
    test_operator_plus();

    std::cout << "\n=== All comprehensive tests passed! ===" << std::endl;

    return 0;
}
