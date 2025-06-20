/**
 * staff.c - Staff dashboard (view/edit shift info)
 * Hospital Management System
 * This module provides staff-specific functionalities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Structures
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

typedef struct {
    int id;
    int staffID;
    char equipmentName[50];
    char assignDate[15];
    char returnDate[15];
    char status[20]; // assigned, returned
} EquipmentAssignment;

// Function to get staff data by ID
Staff getStaffById(int staffID) {
    FILE *file = fopen("data/staff.dat", "rb");
    Staff staff;
    
    if (file == NULL) {
        printf("\t\t\tError: Cannot open staff file!\n");
        staff.id = -1; // Invalid ID to indicate error
        return staff;
    }
    
    while (fread(&staff, sizeof(Staff), 1, file)) {
        if (staff.id == staffID) {
            fclose(file);
            return staff;
        }
    }
    
    fclose(file);
    staff.id = -1; // Staff not found
    return staff;
}

// Function to view staff profile
void viewStaffProfile(int staffID) {
    Staff staff = getStaffById(staffID);
    
    if (staff.id == -1) {
        printf("\t\t\tStaff not found!\n");
        return;
    }
    
    system("cls");    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|              STAFF PROFILE                |\n");
    printf("\t\t\t+========================================+\n\n");
    
    printf("\t\t\tStaff ID: %d\n", staff.id);
    printf("\t\t\tName: %s\n", staff.name);
    printf("\t\t\tRole: %s\n", staff.role);
    printf("\t\t\tContact: %s\n", staff.contact);
    printf("\t\t\tEmail: %s\n", staff.email);
    printf("\t\t\tShift: %s\n", staff.shift);
}

// Function to update staff profile
void updateStaffProfile(int staffID) {
    FILE *file = fopen("data/staff.dat", "rb+");
    Staff staff;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tError: Cannot open staff file!\n");
        return;
    }
    
    while (fread(&staff, sizeof(Staff), 1, file)) {
        if (staff.id == staffID) {
            found = 1;
            
            system("cls");            printf("\n\t\t\t+========================================+\n");
            printf("\t\t\t|            UPDATE STAFF PROFILE           |\n");
            printf("\t\t\t+========================================+\n\n");
            
            printf("\t\t\tStaff ID: %d (Cannot be changed)\n", staff.id);
            printf("\t\t\tCurrent Contact: %s\n", staff.contact);
            printf("\t\t\tNew Contact (press enter to keep current): ");
            fflush(stdin);
            char tempContact[15];
            gets(tempContact);
            if (strlen(tempContact) > 0) {
                strcpy(staff.contact, tempContact);
            }
            
            printf("\t\t\tCurrent Email: %s\n", staff.email);
            printf("\t\t\tNew Email (press enter to keep current): ");
            char tempEmail[50];
            gets(tempEmail);
            if (strlen(tempEmail) > 0) {
                strcpy(staff.email, tempEmail);
            }
            
            printf("\t\t\tCurrent Password: %s\n", staff.password);
            printf("\t\t\tNew Password (press enter to keep current): ");
            char tempPassword[20];
            gets(tempPassword);
            if (strlen(tempPassword) > 0) {
                strcpy(staff.password, tempPassword);
            }
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(Staff), SEEK_CUR);
            fwrite(&staff, sizeof(Staff), 1, file);
            
            printf("\n\t\t\tProfile updated successfully!\n");
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tStaff not found!\n");
    }
}

// Function to view staff's equipment assignments
void viewStaffEquipmentAssignments(int staffID) {
    FILE *file = fopen("data/equipment_assignments.dat", "rb");
    EquipmentAssignment assignment;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo equipment assignments found!\n");
        return;
    }
    
    system("cls");    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|          EQUIPMENT ASSIGNMENTS            |\n");
    printf("\t\t\t+========================================+\n\n");
    
    printf("\t\t\t%-5s %-20s %-15s %-15s %-15s\n", 
           "ID", "Equipment", "Assigned Date", "Return Date", "Status");
    printf("\t\t\t----------------------------------------------------------------\n");
    
    while (fread(&assignment, sizeof(EquipmentAssignment), 1, file)) {
        if (assignment.staffID == staffID) {
            printf("\t\t\t%-5d %-20s %-15s %-15s %-15s\n", 
                   assignment.id, assignment.equipmentName,
                   assignment.assignDate, assignment.returnDate, assignment.status);
            found = 1;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tNo equipment assignments found!\n");
    }
}

// Function to view staff schedule
void viewStaffSchedule(int staffID) {
    Staff staff = getStaffById(staffID);
    
    if (staff.id == -1) {
        printf("\t\t\tStaff not found!\n");
        return;
    }
    
    system("cls");    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|              STAFF SCHEDULE               |\n");
    printf("\t\t\t+========================================+\n\n");
    
    printf("\t\t\tStaff ID: %d\n", staff.id);
    printf("\t\t\tName: %s\n", staff.name);
    printf("\t\t\tRole: %s\n", staff.role);
    printf("\t\t\tCurrent Shift: %s\n", staff.shift);
    
    // You could add more functionality here like calendar view, shift swaps, etc.
    printf("\n\t\t\tNote: For shift changes, please contact the admin.\n");
}

// Function to display staff dashboard
void staffDashboard(int staffID) {
    int choice;
    
    do {
        system("cls");        printf("\n\t\t\t+========================================+\n");
        printf("\t\t\t|              STAFF DASHBOARD              |\n");
        printf("\t\t\t+========================================+\n\n");
        
        printf("\t\t\t1. View Profile\n");
        printf("\t\t\t2. Update Profile\n");
        printf("\t\t\t3. View Equipment Assignments\n");
        printf("\t\t\t4. View Schedule\n");
        printf("\t\t\t0. Logout\n\n");
        
        printf("\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewStaffProfile(staffID);
                break;
            case 2:
                updateStaffProfile(staffID);
                break;            case 3:
                viewStaffEquipmentAssignments(staffID);
                break;
            case 4:
                viewStaffSchedule(staffID);
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
