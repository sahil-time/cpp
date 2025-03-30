#pragma once

#include <fcntl.h> // O_CREAT etc.
#include <unistd.h> // ftruncate
#include <sys/mman.h> // shm_open etc.

#include "lockfreeq.hpp"

/*
 * What we need to talk about here, is the 'sizeof(lockfreeq<PACKET>)'
 * Here the 'lockfreeq' has N elements, could be 10 or could be 100 etc.
 * And we NEED a fixed size here for the SHM. So we need N blocks of PACKET size right?
 * But what is a PACKET size? If the PACKET class has ints, strings, etc. within it whose
 * size is NOT known till runtime, how is the SHM pre-allocated? Well we calculate the size
 * of N PACKET Objects, now if they contain Strings, those will be DYNAMICALLY allocated
 * automatically. So eventually, we will have pointers to it anyway whose size has ALREADY
 * been accounted for in the PACKET object.
 *
 * Just like Pyton and other languages, the actual data of these std:: types will be on the
 * heap, BUT the variable or the object itself can be on the HEAP or the STACK or SHM
 *
 */
class CABLE {
private:
    void* _cable;
    int _size;
    int _flags;
    int _fd;
    std::string _name;
    lockfreeq<PACKET>* _q;

public:
    CABLE(const std::string& name) : _name{name} {

        // Unlink the previous cable IF any
        shm_unlink(_name.c_str());

        _size = sizeof(lockfreeq<PACKET>);

        // Lay the new cable
        _flags = O_CREAT | O_RDWR;

        _fd = shm_open(_name.c_str(), _flags, 0666);
        if (_fd == -1) {
            std::cerr << "ERROR: Could not 'shm_open' the cable: " << _name << std::endl;
            return;
        }

        if (ftruncate(_fd, _size) == -1) {
            std::cerr << "ERROR: Could not 'ftruncate' the cable: " << _name << std::endl;
            close(_fd);
            shm_unlink(_name.c_str());
            return;
        }

        _cable = mmap(nullptr, _size, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0); // Lay out space for the cable
        if (_cable == MAP_FAILED) {
            std::cerr << "ERROR: Could not 'mmap' the cable: " << _name << std::endl;
            close(_fd);
            shm_unlink(_name.c_str());
            return;
        }

        _q = new(_cable) lockfreeq<PACKET>(); // Lay the actual cable

        close(_fd);
    }

    std::optional<PACKET> pull_packet() {
        return _q->dequeue();
    }

    int push_packet(PACKET& packet) {
        return _q->enqueue(packet);
    }

    ~CABLE() {
        shm_unlink(_name.c_str());
        munmap(_cable, _size);
        close(_fd);
    }
};
