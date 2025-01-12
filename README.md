# Documentation

**Compile/Run Instructions**
- Link `socat` ports as described below
- `make run`

**Controller**
The commands from the pump controller are `READ`,  `ON`, and `OFF`.
- This `READ` command currently has no function other than printing to console, as there are no fault signals described in the writeup.
- The `ON` and `OFF` commands are set by the temperature logic, so the pump controller will send whichever command has been determined previously.

The only command from the temperature sensor is `READ`.
- The sensor `READ`s the current temperature and turns the cooling pump `ON` if above the target temperature, and `OFF` if below the target temperature.
- The sensor will assume the temperature remained the same if the reading is invalid.

**Simulator**
 When the simulator receives a `READ` on the pump port, it will send back the current state to the pump controller. On the reception of an `ON` or `OFF`, the pump will change its state accordingly.
 
 When the simulator receives a `READ` on the temperature port, it will send back the current temperature to the sensor, as well as update the current temperature based on the pump state. This follows a similar linear function as the instructions.
-  T(t) = T(t-1) + (pump is ON) ? -1 : 1
- dt is accounted for here as we assume the temperature sensor will send a read once a second, so the temperature will update every second.

Note that the values have been flipped from the given formula, as a cooling pump is assumed to cool the water down when `ON` and let it warm when `OFF`.

**Serial Communication**
The controller and simulator will communicate via two virtual TTY pairs created by `socat` for the pump communication and temperature sensor communication, respectively. They are instantiated like so:

`socat -d -d PTY,link=/tmp/ttyV0,raw,echo=0 PTY,link=/tmp/ttyV1,raw,echo=0`

`socat -d -d PTY,link=/tmp/ttyV2,raw,echo=0 PTY,link=/tmp/ttyV3,raw,echo=0`

These TTY pairs simulate a full-duplex serial port, where the controller can do read/writes at the same time as the simulator. These can now be used as normal ports by opening the file descriptor as you would a regular serial port.

The controller wakes up every 200ms and 1s, sending a command to the simulator to retrieve the most up to date values of pump state and temperature. 
- This design was decided on because having the simulator constantly sending pump state and temperature values would a) be inefficent and b) fill the buffer, so the controller couldn't fetch the most up to date values.
- This is similar to many ready/valid handshake protocols, where the controller asserts a ready singal (like our `READ`) before the peripheral sends its data.

# Fourier Controller/Simulator Challenge Instructions

**Goal**  
Develop two programs:
1.  **Controller (A)**: Controls a cooling pump to maintain a specified temperature.
2.  **Simulator (B)**: Simulates both the pump and a temperature sensor.
    

You may run both (A) and (B) in the same process for simplicity.

**Controller (A) Requirements**

-   **Pump Communication**
    -   Communicate over a serial port.
    -   Perform the following read/write operations every 200 ms:
        -   Send commands to turn the pump on or off.
        -   Read the pump state (on/off) as well as any fault signal.
            
-   **Temperature Sensor Communication**
    -   Communicate over a separate serial port.
    -   Read the sensor temperature every 1 second.
    -   Based on the target temperature, decide whether to turn the pump on/off.
    -   Note that pump communication is done at a higher frequency (200 ms). It is intended to detect faults early.
        
-   **Communication Formats & Documentation**
    -   You are free to define the serial communication protocols and message formats.
    -   Please document your design choices, including the structure of your messages.
        

**Simulator (B) Requirements**

-   **Pump & Sensor Simulation**
    -   Behave like a physical pump and a temperature sensor, responding over the same interfaces.
    -   You can use a simple model (e.g., a linear function) to simulate temperature changes: T(t) = T(t−1) + (pump is ON ? V×dt : −V×dt)
    -   Document your assumptions, formulas, and any simplifications made.