#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include "controller.h"
#include "serial.h"

void pump_comm(std::string& command, std::mutex& command_lock) {
    const char* pump_port = "/tmp/ttyV0";

    // Every 200 ms read the pump state and send on/off commands
    constexpr int SLEEP_DURATION_MS = 200;

    while(true) {
        serial_send(pump_port, "READ");
        std::string pump_state = serial_recv(pump_port);
        std::cout << "STATE: " << pump_state << std::endl;
        std::lock_guard<std::mutex> lock(command_lock);
        serial_send(pump_port, command);
    
        // Manually unlocked at end of scope so we can recv every 200ms
        command_lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION_MS));
    }
}

void temp_comm(std::string& command, std::mutex& command_lock, int target) {
    const char* temp_port = "/tmp/ttyV2";
    int temperature = 0;

    // Every 1s read the temperature and update command based on TARGET temperature
    constexpr int SLEEP_DURATION_S = 1;
    
    while(true) {
        serial_send(temp_port, "READ");
        std::string message = serial_recv(temp_port);
        if (!message.empty()) {
            temperature = std::stoi(message);
            std::cout << "TEMP: " << temperature << std::endl;
        } else {
            std::cerr << "Error: Received temperature is NULL" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        // Temperature logic
        std::lock_guard<std::mutex> lock(command_lock);
        if (temperature > target) {
            command = "ON";
        } else {
            command = "OFF";
        }

        std::this_thread::sleep_for(std::chrono::seconds(SLEEP_DURATION_S));
    }
}

void run_controller() {
    std::string command = "OFF";
    std::mutex command_lock;
    constexpr int TARGET = -10;
    
    std::thread pump_thread(pump_comm, std::ref(command), std::ref(command_lock));
    std::thread temp_thread(temp_comm, std::ref(command), std::ref(command_lock), TARGET);

    pump_thread.join();
    temp_thread.join();
}