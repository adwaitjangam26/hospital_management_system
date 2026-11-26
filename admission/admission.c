/*
 * admission.c
 * Implements admitted patient management with ward hash chains and an admission-order list.
 * Supports admission, discharge, lookup, and patient display across ten general wards.
 */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "admission.h"
#include "../common/helpers/helpers.h"

#define NUM_WARDS 10

typedef struct Admitted
{
    int id;
    char name[100];
    char disease[100];
    int ward;
    struct Admitted *prev;
    struct Admitted *next;
    struct Admitted *wardPrev;
    struct Admitted *wardNext;
} Admitted;

static Admitted *head = NULL, *tail = NULL;
static Admitted *ward[NUM_WARDS];
static Admitted *wardTail[NUM_WARDS];
static int nextId = 1001;

// Admit a patient to the ward selected by the patient ID hash.
void admit_patient_admission(void)
{
    system("cls");

    Admitted *newNode = malloc(sizeof(Admitted));

    if (newNode == NULL)
    {
        printf("\n[!] Memory allocation failed!\n");
        Sleep(1000);
        return;
    }

    printf("\nEnter Patient Name: ");
    inputString(newNode->name);

    printf("Enter Disease: ");
    inputString(newNode->disease);

    int id = nextId;
    int wardIndex = id % NUM_WARDS;

    newNode->id = id;
    newNode->ward = wardIndex + 1;
    newNode->wardPrev = wardTail[wardIndex];
    newNode->wardNext = NULL;

    if (wardTail[wardIndex] == NULL)
        ward[wardIndex] = newNode;
    else
        wardTail[wardIndex]->wardNext = newNode;

    wardTail[wardIndex] = newNode;

    newNode->prev = tail;
    newNode->next = NULL;

    if (tail == NULL)
        head = newNode;
    else
        tail->next = newNode;

    tail = newNode;
    nextId++;

    printf("\nPatient admitted successfully!\n");
    printf("Patient ID : %d\n", newNode->id);
    printf("Ward       : %d\n", newNode->ward);
    printf("Disease    : %s\n", newNode->disease);
    Sleep(2000);
}

// Discharge a patient by unlinking the node from its ward chain and admission list.
void discharge_patient_admission(void)
{
    system("cls");

    printf("\nEnter Patient ID: ");
    int id = inputInteger();

    if (id < 0)
    {
        printf("\n[!] Patient not found!\n");
        Sleep(2000);
        return;
    }

    int wardIndex = id % NUM_WARDS;
    Admitted *temp = ward[wardIndex];

    while (temp != NULL && temp->id != id)
        temp = temp->wardNext;

    if (temp == NULL)
    {
        printf("\n[!] Patient not found!\n");
        Sleep(2000);
        return;
    }

    // Update the ward head or previous link when removing this patient.
    if (temp->wardPrev == NULL)
        ward[wardIndex] = temp->wardNext;
    else
        temp->wardPrev->wardNext = temp->wardNext;

    // Update the ward tail or next link to keep both directions connected.
    if (temp->wardNext == NULL)
        wardTail[wardIndex] = temp->wardPrev;
    else
        temp->wardNext->wardPrev = temp->wardPrev;

    // Repair both neighboring links, including the head and tail boundaries.
    if (temp->prev == NULL)
        head = temp->next;
    else
        temp->prev->next = temp->next;

    if (temp->next == NULL)
        tail = temp->prev;
    else
        temp->next->prev = temp->prev;

    free(temp);

    printf("\nPatient discharged successfully!\n");
    Sleep(2000);
}

// Find a patient in the ward selected by the patient's ID hash.
void search_patient_admission(void)
{
    system("cls");

    printf("\nEnter Patient ID: ");
    int id = inputInteger();

    if (id < 0)
    {
        printf("\n[!] Patient not found!\n");
        Sleep(2000);
        return;
    }

    int wardIndex = id % NUM_WARDS;
    Admitted *temp = ward[wardIndex];

    while (temp != NULL && temp->id != id)
        temp = temp->wardNext;

    if (temp == NULL)
    {
        printf("\n[!] Patient not found!\n");
        Sleep(2000);
        return;
    }

    printf("\n==========================================\n");
    printf("          ADMITTED PATIENT DETAILS        \n");
    printf("==========================================\n");
    printf("Name : %s\n", temp->name);
    printf("ID   : %d\n", temp->id);
    printf("Ward : %d\n", temp->ward);
    printf("Disease : %s\n", temp->disease);
    printf("------------------------------------------\n");
    Sleep(5000);
}

// Display all admitted patients grouped by ward, including empty wards.
void display_all_patients_admission(void)
{
    system("cls");

    printf("\n==========================================\n");
    printf("          ADMITTED PATIENT LIST            \n");
    printf("==========================================\n");

    int count = 0;

    for (int wardIndex = 0; wardIndex < NUM_WARDS; wardIndex++)
    {
        Admitted *temp = ward[wardIndex];

        printf("\n==========================================\n");
        printf("                 WARD %d                   \n", wardIndex + 1);
        printf("==========================================\n");

        if (temp == NULL)
            printf("[!] No patients in this ward!\n");

        while (temp != NULL)
        {
            printf("\nPatient ID : %d\n", temp->id);
            printf("Name       : %s\n", temp->name);
            printf("Disease    : %s\n", temp->disease);
            printf("------------------------------------------\n");

            temp = temp->wardNext;
            count++;
        }
    }

    if (count == 0)
        printf("\n[!] No admitted patients found in any ward!\n");

    printf("\nTotal Admitted Patients: %d\n", count);
    Sleep(5000);
}

// Display the first patient's record from one selected ward.
void show_patients_record_in_ward(void)
{
    system("cls");

    printf("\nEnter Ward Number (1-%d): ", NUM_WARDS);
    int wardNumber = inputInteger();

    if (wardNumber < 1 || wardNumber > NUM_WARDS)
    {
        printf("\n[!] Invalid ward number!\n");
        Sleep(2000);
        return;
    }

    int wardIndex = wardNumber - 1;
    Admitted *current = ward[wardIndex];

    if (current == NULL)
    {
        printf("\n[!] No patients in this ward!\n");
        Sleep(2000);
        return;
    }

    printf("\n==========================================\n");
    printf("          WARD %d FIRST PATIENT             \n", wardNumber);
    printf("==========================================\n");
    printf("Patient ID : %d\n", current->id);
    printf("Name       : %s\n", current->name);
    printf("Disease    : %s\n", current->disease);
    printf("Ward       : %d\n", current->ward);
    printf("------------------------------------------\n");
    Sleep(5000);
}

// Display the admitted-patient menu and route operations until returning to main.
void admissionManagement_menu(void)
{
    int flag = 0;
    int choice;

    while (flag == 0)
    {
    MENU:
        system("cls");

        printf("==========================================\n");
        printf("        ADMITTED PATIENT MANAGEMENT       \n");
        printf("==========================================\n");
        printf("  [1] Admit Patient                       \n");
        printf("  [2] Discharge Patient                   \n");
        printf("  [3] Search Patient by ID                \n");
        printf("  [4] Display All Patients                \n");
        printf("  [5] Show First Patient in a Ward         \n");
        printf("  [6] Back to Main Menu                   \n");
        printf("------------------------------------------\n");
        printf("  Enter your choice: ");

        choice = inputInteger();

        switch (choice)
        {
        case 1:
            admit_patient_admission();
            break;

        case 2:
            discharge_patient_admission();
            break;

        case 3:
            search_patient_admission();
            break;

        case 4:
            display_all_patients_admission();
            break;

        case 5:
            show_patients_record_in_ward();
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
