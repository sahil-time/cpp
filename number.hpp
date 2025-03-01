#pragma once

#include <string>

class INTEGER {
private:
    long long int _num;

public:
    // Constructor
    INTEGER(long long int num)
        : _num{num} {}

    // CRUD Operations
    void update_num(long long int new_num);
    long long int get_num();

    // Math Functions
    bool is_palindrome();
    bool is_prime();
    bool is_armstrong();
    bool is_square();
    bool is_cube();

    long long int next_prime();
    long long int prev_prime();
    double sqrt();
    long long int reverse();

    std::string to_string();
    std::string to_roman();
    std::string to_boolean();
};

INTEGER x{4};
INTEGER y{4.5};

/*

- In C++ there are 3 types of initializations...weird!!!
    1. int x = 5;
    2. int b(5);
    3. int c{5};

All the above are the same...pretty much!

However, the 3. does TYPE CHECKING!!

REF: https://ianyepan.github.io/posts/cpp-uniform-initialization/
     https://herbsutter.com/2013/05/09/gotw-1-solution/

“C++’s most vexing parse,” because the standard resolves the parsing ambiguity by saying: “if it can be a function declaration, it is.”
*/
