/**
 * billing.c - Billing using linked records (admin-only)
 * Hospital Management System
 * This module handles patient billing functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

// Structures
typedef struct {
    int id;
    int patientID;          // Patient ID (foreign key)
    char patientName[50];   // Patient name
    char date[15];          // Billing date
    float opdCharges;       // OPD charges
    float medicineCharges;  // Medicine charges
    float testCharges;      // Test charges
    float otherCharges;     // Other charges
    float totalAmount;      // Total bill amount
    char paymentStatus[20]; // Paid, Pending, Partial
    float paidAmount;       // Amount paid
    char paymentMethod[20]; // Cash, Card, Insurance
} Bill;

typedef struct {
    int id;                 // Appointment ID
    int patientID;          // Patient ID (foreign key)
    char symptoms[100];     // Patient's symptoms
    char diagnosis[200];    // Doctor's diagnosis
    char prescription[300]; // Prescribed medicines
} Appointment;

typedef struct {
    int id;                 // Prescription ID
    int patientID;          // Patient ID (foreign key)
    char medicines[300];    // List of medicines
    char dosage[200];       // Dosage instructions
} Prescription;

// Function to get current date in DD/MM/YYYY format
void getBillingCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(date, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

// Function to get patient name by ID
char* getBillingPatientName(int patientID) {
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

// Function to view all bills (for admin)
void viewAllBills() {
    FILE *file = fopen("data/bills.dat", "rb");
    Bill bill;
    
    if (file == NULL) {
        printf("\t\t\tNo bills found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║                 ALL BILLS                 ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\t%-5s %-7s %-15s %-12s %-12s %-15s\n", 
           "ID", "Patient", "Name", "Date", "Amount", "Status");
    printf("\t\t\t----------------------------------------------------------------\n");
    
    while (fread(&bill, sizeof(Bill), 1, file)) {
        printf("\t\t\t%-5d %-7d %-15s %-12s $%-11.2f %-15s\n", 
               bill.id, bill.patientID, bill.patientName,
               bill.date, bill.totalAmount, bill.paymentStatus);
    }
    
    fclose(file);
}

// Function to generate a new bill (for admin)
void generateBill() {
    Bill newBill;
    FILE *file;
    int lastID = 0;
    int patientID;
    
    // Get last bill ID
    file = fopen("data/bills.dat", "rb");
    if (file != NULL) {
        Bill temp;
        while (fread(&temp, sizeof(Bill), 1, file)) {
            if (temp.id > lastID) {
                lastID = temp.id;
            }
        }
        fclose(file);
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║              GENERATE NEW BILL            ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    // Generate new ID
    newBill.id = lastID + 1;
    
    // Get patient ID
    printf("\t\t\tEnter Patient ID: ");
    scanf("%d", &patientID);
    
    // Get patient name
    char patientName[50];
    strcpy(patientName, getBillingPatientName(patientID));
    
    if (strcmp(patientName, "Unknown") == 0) {
        printf("\t\t\tPatient not found! Cannot generate bill.\n");
        return;
    }
    
    newBill.patientID = patientID;
    strcpy(newBill.patientName, patientName);
    
    // Set billing date to current date
    getBillingCurrentDate(newBill.date);
    
    // Calculate OPD charges
    float opdCharges = 0.0;
    FILE *opdFile = fopen("data/appointments.dat", "rb");
    
    if (opdFile != NULL) {
        Appointment appointment;
        int appointmentCount = 0;
        
        while (fread(&appointment, sizeof(appointment), 1, opdFile)) {
            if (appointment.patientID == patientID) {
                appointmentCount++;
            }
        }
        
        fclose(opdFile);
        
        // Assume each OPD visit costs $50
        opdCharges = appointmentCount * 50.0;
    }
    
    newBill.opdCharges = opdCharges;
    
    // Calculate medicine charges
    float medicineCharges = 0.0;
    FILE *presFile = fopen("data/prescriptions.dat", "rb");
    
    if (presFile != NULL) {
        Prescription prescription;
        int prescriptionCount = 0;
        
        while (fread(&prescription, sizeof(prescription), 1, presFile)) {
            if (prescription.patientID == patientID) {
                prescriptionCount++;
                
                // Here we would ideally parse the medicines and calculate their cost
                // For simplicity, we'll just estimate based on prescription count
                medicineCharges += 75.0; // Assume average medicine charge of $75 per prescription
            }
        }
        
        fclose(presFile);
    }
    
    newBill.medicineCharges = medicineCharges;
    
    // Allow manual entry for test charges and other charges
    printf("\t\t\tOPD Charges: $%.2f (Auto-calculated from %d visits at $50 each)\n", 
           opdCharges, (int)(opdCharges / 50.0));
    
    printf("\t\t\tMedicine Charges: $%.2f (Auto-calculated from prescriptions)\n", 
           medicineCharges);
    
    printf("\t\t\tEnter Test Charges: $");
    scanf("%f", &newBill.testCharges);
    
    printf("\t\t\tEnter Other Charges (Room, etc.): $");
    scanf("%f", &newBill.otherCharges);
    
    // Calculate total amount
    newBill.totalAmount = newBill.opdCharges + newBill.medicineCharges + 
                          newBill.testCharges + newBill.otherCharges;
    
    printf("\n\t\t\tTotal Bill Amount: $%.2f\n", newBill.totalAmount);
    
    // Get payment details
    printf("\t\t\tEnter Payment Status (Paid/Pending/Partial): ");
    fflush(stdin);
    gets(newBill.paymentStatus);
    
    if (strcmp(newBill.paymentStatus, "Paid") == 0) {
        newBill.paidAmount = newBill.totalAmount;
    } else if (strcmp(newBill.paymentStatus, "Partial") == 0) {
        printf("\t\t\tEnter Amount Paid: $");
        scanf("%f", &newBill.paidAmount);
    } else {
        newBill.paidAmount = 0.0;
    }
    
    printf("\t\t\tEnter Payment Method (Cash/Card/Insurance): ");
    fflush(stdin);
    gets(newBill.paymentMethod);
    
    // Save to file
    file = fopen("data/bills.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    fwrite(&newBill, sizeof(Bill), 1, file);
    fclose(file);
    
    printf("\n\t\t\tBill generated successfully!\n");
    
    // Display bill summary
    printf("\n\t\t\t--- BILL SUMMARY ---\n");
    printf("\t\t\tBill ID: %d\n", newBill.id);
    printf("\t\t\tPatient ID: %d\n", newBill.patientID);
    printf("\t\t\tPatient Name: %s\n", newBill.patientName);
    printf("\t\t\tDate: %s\n", newBill.date);
    printf("\t\t\tOPD Charges: $%.2f\n", newBill.opdCharges);
    printf("\t\t\tMedicine Charges: $%.2f\n", newBill.medicineCharges);
    printf("\t\t\tTest Charges: $%.2f\n", newBill.testCharges);
    printf("\t\t\tOther Charges: $%.2f\n", newBill.otherCharges);
    printf("\t\t\tTotal Amount: $%.2f\n", newBill.totalAmount);
    printf("\t\t\tPayment Status: %s\n", newBill.paymentStatus);
    printf("\t\t\tPaid Amount: $%.2f\n", newBill.paidAmount);
    printf("\t\t\tBalance Due: $%.2f\n", newBill.totalAmount - newBill.paidAmount);
    printf("\t\t\tPayment Method: %s\n", newBill.paymentMethod);
}

// Function to view a specific bill (for admin)
void viewBillDetails() {
    FILE *file = fopen("data/bills.dat", "rb");
    Bill bill;
    int billID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo bills found!\n");
        return;
    }
    
    // First display all bills
    viewAllBills();
    
    printf("\n\t\t\tEnter Bill ID to view details: ");
    scanf("%d", &billID);
    
    while (fread(&bill, sizeof(Bill), 1, file)) {
        if (bill.id == billID) {
            found = 1;
            
            system("cls");
            printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
            printf("\t\t\t║                BILL DETAILS               ║\n");
            printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
            
            printf("\t\t\tBill ID: %d\n", bill.id);
            printf("\t\t\tPatient ID: %d\n", bill.patientID);
            printf("\t\t\tPatient Name: %s\n", bill.patientName);
            printf("\t\t\tDate: %s\n", bill.date);
            printf("\t\t\tOPD Charges: $%.2f\n", bill.opdCharges);
            printf("\t\t\tMedicine Charges: $%.2f\n", bill.medicineCharges);
            printf("\t\t\tTest Charges: $%.2f\n", bill.testCharges);
            printf("\t\t\tOther Charges: $%.2f\n", bill.otherCharges);
            printf("\t\t\tTotal Amount: $%.2f\n", bill.totalAmount);
            printf("\t\t\tPayment Status: %s\n", bill.paymentStatus);
            printf("\t\t\tPaid Amount: $%.2f\n", bill.paidAmount);
            printf("\t\t\tBalance Due: $%.2f\n", bill.totalAmount - bill.paidAmount);
            printf("\t\t\tPayment Method: %s\n", bill.paymentMethod);
            
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tBill not found!\n");
    }
}

// Function to update payment status (for admin)
void updatePaymentStatus() {
    FILE *file = fopen("data/bills.dat", "rb+");
    Bill bill;
    int billID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo bills found!\n");
        return;
    }
    
    // First display all bills
    viewAllBills();
    
    printf("\n\t\t\tEnter Bill ID to update payment: ");
    scanf("%d", &billID);
    
    while (fread(&bill, sizeof(Bill), 1, file)) {
        if (bill.id == billID) {
            found = 1;
            
            printf("\n\t\t\tCurrent Payment Status: %s\n", bill.paymentStatus);
            printf("\t\t\tTotal Amount: $%.2f\n", bill.totalAmount);
            printf("\t\t\tPaid Amount: $%.2f\n", bill.paidAmount);
            printf("\t\t\tBalance Due: $%.2f\n", bill.totalAmount - bill.paidAmount);
            
            printf("\n\t\t\tChoose New Payment Status:\n");
            printf("\t\t\t1. Paid\n");
            printf("\t\t\t2. Pending\n");
            printf("\t\t\t3. Partial\n");
            printf("\t\t\tEnter choice: ");
            
            int choice;
            scanf("%d", &choice);
            
            switch(choice) {
                case 1:
                    strcpy(bill.paymentStatus, "Paid");
                    bill.paidAmount = bill.totalAmount;
                    break;
                case 2:
                    strcpy(bill.paymentStatus, "Pending");
                    bill.paidAmount = 0.0;
                    break;
                case 3:
                    strcpy(bill.paymentStatus, "Partial");
                    printf("\t\t\tEnter Amount Paid: $");
                    scanf("%f", &bill.paidAmount);
                    break;
                default:
                    printf("\t\t\tInvalid choice. Payment status not updated.\n");
                    fclose(file);
                    return;
            }
            
            printf("\t\t\tEnter Payment Method (Cash/Card/Insurance): ");
            fflush(stdin);
            gets(bill.paymentMethod);
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(Bill), SEEK_CUR);
            fwrite(&bill, sizeof(Bill), 1, file);
            
            printf("\n\t\t\tPayment status updated successfully!\n");
            printf("\t\t\tNew Payment Status: %s\n", bill.paymentStatus);
            printf("\t\t\tPaid Amount: $%.2f\n", bill.paidAmount);
            printf("\t\t\tBalance Due: $%.2f\n", bill.totalAmount - bill.paidAmount);
            
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tBill not found!\n");
    }
}

// Function to delete a bill (for admin)
void deleteBill() {
    FILE *file = fopen("data/bills.dat", "rb");
    FILE *tempFile = fopen("data/temp.dat", "wb");
    Bill bill;
    int billID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo bills found!\n");
        return;
    }
    
    // First display all bills
    viewAllBills();
    
    printf("\n\t\t\tEnter Bill ID to delete: ");
    scanf("%d", &billID);
    
    while (fread(&bill, sizeof(Bill), 1, file)) {
        if (bill.id == billID) {
            found = 1;
            printf("\n\t\t\tBill with ID %d will be deleted!\n", billID);
        } else {
            fwrite(&bill, sizeof(Bill), 1, tempFile);
        }
    }
    
    fclose(file);
    fclose(tempFile);
    
    if (found) {
        remove("data/bills.dat");
        rename("data/temp.dat", "data/bills.dat");
        printf("\t\t\tBill deleted successfully!\n");
    } else {
        remove("data/temp.dat");
        printf("\t\t\tBill not found!\n");
    }
}

// Function to view patient's bills (for admin)
void viewPatientBills() {
    FILE *file = fopen("data/bills.dat", "rb");
    Bill bill;
    int patientID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo bills found!\n");
        return;
    }
    
    printf("\n\t\t\tEnter Patient ID to view their bills: ");
    scanf("%d", &patientID);
    
    // Get patient name
    char patientName[50];
    strcpy(patientName, getBillingPatientName(patientID));
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║              PATIENT BILLS                ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\tPatient ID: %d\n", patientID);
    printf("\t\t\tPatient Name: %s\n\n", patientName);
    
    printf("\t\t\t%-5s %-12s %-12s %-15s\n", 
           "ID", "Date", "Amount", "Status");
    printf("\t\t\t-----------------------------------------\n");
    
    while (fread(&bill, sizeof(Bill), 1, file)) {
        if (bill.patientID == patientID) {
            found = 1;
            
            printf("\t\t\t%-5d %-12s $%-11.2f %-15s\n", 
                   bill.id, bill.date, bill.totalAmount, bill.paymentStatus);
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tNo bills found for this patient!\n");
    }
}

// Function to display billing menu (for admin)
void viewBillingMenu() {
    int choice;
    
    do {
        system("cls");
        printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
        printf("\t\t\t║            BILLING MANAGEMENT             ║\n");
        printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
        
        printf("\t\t\t1. View All Bills\n");
        printf("\t\t\t2. Generate New Bill\n");
        printf("\t\t\t3. View Bill Details\n");
        printf("\t\t\t4. Update Payment Status\n");
        printf("\t\t\t5. Delete Bill\n");
        printf("\t\t\t6. View Patient's Bills\n");
        printf("\t\t\t0. Back to Admin Dashboard\n\n");
        
        printf("\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewAllBills();
                break;
            case 2:
                generateBill();
                break;
            case 3:
                viewBillDetails();
                break;
            case 4:
                updatePaymentStatus();
                break;
            case 5:
                deleteBill();
                break;
            case 6:
                viewPatientBills();
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
