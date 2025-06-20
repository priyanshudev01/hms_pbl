/**
 * patient.c - Patient dashboard (view linked info)
 * Hospital Management System
 * This module provides patient-specific functionalities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Structures
typedef struct {
    int id;                 // 7-digit unique ID
    char name[50];          // Full name
    int age;                // Age
    char gender;            // M/F/O
    char address[100];      // Address
    char contact[15];       // Contact number
    char email[50];         // Email address
    char bloodGroup[5];     // Blood group
    char username[20];      // Login username
    char password[20];      // Login password
    char medicalHistory[200]; // Brief medical history
    char emergencyContact[50]; // Emergency contact
} Patient;

typedef struct {
    int id;                 // Appointment ID
    int patientID;          // Patient ID (foreign key)
    int doctorID;           // Doctor ID (foreign key)
    char date[15];          // Date of appointment
    char time[10];          // Time of appointment
    char status[20];        // Status (scheduled, completed, cancelled)
    char symptoms[100];     // Patient's symptoms
    char diagnosis[200];    // Doctor's diagnosis
    char prescription[300]; // Prescribed medicines
} Appointment;

typedef struct {
    int id;                 // Prescription ID
    int patientID;          // Patient ID (foreign key)
    int doctorID;           // Doctor ID (foreign key)
    char date[15];          // Date of prescription
    char medicines[300];    // List of medicines
    char dosage[200];       // Dosage instructions
    char status[20];        // Status (pending, dispensed)
} Prescription;

// Function to get patient data by ID
Patient getPatientById(int patientID) {
    FILE *file = fopen("data/patients.dat", "rb");
    Patient patient;
    
    if (file == NULL) {
        printf("\t\t\tError: Cannot open patients file!\n");
        patient.id = -1; // Invalid ID to indicate error
        return patient;
    }
    
    while (fread(&patient, sizeof(Patient), 1, file)) {
        if (patient.id == patientID) {
            fclose(file);
            return patient;
        }
    }
    
    fclose(file);
    patient.id = -1; // Patient not found
    return patient;
}

// Function to view patient profile
void viewPatientProfile(int patientID) {
    Patient patient = getPatientById(patientID);
    
    if (patient.id == -1) {
        printf("\t\t\tPatient not found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║             PATIENT PROFILE               ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\tPatient ID: %d\n", patient.id);
    printf("\t\t\tName: %s\n", patient.name);
    printf("\t\t\tAge: %d\n", patient.age);
    printf("\t\t\tGender: %c\n", patient.gender);
    printf("\t\t\tBlood Group: %s\n", patient.bloodGroup);
    printf("\t\t\tAddress: %s\n", patient.address);
    printf("\t\t\tContact: %s\n", patient.contact);
    printf("\t\t\tEmail: %s\n", patient.email);
    printf("\t\t\tMedical History: %s\n", patient.medicalHistory);
    printf("\t\t\tEmergency Contact: %s\n", patient.emergencyContact);
}

// Function to update patient profile
void updatePatientProfile(int patientID) {
    FILE *file = fopen("data/patients.dat", "rb+");
    Patient patient;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tError: Cannot open patients file!\n");
        return;
    }
    
    while (fread(&patient, sizeof(Patient), 1, file)) {
        if (patient.id == patientID) {
            found = 1;
            
            system("cls");
            printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
            printf("\t\t\t║          UPDATE PATIENT PROFILE           ║\n");
            printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
            
            printf("\t\t\tPatient ID: %d (Cannot be changed)\n", patient.id);
            printf("\t\t\tCurrent Contact: %s\n", patient.contact);
            printf("\t\t\tNew Contact (press enter to keep current): ");
            fflush(stdin);
            char tempContact[15];
            gets(tempContact);
            if (strlen(tempContact) > 0) {
                strcpy(patient.contact, tempContact);
            }
            
            printf("\t\t\tCurrent Email: %s\n", patient.email);
            printf("\t\t\tNew Email (press enter to keep current): ");
            char tempEmail[50];
            gets(tempEmail);
            if (strlen(tempEmail) > 0) {
                strcpy(patient.email, tempEmail);
            }
            
            printf("\t\t\tCurrent Address: %s\n", patient.address);
            printf("\t\t\tNew Address (press enter to keep current): ");
            char tempAddress[100];
            gets(tempAddress);
            if (strlen(tempAddress) > 0) {
                strcpy(patient.address, tempAddress);
            }
            
            printf("\t\t\tCurrent Emergency Contact: %s\n", patient.emergencyContact);
            printf("\t\t\tNew Emergency Contact (press enter to keep current): ");
            char tempEmergency[50];
            gets(tempEmergency);
            if (strlen(tempEmergency) > 0) {
                strcpy(patient.emergencyContact, tempEmergency);
            }
            
            printf("\t\t\tCurrent Password: %s\n", patient.password);
            printf("\t\t\tNew Password (press enter to keep current): ");
            char tempPassword[20];
            gets(tempPassword);
            if (strlen(tempPassword) > 0) {
                strcpy(patient.password, tempPassword);
            }
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(Patient), SEEK_CUR);
            fwrite(&patient, sizeof(Patient), 1, file);
            
            printf("\n\t\t\tProfile updated successfully!\n");
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tPatient not found!\n");
    }
}

// Function to view patient appointments
void viewPatientAppointments(int patientID) {
    FILE *file = fopen("data/appointments.dat", "rb");
    Appointment appointment;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo appointments found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║            YOUR APPOINTMENTS              ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\t%-5s %-12s %-12s %-15s %-15s\n", "ID", "Date", "Time", "Status", "Symptoms");
    printf("\t\t\t--------------------------------------------------------\n");
    
    while (fread(&appointment, sizeof(Appointment), 1, file)) {
        if (appointment.patientID == patientID) {
            printf("\t\t\t%-5d %-12s %-12s %-15s %-15s\n", 
                   appointment.id, appointment.date, appointment.time, 
                   appointment.status, appointment.symptoms);
            found = 1;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tNo appointments found!\n");
    }
}

// Function to view patient prescriptions
void viewPatientPrescriptions(int patientID) {
    FILE *file = fopen("data/prescriptions.dat", "rb");
    Prescription prescription;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo prescriptions found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║            YOUR PRESCRIPTIONS             ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\t%-5s %-12s %-15s %-15s\n", "ID", "Date", "Status", "Medicines");
    printf("\t\t\t--------------------------------------------------\n");
    
    while (fread(&prescription, sizeof(Prescription), 1, file)) {
        if (prescription.patientID == patientID) {
            printf("\t\t\t%-5d %-12s %-15s %-15s\n", 
                   prescription.id, prescription.date, 
                   prescription.status, prescription.medicines);
            
            printf("\t\t\tDosage: %s\n\n", prescription.dosage);
            found = 1;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tNo prescriptions found!\n");
    }
}

// Function to display patient dashboard
void patientDashboard(int patientID) {
    int choice;
    
    do {
        system("cls");
        printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
        printf("\t\t\t║            PATIENT DASHBOARD              ║\n");
        printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
        
        printf("\t\t\t1. View Profile\n");
        printf("\t\t\t2. Update Profile\n");
        printf("\t\t\t3. View Appointments\n");
        printf("\t\t\t4. View Prescriptions\n");
        printf("\t\t\t0. Logout\n\n");
        
        printf("\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewPatientProfile(patientID);
                break;
            case 2:
                updatePatientProfile(patientID);
                break;
            case 3:
                viewPatientAppointments(patientID);
                break;
            case 4:
                viewPatientPrescriptions(patientID);
                break;
            case 0:
                printf("\n\t\t\tLogging out...\n");
                break;
            default:
                printf("\t\t\tInvalid choice. Please try again.\n");
        }
        
        if(choice != 0) {
            printf("\n\t\t\tPress any key to continue...");
            getch();
        }
        
    } while(choice != 0);
}
