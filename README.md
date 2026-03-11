# stepper-motor-simulation
# Interactive Stepper Motor Simulation: 1-Phase, 2-Phase, Half & Microstepping (Button Controlled)

![C](https://img.shields.io/badge/Language-C-blue.svg)
![MCU](https://img.shields.io/badge/MCU-STM32-orange.svg)
![Framework](https://img.shields.io/badge/Framework-STM32%20HAL-green.svg)

## About The Project
This repository contains the C source code (using STM32 HAL Library) for simulating and controlling a stepper motor. The project demonstrates four different excitation modes, allowing users to interactively switch between them using push buttons. Real-time parameters such as pulse count, revolution count, and step angles are displayed on a 16x2 LCD.

### Video Demonstration
> [Watch the simulation video on YouTube](https://youtu.be/_Q8YOo881Ik)

---

## Features
* **4 Driving Modes:**
    * **1-Phase On:** One stator electromagnet is energized at a time.
    * **Full Step (2-Phase On):** Two stators are energized simultaneously for higher torque.
    * **Half Step:** Alternates between 1-phase and 2-phase on to double the resolution.
    * **Microstepping:** Uses Timer 2 (TIM2) PWM signals to smoothly vary the current in the coils, providing ultra-smooth movement.
* **Interactive Control:** 4 push buttons to trigger and interrupt the respective motor modes.
* **Real-time Monitoring:** Interfaced with an I2C/Standard 16x2 LCD to display:
    * `So xung` (Pulse Count)
    * `So vong` (Revolution Count)
    * `Goc buoc` (Step Angle for Microstepping)

---

## Hardware & Pin Configuration (STM32)

### Stepper Motor Control
* **PA2, PA3, PA4, PA5:** Motor coil control pins (Output PP).
* **TIM2_CH1 & TIM2_CH2:** PWM outputs for Microstepping current control.

### User Interface (Push Buttons - Active Low / Pull-Up)
* **PA9:** Button 1 (Trigger 1-Phase On)
* **PA10:** Button 2 (Trigger 2-Phase On)
* **PA11:** Button 3 (Trigger Half-Step)
* **PA12:** Button 4 (Trigger Microstepping)

### Status Indicator
* **PB11:** Status LED.

---

## How to Run
1.  Open the project in **STM32CubeIDE** or **Keil uVision**.
2.  Compile and build the project.
3.  Flash the `.hex` or `.bin` file into your STM32 microcontroller.
4.  Connect the hardware (or run the simulation in Proteus) according to the pinout above.
5.  Press the buttons to observe the motor behavior and LCD outputs.
