/*
 * patient.h
 * Defines a common, reusable patient data shape for all current and future data structures.

 * This shared base type may be embedded or extended with module-local fields by individual
 * queue implementations and functions when additional context is required.

 * Common patient information should remain centralized here, while future members such as
 * a patient identifier or other application-wide attributes may be appended as the system grows.
 */

#ifndef PATIENT_H
#define PATIENT_H

typedef struct {
    char name[100];
} Patient;

#endif