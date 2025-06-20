# Hospital Management System

A simple, modular Hospital Management System built in C for educational purposes. This system demonstrates basic hospital management operations with separate modules for different functions.

## Features

- Patient registration with unique 7-digit ID generation
- Multiple user roles (Admin, Doctor, Staff, Patient) with separate login
- OPD appointment scheduling and management
- Pharmacy inventory and medicine dispensing
- Medical equipment tracking and assignment
- Blood bank with donation and request management
- Patient billing with detailed breakdown
- Binary file storage for all records

## File Structure

- `main.c` - Homepage UI (read-only access to modules)
- `register.c` - Patient registration with 7-digit ID generation
- `login.c` - Login for Patient, Doctor, and Staff
- `admin_login.c` - Separate Admin Login with hardcoded credentials
- `admin.c` - Admin dashboard for editing modules + adding doctors/staff
- `patient.c` - Patient dashboard (view linked info)
- `doctor.c` - Doctor dashboard (view patients, add notes)
- `staff.c` - Staff dashboard (view/edit shift info)
- `opd.c` - OPD appointments, tokens
- `pharmacy.c` - Medicine inventory + patient dispensing
- `equipment.c` - Medical equipment records
- `bloodbank.c` - Blood group stock and donations
- `billing.c` - Billing using linked records (admin-only)
- `data/*.dat` - Folder storing all binary records

## System Requirements

- C compiler (GCC recommended)
- Windows operating system (for console commands)

## Compilation

### Option 1: Using the Batch File (Recommended for Windows Users)

Simply double-click the `compile.bat` file or run it from the command prompt:

```
compile.bat
```

This batch file will check for GCC, compile the project, and offer to run it immediately.

### Option 2: Using the Makefile (If Make is installed)

```
make
```

### Option 3: Manual Compilation

```
gcc -Wall -Wextra -o hospital main.c register.c login.c admin_login.c admin.c patient.c doctor.c staff.c opd.c pharmacy.c equipment.c bloodbank.c billing.c
```

## Running the Program

```
./hospital
```

## Admin Credentials

- Username: admin
- Password: admin123

## Educational Purpose

This system is designed for educational purposes to help college students understand:

1. Modular C programming
2. Binary file handling
3. Structured programming concepts
4. Data relationships across modules
5. Role-based access control
6. Simple menu-based interfaces

## Development Notes

- Each module is kept under 400 lines for readability
- No header (.h) files are used to simplify understanding
- Binary file storage demonstrates file I/O operations
- Comments are included throughout the code for learning
- The system uses simple printf/scanf interfaces (no GUI)

## Disclaimer

This system is designed for educational purposes and is not intended for production use in actual medical facilities. Proper medical systems require robust security, data validation, and compliance with healthcare regulations.
