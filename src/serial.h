// Header file for serial communication functions
#ifndef SERIAL_H
#define SERIAL_H
#include <string>
void serial_send(const char* port, std::string message);
std::string serial_recv(const char* port);
#endif