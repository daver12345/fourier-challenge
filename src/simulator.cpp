#include <thread>
#include "simulator.h"
#include "serial.h"
std::string pump_state = "OFF";

void pump_sim() {
    const char* pump_port = "/tmp/ttyV1";

    while(true) {
        std::string command = serial_recv(pump_port);
        if (command == "READ") {
            serial_send(pump_port, pump_state);
        } else if (command == "ON" || command == "OFF") {
            pump_state = command;
        }
    }
}

void temp_sim() {
    const char* temp_port = "/tmp/ttyV3";
    int temperature = 0;
    while(true) {
        std::string command = serial_recv(temp_port);
        if (command == "READ") {
            serial_send(temp_port, std::to_string(temperature));
        }
        temperature += (pump_state == "ON") ? -1 : 1;
    }
}

void run_simulator() {
    std::thread pump_thread(pump_sim);
    std::thread temp_thread(temp_sim);

    pump_thread.join();
    temp_thread.join();
}