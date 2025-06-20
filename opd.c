/**
 * opd.c - OPD appointments, tokens
 * Hospital Management System
 * This module handles outpatient department functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

// Structures
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
    int tokenNumber;        // OPD token number
} Appointment;

typedef struct {
    int id;
    char name[50];
    char specialization[50];
} Doctor;

typedef struct {
    int id;
    char name[50];
} Patient;

// Function to get doctor list
void getDoctorList() {
    FILE *file = fopen("data/doctors.dat", "rb");
    Doctor doctor;
    
    if (file == NULL) {
        printf("\t\t\tNo doctors found!\n");
        return;
    }
    
    printf("\t\t\t%-5s %-30s %-20s\n", "ID", "Name", "Specialization");
    printf("\t\t\t--------------------------------------------------\n");
    
    while (fread(&doctor, sizeof(Doctor), 1, file)) {
        printf("\t\t\t%-5d %-30s %-20s\n", doctor.id, doctor.name, doctor.specialization);
    }
    
    fclose(file);
}

// Function to get doctor name by ID
char* getDoctorName(int doctorID) {
    static char name[50] = "Unknown";
    FILE *file = fopen("data/doctors.dat", "rb");
    Doctor doctor;
    
    if (file == NULL) {
        return name;
    }
    
    while (fread(&doctor, sizeof(Doctor), 1, file)) {
        if (doctor.id == doctorID) {
            strcpy(name, doctor.name);
            break;
        }
    }
    
    fclose(file);
    return name;
}

// Function to get patient name by ID
char* getPatientName(int patientID) {
    static char name[50] = "Unknown";
    FILE *file = fopen("data/patients.dat", "rb");
    Patient patient;
    
    if (file == NULL) {
        return name;
    }
    
    while (fread(&patient, sizeof(Patient), 1, file)) {
        if (patient.id == patientID) {
            strcpy(name, patient.name);
            break;
        }
    }
    
    fclose(file);
    return name;
}

// Function to generate token number
int generateTokenNumber(const char *date) {
    FILE *file = fopen("data/appointments.dat", "rb");
    Appointment appointment;
    int maxToken = 0;
    
    if (file == NULL) {
        return 1; // First token of the day
    }
    
    while (fread(&appointment, sizeof(Appointment), 1, file)) {
        if (strcmp(appointment.date, date) == 0 && appointment.tokenNumber > maxToken) {
            maxToken = appointment.tokenNumber;
        }
    }
    
    fclose(file);
    return maxToken + 1;
}

// Function to view OPD records (for all users)
void viewOPDRecords() {
    FILE *file = fopen("data/appointments.dat", "rb");
    Appointment appointment;
    
    if (file == NULL) {
        printf("\t\t\tNo OPD records found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║                OPD RECORDS                 ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\t%-5s %-10s %-15s %-15s %-10s %-15s\n", 
           "ID", "Token", "Date", "Time", "Status", "Doctor");
    printf("\t\t\t----------------------------------------------------------------\n");
    
    while (fread(&appointment, sizeof(Appointment), 1, file)) {
        char doctorName[50];
        strcpy(doctorName, getDoctorName(appointment.doctorID));
        
        printf("\t\t\t%-5d %-10d %-15s %-15s %-10s %-15s\n", 
               appointment.id, appointment.tokenNumber, appointment.date, 
               appointment.time, appointment.status, doctorName);
    }
    
    fclose(file);
}

// Function to add a new appointment (for admin)
void addAppointment() {
    Appointment newAppointment;
    FILE *file;
    int lastID = 0;
    
    // Get last appointment ID
    file = fopen("data/appointments.dat", "rb");
    if (file != NULL) {
        Appointment temp;
        while (fread(&temp, sizeof(Appointment), 1, file)) {
            if (temp.id > lastID) {
                lastID = temp.id;
            }
        }
        fclose(file);
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║            ADD NEW APPOINTMENT            ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    // Generate new ID
    newAppointment.id = lastID + 1;
    
    // Get patient ID
    printf("\t\t\tEnter Patient ID: ");
    scanf("%d", &newAppointment.patientID);
    
    // Show doctor list and get doctor ID
    printf("\n\t\t\tAvailable Doctors:\n");
    getDoctorList();
    printf("\n\t\t\tEnter Doctor ID: ");
    scanf("%d", &newAppointment.doctorID);
    
    // Get appointment date
    printf("\t\t\tEnter Appointment Date (DD/MM/YYYY): ");
    fflush(stdin);
    gets(newAppointment.date);
    
    // Get appointment time
    printf("\t\t\tEnter Appointment Time (HH:MM): ");
    gets(newAppointment.time);
    
    // Generate token number
    newAppointment.tokenNumber = generateTokenNumber(newAppointment.date);
    
    // Get symptoms
    printf("\t\t\tEnter Patient's Symptoms: ");
    gets(newAppointment.symptoms);
    
    // Set initial values
    strcpy(newAppointment.status, "scheduled");
    strcpy(newAppointment.diagnosis, "");
    strcpy(newAppointment.prescription, "");
    
    // Save to file
    file = fopen("data/appointments.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    fwrite(&newAppointment, sizeof(Appointment), 1, file);
    fclose(file);
    
    printf("\n\t\t\tAppointment added successfully!\n");
    printf("\t\t\tToken Number: %d\n", newAppointment.tokenNumber);
}

// Function to update appointment status (for admin)
void updateAppointmentStatus() {
    FILE *file = fopen("data/appointments.dat", "rb+");
    Appointment appointment;
    int appointmentID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo appointments found!\n");
        return;
    }
    
    // First display all appointments
    viewOPDRecords();
    
    printf("\n\t\t\tEnter Appointment ID to update status: ");
    scanf("%d", &appointmentID);
    
    while (fread(&appointment, sizeof(Appointment), 1, file)) {
        if (appointment.id == appointmentID) {
            found = 1;
            
            printf("\n\t\t\tCurrent Status: %s\n", appointment.status);
            printf("\t\t\tChoose New Status:\n");
            printf("\t\t\t1. Scheduled\n");
            printf("\t\t\t2. Completed\n");
            printf("\t\t\t3. Cancelled\n");
            printf("\t\t\tEnter choice: ");
            
            int choice;
            scanf("%d", &choice);
            
            switch(choice) {
                case 1:
                    strcpy(appointment.status, "scheduled");
                    break;
                case 2:
                    strcpy(appointment.status, "completed");
                    break;
                case 3:
                    strcpy(appointment.status, "cancelled");
                    break;
                default:
                    printf("\t\t\tInvalid choice. Status not updated.\n");
                    fclose(file);
                    return;
            }
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(Appointment), SEEK_CUR);
            fwrite(&appointment, sizeof(Appointment), 1, file);
            
            printf("\n\t\t\tAppointment status updated successfully!\n");
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tAppointment not found!\n");
    }
}

// Function to delete appointment (for admin)
void deleteAppointment() {
    FILE *file = fopen("data/appointments.dat", "rb");
    FILE *tempFile = fopen("data/temp.dat", "wb");
    Appointment appointment;
    int appointmentID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo appointments found!\n");
        return;
    }
    
    // First display all appointments
    viewOPDRecords();
    
    printf("\n\t\t\tEnter Appointment ID to delete: ");
    scanf("%d", &appointmentID);
    
    while (fread(&appointment, sizeof(Appointment), 1, file)) {
        if (appointment.id == appointmentID) {
            found = 1;
            printf("\n\t\t\tAppointment with ID %d will be deleted!\n", appointmentID);
        } else {
            fwrite(&appointment, sizeof(Appointment), 1, tempFile);
        }
    }
    
    fclose(file);
    fclose(tempFile);
    
    if (found) {
        remove("data/appointments.dat");
        rename("data/temp.dat", "data/appointments.dat");
        printf("\t\t\tAppointment deleted successfully!\n");
    } else {
        remove("data/temp.dat");
        printf("\t\t\tAppointment not found!\n");
    }
}

// Function to display OPD edit menu (for admin)
void editOPDMenu() {
    int choice;
    
    do {
        system("cls");
        printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
        printf("\t\t\t║               OPD MANAGEMENT              ║\n");
        printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
        
        printf("\t\t\t1. View All OPD Records\n");
        printf("\t\t\t2. Add New Appointment\n");
        printf("\t\t\t3. Update Appointment Status\n");
        printf("\t\t\t4. Delete Appointment\n");
        printf("\t\t\t0. Back to Admin Dashboard\n\n");
        
        printf("\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewOPDRecords();
                break;
            case 2:
                addAppointment();
                break;
            case 3:
                updateAppointmentStatus();
                break;
            case 4:
                deleteAppointment();
                break;
            case 0:
                return;
            default:
                printf("\t\t\tInvalid choice. Please try again.\n");
        }
        
        if(choice != 0) {
            printf("\n\t\t\tPress any key to continue...");
            getch();
        }
        
    } while(choice != 0);
}
