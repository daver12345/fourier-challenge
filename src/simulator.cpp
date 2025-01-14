#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include "simulator.h"
#include "serial.h"

void pump_sim(std::string& pump_state, std::mutex& pump_state_lock) {
    const char* pump_port = "/tmp/ttyV1";

    // Wait on a command from the controller and send/update pump state in response
    while(true) {
        std::string command = serial_recv(pump_port);
        std::lock_guard<std::mutex> lock(pump_state_lock);
        if (command == "READ") {
            serial_send(pump_port, pump_state);
        } else if (command == "ON" || command == "OFF") {
            pump_state = command;
        } else {
            std::cerr << "Error: Received pump command is improperly formatted" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void temp_sim(std::string& pump_state, std::mutex& pump_state_lock) {
    const char* temp_port = "/tmp/ttyV3";
    int temperature = 0;
    std::chrono::system_clock::time_point time = std::chrono::system_clock::now();

    // Wait on a command from the sensor and send temperature in response
    while(true) {
        std::string command = serial_recv(temp_port);
        if (command == "READ") {
            serial_send(temp_port, std::to_string(temperature));
        } else {
            std::cerr << "Error: Received sensor command is improperly formatted" << std::endl;
            exit(EXIT_FAILURE);
        }
        // Update temperature based on pump state
        std::chrono::duration<double> delta_t = std::chrono::system_clock::now() - time;
        std::lock_guard<std::mutex> lock(pump_state_lock);
        temperature += (pump_state == "ON") ? -static_cast<int>(delta_t.count()) : static_cast<int>(delta_t.count());
        time = std::chrono::system_clock::now();
    }
}

void run_simulator() {
    std::string pump_state = "OFF";
    std::mutex pump_state_lock;

    std::thread pump_thread(pump_sim, std::ref(pump_state), std::ref(pump_state_lock));
    std::thread temp_thread(temp_sim, std::ref(pump_state), std::ref(pump_state_lock));

    pump_thread.join();
    temp_thread.join();
}