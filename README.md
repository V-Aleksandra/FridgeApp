# Fridge Controller

This project simulates a fridge controller using a button, a LED, and a buzzer from the Sensor Kit.

## Description
The button from the Sensor Kit acts as a fridge door switch that signals the microcontroller when the fridge door is opened. The LED simulates the light inside the fridge, while the buzzer simulates the open door alarm.

## Features
- When the button is pressed, the LED is turned on, simulating the light inside the fridge.
- When the button is released, the LED turns off.
- If the button is pressed for more than 20 seconds, the alarm is activated to signal that the fridge door is open.
- During the activated alarm, the buzzer periodically turns on and off.

## Technical Details
The project is implemented using timer interrupt routines. The development board used is the "Tiva TM4C123G LaunchPad Evaluation Board (EK-TM4C123GXL)" with Seeed Arduino Sensor Kit Base, which integrates sensors and actuators.

## How to Use
1. Connect the development board to the Sensor Kit Base.
2. Program the microcontroller with the code from this repository.
3. Run the program on the development board.
4. Press the button to simulate opening the fridge door and test the functionality of the LED and buzzer.
