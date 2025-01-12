#include <thread>
#include "simulator.h"
#include "serial.h"
#include <iostream>
std::string pump_state = "OFF";

void pump_sim() {
    const char* pump_port = "/tmp/ttyV1";

    // Wait on a command from the controller and send/update pump state in response
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
    using namespace std::chrono;
    system_clock::time_point time = system_clock::now();

    // Wait on a command from the sensor and send temperature in response
    while(true) {
        std::string command = serial_recv(temp_port);
        if (command == "READ") {
            serial_send(temp_port, std::to_string(temperature));
        }
        // Update temperature based on pump state
        std::chrono::duration<double> dt = system_clock::now() - time;
        temperature += (pump_state == "ON") ? int(-dt.count()) : int(dt.count());
        time = system_clock::now();
    }
}

void run_simulator() {
    std::thread pump_thread(pump_sim);
    std::thread temp_thread(temp_sim);

    pump_thread.join();
    temp_thread.join();
}