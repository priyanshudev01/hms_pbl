/**
 * bloodbank.c - Blood group stock and donations
 * Hospital Management System
 * This module handles blood bank functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

// Structures
typedef struct {
    int id;
    char bloodGroup[5];    // A+, A-, B+, B-, AB+, AB-, O+, O-
    int quantity;          // in units/bags
    char lastUpdated[15];  // date of last update
} BloodStock;

typedef struct {
    int id;
    int donorID;          // Patient ID for registered donors
    char donorName[50];   // For non-registered donors
    char bloodGroup[5];
    int units;            // Number of units donated
    char donationDate[15];
    char status[20];      // Collected, Tested, Available, Discarded
} BloodDonation;

typedef struct {
    int id;
    int patientID;
    char patientName[50];
    char bloodGroup[5];
    int units;            // Number of units requested
    char requestDate[15];
    char status[20];      // Pending, Approved, Issued, Rejected
} BloodRequest;

// Function to get current date in DD/MM/YYYY format
void getCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(date, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

// Function to get patient name by ID
char* getBloodBankPatientName(int patientID) {
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

// Function to view blood stock (for all users)
void viewBloodBankRecords() {
    FILE *file = fopen("data/blood_stock.dat", "rb");
    BloodStock stock;
    
    if (file == NULL) {
        printf("\t\t\tNo blood stock records found!\n");
        return;
    }
      system("cls");
    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|               BLOOD STOCK                 |\n");
    printf("\t\t\t+========================================+\n\n");
    
    printf("\t\t\t%-5s %-10s %-10s %-15s\n", 
           "ID", "Group", "Units", "Last Updated");
    printf("\t\t\t-----------------------------------------\n");
    
    while (fread(&stock, sizeof(BloodStock), 1, file)) {
        printf("\t\t\t%-5d %-10s %-10d %-15s\n", 
               stock.id, stock.bloodGroup, stock.quantity, stock.lastUpdated);
    }
    
    fclose(file);
}

// Function to initialize blood stock (for admin)
void initializeBloodStock() {
    FILE *file = fopen("data/blood_stock.dat", "rb");
    
    if (file != NULL) {
        fclose(file);
        printf("\t\t\tBlood stock already initialized!\n");
        return;
    }
    
    file = fopen("data/blood_stock.dat", "wb");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    char bloodGroups[8][5] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    char currentDate[15];
    getCurrentDate(currentDate);
    
    for (int i = 0; i < 8; i++) {
        BloodStock stock;
        stock.id = i + 1;
        strcpy(stock.bloodGroup, bloodGroups[i]);
        stock.quantity = 0;  // Initial stock is zero
        strcpy(stock.lastUpdated, currentDate);
        
        fwrite(&stock, sizeof(BloodStock), 1, file);
    }
    
    fclose(file);
    
    printf("\t\t\tBlood stock initialized successfully!\n");
}

// Function to update blood stock (for admin)
void updateBloodStock() {
    FILE *file = fopen("data/blood_stock.dat", "rb+");
    BloodStock stock;
    int found = 0;
    char bloodGroup[5];
    
    if (file == NULL) {
        printf("\t\t\tNo blood stock records found! Initializing...\n");
        initializeBloodStock();
        file = fopen("data/blood_stock.dat", "rb+");
        if (file == NULL) {
            printf("\t\t\tError opening file!\n");
            return;
        }
    }
    
    // First display current stock
    viewBloodBankRecords();
    
    printf("\n\t\t\tEnter Blood Group to update (A+, A-, B+, B-, AB+, AB-, O+, O-): ");
    fflush(stdin);
    gets(bloodGroup);
    
    while (fread(&stock, sizeof(BloodStock), 1, file)) {
        if (strcmp(stock.bloodGroup, bloodGroup) == 0) {
            found = 1;
            
            printf("\n\t\t\tCurrent Quantity: %d units\n", stock.quantity);
            printf("\t\t\tEnter New Quantity: ");
            scanf("%d", &stock.quantity);
            
            // Update last updated date
            getCurrentDate(stock.lastUpdated);
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(BloodStock), SEEK_CUR);
            fwrite(&stock, sizeof(BloodStock), 1, file);
            
            printf("\n\t\t\tBlood stock updated successfully!\n");
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tInvalid blood group!\n");
    }
}

// Function to record blood donation (for admin)
void recordBloodDonation() {
    BloodDonation newDonation;
    FILE *file;
    int lastID = 0;
    char isDonorRegistered;
    
    // Get last donation ID
    file = fopen("data/blood_donations.dat", "rb");
    if (file != NULL) {
        BloodDonation temp;
        while (fread(&temp, sizeof(BloodDonation), 1, file)) {
            if (temp.id > lastID) {
                lastID = temp.id;
            }
        }
        fclose(file);
    }
      system("cls");
    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|           RECORD BLOOD DONATION           |\n");
    printf("\t\t\t+========================================+\n\n");
    
    // Generate new ID
    newDonation.id = lastID + 1;
    
    // Check if donor is registered
    printf("\t\t\tIs the donor a registered patient? (Y/N): ");
    fflush(stdin);
    scanf("%c", &isDonorRegistered);
    
    if (isDonorRegistered == 'Y' || isDonorRegistered == 'y') {
        printf("\t\t\tEnter Patient ID: ");
        scanf("%d", &newDonation.donorID);
        
        // Get patient name
        strcpy(newDonation.donorName, getBloodBankPatientName(newDonation.donorID));
        
        if (strcmp(newDonation.donorName, "Unknown") == 0) {
            printf("\t\t\tPatient not found! Enter donor name manually: ");
            fflush(stdin);
            gets(newDonation.donorName);
        }
    } else {
        newDonation.donorID = 0; // Not a registered patient
        printf("\t\t\tEnter Donor Name: ");
        fflush(stdin);
        gets(newDonation.donorName);
    }
    
    printf("\t\t\tBlood Group (A+, A-, B+, B-, AB+, AB-, O+, O-): ");
    gets(newDonation.bloodGroup);
    
    printf("\t\t\tUnits Donated: ");
    scanf("%d", &newDonation.units);
    
    printf("\t\t\tDonation Date (DD/MM/YYYY, press Enter for today): ");
    fflush(stdin);
    gets(newDonation.donationDate);
    
    if (strlen(newDonation.donationDate) == 0) {
        getCurrentDate(newDonation.donationDate);
    }
    
    strcpy(newDonation.status, "Collected");
    
    // Save to file
    file = fopen("data/blood_donations.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    fwrite(&newDonation, sizeof(BloodDonation), 1, file);
    fclose(file);
    
    printf("\n\t\t\tBlood donation recorded successfully!\n");
    
    // Ask if the blood is already tested and available
    char isTested;
    printf("\t\t\tIs the blood already tested and available? (Y/N): ");
    fflush(stdin);
    scanf("%c", &isTested);
    
    if (isTested == 'Y' || isTested == 'y') {
        // Update blood stock
        FILE *stockFile = fopen("data/blood_stock.dat", "rb+");
        
        if (stockFile == NULL) {
            printf("\t\t\tBlood stock not found! Initializing...\n");
            initializeBloodStock();
            stockFile = fopen("data/blood_stock.dat", "rb+");
        }
        
        if (stockFile != NULL) {
            BloodStock stock;
            int found = 0;
            
            while (fread(&stock, sizeof(BloodStock), 1, stockFile)) {
                if (strcmp(stock.bloodGroup, newDonation.bloodGroup) == 0) {
                    found = 1;
                    
                    stock.quantity += newDonation.units;
                    getCurrentDate(stock.lastUpdated);
                    
                    // Move file pointer back to update the record
                    fseek(stockFile, -sizeof(BloodStock), SEEK_CUR);
                    fwrite(&stock, sizeof(BloodStock), 1, stockFile);
                    
                    printf("\t\t\tBlood stock updated successfully!\n");
                    break;
                }
            }
            
            fclose(stockFile);
            
            if (!found) {
                printf("\t\t\tError: Blood group not found in stock!\n");
            }
            
            // Update donation status
            FILE *donationFile = fopen("data/blood_donations.dat", "rb+");
            
            if (donationFile != NULL) {
                BloodDonation donation;
                
                // Go to the end of file
                fseek(donationFile, -sizeof(BloodDonation), SEEK_END);
                
                // Read the last record
                fread(&donation, sizeof(BloodDonation), 1, donationFile);
                
                if (donation.id == newDonation.id) {
                    strcpy(donation.status, "Available");
                    
                    // Move file pointer back to update the record
                    fseek(donationFile, -sizeof(BloodDonation), SEEK_CUR);
                    fwrite(&donation, sizeof(BloodDonation), 1, donationFile);
                }
                
                fclose(donationFile);
            }
        }
    }
}

// Function to view blood donations (for admin)
void viewBloodDonations() {
    FILE *file = fopen("data/blood_donations.dat", "rb");
    BloodDonation donation;
    
    if (file == NULL) {
        printf("\t\t\tNo blood donations found!\n");
        return;
    }
    
    system("cls");    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|              BLOOD DONATIONS              |\n");
    printf("\t\t\t+========================================+\n\n");
    
    printf("\t\t\t%-5s %-20s %-8s %-8s %-15s %-15s\n", 
           "ID", "Donor", "Group", "Units", "Date", "Status");
    printf("\t\t\t--------------------------------------------------------------------\n");
    
    while (fread(&donation, sizeof(BloodDonation), 1, file)) {
        printf("\t\t\t%-5d %-20s %-8s %-8d %-15s %-15s\n", 
               donation.id, donation.donorName, donation.bloodGroup, 
               donation.units, donation.donationDate, donation.status);
    }
    
    fclose(file);
}

// Function to record blood request (for admin)
void recordBloodRequest() {
    BloodRequest newRequest;
    FILE *file;
    int lastID = 0;
    char isPatientRegistered;
    
    // Get last request ID
    file = fopen("data/blood_requests.dat", "rb");
    if (file != NULL) {
        BloodRequest temp;
        while (fread(&temp, sizeof(BloodRequest), 1, file)) {
            if (temp.id > lastID) {
                lastID = temp.id;
            }
        }
        fclose(file);
    }
    
    system("cls");    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|            RECORD BLOOD REQUEST           |\n");
    printf("\t\t\t+========================================+\n\n");
    
    // Generate new ID
    newRequest.id = lastID + 1;
    
    // Check if patient is registered
    printf("\t\t\tIs the requester a registered patient? (Y/N): ");
    fflush(stdin);
    scanf("%c", &isPatientRegistered);
    
    if (isPatientRegistered == 'Y' || isPatientRegistered == 'y') {
        printf("\t\t\tEnter Patient ID: ");
        scanf("%d", &newRequest.patientID);
        
        // Get patient name
        strcpy(newRequest.patientName, getBloodBankPatientName(newRequest.patientID));
        
        if (strcmp(newRequest.patientName, "Unknown") == 0) {
            printf("\t\t\tPatient not found! Enter requester name manually: ");
            fflush(stdin);
            gets(newRequest.patientName);
        }
    } else {
        newRequest.patientID = 0; // Not a registered patient
        printf("\t\t\tEnter Requester Name: ");
        fflush(stdin);
        gets(newRequest.patientName);
    }
    
    printf("\t\t\tBlood Group Needed (A+, A-, B+, B-, AB+, AB-, O+, O-): ");
    gets(newRequest.bloodGroup);
    
    printf("\t\t\tUnits Required: ");
    scanf("%d", &newRequest.units);
    
    printf("\t\t\tRequest Date (DD/MM/YYYY, press Enter for today): ");
    fflush(stdin);
    gets(newRequest.requestDate);
    
    if (strlen(newRequest.requestDate) == 0) {
        getCurrentDate(newRequest.requestDate);
    }
    
    strcpy(newRequest.status, "Pending");
    
    // Save to file
    file = fopen("data/blood_requests.dat", "ab");
    
    if (file == NULL) {
        printf("\t\t\tError opening file!\n");
        return;
    }
    
    fwrite(&newRequest, sizeof(BloodRequest), 1, file);
    fclose(file);
    
    printf("\n\t\t\tBlood request recorded successfully!\n");
    
    // Check blood stock for availability
    FILE *stockFile = fopen("data/blood_stock.dat", "rb");
    
    if (stockFile == NULL) {
        printf("\t\t\tCannot check blood stock availability!\n");
        return;
    }
    
    BloodStock stock;
    int found = 0;
    
    while (fread(&stock, sizeof(BloodStock), 1, stockFile)) {
        if (strcmp(stock.bloodGroup, newRequest.bloodGroup) == 0) {
            found = 1;
            
            if (stock.quantity >= newRequest.units) {
                printf("\t\t\tBlood is available in stock (%d units).\n", stock.quantity);
                
                char approveRequest;
                printf("\t\t\tApprove and issue blood now? (Y/N): ");
                fflush(stdin);
                scanf("%c", &approveRequest);
                
                if (approveRequest == 'Y' || approveRequest == 'y') {
                    // Update request status
                    FILE *requestFile = fopen("data/blood_requests.dat", "rb+");
                    
                    if (requestFile != NULL) {
                        BloodRequest request;
                        
                        // Go to the end of file
                        fseek(requestFile, -sizeof(BloodRequest), SEEK_END);
                        
                        // Read the last record
                        fread(&request, sizeof(BloodRequest), 1, requestFile);
                        
                        if (request.id == newRequest.id) {
                            strcpy(request.status, "Issued");
                            
                            // Move file pointer back to update the record
                            fseek(requestFile, -sizeof(BloodRequest), SEEK_CUR);
                            fwrite(&request, sizeof(BloodRequest), 1, requestFile);
                        }
                        
                        fclose(requestFile);
                    }
                    
                    // Update blood stock
                    FILE *updateStockFile = fopen("data/blood_stock.dat", "rb+");
                    
                    if (updateStockFile != NULL) {
                        BloodStock updateStock;
                        
                        while (fread(&updateStock, sizeof(BloodStock), 1, updateStockFile)) {
                            if (strcmp(updateStock.bloodGroup, newRequest.bloodGroup) == 0) {
                                updateStock.quantity -= newRequest.units;
                                getCurrentDate(updateStock.lastUpdated);
                                
                                // Move file pointer back to update the record
                                fseek(updateStockFile, -sizeof(BloodStock), SEEK_CUR);
                                fwrite(&updateStock, sizeof(BloodStock), 1, updateStockFile);
                                
                                printf("\t\t\tBlood issued and stock updated successfully!\n");
                                break;
                            }
                        }
                        
                        fclose(updateStockFile);
                    }
                }
            } else {
                printf("\t\t\tWarning: Insufficient blood stock! Only %d units available.\n", stock.quantity);
            }
            
            break;
        }
    }
    
    fclose(stockFile);
    
    if (!found) {
        printf("\t\t\tError: Blood group not found in stock!\n");
    }
}

// Function to view blood requests (for admin)
void viewBloodRequests() {
    FILE *file = fopen("data/blood_requests.dat", "rb");
    BloodRequest request;
    
    if (file == NULL) {
        printf("\t\t\tNo blood requests found!\n");
        return;
    }
    
    system("cls");    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|              BLOOD REQUESTS               |\n");
    printf("\t\t\t+========================================+\n\n");
    
    printf("\t\t\t%-5s %-20s %-8s %-8s %-15s %-15s\n", 
           "ID", "Requester", "Group", "Units", "Date", "Status");
    printf("\t\t\t--------------------------------------------------------------------\n");
    
    while (fread(&request, sizeof(BloodRequest), 1, file)) {
        printf("\t\t\t%-5d %-20s %-8s %-8d %-15s %-15s\n", 
               request.id, request.patientName, request.bloodGroup, 
               request.units, request.requestDate, request.status);
    }
    
    fclose(file);
}

// Function to update blood request status (for admin)
void updateBloodRequestStatus() {
    FILE *file = fopen("data/blood_requests.dat", "rb+");
    BloodRequest request;
    int requestID;
    int found = 0;
    
    if (file == NULL) {
        printf("\t\t\tNo blood requests found!\n");
        return;
    }
    
    // First display all requests
    viewBloodRequests();
    
    printf("\n\t\t\tEnter Request ID to update status: ");
    scanf("%d", &requestID);
    
    while (fread(&request, sizeof(BloodRequest), 1, file)) {
        if (request.id == requestID) {
            found = 1;
            
            printf("\n\t\t\tCurrent Status: %s\n", request.status);
            printf("\t\t\tChoose New Status:\n");
            printf("\t\t\t1. Pending\n");
            printf("\t\t\t2. Approved\n");
            printf("\t\t\t3. Issued\n");
            printf("\t\t\t4. Rejected\n");
            printf("\t\t\tEnter choice: ");
            
            int choice;
            scanf("%d", &choice);
            
            char oldStatus[20];
            strcpy(oldStatus, request.status);
            
            switch(choice) {
                case 1:
                    strcpy(request.status, "Pending");
                    break;
                case 2:
                    strcpy(request.status, "Approved");
                    break;
                case 3:
                    strcpy(request.status, "Issued");
                    break;
                case 4:
                    strcpy(request.status, "Rejected");
                    break;
                default:
                    printf("\t\t\tInvalid choice. Status not updated.\n");
                    fclose(file);
                    return;
            }
            
            // Move file pointer back to update the record
            fseek(file, -sizeof(BloodRequest), SEEK_CUR);
            fwrite(&request, sizeof(BloodRequest), 1, file);
            
            printf("\n\t\t\tRequest status updated successfully!\n");
            
            // If status changed to "Issued", update blood stock
            if (strcmp(request.status, "Issued") == 0 && strcmp(oldStatus, "Issued") != 0) {
                FILE *stockFile = fopen("data/blood_stock.dat", "rb+");
                
                if (stockFile == NULL) {
                    printf("\t\t\tCannot update blood stock!\n");
                    break;
                }
                
                BloodStock stock;
                int stockFound = 0;
                
                while (fread(&stock, sizeof(BloodStock), 1, stockFile)) {
                    if (strcmp(stock.bloodGroup, request.bloodGroup) == 0) {
                        stockFound = 1;
                        
                        if (stock.quantity >= request.units) {
                            stock.quantity -= request.units;
                            getCurrentDate(stock.lastUpdated);
                            
                            // Move file pointer back to update the record
                            fseek(stockFile, -sizeof(BloodStock), SEEK_CUR);
                            fwrite(&stock, sizeof(BloodStock), 1, stockFile);
                            
                            printf("\t\t\tBlood stock updated successfully!\n");
                        } else {
                            printf("\t\t\tWarning: Insufficient blood stock! Request status updated anyway.\n");
                        }
                        
                        break;
                    }
                }
                
                fclose(stockFile);
                
                if (!stockFound) {
                    printf("\t\t\tError: Blood group not found in stock!\n");
                }
            }
            
            // If status changed from "Issued" to something else, add blood back to stock
            if (strcmp(oldStatus, "Issued") == 0 && strcmp(request.status, "Issued") != 0) {
                FILE *stockFile = fopen("data/blood_stock.dat", "rb+");
                
                if (stockFile == NULL) {
                    printf("\t\t\tCannot update blood stock!\n");
                    break;
                }
                
                BloodStock stock;
                int stockFound = 0;
                
                while (fread(&stock, sizeof(BloodStock), 1, stockFile)) {
                    if (strcmp(stock.bloodGroup, request.bloodGroup) == 0) {
                        stockFound = 1;
                        
                        stock.quantity += request.units;
                        getCurrentDate(stock.lastUpdated);
                        
                        // Move file pointer back to update the record
                        fseek(stockFile, -sizeof(BloodStock), SEEK_CUR);
                        fwrite(&stock, sizeof(BloodStock), 1, stockFile);
                        
                        printf("\t\t\tBlood returned to stock successfully!\n");
                        break;
                    }
                }
                
                fclose(stockFile);
                
                if (!stockFound) {
                    printf("\t\t\tError: Blood group not found in stock!\n");
                }
            }
            
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("\t\t\tRequest not found!\n");
    }
}

// Function to display blood bank edit menu (for admin)
void editBloodBankMenu() {
    int choice;
    
    do {        system("cls");
        printf("\n\t\t\t+========================================+\n");
        printf("\t\t\t|           BLOOD BANK MANAGEMENT           |\n");
        printf("\t\t\t+========================================+\n\n");
        
        printf("\t\t\t1. View Blood Stock\n");
        printf("\t\t\t2. Update Blood Stock\n");
        printf("\t\t\t3. Record Blood Donation\n");
        printf("\t\t\t4. View Blood Donations\n");
        printf("\t\t\t5. Record Blood Request\n");
        printf("\t\t\t6. View Blood Requests\n");
        printf("\t\t\t7. Update Blood Request Status\n");
        printf("\t\t\t0. Back to Admin Dashboard\n\n");
        
        printf("\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewBloodBankRecords();
                break;
            case 2:
                updateBloodStock();
                break;
            case 3:
                recordBloodDonation();
                break;
            case 4:
                viewBloodDonations();
                break;
            case 5:
                recordBloodRequest();
                break;
            case 6:
                viewBloodRequests();
                break;
            case 7:
                updateBloodRequestStatus();
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
