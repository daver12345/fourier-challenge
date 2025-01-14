// Header file for serial communication functions
#ifndef SERIAL_H
#define SERIAL_H
void serial_send(const char* port, std::string message);
auto serial_recv(const char* port) -> std::string;
#endif