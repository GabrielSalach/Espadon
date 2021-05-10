#ifndef PERCEMAILLE
#define PERCEMAILLE _

/* ---------- INCLUDES ---------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* ---------- STRUCTS ---------- */

typedef struct{
    char*** matrix;
    int section_count, value_count;
    int subsectiont_values[32];
    char path[64];
}Pe_File;


/* ---------- FUNCTIONS HEADERS ---------- */

Pe_File*            pe_file_create(char *path);                                                 /* Creates a Pe_File struct */
int                 pe_file_read(Pe_File* file);                                                /* Reads the content of the file associated with the Pe_File passed as argument and stores it inside the Pe_File */
void                pe_file_destroy(Pe_File* file);                                             /* Frees the memory of a Pe_File */
void                pe_file_display(Pe_File* file);                                             /* Displays the content of a Pe_File */
char*               pe_value_get(Pe_File* file, char* section, char* variable);                 /* Returns the value of the associated variable name underneath the section passed as arguments */
int                 pe_value_get_as_int(Pe_File* file, char* section, char* variable);          /* Returns the value of the associated variable name underneath the section passed as arguments as an int */



#endif