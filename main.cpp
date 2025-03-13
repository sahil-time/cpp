#include <iostream>
#include <string>
#include <vector>

#ifdef GITHUB_ASSERT
#include <libassert/assert.hpp>
#else
#include <cassert>
#endif

#include "number.hpp"
#include "lockfreeq.hpp"

static void test_references() {
    std::vector<std::string> all_caps {"CAPS", "caps"};
    for(auto str : all_caps) {
        str = "CAPS"; // If 'str' was a *, below ASSERT would have passed
    }
    for(auto str : all_caps) {
        // DEBUG_ASSERT(str == "CAPS", "ERROR: Not caps");
    }
    for(auto& str: all_caps) {
        str = "CAPS";
    }
    for(auto str: all_caps) {
        DEBUG_ASSERT(str == "CAPS", "ERROR: Not caps");
    }
}

int main(int argc, char* argv[]) {
#ifdef GITHUB_ASSERT
    // New ASSERT => https://github.com/jeremy-rifkin/libassert
    DEBUG_ASSERT(argc == 2, "ERROR: Invalid Number of Parameters");
#else
    assert(argc == 2);
#endif

    std::cout << "MACHINE DATA:" << std::endl;
    std::cout << "  SIZE OF POINTER: " << sizeof(void*) << " bytes" << std::endl;
    std::cout << std::endl;

    std::cout << "NUMBER CRUNCHING:" << std::endl;
    INTEGER* num = new INTEGER{std::stoi(argv[1])};
    std::cout << "  Number is: " << num->get_num() << std::endl;
    std::cout << "    Is Palindrome: " << num->is_palindrome() << std::endl;

    // TESTING REFERENCES
    test_references();

    return 0;
}


/*
- References CANNOT be made to point to a different variable
- They MUST be initialized to a real variable when declared
- References do NOT take memory
*/
