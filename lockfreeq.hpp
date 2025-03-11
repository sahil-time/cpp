#include <optional>

/* INVARIANTS: '_cp <= _pp' i.e. '_cp' will ALWAYS be less than or = to '_pp'
 * Now we can design the queue such for an EMPTY QUEUE:
 *  1. '_cp' can at-most equal '_pp' OR
 *  2. '_cp' can at-most equal '_pp - 1'
 *
 * Let us go with the 2. option [ remember you can go with any option ]
 *  - If '_cp' == 0 then '_pp' == 1 for an Empty Queue
 *  - This implies that Enqueue will Enqueue then move & Dequeue will Move then Dequeue
 *  - If Enqueue will first Enqueue then move, what is the Full Queue Invariant?
 *  - '_pp == _cp' is the Full Queue Invariant because now if we Enqueue & Move then
 *      the Invariant becomes '_cp == _pp - 1' which is ACTUALLY an Empty Queue Invariant
 *
 * THEREFORE:
 *  - EMPTY QUEUE INVARIANT: '_cp == _pp - 1' [ Stop DEQUEUE ]
 *  - FULL QUEUE INVARIANT: '_pp == _cp' [ Stop ENQUEUE ]
 *
 * WHAT IF WE GO WITH OPTION 1.?
 *  - If '_cp' == 0 then '_pp' == 0 for an Empty Queue
 *  - This implies that Enqueue will Enqueue and Move & Dequeue will Dequeue & Move
 *  - The Full Queue Invariant will be '_pp == _cp - 1' because a step further and the
 *      the Invariant becomes  '_pp == _cp' which is an Empty Queue Invariant
 *
 * THEREFORE:
 *  - EMPTY QUEUE INVARIANT: '_cp == _pp' [ Stop DEQUEUE ]
 *  - FULL QUEUE INVARIANT: '_pp == _cp - 1' [ STOP ENQUEUE ]
 */
template <typename T>
class lockfreeq {
private:
    std::array<T, QLEN> _q;
    unsigned int _cp;
    unsigned int _pp;
public:
    lockfreeq () : _cp{0}, _pp{1} {}
    int enqueue (T val) {
        if (_pp == _cp) {
            return -1;
        }
        _q[_pp] = val;
        _pp = (_pp + 1)%QLEN;
        return 0;
    }
    std::optional<T> dequeue () {
        unsigned int next = (_cp + 1)%QLEN;
        if (next == _pp) {
            return {};
        }
        _cp = next;
        return _q[_cp];
    }
};
