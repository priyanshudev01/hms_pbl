/**
 * doctor.c - Doctor dashboard (view patients, add notes)
 * Hospital Management System
 * This module provides doctor-specific functionalities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Structures
typedef struct {
    int id;
    char name[50];
    char specialization[50];
    char username[20];
    char password[20];
    char contact[15];
    char email[50];
    char schedule[100];
} Doctor;

typedef struct {
    int id;                 // Patient ID
    char name[50];          // Full name
    int age;                // Age
    char gender;            // M/F/O
    char bloodGroup[5];     // Blood group
    char medicalHistory[200]; // Brief medical history
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

// Function to get doctor data by ID
Doctor getDoctorById(int doctorID) {
    FILE *file = fopen("data/doctors.dat", "rb");
    Doctor doctor;
    
    if (file == NULL) {
        printf("\t\t\tError: Cannot open doctors file!\n");
        doctor.id = -1; // Invalid ID to indicate error
        return doctor;
    }
    
    while (fread(&doctor, sizeof(Doctor), 1, file)) {
        if (doctor.id == doctorID) {
            fclose(file);
            return doctor;
        }
    }
    
    fclose(file);
    doctor.id = -1; // Doctor not found
    return doctor;
}

// Function to get patient data by ID
Patient getPatientBasicInfo(int patientID) {
    FILE *file = fopen("data/patients.dat", "rb");
    Patient patient;
    
    if (file == NULL) {
        printf("\t\t\tError: Cannot open patients file!\n");
        patient.id = -1; // Invalid ID to indicate error
        return patient;
    }
    
    // Read patient record
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

// Function to view doctor profile
void viewDoctorProfile(int doctorID) {
    Doctor doctor = getDoctorById(doctorID);
    
    if (doctor.id == -1) {
        printf("\t\t\tDoctor not found!\n");
        return;
    }
      system("cls");
    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|             DOCTOR PROFILE                |\n");
    printf("\t\t\t+========================================+\n\n");
    
    printf("\t\t\tDoctor ID: %d\n", doctor.id);
    printf("\t\t\tName: %s\n", doctor.name);
    printf("\t\t\tSpecialization: %s\n", doctor.specialization);
    printf("\t\t\tContact: %s\n", doctor.contact);
    printf("\t\t\tEmail: %s\n", doctor.email);
    printf("\t\t\tSchedule: %s\n", doctor.schedule);
}

// Function to update doctor profile
void updateDoctorProfile(int doctorID) {
    FILE *file = fopen("data/doctors.dat", "rb+");
    Doctor doctor;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tError: Cannot open doctors file!\n");
        return;
    }
    
    while (fread(&doctor, sizeof(Doctor), 1, file)) {
        if (doctor.id == doctorID) {
            found = 1;
            
            system("cls");
            printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
            printf("\t\t\t║           UPDATE DOCTOR PROFILE           ║\n");
            printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
            
            printf("\t\t\tDoctor ID: %d (Cannot be changed)\n", doctor.id);
            printf("\t\t\tCurrent Contact: %s\n", doctor.contact);
            printf("\t\t\tNew Contact (press enter to keep current): ");
            fflush(stdin);
            char tempContact[15];
            gets(tempContact);
            if (strlen(tempContact) > 0) {
                strcpy(doctor.contact, tempContact);
            }
            
            printf("\t\t\tCurrent Email: %s\n", doctor.email);
            printf("\t\t\tNew Email (press enter to keep current): ");
            char tempEmail[50];
            gets(tempEmail);
            if (strlen(tempEmail) > 0) {
                strcpy(doctor.email, tempEmail);
            }
            
            printf("\t\t\tCurrent Schedule: %s\n", doctor.schedule);
            printf("\t\t\tNew Schedule (press enter to keep current): ");
            char tempSchedule[100];
            gets(tempSchedule);
            if (strlen(tempSchedule) > 0) {
                strcpy(doctor.schedule, tempSchedule);
            }
            
            printf("\t\t\tCurrent Password: %s\n", doctor.password);
            printf("\t\t\tNew Password (press enter to keep current): ");
            char tempPassword[20];
            gets(tempPassword);
            if (strlen(tempPassword) > 0) {
                strcpy(doctor.password, tempPassword);
            }
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(Doctor), SEEK_CUR);
            fwrite(&doctor, sizeof(Doctor), 1, file);
            
            printf("\n\t\t\tProfile updated successfully!\n");
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tDoctor not found!\n");
    }
}

// Function to view doctor's appointments
void viewDoctorAppointments(int doctorID) {
    FILE *file = fopen("data/appointments.dat", "rb");
    Appointment appointment;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo appointments found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║           YOUR APPOINTMENTS                ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\t%-5s %-7s %-15s %-12s %-12s %-15s\n", 
           "ID", "Patient", "Name", "Date", "Time", "Status");
    printf("\t\t\t----------------------------------------------------------------\n");
    
    while (fread(&appointment, sizeof(Appointment), 1, file)) {
        if (appointment.doctorID == doctorID) {
            // Get patient name
            Patient patient = getPatientBasicInfo(appointment.patientID);
            char patientName[50] = "Unknown";
            
            if (patient.id != -1) {
                strcpy(patientName, patient.name);
            }
            
            printf("\t\t\t%-5d %-7d %-15s %-12s %-12s %-15s\n", 
                   appointment.id, appointment.patientID, patientName,
                   appointment.date, appointment.time, appointment.status);
            found = 1;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tNo appointments found!\n");
    }
}

// Function to add diagnosis and prescription
void addDiagnosis(int doctorID) {
    FILE *file = fopen("data/appointments.dat", "rb+");
    Appointment appointment;
    int found = 0;
    int appointmentID;
    
    if (file == NULL) {
        printf("\t\t\tNo appointments found!\n");
        return;
    }
    
    // First display all appointments for this doctor
    viewDoctorAppointments(doctorID);
    
    printf("\n\t\t\tEnter Appointment ID to add diagnosis: ");
    scanf("%d", &appointmentID);
    
    // Reset file pointer to beginning
    rewind(file);
    
    while (fread(&appointment, sizeof(Appointment), 1, file)) {
        if (appointment.id == appointmentID && appointment.doctorID == doctorID) {
            found = 1;
            
            system("cls");
            printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
            printf("\t\t\t║              ADD DIAGNOSIS                ║\n");
            printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
            
            printf("\t\t\tAppointment ID: %d\n", appointment.id);
            printf("\t\t\tPatient ID: %d\n", appointment.patientID);
            printf("\t\t\tDate: %s\n", appointment.date);
            printf("\t\t\tSymptoms: %s\n", appointment.symptoms);
            
            printf("\n\t\t\tEnter Diagnosis: ");
            fflush(stdin);
            gets(appointment.diagnosis);
            
            printf("\t\t\tEnter Prescription: ");
            gets(appointment.prescription);
            
            strcpy(appointment.status, "completed");
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(Appointment), SEEK_CUR);
            fwrite(&appointment, sizeof(Appointment), 1, file);
            
            printf("\n\t\t\tDiagnosis and prescription added successfully!\n");
            
            // Now also add this as a prescription record
            FILE *presFile = fopen("data/prescriptions.dat", "ab");
            if (presFile != NULL) {
                Prescription newPres;
                
                // Find last prescription ID
                FILE *tempFile = fopen("data/prescriptions.dat", "rb");
                Prescription tempPres;
                int lastID = 0;
                
                if (tempFile != NULL) {
                    while (fread(&tempPres, sizeof(Prescription), 1, tempFile)) {
                        if (tempPres.id > lastID) {
                            lastID = tempPres.id;
                        }
                    }
                    fclose(tempFile);
                }
                
                newPres.id = lastID + 1;
                newPres.patientID = appointment.patientID;
                newPres.doctorID = doctorID;
                strcpy(newPres.date, appointment.date);
                strcpy(newPres.medicines, appointment.prescription);
                strcpy(newPres.dosage, "As directed by doctor");
                strcpy(newPres.status, "pending");
                
                fwrite(&newPres, sizeof(Prescription), 1, presFile);
                fclose(presFile);
                
                printf("\t\t\tPrescription record created!\n");
            }
            
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tAppointment not found or not assigned to you!\n");
    }
}

// Function to display doctor dashboard
void doctorDashboard(int doctorID) {
    int choice;
    
    do {
        system("cls");
        printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
        printf("\t\t\t║             DOCTOR DASHBOARD              ║\n");
        printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
        
        printf("\t\t\t1. View Profile\n");
        printf("\t\t\t2. Update Profile\n");
        printf("\t\t\t3. View Appointments\n");
        printf("\t\t\t4. Add Diagnosis & Prescription\n");
        printf("\t\t\t0. Logout\n\n");
        
        printf("\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewDoctorProfile(doctorID);
                break;
            case 2:
                updateDoctorProfile(doctorID);
                break;
            case 3:
                viewDoctorAppointments(doctorID);
                break;
            case 4:
                addDiagnosis(doctorID);
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
