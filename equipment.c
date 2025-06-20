/**
 * equipment.c - Medical equipment records
 * Hospital Management System
 * This module handles medical equipment functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Structures
typedef struct {
    int id;
    char name[50];
    char type[30];        // Surgical, Diagnostic, Monitoring, etc.
    char model[50];
    char manufacturer[50];
    float purchasePrice;
    char purchaseDate[15];
    char lastMaintenanceDate[15];
    char nextMaintenanceDate[15];
    char status[20];      // Available, In-use, Maintenance, Retired
    int quantity;
} Equipment;

typedef struct {
    int id;
    int staffID;
    char staffName[50];
    int equipmentID;
    char equipmentName[50];
    char assignDate[15];
    char returnDate[15];
    char status[20]; // assigned, returned
} EquipmentAssignment;

// Function to get staff name by ID
char* getStaffName(int staffID) {
    static char name[50] = "Unknown";
    FILE *file = fopen("data/staff.dat", "rb");
    
    if (file == NULL) {
        return name;
    }
    
    struct {
        int id;
        char name[50];
        char other[200]; // Buffer for other fields
    } staff;
    
    while (fread(&staff, sizeof(staff), 1, file)) {
        if (staff.id == staffID) {
            strcpy(name, staff.name);
            break;
        }
    }
    
    fclose(file);
    return name;
}

// Function to view equipment (for all users)
void viewEquipmentRecords() {
    FILE *file = fopen("data/equipment.dat", "rb");
    Equipment equipment;
    
    if (file == NULL) {
        printf("\t\t\tNo equipment found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║            EQUIPMENT INVENTORY            ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\t%-5s %-20s %-15s %-12s %-15s %-8s\n", 
           "ID", "Name", "Type", "Status", "Next Maint.", "Quantity");
    printf("\t\t\t------------------------------------------------------------------\n");
    
    while (fread(&equipment, sizeof(Equipment), 1, file)) {
        printf("\t\t\t%-5d %-20s %-15s %-12s %-15s %-8d\n", 
               equipment.id, equipment.name, equipment.type, 
               equipment.status, equipment.nextMaintenanceDate, equipment.quantity);
    }
    
    fclose(file);
}

// Function to add new equipment (for admin)
void addEquipment() {
    Equipment newEquipment;
    FILE *file;
    int lastID = 0;
    
    // Get last equipment ID
    file = fopen("data/equipment.dat", "rb");
    if (file != NULL) {
        Equipment temp;
        while (fread(&temp, sizeof(Equipment), 1, file)) {
            if (temp.id > lastID) {
                lastID = temp.id;
            }
        }
        fclose(file);
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║             ADD NEW EQUIPMENT             ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    // Generate new ID
    newEquipment.id = lastID + 1;
    
    // Get equipment details
    printf("\t\t\tEquipment ID: %d (auto-generated)\n", newEquipment.id);
    
    printf("\t\t\tName: ");
    fflush(stdin);
    gets(newEquipment.name);
    
    printf("\t\t\tType (Surgical/Diagnostic/Monitoring/etc.): ");
    gets(newEquipment.type);
    
    printf("\t\t\tModel: ");
    gets(newEquipment.model);
    
    printf("\t\t\tManufacturer: ");
    gets(newEquipment.manufacturer);
    
    printf("\t\t\tPurchase Price: $");
    scanf("%f", &newEquipment.purchasePrice);
    
    printf("\t\t\tPurchase Date (DD/MM/YYYY): ");
    fflush(stdin);
    gets(newEquipment.purchaseDate);
    
    printf("\t\t\tLast Maintenance Date (DD/MM/YYYY): ");
    gets(newEquipment.lastMaintenanceDate);
    
    printf("\t\t\tNext Maintenance Date (DD/MM/YYYY): ");
    gets(newEquipment.nextMaintenanceDate);
    
    printf("\t\t\tStatus (Available/In-use/Maintenance/Retired): ");
    gets(newEquipment.status);
    
    printf("\t\t\tQuantity: ");
    scanf("%d", &newEquipment.quantity);
    
    // Save to file
    file = fopen("data/equipment.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    fwrite(&newEquipment, sizeof(Equipment), 1, file);
    fclose(file);
    
    printf("\n\t\t\tEquipment added successfully!\n");
}

// Function to update equipment details (for admin)
void updateEquipment() {
    FILE *file = fopen("data/equipment.dat", "rb+");
    Equipment equipment;
    int equipmentID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo equipment found!\n");
        return;
    }
    
    // First display all equipment
    viewEquipmentRecords();
    
    printf("\n\t\t\tEnter Equipment ID to update: ");
    scanf("%d", &equipmentID);
    
    while (fread(&equipment, sizeof(Equipment), 1, file)) {
        if (equipment.id == equipmentID) {
            found = 1;
            
            system("cls");
            printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
            printf("\t\t\t║            UPDATE EQUIPMENT               ║\n");
            printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
            
            printf("\t\t\tEquipment ID: %d (Cannot be changed)\n", equipment.id);
            
            printf("\t\t\tCurrent Last Maintenance Date: %s\n", equipment.lastMaintenanceDate);
            printf("\t\t\tNew Last Maintenance Date (press enter to keep current): ");
            fflush(stdin);
            char tempDate[15];
            gets(tempDate);
            if (strlen(tempDate) > 0) {
                strcpy(equipment.lastMaintenanceDate, tempDate);
            }
            
            printf("\t\t\tCurrent Next Maintenance Date: %s\n", equipment.nextMaintenanceDate);
            printf("\t\t\tNew Next Maintenance Date (press enter to keep current): ");
            gets(tempDate);
            if (strlen(tempDate) > 0) {
                strcpy(equipment.nextMaintenanceDate, tempDate);
            }
            
            printf("\t\t\tCurrent Status: %s\n", equipment.status);
            printf("\t\t\tNew Status (Available/In-use/Maintenance/Retired): ");
            char tempStatus[20];
            gets(tempStatus);
            if (strlen(tempStatus) > 0) {
                strcpy(equipment.status, tempStatus);
            }
            
            printf("\t\t\tCurrent Quantity: %d\n", equipment.quantity);
            printf("\t\t\tNew Quantity (press enter to keep current): ");
            char tempQuantity[10];
            gets(tempQuantity);
            if (strlen(tempQuantity) > 0) {
                equipment.quantity = atoi(tempQuantity);
            }
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(Equipment), SEEK_CUR);
            fwrite(&equipment, sizeof(Equipment), 1, file);
            
            printf("\n\t\t\tEquipment updated successfully!\n");
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tEquipment not found!\n");
    }
}

// Function to delete equipment (for admin)
void deleteEquipment() {
    FILE *file = fopen("data/equipment.dat", "rb");
    FILE *tempFile = fopen("data/temp.dat", "wb");
    Equipment equipment;
    int equipmentID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo equipment found!\n");
        return;
    }
    
    // First display all equipment
    viewEquipmentRecords();
    
    printf("\n\t\t\tEnter Equipment ID to delete: ");
    scanf("%d", &equipmentID);
    
    while (fread(&equipment, sizeof(Equipment), 1, file)) {
        if (equipment.id == equipmentID) {
            found = 1;
            printf("\n\t\t\tEquipment with ID %d will be deleted!\n", equipmentID);
        } else {
            fwrite(&equipment, sizeof(Equipment), 1, tempFile);
        }
    }
    
    fclose(file);
    fclose(tempFile);
    
    if (found) {
        remove("data/equipment.dat");
        rename("data/temp.dat", "data/equipment.dat");
        printf("\t\t\tEquipment deleted successfully!\n");
    } else {
        remove("data/temp.dat");
        printf("\t\t\tEquipment not found!\n");
    }
}

// Function to view equipment assignments (for admin)
void viewEquipmentAssignments() {
    FILE *file = fopen("data/equipment_assignments.dat", "rb");
    EquipmentAssignment assignment;
    
    if (file == NULL) {
        printf("\t\t\tNo equipment assignments found!\n");
        return;
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║          EQUIPMENT ASSIGNMENTS            ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    printf("\t\t\t%-5s %-20s %-20s %-15s %-15s %-12s\n", 
           "ID", "Equipment", "Staff", "Assign Date", "Return Date", "Status");
    printf("\t\t\t-------------------------------------------------------------------------\n");
    
    while (fread(&assignment, sizeof(EquipmentAssignment), 1, file)) {
        printf("\t\t\t%-5d %-20s %-20s %-15s %-15s %-12s\n", 
               assignment.id, assignment.equipmentName, assignment.staffName,
               assignment.assignDate, assignment.returnDate, assignment.status);
    }
    
    fclose(file);
}

// Function to assign equipment to staff (for admin)
void assignEquipment() {
    EquipmentAssignment newAssignment;
    FILE *file;
    int lastID = 0;
    int staffID, equipmentID;
    
    // Get last assignment ID
    file = fopen("data/equipment_assignments.dat", "rb");
    if (file != NULL) {
        EquipmentAssignment temp;
        while (fread(&temp, sizeof(EquipmentAssignment), 1, file)) {
            if (temp.id > lastID) {
                lastID = temp.id;
            }
        }
        fclose(file);
    }
    
    system("cls");
    printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
    printf("\t\t\t║            ASSIGN EQUIPMENT               ║\n");
    printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
    
    // Generate new ID
    newAssignment.id = lastID + 1;
    
    // Show available equipment
    printf("\t\t\tAvailable Equipment:\n");
    viewEquipmentRecords();
    
    printf("\n\t\t\tEnter Equipment ID to assign: ");
    scanf("%d", &equipmentID);
    
    // Check if equipment exists and is available
    FILE *equipFile = fopen("data/equipment.dat", "rb+");
    Equipment equipment;
    int equipmentFound = 0;
    
    if (equipFile == NULL) {
        printf("\t\t\tError opening equipment file!\n");
        return;
    }
    
    while (fread(&equipment, sizeof(Equipment), 1, equipFile)) {
        if (equipment.id == equipmentID) {
            equipmentFound = 1;
            
            if (strcmp(equipment.status, "Available") != 0 || equipment.quantity <= 0) {
                printf("\t\t\tThis equipment is not available for assignment!\n");
                fclose(equipFile);
                return;
            }
            
            // Decrease quantity by 1
            equipment.quantity--;
            if (equipment.quantity == 0) {
                strcpy(equipment.status, "In-use");
            }
            
            // Move file pointer back to update the record
            fseek(equipFile, -sizeof(Equipment), SEEK_CUR);
            fwrite(&equipment, sizeof(Equipment), 1, equipFile);
            
            // Store equipment name
            strcpy(newAssignment.equipmentName, equipment.name);
            newAssignment.equipmentID = equipmentID;
            
            break;
        }
    }
    
    fclose(equipFile);
    
    if (!equipmentFound) {
        printf("\t\t\tEquipment not found!\n");
        return;
    }
    
    // Get staff ID
    printf("\n\t\t\tEnter Staff ID to assign equipment to: ");
    scanf("%d", &staffID);
    
    // Check if staff exists
    FILE *staffFile = fopen("data/staff.dat", "rb");
    int staffFound = 0;
    
    if (staffFile == NULL) {
        printf("\t\t\tError opening staff file!\n");
        return;
    }
    
    struct {
        int id;
        char name[50];
        char other[200]; // Buffer for other fields
    } staff;
    
    while (fread(&staff, sizeof(staff), 1, staffFile)) {
        if (staff.id == staffID) {
            staffFound = 1;
            
            // Store staff name
            strcpy(newAssignment.staffName, staff.name);
            newAssignment.staffID = staffID;
            
            break;
        }
    }
    
    fclose(staffFile);
    
    if (!staffFound) {
        printf("\t\t\tStaff not found!\n");
        return;
    }
    
    // Get assignment details
    printf("\t\t\tAssign Date (DD/MM/YYYY): ");
    fflush(stdin);
    gets(newAssignment.assignDate);
    
    printf("\t\t\tExpected Return Date (DD/MM/YYYY): ");
    gets(newAssignment.returnDate);
    
    strcpy(newAssignment.status, "assigned");
    
    // Save to file
    file = fopen("data/equipment_assignments.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    fwrite(&newAssignment, sizeof(EquipmentAssignment), 1, file);
    fclose(file);
    
    printf("\n\t\t\tEquipment assigned successfully!\n");
}

// Function to mark equipment as returned (for admin)
void returnEquipment() {
    FILE *file = fopen("data/equipment_assignments.dat", "rb+");
    EquipmentAssignment assignment;
    int assignmentID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo equipment assignments found!\n");
        return;
    }
    
    // First display all assignments
    viewEquipmentAssignments();
    
    printf("\n\t\t\tEnter Assignment ID to mark as returned: ");
    scanf("%d", &assignmentID);
    
    while (fread(&assignment, sizeof(EquipmentAssignment), 1, file)) {
        if (assignment.id == assignmentID) {
            found = 1;
            
            if (strcmp(assignment.status, "returned") == 0) {
                printf("\n\t\t\tThis equipment has already been returned!\n");
                fclose(file);
                return;
            }
            
            strcpy(assignment.status, "returned");
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(EquipmentAssignment), SEEK_CUR);
            fwrite(&assignment, sizeof(EquipmentAssignment), 1, file);
            
            printf("\n\t\t\tEquipment marked as returned successfully!\n");
            
            // Update equipment quantity
            FILE *equipFile = fopen("data/equipment.dat", "rb+");
            Equipment equipment;
            
            if (equipFile == NULL) {
                printf("\t\t\tError opening equipment file!\n");
                fclose(file);
                return;
            }
            
            while (fread(&equipment, sizeof(Equipment), 1, equipFile)) {
                if (equipment.id == assignment.equipmentID) {
                    equipment.quantity++;
                    strcpy(equipment.status, "Available");
                    
                    // Move file pointer back to update the record
                    fseek(equipFile, -sizeof(Equipment), SEEK_CUR);
                    fwrite(&equipment, sizeof(Equipment), 1, equipFile);
                    break;
                }
            }
            
            fclose(equipFile);
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tAssignment not found!\n");
    }
}

// Function to display equipment edit menu (for admin)
void editEquipmentMenu() {
    int choice;
    
    do {
        system("cls");
        printf("\n\t\t\t╔════════════════════════════════════════════╗\n");
        printf("\t\t\t║           EQUIPMENT MANAGEMENT            ║\n");
        printf("\t\t\t╚════════════════════════════════════════════╝\n\n");
        
        printf("\t\t\t1. View Equipment Inventory\n");
        printf("\t\t\t2. Add New Equipment\n");
        printf("\t\t\t3. Update Equipment Details\n");
        printf("\t\t\t4. Delete Equipment\n");
        printf("\t\t\t5. View Equipment Assignments\n");
        printf("\t\t\t6. Assign Equipment to Staff\n");
        printf("\t\t\t7. Mark Equipment as Returned\n");
        printf("\t\t\t0. Back to Admin Dashboard\n\n");
        
        printf("\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewEquipmentRecords();
                break;
            case 2:
                addEquipment();
                break;
            case 3:
                updateEquipment();
                break;
            case 4:
                deleteEquipment();
                break;
            case 5:
                viewEquipmentAssignments();
                break;
            case 6:
                assignEquipment();
                break;
            case 7:
                returnEquipment();
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
