#include <iostream>
#include <thread>
#include "controller.h"
#include "serial.h"

std::string command = "OFF";
#define TARGET -10

void pump_comm() {
    const char* pump_port = "/tmp/ttyV0";
    // Every 200 ms read the pump state and send on/off commands
    while(true) {
        serial_send(pump_port, "READ");
        std::string pump_state = serial_recv(pump_port);
        std::cout << "STATE: " << pump_state << std::endl;
        serial_send(pump_port, command);

        using namespace std::chrono;
        std::this_thread::sleep_for(milliseconds(200));
    }
}

void temp_comm() {
    const char* temp_port = "/tmp/ttyV2";
    int temperature = 0;
    // Every 1s read the temperature and update command based on TARGET temperature
    while(true) {
        serial_send(temp_port, "READ");
        std::string message = serial_recv(temp_port);
        if (!message.empty()) {
            temperature = std::stoi(message);
            std::cout << "TEMP: " << temperature << std::endl;
        }
        
        // Temperature logic
        if (temperature > TARGET) {
            command = "ON";
        } else {
            command = "OFF";
        }

        using namespace std::chrono;
        std::this_thread::sleep_for(seconds(1));
    }
}

void run_controller() {
    std::thread pump_thread(pump_comm);
    std::thread temp_thread(temp_comm);

    pump_thread.join();
    temp_thread.join();
}