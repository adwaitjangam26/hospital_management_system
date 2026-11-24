// Main entry point and top-level menu for the hospital management system.
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "./common/helpers/helpers.h"
#include "./emergency/priorityQueue.h"
#include "./appointment/circularQueue.h"

int main(){
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
        printf("  [3] Exit                                \n");
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
            case 3:
                flag = 1;
                break;
            default:
                system("cls");
                printf("==========================================\n");
                printf("  [!] %d is an Invalid choice!            \n", choice); //This is to check if the given integer is within allowed range. (1-3)
                printf("==========================================\n");
                Sleep(1000);
                goto MENU; 
        }
    }

    return 0;
}