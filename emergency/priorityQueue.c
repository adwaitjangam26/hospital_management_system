/*
 * priorityQueue.c
 * Implements emergency patient management with a priority-sorted linked queue.
 * Critical, high, and low-priority patients are ordered while preserving arrival order within each priority.
 */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "priorityQueue.h"
#include "../common/patient.h"
#include "../common/helpers/helpers.h"

// Each node contains one patient and a link to the next waiting patient.
typedef struct EmergencyPatient
{
    Patient patient; //this is taken from ../common/patient.h
    int priority;
    struct EmergencyPatient *next;
} EmergencyPatient;

static EmergencyPatient *head = NULL;

// Inserts a patient according to priority; equal priorities preserve arrival order.
void add_patient_pqueue(void)
{
    system("cls");
    // Allocate memory for a new queue node before collecting patient details.
    EmergencyPatient *newNode = malloc(sizeof(EmergencyPatient));

    if (newNode == NULL)
    {
        printf("\n[!] Memory allocation failed!\n");
        Sleep(1000);
        return;
    }

    // Input patient details
    printf("\nEnter Patient Name: ");
    inputString(newNode->patient.name);

    do
    {

        printf("==========================================\n");
        printf("          SELECT PATIENT PRIORITY         \n");
        printf("==========================================\n");
        printf("  [1] Critical\n");
        printf("  [2] High\n");
        printf("  [3] Low\n");
        printf("------------------------------------------\n");
        printf("  Enter priority: ");

        newNode->priority = inputInteger();

        if (newNode->priority < 1 || newNode->priority > 3)
        {
            printf("\n[!] Invalid priority! Try again.\n");
            Sleep(2000);
            system("cls");
        }

    } while (newNode->priority < 1 || newNode->priority > 3);

    newNode->next = NULL;

    if (head == NULL || newNode->priority < head->priority) //newNode->priority < head->priorit is required if head has lesser priority than new patient. (basically insert at front condition)
    {
        newNode->next = head;
        head = newNode;
    }
    else
    {
        EmergencyPatient *temp = head;

        while (temp->next != NULL &&
               temp->next->priority <= newNode->priority)
        {
            temp = temp->next;
        }

        newNode->next = temp->next;
        temp->next = newNode;
    }

    printf("\nPatient added successfully!\n");
    Sleep(1000);
}

// Removes and frees the highest-priority patient at the head of the queue.
void treat_patient_pqueue(void)
{
    system("cls");
    if (head == NULL)
    {
        printf("\n[!] No emergency patients waiting!\n");
        Sleep(2000);
        return;
    }

    EmergencyPatient *temp = head;

    printf("\n==========================================\n");
    printf("          PATIENT BEING TREATED           \n");
    printf("==========================================\n");
    printf("  Patient Name : %s\n", temp->patient.name);
    printf("  Priority     : %d\n", temp->priority);
    printf("------------------------------------------\n");

    head = head->next; //if head is the only node, then head->next is already NULL

    free(temp);

    printf("\nPatient successfully removed from queue!\n");
    Sleep(5000);
}

// Traverses the queue from head to tail and prints every waiting patient.
void display_all_patients_pqueue(void)
{
    system("cls");
    if (head == NULL)
    {
        printf("\n[!] No patients in the emergency queue!\n");
        Sleep(2000);
        return;
    }

    EmergencyPatient *temp = head;
    int count = 1;

    printf("\n==========================================\n");
    printf("          EMERGENCY PATIENT LIST          \n");
    printf("==========================================\n");

    while (temp != NULL)
    {
        printf("\nPatient #%d\n", count);
        printf("------------------------------------------\n");
        printf("Name     : %s\n", temp->patient.name);
        printf("Priority : %d \n", temp->priority);

        printf("------------------------------------------\n");

        temp = temp->next;
        count++;
    }

    printf("\nTotal Waiting Patients: %d\n", count - 1);
    Sleep(10000);
}

//peek function
void display_next_patient_pqueue(void)
{
    system("cls");
    if (head == NULL)
    {
        printf("\n[!] No patients in the emergency queue!\n");
        Sleep(2000);
        return;
    }

    printf("\n==========================================\n");
    printf("           NEXT EMERGENCY PATIENT         \n");
    printf("==========================================\n");

    printf("\nName     : %s\n", head->patient.name);
    printf("Priority : %d\n", head->priority);

    printf("------------------------------------------\n");

    Sleep(50000);
}

// Displays the emergency menu and routes operations to queue functions.
void emergencyManagement_menu(void)
{
    int flag = 0;
    int choice;

    while (flag == 0)
    {
    MENU:
        system("cls");

        printf("==========================================\n");
        printf("           EMERGENCY MANAGEMENT           \n");
        printf("==========================================\n");
        printf("  [1] Add Patient                         \n");
        printf("  [2] Treat Patient                       \n");
        printf("  [3] Display All Patients                \n");
        printf("  [4] Display Next Patient                \n");
        printf("  [5] Back to Main Menu                   \n");
        printf("------------------------------------------\n");
        printf("  Enter your choice: ");

        choice = inputInteger();

        switch (choice)
        {
        case 1:
            add_patient_pqueue();
            break;

        case 2:
            treat_patient_pqueue();
            break;

        case 3:
            display_all_patients_pqueue();
            break;

        case 4:
            display_next_patient_pqueue();
            break;

        case 5:
            flag = 1;
            break;

        default:
            system("cls");
            printf("==========================================\n");
            printf("  [!] %d is an Invalid choice!            \n", choice);
            printf("==========================================\n");
            Sleep(2000);
            goto MENU;
        }
    }

    return;
}