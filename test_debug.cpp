#include "SimpleString.hpp"
#include <iostream>

int main() {
    MyString s3("This is a longer string that exceeds SSO");
    std::cout << "s3.size() = " << s3.size() << std::endl;
    std::cout << "s3.c_str() = " << s3.c_str() << std::endl;
    std::cout << "s3.capacity() = " << s3.capacity() << std::endl;
    return 0;
}
