/**
 * main.c - Homepage UI (read-only access to modules)
 * Hospital Management System
 * This module provides the main interface to access all functionalities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Function declarations from other modules
void registerPatient();
void listAllPatients(); // Debug function
int loginUser(int role); // role: 1=Patient, 2=Doctor, 3=Staff
int adminLogin();
void viewOPDRecords();
void viewPharmacyRecords();
void viewEquipmentRecords();
void viewBloodBankRecords();
void adminDashboard();

void displayHeader() {
    system("cls");
    printf("\n");
    printf("\t\t\t+========================================+\n");
    printf("\t\t\t|        HOSPITAL MANAGEMENT SYSTEM         |\n");
    printf("\t\t\t+========================================+\n");
    printf("\n");
}

void displayMainMenu() {
    displayHeader();
    printf("\t\t\t           MAIN MENU\n");
    printf("\t\t\t-----------------------------------------\n\n");
    printf("\t\t\t1. OPD (View Only)\n");
    printf("\t\t\t2. Pharmacy (View Only)\n");
    printf("\t\t\t3. Equipment (View Only)\n");
    printf("\t\t\t4. Blood Bank (View Only)\n");
    printf("\t\t\t5. Login (Doctor / Staff / Patient)\n");
    printf("\t\t\t6. Register (Patient Only)\n");
    printf("\t\t\t7. Admin Login\n");
    printf("\t\t\t8. Debug - List Patients\n");
    printf("\t\t\t0. Exit\n\n");
    printf("\t\t\tEnter your choice: ");
}

int main() {
    int choice;
    int role;
    
    do {
        displayMainMenu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewOPDRecords();
                break;
            case 2:
                viewPharmacyRecords();
                break;
            case 3:
                viewEquipmentRecords();
                break;
            case 4:
                viewBloodBankRecords();
                break;
            case 5:
                printf("\n\t\t\tSelect Role:\n");
                printf("\t\t\t1. Patient\n");
                printf("\t\t\t2. Doctor\n");
                printf("\t\t\t3. Staff\n");
                printf("\t\t\tEnter choice: ");
                scanf("%d", &role);
                  if(role >= 1 && role <= 3) {
                    int loggedIn = loginUser(role);
                    // The respective dashboard functions will be called from login.c
                } else {
                    printf("\t\t\tInvalid role selected!\n");
                }
                break;
            case 6:
                registerPatient();
                break;            case 7:
                if(adminLogin()) {
                    adminDashboard();
                } else {
                    printf("\t\t\tAdmin login failed!\n");
                }
                break;
            case 8:
                listAllPatients(); // Call our debug function
                break;
            case 0:
                printf("\n\t\t\tThank you for using Hospital Management System!\n");
                printf("\t\t\tExiting...\n");
                break;
            default:
                printf("\t\t\tInvalid choice. Please try again.\n");
        }
        
        if(choice != 0) {
            printf("\n\t\t\tPress any key to continue...");
            getch();
        }
        
    } while(choice != 0);
    
    return 0;
}
