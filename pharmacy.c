/**
 * pharmacy.c - Medicine inventory + patient dispensing
 * Hospital Management System
 * This module handles pharmacy-related functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

// Structures
typedef struct {
    int id;
    char name[50];
    char type[30];        // Tablet, Syrup, Injection, etc.
    char manufacturer[50];
    float unitPrice;
    int stockQuantity;
    char expiryDate[15];
} Medicine;

typedef struct {
    int id;                 // Prescription ID
    int patientID;          // Patient ID (foreign key)
    int doctorID;           // Doctor ID (foreign key)
    char date[15];          // Date of prescription
    char medicines[300];    // List of medicines
    char dosage[200];       // Dosage instructions
    char status[20];        // Status (pending, dispensed)
} Prescription;

// Function to get doctor name by ID
char* getPharmacyDoctorName(int doctorID) {
    static char name[50] = "Unknown";
    FILE *file = fopen("data/doctors.dat", "rb");
    
    if (file == NULL) {
        return name;
    }
    
    struct {
        int id;
        char name[50];
        char specialization[50];
        char other[200]; // Buffer for other fields
    } doctor;
    
    while (fread(&doctor, sizeof(doctor), 1, file)) {
        if (doctor.id == doctorID) {
            strcpy(name, doctor.name);
            break;
        }
    }
    
    fclose(file);
    return name;
}

// Function to get patient name by ID
char* getPharmacyPatientName(int patientID) {
    static char name[50] = "Unknown";
    FILE *file = fopen("data/patients.dat", "rb");
    
    if (file == NULL) {
        return name;
    }
    
    struct {
        int id;
        char name[50];
        char other[400]; // Buffer for other fields
    } patient;
    
    while (fread(&patient, sizeof(patient), 1, file)) {
        if (patient.id == patientID) {
            strcpy(name, patient.name);
            break;
        }
    }
    
    fclose(file);
    return name;
}

// Function to view medicines (for all users)
void viewPharmacyRecords() {
    FILE *file = fopen("data/medicines.dat", "rb");
    Medicine medicine;
    
    if (file == NULL) {
        printf("\t\t\tNo medicines found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║             MEDICINE INVENTORY            ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\t%-5s %-20s %-15s %-10s %-8s %-12s\n", 
           "ID", "Name", "Type", "Price", "Stock", "Expiry");
    printf("\t\t\t----------------------------------------------------------------\n");
    
    while (fread(&medicine, sizeof(Medicine), 1, file)) {
        printf("\t\t\t%-5d %-20s %-15s $%-9.2f %-8d %-12s\n", 
               medicine.id, medicine.name, medicine.type, 
               medicine.unitPrice, medicine.stockQuantity, medicine.expiryDate);
    }
    
    fclose(file);
}

// Function to add a new medicine (for admin)
void addMedicine() {
    Medicine newMedicine;
    FILE *file;
    int lastID = 0;
    
    // Get last medicine ID
    file = fopen("data/medicines.dat", "rb");
    if (file != NULL) {
        Medicine temp;
        while (fread(&temp, sizeof(Medicine), 1, file)) {
            if (temp.id > lastID) {
                lastID = temp.id;
            }
        }
        fclose(file);
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║              ADD NEW MEDICINE             ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    // Generate new ID
    newMedicine.id = lastID + 1;
    
    // Get medicine details
    printf("\t\t\tMedicine ID: %d (auto-generated)\n", newMedicine.id);
    
    printf("\t\t\tName: ");
    fflush(stdin);
    gets(newMedicine.name);
    
    printf("\t\t\tType (Tablet/Syrup/Injection/etc.): ");
    gets(newMedicine.type);
    
    printf("\t\t\tManufacturer: ");
    gets(newMedicine.manufacturer);
    
    printf("\t\t\tUnit Price: $");
    scanf("%f", &newMedicine.unitPrice);
    
    printf("\t\t\tStock Quantity: ");
    scanf("%d", &newMedicine.stockQuantity);
    
    printf("\t\t\tExpiry Date (DD/MM/YYYY): ");
    fflush(stdin);
    gets(newMedicine.expiryDate);
    
    // Save to file
    file = fopen("data/medicines.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    fwrite(&newMedicine, sizeof(Medicine), 1, file);
    fclose(file);
    
    printf("\n\t\t\tMedicine added successfully!\n");
}

// Function to update medicine stock (for admin)
void updateMedicineStock() {
    FILE *file = fopen("data/medicines.dat", "rb+");
    Medicine medicine;
    int medicineID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo medicines found!\n");
        return;
    }
    
    // First display all medicines
    viewPharmacyRecords();
    
    printf("\n\t\t\tEnter Medicine ID to update stock: ");
    scanf("%d", &medicineID);
    
    while (fread(&medicine, sizeof(Medicine), 1, file)) {
        if (medicine.id == medicineID) {
            found = 1;
            
            printf("\n\t\t\tCurrent Stock: %d\n", medicine.stockQuantity);
            printf("\t\t\tEnter New Stock Quantity: ");
            scanf("%d", &medicine.stockQuantity);
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(Medicine), SEEK_CUR);
            fwrite(&medicine, sizeof(Medicine), 1, file);
            
            printf("\n\t\t\tMedicine stock updated successfully!\n");
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tMedicine not found!\n");
    }
}

// Function to delete medicine (for admin)
void deleteMedicine() {
    FILE *file = fopen("data/medicines.dat", "rb");
    FILE *tempFile = fopen("data/temp.dat", "wb");
    Medicine medicine;
    int medicineID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo medicines found!\n");
        return;
    }
    
    // First display all medicines
    viewPharmacyRecords();
    
    printf("\n\t\t\tEnter Medicine ID to delete: ");
    scanf("%d", &medicineID);
    
    while (fread(&medicine, sizeof(Medicine), 1, file)) {
        if (medicine.id == medicineID) {
            found = 1;
            printf("\n\t\t\tMedicine with ID %d will be deleted!\n", medicineID);
        } else {
            fwrite(&medicine, sizeof(Medicine), 1, tempFile);
        }
    }
    
    fclose(file);
    fclose(tempFile);
    
    if (found) {
        remove("data/medicines.dat");
        rename("data/temp.dat", "data/medicines.dat");
        printf("\t\t\tMedicine deleted successfully!\n");
    } else {
        remove("data/temp.dat");
        printf("\t\t\tMedicine not found!\n");
    }
}

// Function to view prescriptions (for admin)
void viewPrescriptions() {
    FILE *file = fopen("data/prescriptions.dat", "rb");
    Prescription prescription;
    
    if (file == NULL) {
        printf("\t\t\tNo prescriptions found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║              PRESCRIPTIONS                ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\t%-5s %-15s %-15s %-12s %-15s\n", 
           "ID", "Patient", "Doctor", "Date", "Status");
    printf("\t\t\t----------------------------------------------------------------\n");
    
    while (fread(&prescription, sizeof(Prescription), 1, file)) {
        char patientName[50], doctorName[50];
        
        strcpy(patientName, getPharmacyPatientName(prescription.patientID));
        strcpy(doctorName, getPharmacyDoctorName(prescription.doctorID));
        
        printf("\t\t\t%-5d %-15s %-15s %-12s %-15s\n", 
               prescription.id, patientName, doctorName, 
               prescription.date, prescription.status);
    }
    
    fclose(file);
}

// Function to dispense medicine (for admin)
void dispenseMedicine() {
    FILE *file = fopen("data/prescriptions.dat", "rb+");
    Prescription prescription;
    int prescriptionID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo prescriptions found!\n");
        return;
    }
    
    // First display all prescriptions
    viewPrescriptions();
    
    printf("\n\t\t\tEnter Prescription ID to dispense: ");
    scanf("%d", &prescriptionID);
    
    while (fread(&prescription, sizeof(Prescription), 1, file)) {
        if (prescription.id == prescriptionID) {
            found = 1;
            
            if (strcmp(prescription.status, "dispensed") == 0) {
                printf("\n\t\t\tThis prescription has already been dispensed!\n");
                fclose(file);
                return;
            }
            
            system("cls");
            printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
            printf("\t\t\t║             DISPENSE MEDICINE             ║\n");
            printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
            
            printf("\t\t\tPrescription ID: %d\n", prescription.id);
            printf("\t\t\tPatient: %s (ID: %d)\n", 
                   getPharmacyPatientName(prescription.patientID), prescription.patientID);
            printf("\t\t\tDoctor: %s (ID: %d)\n", 
                   getPharmacyDoctorName(prescription.doctorID), prescription.doctorID);
            printf("\t\t\tDate: %s\n", prescription.date);
            printf("\t\t\tMedicines: %s\n", prescription.medicines);
            printf("\t\t\tDosage: %s\n", prescription.dosage);
            
            printf("\n\t\t\tConfirm dispensing (Y/N): ");
            fflush(stdin);
            char confirm;
            scanf("%c", &confirm);
            
            if (confirm == 'Y' || confirm == 'y') {
                strcpy(prescription.status, "dispensed");
                
                // Move file pointer back to update the record
                fseek(file, -sizeof(Prescription), SEEK_CUR);
                fwrite(&prescription, sizeof(Prescription), 1, file);
                
                printf("\n\t\t\tMedicine dispensed successfully!\n");
                
                // Update medicine stock
                FILE *medFile = fopen("data/medicines.dat", "rb+");
                if (medFile != NULL) {
                    // Logic to update stock would go here
                    // For simplicity, we're not implementing this
                    // as it would require parsing the medicine names
                    fclose(medFile);
                }
            } else {
                printf("\n\t\t\tDispensing cancelled.\n");
            }
            
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tPrescription not found!\n");
    }
}

// Function to display pharmacy edit menu (for admin)
void editPharmacyMenu() {
    int choice;
    
    do {
        system("cls");
        printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
        printf("\t\t\t║            PHARMACY MANAGEMENT            ║\n");
        printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
        
        printf("\t\t\t1. View Medicine Inventory\n");
        printf("\t\t\t2. Add New Medicine\n");
        printf("\t\t\t3. Update Medicine Stock\n");
        printf("\t\t\t4. Delete Medicine\n");
        printf("\t\t\t5. View Prescriptions\n");
        printf("\t\t\t6. Dispense Medicine\n");
        printf("\t\t\t0. Back to Admin Dashboard\n\n");
        
        printf("\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewPharmacyRecords();
                break;
            case 2:
                addMedicine();
                break;
            case 3:
                updateMedicineStock();
                break;
            case 4:
                deleteMedicine();
                break;
            case 5:
                viewPrescriptions();
                break;
            case 6:
                dispenseMedicine();
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
