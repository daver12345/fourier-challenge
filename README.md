# fourier-challenge

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
    -   You can use a simple model (e.g., a linear function) to simulate temperature changes: T(t)=T(t−1)+(pump is ON?V×dt:−V×dt)
    -   Document your assumptions, formulas, and any simplifications made.