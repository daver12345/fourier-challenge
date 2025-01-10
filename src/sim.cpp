#include "controller.h"
#include "simulator.h"
#include <thread>

int main() {
    std::thread thread1(print1);
    std::thread thread2(print2);

    thread1.join();
    thread2.join();
    return 0;
}