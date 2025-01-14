#include <iostream>
#include <fcntl.h>
#include <unistd.h>

void serial_send(const char* port, std::string message) {
    // NOCTTY to prevent port from become controlling terminal, i.e. so Ctrl-C works
    // SYNC to make sure data is fully written before sent
    int file_desc = open(port, O_WRONLY | O_NOCTTY | O_SYNC);
    if (file_desc == -1) {
        std::cerr << "Error: Failed to open port " << port << std::endl;
        exit(EXIT_FAILURE);
    }

    const char* c_message = message.c_str();
    write(file_desc, c_message, strlen(c_message));
    close(file_desc);
}

auto serial_recv(const char* port) -> std::string {
    // NOCTTY to prevent port from become controlling terminal, i.e. so Ctrl-C works
    int file_desc = open(port, O_RDONLY | O_NOCTTY);
    if (file_desc == -1) {
        std::cerr << "Error: Failed to open port " << port << std::endl;
        exit(EXIT_FAILURE);
    }

    constexpr int BUFFER_SIZE = 256;

    std::array<char, BUFFER_SIZE> buf;
    int bytes_read = read(file_desc, buf.data(), buf.size());
    close(file_desc);
    return {buf.data(), static_cast<size_t>(bytes_read)};
}