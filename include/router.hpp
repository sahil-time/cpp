#pragma once

#include <map>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <optional>
#include <unordered_set>

#include "packet.hpp"
#include "cable.hpp"

class ROUTER {
private:
    std::string _name;
    std::unordered_map<std::string, std::unordered_set<CABLE*>> _routing_table;
    long long _packet_counter;

    CABLE* _get_egress_interface(PACKET& packet) {
    /* There are multiple ways to supress the Unused Variable error -
        1. Do NOT provide a variable name like above
        2. Use '[[maybe_unused]]' before the declaration
        3. Use the pragma push/pop i.e.
            #pragma GCC diagnostic pusoh
            #pragma GCC diagnostic ignored "-Wunused-parameter"
            #pragma GCC diagnostic pop
    */
        std::string dest = packet.get_dest();
        if (dest == _name) {
            std::cout << "ROUTER: " << _name << std::endl;
            packet.dump_packet();
            return nullptr;
        }

        std::unordered_set<CABLE*> intfs = _routing_table[dest]; // Find the list of egress interfaces for the destination
        //int index = _packet_counter%intfs.size();
        return *intfs.begin();
    }

    void _push_packet(PACKET& packet) {
        CABLE* egress = _get_egress_interface(packet);
        if (egress != nullptr) {
            while(egress->push_packet(packet)); // Repeat Enqueue till successfull
        }
    }

    void _user_helper_message() {
        std::cout << "USER THREAD: Use any of the following options:" << std::endl;
        std::cout << "  Packets" << std::endl;
        std::cout << "  Routing" << std::endl;
        std::cout << "" << std::endl;
    }

    void _user_thread() {
        std::string input;
        while(true) {
            _user_helper_message();
            std::cin >> input;

            if (input == "Packets") {
                std::cout << _name << std::endl;
                std::cout << _packet_counter << std::endl;
            } else {
                std::cout << "ERROR: Invalid Input. Use the below options!" << std::endl;
            }

            std::cout << "" << std::endl;
        }
    }

public:
    ROUTER(const std::string& name, std::unordered_map<std::string, std::unordered_set<CABLE*>> routing_table) :
        _name{name}, _routing_table{routing_table}, _packet_counter{0} {}

    void run() {
        // Start the User Interfacing thread
        //std::thread user_thread(_user_thread); <--- DOES NOT WORK, since this function is INSIDE a class
        //std::thread user_thread(std::bind(&ROUTER::_user_thread, this));

        // Star the ROUTER
        std::cout << "Starting Router " << _name << "..." << std::endl;
        std::optional<PACKET> packet;
        while(true) {
            for (auto& cable : _routing_table[_name]) { // Iterate over each Ingress in round-robin
                packet = cable->pull_packet();
                if(packet.has_value()) {
                    _push_packet(packet.value());
                    _packet_counter++;
                } else {
                    std::cout << "ROUTER: " << _name << " PACKETS: " << _packet_counter << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }
        }
    }
};
