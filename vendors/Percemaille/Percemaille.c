#include "Percemaille.h"

/* ---------- STRUCT ALLOCATION ---------- */

Pe_File* pe_file_create(char* path) {
    Pe_File* rtrn_file;
    rtrn_file = (Pe_File*) malloc(sizeof (Pe_File));
    strcpy(rtrn_file->path, path);
    return rtrn_file;
}

/* ---------- FILE INPUT ---------- */

int pe_file_read(Pe_File* file) {
    /* Declarations */
    FILE* stream;
    int buffer;
    unsigned int i, j, k;
    int section_count, value_count;
    int subsection_values[32];
    section_count = 0;
    value_count = 0;

    /* NEED TO CHECK IF THE FILE IS A .INI */

    for(i = 0; i < 32; i++) {                                                                                   
        subsection_values[i] = 0;
    }

    stream = fopen(file->path, "r");
    if(file == NULL) {
        printf("The file does not exist.");
        return -1;
    }
    buffer = fgetc(stream);
    
    /* COUNTING THE AMOUNT OF SECTIONS AND VALUES */

    while(buffer != EOF) { 
        /* Ignores the comment */                                                                                     
        if(buffer == ';') {
            /* Read until the end of line */                                                                                     
            while(buffer != '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }
            /* Skips all the new lines until a new character */
            while(buffer == '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }
        } 
        /* Add 1 to the section count and count all the values inside the section */
        else if(buffer == '[') {                                                                              
            /* Read until the end of line */
            while(buffer != '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }
            /* Skips all the new lines until a new character */
            while(buffer == '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }      
            /* Count all the values inside the section */                                                                       
            while(buffer != '[' && buffer != EOF) {
                /* Add 1 to both of the value counters unless it's a comment */
                if(buffer != ';') {
                    /* Read until the end of line */                                                                             
                    while(buffer != '\n' && buffer != EOF) {
                        buffer = fgetc(stream);
                    }
                    value_count++;
                    subsection_values[section_count] += 1;
                } else {
                    /* Read until the end of line */                                                                                       
                    while(buffer != '\n' && buffer != EOF) {
                        buffer = fgetc(stream);
                    }
                }
                /* Skips all the new lines until a new character */
                while(buffer == '\n' && buffer != EOF) {
                    buffer = fgetc(stream);
                }                                                                          
            }
            section_count++;
        }
        /* In the case that the file starts without a section, count all the values and offsets the section count by 1 */
        else {    
            /* Read until the end of line */                                                                                          
            while(buffer != '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }
            /* Skips all the new lines until a new character */
            while(buffer == '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }
            section_count = 1; 
            value_count++;
            subsection_values[0]++;
        }
    }
    /* Sets the Pe_File values to the right ammount */
    file->section_count = section_count;
    file->value_count = value_count;
    for(i = 0; i < section_count; i++) {
        file->subsectiont_values[i] = subsection_values[i];
    }

    /* Debug */
    /*printf("The program has found %d Sections and %d values.\n", section_count, value_count);
    for(i = 0; i < section_count; i++) {
        printf("    Section %d : %d values\n", i, subsection_values[i]);
    }*/
    

    /* MEMORY ALLOCATION */
    
    file->matrix = malloc(section_count * sizeof *file->matrix);
    for(i = 0; i < section_count; i++) {
        file->matrix[i] = malloc((subsection_values[i] * 2 + 1) * sizeof *file->matrix[i]);
        for(j = 0; j < subsection_values[i] * 2 + 1; j++) {
            file->matrix[i][j] = malloc(32 * sizeof *file->matrix[i][j]);
        }
    }

    /* Initializes the file matrix to to 0 */
    for(i = 0; i < section_count; i++) {
        for(j = 0; j < subsection_values[i] * 2 + 1; j++) {
            for(k = 0; k < 32; k++) {
                file->matrix[i][j][k] = 0;
            } 
        }
    }
    
    
    /* Resets the counting values to 0 */
    section_count = 0;
    value_count = 0;
    for(i = 0; i < file->section_count; i++) {
        subsection_values[i] = 0;
    }

    /* READ ALL VALUES */

    /* Return to the beginning of the file */
    fseek(stream, 0, SEEK_SET);

    buffer = fgetc(stream);
    while(buffer != EOF) {
        /* Ignores comments */
        if(buffer == ';') {
            /* Read until the end of line */
            while(buffer != '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }
            /* Skips all the new lines until a new character */
            while(buffer == '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }
        } 
        /* If it encounters a section, stores the Section name in matrix[i][0], then proceed to store the data in matrix[i][j] */
        else if(buffer == '[') {
            buffer = fgetc(stream);
            i = 0;
            /* Read until it hits ] and store the section name */
            while(buffer != ']' && buffer != '\n' && buffer != EOF && i < 32) {
                file->matrix[section_count][0][i] = buffer;
                i++;
                buffer = fgetc(stream);
            }
            /* Skips the ] character */
            buffer = fgetc(stream);
            /* Skips all the new lines until a new character */                                                                                     
            while(buffer == '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }
            /* Store all the data in the matrix until it hits the beginning of another section */
            while(buffer != '[' && buffer != EOF) {
                /* Ignores if it's a comment */
                if(buffer == ';') {
                    /* Read until the end of line */
                    while(buffer != '\n' && buffer != EOF) {
                        buffer = fgetc(stream);
                    }
                    /* Skips all the new lines until a new character */
                    while(buffer == '\n' && buffer != EOF) {
                        buffer = fgetc(stream);
                    }
                } else {
                    i = 0;
                    /* Stores the variable name in matrix[current section][j+1] */
                    while(buffer != '=' && buffer != '\n' && buffer != EOF && i < 32) {
                        file->matrix[section_count][subsection_values[section_count] + 1][i] = buffer;
                        buffer = fgetc(stream);
                        i++;
                    }
                    /* Skips the = character */
                    buffer = fgetc(stream);
                    i = 0;
                    /* Stores the variable value in matrix[current section][j+2] */
                    while(buffer != '\n' && buffer != EOF && i < 32) {
                        file->matrix[section_count][subsection_values[section_count] + 2][i] = buffer;
                        buffer = fgetc(stream);
                        i++;
                    }
                    /* Skips all the new lines until a new character */
                    while(buffer == '\n' && buffer != EOF) {
                        buffer = fgetc(stream);
                    }
                    subsection_values[section_count] += 2;
                }
            }
            section_count++;
        } 
        /* In the case that the file starts without a section, stores the first values in the PE_DEFAULT section */
        else {
            i = 0;
            /* Stores the default section name */
            strcpy(file->matrix[0][0], "PE_DEFAULT");
            /* Stores the variable name in matrix[PE_DEFAULT]][j+1] */
            while(buffer != '=' && buffer != '\n' && buffer != EOF && i < 32) {
                file->matrix[0][value_count+1][i] = buffer;
                buffer = fgetc(stream);
                i++;
            }
            /* Skips the = character */
            buffer = fgetc(stream);
            i = 0;
            /* Stores the variable value in matrix[PE_DEFAULT]][j+2] */
            while(buffer != '\n' && buffer != EOF && i < 32) {
                file->matrix[0][value_count+2][i] = buffer;
                buffer = fgetc(stream);
                i++;
            }
            value_count += 2;
            subsection_values[0] += 2;
            section_count = 1;
            /* Skips all the new lines until a new character */
            while(buffer == '\n' && buffer != EOF) {
                buffer = fgetc(stream);
            }
        } 
    }
    fclose(stream);
    return 0;
}

/* Frees the file's matrix */
void pe_file_destroy(Pe_File* file) {
    int i, j;
    for(i = 0; i < file->section_count; i++) {
        for(j = 0; j < file->subsectiont_values[i] * 2 + 1; j++) {
            free(file->matrix[i][j]);
        }
        free(file->matrix[i]);
    }
    free(file->matrix);
}

/* ---------- DISPLAY AND GET VALUES ---------- */

void pe_file_display(Pe_File* file) {
    int i, j;
    if(file->value_count == 0) {
        fprintf(stdout, "The file is empty.\n");
    } else {
       for(i = 0; i < file->section_count; i++) {
            fprintf(stdout, "[%s]\n", file->matrix[i][0]);
            for(j = 0; j < file->subsectiont_values[i] * 2; j += 2) {
                fprintf(stdout, "    %s = %s\n", file->matrix[i][j+1], file->matrix[i][j+2]);
            }
        }
    }
}

char* pe_value_get(Pe_File* file, char* section, char* variable) {
    int i, section_index;
    char* return_string;
    return_string = malloc(32 * sizeof *return_string);
    for(i = 0; i < 32; i++) 
        return_string[i] = 0;

    for(i = 0; i < file->section_count; i++) {
        if(strcmp(file->matrix[i][0], section) == 0)
            section_index = i;
    }
    for(i = 0; i < file->subsectiont_values[section_index]*2+1; i++) {
        if(strcmp(file->matrix[section_index][i], variable) == 0) 
            strcpy(return_string, file->matrix[section_index][i+1]);
    }
    return return_string;
}

int pe_value_get_as_int(Pe_File* file, char* section, char* variable) {
    int rtrn_value;
    char* string;
    unsigned int i;
    rtrn_value = 0;
    string = malloc(strlen(pe_value_get(file, section, variable)) * sizeof *string);
    strcpy(string, pe_value_get(file, section, variable));
    for(i = 0; i < strlen(string); i++) {
        rtrn_value += (string[i]-48) * pow(10, strlen(string) - i - 1);
    }

    return rtrn_value;

}
