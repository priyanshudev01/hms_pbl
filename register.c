/**
 * register.c - Patient registration with 7-digit ID generation
 * Hospital Management System
 * This module handles new patient registration
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

// Patient structure
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

// Function to generate a unique 7-digit ID
int generatePatientID() {
    FILE *file = fopen("data/patients.dat", "rb");
    int lastID = 1000000; // Starting ID number
    
    if (file != NULL) {
        Patient patient;
        
        // Go to the end of file
        fseek(file, 0, SEEK_END);
        
        // If the file is not empty
        if (ftell(file) > 0) {
            // Move back to the beginning of the last record
            fseek(file, -sizeof(Patient), SEEK_END);
            
            // Read the last record
            fread(&patient, sizeof(Patient), 1, file);
            
            // Get the last ID and increment
            lastID = patient.id + 1;
        }
        
        fclose(file);
    }
    
    return lastID;
}

// Function to check if username already exists
int isUsernameExists(const char *username) {
    FILE *file = fopen("data/patients.dat", "rb");
    
    if (file == NULL) {
        return 0; // File doesn't exist, so username doesn't exist
    }
    
    Patient patient;
    
    while (fread(&patient, sizeof(Patient), 1, file)) {
        if (strcmp(patient.username, username) == 0) {
            fclose(file);
            return 1; // Username exists
        }
    }
    
    fclose(file);
    return 0; // Username doesn't exist
}

// Function to register a new patient
void registerPatient() {
    Patient newPatient;
    FILE *file;
    
    // Initialize the structure with zeros to avoid garbage values
    memset(&newPatient, 0, sizeof(Patient));
    
    system("cls");
    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|           PATIENT REGISTRATION          |\n");
    printf("\t\t\t+========================================+\n\n");
    
    // Generate unique ID
    newPatient.id = generatePatientID();
    
    // Get patient details
    printf("\t\t\tPatient ID: %d (auto-generated)\n", newPatient.id);
    
    printf("\t\t\tFull Name: ");
    fflush(stdin);
    fgets(newPatient.name, sizeof(newPatient.name), stdin);
    newPatient.name[strcspn(newPatient.name, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tAge: ");
    scanf("%d", &newPatient.age);
    fflush(stdin); // Clear the input buffer after numeric input
    
    printf("\t\t\tGender (M/F/O): ");
    scanf(" %c", &newPatient.gender); // Note the space before %c to consume any whitespace
    fflush(stdin);
    
    printf("\t\t\tAddress: ");
    fgets(newPatient.address, sizeof(newPatient.address), stdin);
    newPatient.address[strcspn(newPatient.address, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tContact Number: ");
    fgets(newPatient.contact, sizeof(newPatient.contact), stdin);
    newPatient.contact[strcspn(newPatient.contact, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tEmail: ");
    fgets(newPatient.email, sizeof(newPatient.email), stdin);
    newPatient.email[strcspn(newPatient.email, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tBlood Group: ");
    fgets(newPatient.bloodGroup, sizeof(newPatient.bloodGroup), stdin);
    newPatient.bloodGroup[strcspn(newPatient.bloodGroup, "\n")] = 0;  // Remove newline character
    
    // Username and password
    do {
        printf("\t\t\tCreate Username: ");
        fgets(newPatient.username, sizeof(newPatient.username), stdin);
        newPatient.username[strcspn(newPatient.username, "\n")] = 0;  // Remove newline character
        
        if (isUsernameExists(newPatient.username)) {
            printf("\t\t\tUsername already exists. Please choose another.\n");
        }
    } while (isUsernameExists(newPatient.username));
    
    printf("\t\t\tCreate Password: ");
    fgets(newPatient.password, sizeof(newPatient.password), stdin);
    newPatient.password[strcspn(newPatient.password, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tMedical History (brief): ");
    fgets(newPatient.medicalHistory, sizeof(newPatient.medicalHistory), stdin);
    newPatient.medicalHistory[strcspn(newPatient.medicalHistory, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tEmergency Contact: ");
    fgets(newPatient.emergencyContact, sizeof(newPatient.emergencyContact), stdin);
    newPatient.emergencyContact[strcspn(newPatient.emergencyContact, "\n")] = 0;  // Remove newline character
    
    // Save to file
    ensureDataDirectoryExists();
    file = fopen("data/patients.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
      fwrite(&newPatient, sizeof(Patient), 1, file);
    fclose(file);
    
    printf("\n\t\t\tPatient registered successfully!\n");
    printf("\t\t\tYour Patient ID is: %d\n", newPatient.id);
    printf("\t\t\tUsername: %s\n", newPatient.username);
    printf("\t\t\tPassword: %s\n", newPatient.password);
    printf("\t\t\tPlease remember your username and password for login.\n");
}

// Function to list all registered patients (for debugging)
void listAllPatients() {
    FILE *file = fopen("data/patients.dat", "rb");
    
    if (file == NULL) {
        printf("\n\t\t\tNo registered patients found!\n");
        return;
    }
    
    Patient patient;
    int count = 0;
    
    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|           REGISTERED PATIENTS          |\n");
    printf("\t\t\t+========================================+\n\n");
    
    while (fread(&patient, sizeof(Patient), 1, file)) {
        printf("\t\t\tID: %d\n", patient.id);
        printf("\t\t\tName: %s\n", patient.name);
        printf("\t\t\tUsername: %s\n", patient.username);
        printf("\t\t\tPassword: %s\n", patient.password);
        printf("\t\t\t----------------------------------------\n");
        count++;
    }
    
    if (count == 0) {
        printf("\t\t\tNo patients found in the file.\n");
    } else {
        printf("\t\t\tTotal patients: %d\n", count);
    }
    
    fclose(file);
}
