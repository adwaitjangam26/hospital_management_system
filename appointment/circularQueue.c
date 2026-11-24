#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "circularQueue.h"
#include "../common/patient.h"
#include "../common/helpers/helpers.h"

#define MAX_SIZE 25

static Patient queue[MAX_SIZE];

static int front = -1;
static int rear = -1;
static int count = 0;


void add_patient_cqueue(void)
{
    system("cls");
    if (count == MAX_SIZE)
    {
        printf("\n[!] OPD waiting queue is full!\n");
        Sleep(2000);
        return;
    }

    if (count == 0)
    {
        front = 0;
        rear = 0;
    }
    else
    {
        rear = (rear + 1) % MAX_SIZE;
    }

    printf("\nEnter Patient Name: ");
    inputString(queue[rear].name);

    count++;

    printf("\nPatient added successfully!\n");
    Sleep(2000);
}


void treat_patient_cqueue(void)
{
    system("cls");
    if (count == 0)
    {
        printf("\n[!] No OPD patients waiting!\n");
        Sleep(2000);
        return;
    }

    printf("\n==========================================\n");
    printf("          PATIENT BEING TREATED           \n");
    printf("==========================================\n");
    printf("  Patient Name : %s\n", queue[front].name);
    printf("------------------------------------------\n");

    if (count == 1)
    {
        front = -1;
        rear = -1;
    }
    else
    {
        front = (front + 1) % MAX_SIZE;
    }

    count--;

    printf("\nPatient successfully removed from OPD queue!\n");
    Sleep(5000);
}

void display_all_patients_cqueue(void)
{
    system("cls");
    if (count == 0)
    {
        printf("\n[!] No patients in the OPD queue!\n");
        Sleep(2000);
        return;
    }

    int temp = front;

    printf("\n==========================================\n");
    printf("             OPD PATIENT LIST             \n");
    printf("==========================================\n");

    for (int i = 0; i < count; i++)
    {
        printf("\nPatient #%d\n", i + 1);
        printf("------------------------------------------\n");
        printf("Name : %s\n", queue[temp].name);
        printf("------------------------------------------\n");

        temp = (temp + 1) % MAX_SIZE;
    }

    printf("\nTotal Waiting Patients: %d\n", count);
    Sleep(2000);
}


void display_next_patient_cqueue(void)
{
    system("cls");
    if (count == 0)
    {
        printf("\n[!] No patients in the OPD queue!\n");
        Sleep(2000);
        return;
    }

    printf("\n==========================================\n");
    printf("              NEXT OPD PATIENT            \n");
    printf("==========================================\n");

    printf("\nName : %s\n", queue[front].name);

    printf("------------------------------------------\n");
    printf("This patient will be treated next.\n");

    Sleep(5000);
}

void check_available_slots_cqueue(void)
{
    system("cls");

    printf("\n==========================================\n");
    printf("             OPD QUEUE STATUS             \n");
    printf("==========================================\n");

    printf("Maximum Capacity : %d\n", MAX_SIZE);
    printf("Patients Waiting : %d\n", count);
    printf("Available Slots  : %d\n", MAX_SIZE - count);

    printf("------------------------------------------\n");

    Sleep(5000);
}

void opdManagement_menu(void)
{
    int flag = 0;
    int choice;

    while (flag == 0)
    {
        MENU:
        system("cls");

        printf("==========================================\n");
        printf("          OPD PATIENT MANAGEMENT          \n");
        printf("==========================================\n");
        printf("  [1] Add Patient                         \n");
        printf("  [2] Treat Patient                       \n");
        printf("  [3] Display All Patients                \n");
        printf("  [4] Display Next Patient                \n");
        printf("  [5] Check Available Slots               \n");
        printf("  [6] Back to Main Menu                   \n");
        printf("------------------------------------------\n");
        printf("  Enter your choice: ");

        choice = inputInteger();

        switch (choice)
        {
        case 1:
            add_patient_cqueue();
            break;

        case 2:
            treat_patient_cqueue();
            break;

        case 3:
            display_all_patients_cqueue();
            break;

        case 4:
            display_next_patient_cqueue();
            break;

        case 5:
            check_available_slots_cqueue();
            break;

        case 6:
            flag = 1;
            break;

        default:
            system("cls");
            printf("==========================================\n");
            printf("  [!] %d is an Invalid choice!\n", choice);
            printf("==========================================\n");
            Sleep(1000);
            goto MENU;
        }
    }

    return;
}