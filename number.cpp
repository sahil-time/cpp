#include <libassert/assert.hpp>

#include "number.hpp"

void INTEGER::update_num(long long int num) {
    _num = num;
}

long long int INTEGER::get_num() {
    return _num;
}

bool INTEGER::is_palindrome() {
    // EDGE CASES: If num is -ve OR num has a 0 at the end
    if(_num < 0 || ((_num % 10 == 0) && (_num != 0))) {
        return false;
    }

    /* String Algo:
     * - Space Complexity - Takes O(N) where N is the len of the number
     * - Time Complexity - Takes O(N/2) ~ O(N) where N is the len of the number
     * - Also needs to convert num to a String
     */
    bool algo1_result = false;
    std::string num_str = std::to_string(_num);
    int i = 0;
    int len = num_str.length();

    while(true) {
        if(i >= len/2) {
            algo1_result = true;
            break;
        }
        if(num_str[i] != num_str[len - i - 1]) {
            algo1_result = false;
            break;
        }
        i++;
    }

    /* Revert Half Number:
     * - Space Complexity - O(1)
     * - Time Complexity - O(N/2) ~ O(N) where N is the len of number OR
     *   O(log10(num)) where num is the actual number
     * - Does NOT take any extra space except Constant spaces
     * - Why ONLY half the number? To avoid Int-Overflow since if you reverse
     *   the whole number it could be larger than int.MAX
     */
    bool algo2_result = false;
    int num = _num;
    int reversed = 0;
    while(num > reversed) {
        reversed = reversed*10 + num%10;
        num = num/10;
    }
    if((num == reversed) || (num == reversed/10)) {
        algo2_result = true;
    }

    // Compare both results - should be the same
    DEBUG_ASSERT(algo1_result == algo2_result);

    return algo1_result;
}
