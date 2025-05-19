VendingMachine
📦 Project Description
This project simulates the operation of a vending machine for selling beverages. It allows user interaction via an HW-834 keypad and an LCD1602A display, as well as integration with an application built in the LabVIEW environment.
The FRDM-KL05Z microcontroller, programmed in C using Keil uVision5, reads data from the keypad and transmits it to the LabVIEW application.

🎮 Features
Product selection simulation:
The user can select a product using the keypad, which is read by the microcontroller and sent to the LabVIEW application.

Dynamic product prices and positions:
The LabVIEW interface allows the user to freely change the prices and positions of available products.

Coin insertion simulation:
The user can simulate inserting specific coin denominations into the machine.

Product dispensing and change calculation:
If the inserted amount is sufficient, the machine dispenses the selected product and calculates and returns the correct change.

Information display:
All data, such as balance, product price, and error messages, are displayed on the LCD1602A screen.

🧰 Hardware & Software Requirements
Microcontroller: FRDM-KL05Z

Keypad: HW-834

Display: LCD1602A

IDE: Keil uVision5

Programming Language: C

Visualization Software: LabVIEW

🗂 Repository Structure
VendingMachine_KL05Z/ – Source code for the FRDM-KL05Z microcontroller

VendingMachine_LabView/ – LabVIEW project files responsible for the user interface and logic

How_It_Works.mov – A short demo video showcasing the system in action

README.md – This project description file

🎥 Demo
Watch the demonstration here:
▶️ https://www.youtube.com/shorts/rX9KoflGtr0

📄 License
This project is licensed under the MIT License.
