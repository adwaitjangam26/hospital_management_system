
/*
 * departmentBT.c
 * Implements department hierarchy management using a linked binary tree.
 * Departments are identified by level-based IDs and can be added, inspected,
 * listed, counted, or removed from the hierarchy.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "departmentBT.h"
#include "../common/helpers/helpers.h"

typedef struct Department
{
    // Store department data together with links to its parent and children.
    int departmentID;
    char departmentName[100];
    char headDoctor[100];
    int level;

    struct Department *P, *L, *R;

} Department;

static Department *root = NULL;
static int departmentCount = 0;

// Reuse the next available sequence number when generating an ID for each level.
static int nextNumber[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};


// Build a level-based ID and reserve a sequence number within that level.
int generate_department_id(int level)
{
    if (level < 1 || level > 9)
        return -1;

    if (nextNumber[level] > 99)
        return -1;

    int departmentID = (level * 100) + nextNumber[level];

    nextNumber[level]++;

    return departmentID;
}


// Search both child branches until the requested department is found.
static Department *find_department_recursive(Department *node, int id)
{
    if (node == NULL)
        return NULL;

    if (node->departmentID == id)
        return node;

    Department *found = find_department_recursive(node->L, id);

    if (found != NULL)
        return found;

    return find_department_recursive(node->R, id);
}


// Start department lookup at the hierarchy root.
static Department *find_department_by_id(int id)
{
    return find_department_recursive(root, id);
}

// Draw each department with connectors that preserve the left/right hierarchy.
static void print_department_tree(Department *node, char prefix[], int prefixLength, int isLast, int isRoot, char position)
{
    if (node == NULL)
        return;

    // Carry the branch lines established by ancestor departments into this row.
    printf("%s", prefix);

    // Mark child rows as left or right branches; the root needs no connector.
    if (!isRoot)
    {
        printf(isLast ? "\\-- " : "|-- ");
        printf("%c: ", position);
    }

    printf("[%03d] %s\n", node->departmentID, node->departmentName);

    // A remaining child count determines which connector style is used below.
    int childCount = 0;

    if (node->L != NULL)
        childCount++;

    if (node->R != NULL)
        childCount++;

    // Extend the prefix so child rows align under the correct branch.
    int newLength = prefixLength;

    if (!isRoot)
    {
        if (isLast)
        {
            prefix[newLength++] = ' ';
            prefix[newLength++] = ' ';
            prefix[newLength++] = ' ';
            prefix[newLength++] = ' ';
        }
        else
        {
            prefix[newLength++] = '|';
            prefix[newLength++] = ' ';
            prefix[newLength++] = ' ';
            prefix[newLength++] = ' ';
        }

        prefix[newLength] = '\0';
    }

    // The left branch is placed before the right branch in the displayed hierarchy.
    if (node->L != NULL)
    {
        childCount--;

        print_department_tree(
            node->L,
            prefix,
            newLength,
            childCount == 0,
            0,
            'L'
        );
    }

    // The right branch is always the final child in a department's drawing.
    if (node->R != NULL)
    {
        print_department_tree(
            node->R,
            prefix,
            newLength,
            1,
            0,
            'R'
        );
    }

    // Restore the caller's indentation before returning to the parent branch.
    prefix[prefixLength] = '\0';
}

// Present the departments in a connected tree rather than an unstructured list.
void display_hierarchy(void)
{
    if (root == NULL)
    {
        printf("\n[!] No departments found!\n");
        return;
    }

    char prefix[100] = "";

    printf("\n==========================================\n");
    printf("          DEPARTMENT HIERARCHY            \n");
    printf("==========================================\n\n");

    print_department_tree(root, prefix, 0, 1, 1, ' ');

    printf("\n==========================================\n");
}


// Collect and attach a department while validating its parent and position.
void insert_department(void)
{
    int parentID, position;
    int newID, newLevel;

    Department *parent = NULL;
    Department *newDepartment = NULL;

    system("cls");

    printf("==========================================\n");
    printf("             ADD DEPARTMENT               \n");
    printf("==========================================\n");

    // The first department becomes level zero and has no parent.
    if (root == NULL)
    {
        printf("No departments exist. Creating root department.\n\n");

        newDepartment = (Department *)malloc(sizeof(Department));

        if (newDepartment == NULL)
        {
            printf("\n[!] Memory allocation failed!\n");
            Sleep(2000);
            return;
        }

        newDepartment->departmentID = 0;
        newDepartment->level = 0;
        newDepartment->P = NULL;
        newDepartment->L = NULL;
        newDepartment->R = NULL;

        printf("Enter Department Name: ");
        inputString(newDepartment->departmentName);

        printf("Enter Head Doctor Name: ");
        inputString(newDepartment->headDoctor);

        root = newDepartment;
        departmentCount++;

        printf("\n[+] Root Department Created Successfully!\n");
        printf("Department ID : 000\n");

        Sleep(2000);
        return;
    }

    // Existing departments require a parent and one available child position.

    printf("\nCurrent Department Hierarchy:\n");
    display_hierarchy();

    printf("\nEnter Parent Department ID: ");
    parentID = inputInteger();

    parent = find_department_by_id(parentID);

    if (parent == NULL)
    {
        printf("\n[!] Department ID %03d not found!\n", parentID);
        Sleep(2000);
        return;
    }

    printf("\nParent Department: %s\n", parent->departmentName);

    printf("\nSelect Position:\n");
    printf("[1] Left Child\n");
    printf("[2] Right Child\n");
    printf("------------------------------------------\n");
    printf("Enter your choice: ");

    position = inputInteger();

    if (position != 1 && position != 2)
    {
        printf("\n[!] Invalid position selected!\n");
        Sleep(2000);
        return;
    }

    // Prevent a new department from overwriting an existing child link.
    if (position == 1 && parent->L != NULL)
    {
        printf("\n[!] Left child position is already occupied!\n");
        Sleep(2000);
        return;
    }

    if (position == 2 && parent->R != NULL)
    {
        printf("\n[!] Right child position is already occupied!\n");
        Sleep(2000);
        return;
    }

    // Inherit the next hierarchy level from the selected parent.
    newLevel = parent->level + 1;

    // Reserve the department record before attempting to allocate its ID.
    newDepartment = (Department *)malloc(sizeof(Department));

    if (newDepartment == NULL)
    {
        printf("\n[!] Memory allocation failed!\n");
        Sleep(2000);
        return;
    }

    // Cancel insertion if the level or sequence limit has been exhausted.
    newID = generate_department_id(newLevel);

    if (newID == -1)
    {
        printf("\n[!] Cannot generate Department ID!\n");
        printf("Maximum hierarchy depth or ID limit reached.\n");

        free(newDepartment);

        Sleep(2000);
        return;
    }

    // Set parent and child links before the department enters the hierarchy.
    newDepartment->departmentID = newID;
    newDepartment->level = newLevel;
    newDepartment->P = parent;
    newDepartment->L = NULL;
    newDepartment->R = NULL;

    printf("\nEnter Department Name: ");
    inputString(newDepartment->departmentName);

    printf("Enter Head Doctor Name: ");
    inputString(newDepartment->headDoctor);

    // Link the department to the parent in the position the user selected.
    if (position == 1)
        parent->L = newDepartment;
    else
        parent->R = newDepartment;

    departmentCount++;

    printf("\n==========================================\n");
    printf("     DEPARTMENT ADDED SUCCESSFULLY!       \n");
    printf("==========================================\n");
    printf("Department ID   : %03d\n", newDepartment->departmentID);
    printf("Department Name : %s\n", newDepartment->departmentName);
    printf("Head Doctor     : %s\n", newDepartment->headDoctor);
    printf("Parent ID       : %03d\n", parent->departmentID);
    printf("Level           : %d\n", newDepartment->level);
    printf("==========================================\n");

    Sleep(3000);
}


// Resolve a department ID and show the information stored for that department.
void get_department_details(void)
{
    int departmentID;

    Department *department = NULL;

    system("cls");

    printf("==========================================\n");
    printf("          DEPARTMENT DETAILS              \n");
    printf("==========================================\n");

    if (root == NULL)
    {
        printf("\n[!] No departments available!\n");
        Sleep(2000);
        return;
    }

    printf("\nEnter Department ID: ");
    departmentID = inputInteger();

    department = find_department_by_id(departmentID);

    if (department == NULL)
    {
        printf("\n[!] Department ID %03d not found!\n", departmentID);
        Sleep(2000);
        return;
    }

    printf("\n==========================================\n");
    printf("         DEPARTMENT INFORMATION           \n");
    printf("==========================================\n");

    printf("Department ID    : %03d\n", department->departmentID);
    printf("Department Name  : %s\n", department->departmentName);
    printf("Head Doctor      : %s\n", department->headDoctor);
    printf("Department Level : %d\n", department->level);

    printf("==========================================\n");

    Sleep(5000);
}


// Release child departments first so no parent or child reference remains dangling.
static void free_department_tree(Department *node)
{
    if (node == NULL)
        return;

    free_department_tree(node->L);
    free_department_tree(node->R);

    free(node);
}


// Require confirmation before deleting the entire hierarchy and resetting its state.
void clear_department_tree(void)
{
    int choice;

    system("cls");

    printf("==========================================\n");
    printf("         CLEAR DEPARTMENT HIERARCHY       \n");
    printf("==========================================\n");

    if (root == NULL)
    {
        printf("\n[!] No departments available to clear!\n");
        Sleep(2000);
        return;
    }

    printf("\nCurrent Department Hierarchy:\n");
    display_hierarchy();

    printf("\n[!] WARNING: This will remove ALL departments!\n");
    printf("\nAre you sure you want to continue?\n");
    printf("[1] Yes, Clear All Departments\n");
    printf("[2] No, Go Back\n");
    printf("------------------------------------------\n");
    printf("Enter your choice: ");

    choice = inputInteger();

    if (choice == 1)
    {
        free_department_tree(root);

        root = NULL;
        departmentCount = 0;

        // Start numbering at one again so future departments receive fresh level IDs.
        for (int i = 0; i < 10; i++)
            nextNumber[i] = 1;

        printf("\n[+] All departments deleted successfully!\n");
    }
    else if (choice == 2)
    {
        printf("\n[!] Operation cancelled.\n");
    }
    else
    {
        printf("\n[!] Invalid choice. Operation cancelled.\n");
    }

    Sleep(3000);
}


// Report the number of departments currently held in the hierarchy.
int get_department_count(void)
{
    return departmentCount;
}


// Route department operations until the user returns to the main menu.
void departmentManagement_menu(void)
{
    int flag = 0;
    int choice;

    while (flag == 0)
    {
    MENU:
        system("cls");

        printf("==========================================\n");
        printf("       DEPARTMENT HIERARCHY MANAGEMENT    \n");
        printf("==========================================\n");
        printf("  [1] Add Department                      \n");
        printf("  [2] Clear All Departments               \n");
        printf("  [3] Display Department Hierarchy        \n");
        printf("  [4] Get Department Details               \n");
        printf("  [5] Count Departments                   \n");
        printf("  [6] Back to Main Menu                   \n");
        printf("------------------------------------------\n");
        printf("  Enter your choice: ");

        choice = inputInteger();

        switch (choice)
        {
        case 1:
            insert_department();
            break;

        case 2:
            clear_department_tree();
            break;

        case 3:
            system("cls");
            display_hierarchy();
            Sleep(5000);
            break;

        case 4:
            get_department_details();
            break;

        case 5:
            system("cls");

            printf("==========================================\n");
            printf("           DEPARTMENT COUNT               \n");
            printf("==========================================\n");
            printf("Total Departments : %d\n", get_department_count());
            printf("------------------------------------------\n");

            Sleep(3000);
            break;

        case 6:
            flag = 1;
            break;

        default:
            system("cls");

            printf("==========================================\n");
            printf("  [!] %d is an Invalid choice!\n", choice);
            printf("==========================================\n");

            Sleep(2000);
            goto MENU;
        }
    }

    return;
}
