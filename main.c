/*
 * main.c
 * Application entry point for the hospital management system.
 * Displays the top-level menu and routes users to emergency, OPD, or department operations.
 */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "./common/helpers/helpers.h"
#include "./emergency/priorityQueue.h"
#include "./appointment/circularQueue.h"
#include "./departments/departmentBT.h"

// Runs the hospital menu, calls choices, and exits on request.
int main(void){
    int flag = 0;
    int choice;

    while(flag==0){
        MENU:
        system("cls");

        printf("==========================================\n");
        printf("        HOSPITAL MANAGEMENT SYSTEM        \n");
        printf("==========================================\n");
        printf("  [1] Emergency Management                \n");
        printf("  [2] OPD Appointment Management          \n");
        printf("  [4] Department Management               \n");
        printf("  [5] Exit                                \n");
        printf("------------------------------------------\n");
        printf("  Enter your choice: ");
        choice = inputInteger(); //This is a custom helper function created to accept integer only.

        // Route the validated menu selection to the selected subsystem.
        switch(choice){
            case 1:
                emergencyManagement_menu();
                break;
            case 2:
                opdManagement_menu();
                break;
            case 4:
                departmentManagement_menu();
                break;
            case 5:
                flag = 1;
                break;
            default:
                system("cls");
                printf("==========================================\n");
                printf("  [!] %d is an Invalid choice!            \n", choice); //This is to check if the given integer is within allowed range. (1-5)
                printf("==========================================\n");
                Sleep(1000);
                goto MENU; 
        }
    }

    return 0;
}