# Automated Garage Door System with ESP32

## Project Overview
This project showcases an automated garage door system controlled by an ESP32 microcontroller. The system allows for remote control via Bluetooth using a mobile application created in MIT App Inventor. Key components include a servo motor to open and close the door, an LDR (Light Dependent Resistor) to detect low-light conditions, LED indicators to show the door's status, and a manual push button for alternative control. The system is optimized to reduce energy consumption during servo operation.
![](ExquemaConexionado1.png)
## How It Works
- **Bluetooth Control**: Commands for opening and closing the door are sent from the mobile app to the ESP32 over Bluetooth.
- **Light Detection**: An LDR sensor detects when light is blocked (simulating a vehicle’s presence) and prevents the door from closing for safety.
- **LED Indicators**: A green LED shows when the door is open, and a red LED indicates when it is closed. If the door is blocked by low light, the red LED will flash.
- **Manual Control**: The push button can manually toggle the door open and closed.

## Repository Contents
- **Code Files**: In the folder `CódigoArduino IDE`, you’ll find all the Arduino IDE code files created during development, showing the step-by-step process leading to the final version. The complete code is in the file `PuertaGarajeFINAL.ino`.
- **Mobile App**: The application file (`app.apk`) can be installed on an Android phone to control the system remotely.
- **Connection Diagram**: `EsquemaConexionado1.png` illustrates the wiring and setup of the components used in the project.
- **Detailed Information**: For an in-depth explanation of the project, please refer to the PDF included in this repository.
