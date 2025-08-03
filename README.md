# Digital-Ludo
An embedded Ludo game inspired by Ludo King — built using microcontrollers, real hardware, and pure Logic Result: magic!


# 🎮 Digital Ludo - Embedded Multiplayer Game Board

This is a unique **embedded-based Ludo game** built entirely using microcontrollers and custom hardware. Inspired by the digital Ludo experience from mobile apps like **Ludo King**, this version allows **up to 4 players** to play on a physical board with automatic move handling and display.

---

## 📌 Features

- Playable by **2–4 players** just like the mobile version
- **Custom embedded hardware design**
- Fully standalone – no PC or smartphone required
- Real-time tracking of player turns and dice logic
- Visual feedback using LEDs or displays
- Game logic implemented in C for microcontroller

---

## 🧰 Technologies & Components Used

- **Microcontroller**: 8051 (or specify which one you used)
- **Programming Language**: Embedded C
- **Display**: 7-segment for Dice Role, RGB Led Matrix for Tokens
- **Inputs**: 4 push for Every Player , for Roling the dice and For Token Movements
- **Outputs**: RGB Leds for Tokens and Different Buzzer sound for Dice roll and Token movement
- **Tools**: Keil uVision 5

---

## 👨‍🔧 My Role

- Designed the **entire circuit** from scratch
- Wrote the full **firmware in Embedded C**
- Built the complete **hardware on a Zero PCB And Using Acrylic Sheets**
- Debugged and tested the gameplay logic

---

## 📷 Images / Demo

## 🧠 Hardware Logic Architecture

Below is the hardware logic architecture of the Digital Ludo board.  
It shows how rows and columns are wired to detect player positions and movements.

<p align="center">
  <img src="https://raw.githubusercontent.com/brajeshimas/digital-ludo/main/images/LudoRowAndColumn.png" alt="Ludo Hardware Architecture" width="600"/>
</p>

## 🏗️ Development Phases

Here are some development phase photos showing how the Digital Ludo project evolved from basic wiring to a working prototype.

<p align="center">
  <img src="https://raw.githubusercontent.com/brajeshimas/digital-ludo/main/images/Dev_phase_1.jpg" alt="Development Phase 1" width="400"/>
  <br/><em>Phase 1: Early breadboard wiring and Harnesing</em>
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/brajeshimas/digital-ludo/main/images/Dev_phase_2.jpg" alt="Development Phase 2" width="400"/>
  <br/><em>Phase 2: Code writing and Testing</em>
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/brajeshimas/digital-ludo/main/images/Dev_phase_3.jpg" alt="Development Phase 3" width="400"/>
  <br/><em>Phase 3: All Assembled and Stickers are sticked perfectly</em>
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/brajeshimas/digital-ludo/main/images/Dev_phase_5.jpg" alt="Development Phase 3" width="400"/>
  <br/><em>Phase 3: Final Testing</em>
</p>

---

## 📁 Folder Structure

This project is organized into three main code sections:

- `HardwareCode/` – Contains low-level embedded drivers (GPIO, EEPROM, SPI, etc.)
- `UserCode/` – Application-level logic like dice control, player rules, LED matrix
- `KeilProject/` – Keil uVision5 project files and workspace
- `images/` – Development phase images and hardware architecture diagrams

