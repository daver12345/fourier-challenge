#include <iostream>
#include <fcntl.h>
#include <unistd.h>

void serial_send(const char* port, std::string message) {
    // NOCTTY to prevent port from become controlling terminal, i.e. so Ctrl-C works
    // SYNC to make sure data is fully written before sent
    int fd = open(port, O_WRONLY | O_NOCTTY | O_SYNC);
    if (fd == -1) {
        std::cerr << "Failed to open port " << port << std::endl;
        return;
    }

    const char* c_message = message.c_str();
    write(fd, c_message, strlen(c_message));
    close(fd);
}

std::string serial_recv(const char* port) {
    // NOCTTY to prevent port from become controlling terminal, i.e. so Ctrl-C works
    int fd = open(port, O_RDONLY | O_NOCTTY);
    if (fd == -1) {
        std::cerr << "Failed to open port " << port << std::endl;
        return "";
    }

    char buf[256];
    int n = read(fd, buf, sizeof(buf));
    close(fd);
    return std::string(buf, n);
}