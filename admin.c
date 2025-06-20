/**
 * admin.c - Admin dashboard for editing modules + adding doctors/staff
 * Hospital Management System
 * This module provides admin functionalities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <sys/stat.h>  // For mkdir

// User structures
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
    int id;
    char name[50];
    char role[30];
    char username[20];
    char password[20];
    char contact[15];
    char email[50];
    char shift[20];
} Staff;

// Function declarations from other modules
void editOPDMenu();
void editPharmacyMenu();
void editEquipmentMenu();
void editBloodBankMenu();
void viewBillingMenu();

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

// Function to generate a doctor ID
int generateDoctorID() {
    FILE *file = fopen("data/doctors.dat", "rb");
    int lastID = 5000; // Starting ID for doctors
    
    if (file != NULL) {
        Doctor doctor;
        
        // Go to the end of file
        fseek(file, 0, SEEK_END);
        
        // If the file is not empty
        if (ftell(file) > 0) {
            // Move back to the beginning of the last record
            fseek(file, -sizeof(Doctor), SEEK_END);
            
            // Read the last record
            fread(&doctor, sizeof(Doctor), 1, file);
            
            // Get the last ID and increment
            lastID = doctor.id + 1;
        }
        
        fclose(file);
    }
    
    return lastID;
}

// Function to generate a staff ID
int generateStaffID() {
    FILE *file = fopen("data/staff.dat", "rb");
    int lastID = 8000; // Starting ID for staff
    
    if (file != NULL) {
        Staff staff;
        
        // Go to the end of file
        fseek(file, 0, SEEK_END);
        
        // If the file is not empty
        if (ftell(file) > 0) {
            // Move back to the beginning of the last record
            fseek(file, -sizeof(Staff), SEEK_END);
            
            // Read the last record
            fread(&staff, sizeof(Staff), 1, file);
            
            // Get the last ID and increment
            lastID = staff.id + 1;
        }
        
        fclose(file);
    }
    
    return lastID;
}

// Function to check if username already exists for doctors
int isDoctorUsernameExists(const char *username) {
    FILE *file = fopen("data/doctors.dat", "rb");
    
    if (file == NULL) {
        return 0; // File doesn't exist, so username doesn't exist
    }
    
    Doctor doctor;
    
    while (fread(&doctor, sizeof(Doctor), 1, file)) {
        if (strcmp(doctor.username, username) == 0) {
            fclose(file);
            return 1; // Username exists
        }
    }
    
    fclose(file);
    return 0; // Username doesn't exist
}

// Function to check if username already exists for staff
int isStaffUsernameExists(const char *username) {
    FILE *file = fopen("data/staff.dat", "rb");
    
    if (file == NULL) {
        return 0; // File doesn't exist, so username doesn't exist
    }
    
    Staff staff;
    
    while (fread(&staff, sizeof(Staff), 1, file)) {
        if (strcmp(staff.username, username) == 0) {
            fclose(file);
            return 1; // Username exists
        }
    }
    
    fclose(file);
    return 0; // Username doesn't exist
}

// Function to add a new doctor
void addDoctor() {
    Doctor newDoctor;
    FILE *file;
    
    // Initialize the structure with zeros to avoid garbage values
    memset(&newDoctor, 0, sizeof(Doctor));
    
    system("cls");    
    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|             ADD NEW DOCTOR                |\n");
    printf("\t\t\t+========================================+\n\n");
    
    // Generate unique ID
    newDoctor.id = generateDoctorID();
    
    // Get doctor details
    printf("\t\t\tDoctor ID: %d (auto-generated)\n", newDoctor.id);
    
    printf("\t\t\tFull Name: ");
    fflush(stdin);
    fgets(newDoctor.name, sizeof(newDoctor.name), stdin);
    newDoctor.name[strcspn(newDoctor.name, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tSpecialization: ");
    fgets(newDoctor.specialization, sizeof(newDoctor.specialization), stdin);
    newDoctor.specialization[strcspn(newDoctor.specialization, "\n")] = 0;  // Remove newline character
      printf("\t\t\tContact Number: ");
    fgets(newDoctor.contact, sizeof(newDoctor.contact), stdin);
    newDoctor.contact[strcspn(newDoctor.contact, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tEmail: ");
    fgets(newDoctor.email, sizeof(newDoctor.email), stdin);
    newDoctor.email[strcspn(newDoctor.email, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tSchedule (Days & Hours): ");
    fgets(newDoctor.schedule, sizeof(newDoctor.schedule), stdin);
    newDoctor.schedule[strcspn(newDoctor.schedule, "\n")] = 0;  // Remove newline character
      // Username and password
    do {
        printf("\t\t\tCreate Username: ");
        fgets(newDoctor.username, sizeof(newDoctor.username), stdin);
        newDoctor.username[strcspn(newDoctor.username, "\n")] = 0;  // Remove newline character
        
        if (isDoctorUsernameExists(newDoctor.username)) {
            printf("\t\t\tUsername already exists. Please choose another.\n");
        }
    } while (isDoctorUsernameExists(newDoctor.username));
    
    printf("\t\t\tCreate Password: ");
    fgets(newDoctor.password, sizeof(newDoctor.password), stdin);
    newDoctor.password[strcspn(newDoctor.password, "\n")] = 0;  // Remove newline character
      // Save to file
    ensureDataDirectoryExists();
    file = fopen("data/doctors.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    fwrite(&newDoctor, sizeof(Doctor), 1, file);
    fclose(file);
    
    printf("\n\t\t\tDoctor added successfully!\n");
}

// Function to add a new staff member
void addStaff() {
    Staff newStaff;
    FILE *file;
    
    // Initialize the structure with zeros to avoid garbage values
    memset(&newStaff, 0, sizeof(Staff));
    
    system("cls");
    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|             ADD NEW STAFF                 |\n");
    printf("\t\t\t+========================================+\n\n");
    
    // Generate unique ID
    newStaff.id = generateStaffID();
    
    // Get staff details
    printf("\t\t\tStaff ID: %d (auto-generated)\n", newStaff.id);
    
    printf("\t\t\tFull Name: ");
    fflush(stdin);
    fgets(newStaff.name, sizeof(newStaff.name), stdin);
    newStaff.name[strcspn(newStaff.name, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tRole: ");
    fgets(newStaff.role, sizeof(newStaff.role), stdin);
    newStaff.role[strcspn(newStaff.role, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tContact Number: ");
    fgets(newStaff.contact, sizeof(newStaff.contact), stdin);
    newStaff.contact[strcspn(newStaff.contact, "\n")] = 0;  // Remove newline character
      printf("\t\t\tEmail: ");
    fgets(newStaff.email, sizeof(newStaff.email), stdin);
    newStaff.email[strcspn(newStaff.email, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tShift: ");
    fgets(newStaff.shift, sizeof(newStaff.shift), stdin);
    newStaff.shift[strcspn(newStaff.shift, "\n")] = 0;  // Remove newline character
    
    // Username and password
    do {
        printf("\t\t\tCreate Username: ");
        fgets(newStaff.username, sizeof(newStaff.username), stdin);
        newStaff.username[strcspn(newStaff.username, "\n")] = 0;  // Remove newline character
        
        if (isStaffUsernameExists(newStaff.username)) {
            printf("\t\t\tUsername already exists. Please choose another.\n");
        }
    } while (isStaffUsernameExists(newStaff.username));
    
    printf("\t\t\tCreate Password: ");
    fgets(newStaff.password, sizeof(newStaff.password), stdin);
    newStaff.password[strcspn(newStaff.password, "\n")] = 0;  // Remove newline character
      // Save to file
    ensureDataDirectoryExists();
    file = fopen("data/staff.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    fwrite(&newStaff, sizeof(Staff), 1, file);
    fclose(file);
    
    printf("\n\t\t\tStaff added successfully!\n");
}

// Function to display admin dashboard
void adminDashboard() {
    int choice;
    
    do {        system("cls");
        printf("\n\t\t\t+========================================+\n");
        printf("\t\t\t|             ADMIN DASHBOARD               |\n");
        printf("\t\t\t+========================================+\n\n");
        
        printf("\t\t\t1. Manage OPD\n");
        printf("\t\t\t2. Manage Pharmacy\n");
        printf("\t\t\t3. Manage Equipment\n");
        printf("\t\t\t4. Manage Blood Bank\n");
        printf("\t\t\t5. Add Doctor\n");
        printf("\t\t\t6. Add Staff\n");
        printf("\t\t\t7. Billing Management\n");
        printf("\t\t\t0. Logout\n\n");
        
        printf("\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                editOPDMenu();
                break;
            case 2:
                editPharmacyMenu();
                break;
            case 3:
                editEquipmentMenu();
                break;
            case 4:
                editBloodBankMenu();
                break;
            case 5:
                addDoctor();
                break;
            case 6:
                addStaff();
                break;
            case 7:
                viewBillingMenu();
                break;
            case 0:
                printf("\n\t\t\tLogging out from admin panel...\n");
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
