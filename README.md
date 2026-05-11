MediCore Hospital Management System
Overview

MediCore is an advanced Hospital Management System (HMS) developed using C++ and Object-Oriented Programming (OOP) principles, featuring a graphical user interface (GUI) powered by SFML 3.0.2. The system supports three distinct user types: Patients, Doctors, and Admins, and efficiently manages appointments, prescriptions, billing, and medical records.

Key Features
Patient
Book appointments with doctors by specialization.
Cancel appointments with automatic refund processing.
View appointment history, medical records, and prescriptions.
Manage bills, including viewing and paying.
Top-up balance for additional services.
Doctor
View today's appointments schedule.
Mark appointments as completed or no-show.
Write prescriptions for completed appointments.
View patient medical history for better care management.
Admin
Manage user accounts: add/remove doctors and patients.
Monitor system activity: View all appointments, unpaid bills, and security logs.
Generate daily reports and discharge patients.
Full access to system data and logs for better administrative control.
Tech Stack
Programming Language: C++17
GUI Framework: SFML 3.0.2
Build System: CMake 3.28+
Platform: Windows
Project Structure
cmake-sfml-project/
├── main.cpp
├── Interface.h
├── Person.h
├── Patient.h
├── Doctor.h
├── Admin.h
├── Appointment.h
├── Bill.h
├── Prescription.h
├── Storage.h
├── FileHandler.h
├── Validators.h
├── Exceptions.h
├── plus.png
├── CMakeLists.txt
└── README.md
Data Files

All data is stored in CSV format, located in the same directory as the executable:

patients.txt
doctors.txt
appointments.txt
bills.txt
prescriptions.txt
admin.txt
security_log.txt
discharged.txt
Setup Instructions
Prerequisites
Visual Studio 2022
CMake 3.28+
Git
Active internet connection (for SFML auto-download)
Steps to Run
Clone/download the repository.
Open Visual Studio, and navigate to:
File → Open → Folder
Select the cmake-sfml-project folder.
Visual Studio will auto-detect CMakeLists.txt and configure the project.
Wait for CMake to finish configuring and downloading SFML.
From the top dropdown, select main.exe as the startup item.
Press the green play button or press F5 to build and run.
Command Line Alternative
cd cmake-sfml-project
cmake -B build
cmake --build build
cd build/bin
./main.exe
Default Login Credentials
Admin
ID: 1
Password: admin123
Sample Patient/Doctor (if created via Admin)
ID and password are set upon creation by the Admin.
Additional Notes
First-time Run: Data files are automatically generated.
Required Assets: plus.png and arial.ttf (Windows system font).
File Access: All files are read and written to/from the same directory as main.exe.
Memory Management: The system ensures no memory leaks and proper dynamic memory handling.
GitHub Repository
https://github.com/Zafeer-Zia/Hospital-management-system


