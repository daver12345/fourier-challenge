#include <thread>
#include "controller.h"
#include "simulator.h"

int main() {
    std::thread controller_thread(run_controller);
    std::thread simulator_thread(run_simulator);

    controller_thread.join();
    simulator_thread.join();
    return 0;
}