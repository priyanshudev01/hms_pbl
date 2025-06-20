/**
 * login.c - Login for Patient, Doctor, and Staff
 * Hospital Management System
 * This module handles user authentication for different roles
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <sys/stat.h>  // For mkdir

// Function to ensure data directory exists
static void ensureDataDirectoryExists() {
    struct stat st = {0};
    
    // Check if the directory exists
    if (stat("data", &st) == -1) {
        // Create the directory if it doesn't exist
        #ifdef _WIN32
            system("mkdir data");
        #else
            mkdir("data", 0700);
        #endif
        printf("\t\t\tCreated data directory\n");
    }
}

// Structures for different users
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
    int id;
    char name[50];
    char specialization[50];
    char username[20];
    char password[20];
    // Other doctor fields...
} Doctor;

typedef struct {
    int id;
    char name[50];
    char role[30];
    char username[20];
    char password[20];
    // Other staff fields...
} Staff;

// Function declarations for dashboards
void patientDashboard(int patientID);
void doctorDashboard(int doctorID);
void staffDashboard(int staffID);

// Function to mask password input
void getPassword(char* password) {
    char ch;
    int i = 0;
    
    while (1) {
        ch = getch();
        
        if (ch == 13) { // Enter key
            password[i] = '\0';
            printf("\n"); // Add a newline after password input
            break;
        } else if (ch == 8 && i > 0) { // Backspace
            i--;
            printf("\b \b");
        } else if (ch != 8 && i < 19) { // Limit password to 19 chars + null terminator
            password[i] = ch;
            printf("*");
            i++;
        }
    }
    
    printf("\t\t\tPassword entered: %s (Length: %d)\n", password, i); // Debug info
}

// Function to login users based on role
int loginUser(int role) {
    char username[20];
    char password[20];
    int found = 0;
    int userID = 0;
      system("cls");
    printf("\n\t\t\t+========================================+\n");
    
    if (role == 1) {
        printf("\t\t\t|              PATIENT LOGIN               |\n");
    } else if (role == 2) {
        printf("\t\t\t|              DOCTOR LOGIN                |\n");
    } else if (role == 3) {
        printf("\t\t\t|              STAFF LOGIN                 |\n");
    }
    
    printf("\t\t\t+========================================+\n\n");
      printf("\t\t\tUsername: ");
    fflush(stdin);
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tPassword: ");
    getPassword(password);
    
    FILE *file;    // Check login based on role
    if (role == 1) {
        Patient patient;
        ensureDataDirectoryExists();
        file = fopen("data/patients.dat", "rb");
        
        if (file == NULL) {
            printf("\n\t\t\tNo registered patients found!\n");
            return 0;
        }
        
        printf("\n\t\t\tAttempting to login with username: %s\n", username);
        
        while (fread(&patient, sizeof(Patient), 1, file)) {
            printf("\t\t\tChecking patient: ID=%d, Username=%s\n", patient.id, patient.username);
            
            if (strcmp(patient.username, username) == 0 && strcmp(patient.password, password) == 0) {
                found = 1;
                userID = patient.id;
                break;
            }
        }
        
        fclose(file);
        
        if (found) {
            printf("\n\t\t\tLogin successful!\n");
            patientDashboard(userID);
            return 1;
        }} else if (role == 2) {
        Doctor doctor;
        ensureDataDirectoryExists();
        file = fopen("data/doctors.dat", "rb");
        
        if (file == NULL) {
            printf("\n\t\t\tNo registered doctors found!\n");
            return 0;
        }
        
        while (fread(&doctor, sizeof(Doctor), 1, file)) {
            if (strcmp(doctor.username, username) == 0 && strcmp(doctor.password, password) == 0) {
                found = 1;
                userID = doctor.id;
                break;
            }
        }
        
        fclose(file);
        
        if (found) {
            printf("\n\t\t\tLogin successful!\n");
            doctorDashboard(userID);
            return 1;
        }    } else if (role == 3) {
        Staff staff;
        ensureDataDirectoryExists();
        file = fopen("data/staff.dat", "rb");
        
        if (file == NULL) {
            printf("\n\t\t\tNo registered staff found!\n");
            return 0;
        }
        
        while (fread(&staff, sizeof(Staff), 1, file)) {
            if (strcmp(staff.username, username) == 0 && strcmp(staff.password, password) == 0) {
                found = 1;
                userID = staff.id;
                break;
            }
        }
        
        fclose(file);
        
        if (found) {
            printf("\n\t\t\tLogin successful!\n");
            staffDashboard(userID);
            return 1;
        }
    }
    
    if (!found) {
        printf("\n\t\t\tInvalid username or password!\n");
        return 0;
    }
    
    return 0;
}
