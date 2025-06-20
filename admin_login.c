/**
 * admin_login.c - Separate Admin Logi    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|              ADMIN LOGIN                 |\n");
    printf("\t\t\t+========================================+\n\n");ith hardcoded credentials
 * Hospital Management System
 * This module handles admin authentication
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Admin credentials (hardcoded for simplicity)
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

// Function to mask password input
void getAdminPassword(char* password) {
    char ch;
    int i = 0;
    
    while (1) {
        ch = getch();
        
        if (ch == 13) { // Enter key
            password[i] = '\0';
            break;
        } else if (ch == 8 && i > 0) { // Backspace
            i--;
            printf("\b \b");
        } else if (ch != 8) {
            password[i] = ch;
            printf("*");
            i++;
        }
    }
}

// Function to verify admin login
int adminLogin() {
    char username[20];
    char password[20];
      system("cls");
    printf("\n\t\t\t+========================================+\n");
    printf("\t\t\t|               ADMIN LOGIN                 |\n");
    printf("\t\t\t+========================================+\n\n");
      printf("\t\t\tUsername: ");
    fflush(stdin);
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;  // Remove newline character
    
    printf("\t\t\tPassword: ");
    getAdminPassword(password);
    
    // Check credentials
    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("\n\t\t\tAdmin login successful!\n");
        return 1;
    } else {
        printf("\n\t\t\tInvalid admin credentials!\n");
        return 0;
    }
}
