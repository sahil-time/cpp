#pragma once

#include <iostream>
#include <string>

class PACKET {
private:
    std::string _source;
    std::string _dest;
    long long _seq_num;
    long double _val;
    std::string _word;
public:
    PACKET(const std::string& source, const std::string& dest, long long seq_num, long double val) :
        _source{source}, _dest{dest}, _seq_num{seq_num}, _val{val}  {}
    PACKET() {}

    std::string& get_source() {
        return _source;
    }

    std::string& get_dest() {
        return _dest;
    }

    long long get_seq_num() {
        return _seq_num;
    }

    long double get_val() {
        return _val;
    }

    std::string& get_word() {
        return _word;
    }

    void dump_packet() {
        std::cout << "PACKET: " << std::endl;
        std::cout << "  Source  : " << _source << std::endl;
        std::cout << "  Dest    : " << _dest << std::endl;
        std::cout << "  Seq No  : " << _seq_num << std::endl;
        std::cout << "  Value   : " << _val << std::endl;
        std::cout << "  Word    : " << _word << std::endl;
    }
};
