#include <iostream>
#include <string>

#include <libassert/assert.hpp>

#include "number.hpp"

int main(int argc, char* argv[]) {
    // New ASSERT => https://github.com/jeremy-rifkin/libassert
    DEBUG_ASSERT(argc == 2, "ERROR: Invalid Number of Parameters");

    std::cout << "MACHINE DATA:" << std::endl;
    std::cout << "  SIZE OF POINTER: " << sizeof(void*) << " bytes" << std::endl;
    std::cout << std::endl;

    std::cout << "NUMBER CRUNCHING:" << std::endl;
    INTEGER* num = new INTEGER{std::stoi(argv[1])};
    std::cout << "  Number is: " << num->get_num() << std::endl;
    std::cout << "    Is Palindrome: " << num->is_palindrome() << std::endl;
    return 0;
}

