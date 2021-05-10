#include "../Espadon.h"

float es_dot_product(float* array1, float* array2, uint16_t length) {
    uint16_t i;
    float result;
    result = 0;
    for(i = 0; i < length; i++) {
        result += array1[i] * array2[i];
    }
    return result;
}

float es_radians(float angle) {
    return (float) ((double)angle / (180.0/ES_PI));
}

/* Es_Matrix */

Es_Matrix* es_matrix_create(uint16_t rows, uint16_t columns) {
    Es_Matrix* rtrn_matrix;
    uint16_t i;
    rtrn_matrix = malloc(sizeof *rtrn_matrix + sizeof (float) * (rows * columns -1));               /* Allocate the size of the matrix to be the size of the struct + the matrix itself so they are one block of memory */
    rtrn_matrix->columns = columns;
    rtrn_matrix->rows = rows;
    for(i = 0; i < rows * columns; i++) {                                                           /* Fills the matrix with 0 */
        rtrn_matrix->matrix[i] = 0;
    }
    return rtrn_matrix;
}

Es_Matrix* es_matrix_create_identity(uint16_t rows, uint16_t columns) {
    if(rows == columns) {
        uint16_t i;                                                                           /* Checks if it's a square matrix */
        Es_Matrix* rtrn_matrix;

        rtrn_matrix = es_matrix_create(rows, columns);
        
        for(i = 0; i < rows; i++) {                                                            /* Fills the diagonals with 1 */
            *es_matrix_get_value(rtrn_matrix, i, i) = 1;
        }

        return rtrn_matrix;
    } else {
        printf("The matrix must be a square matrix.\n");
        return NULL;
    }
}

Es_Matrix* es_matrix_create_translation_2D(float x, float y) {                                      /* Creates a matrix like so : */
    Es_Matrix* rtrn_matrix;
    rtrn_matrix = es_matrix_create_identity(3, 3);                                                  /* [1 0 x]                    */
    *es_matrix_get_value(rtrn_matrix, 0, 2) = x;                                                    /* [0 1 y]                    */
    *es_matrix_get_value(rtrn_matrix, 1, 2) = y;                                                    /* [0 0 1]                    */
    return rtrn_matrix;
}

Es_Matrix* es_matrix_create_scale_2D(float x, float y) {                                            /* Creates a matrix like so : */
    Es_Matrix* rtrn_matrix;
    rtrn_matrix = es_matrix_create_identity(3, 3);                                       /* [x 0 0]                    */
    *es_matrix_get_value(rtrn_matrix, 0, 0) = x;                                                    /* [0 y 0]                    */
    *es_matrix_get_value(rtrn_matrix, 1, 1) = y;                                                    /* [0 0 1]                    */
    return rtrn_matrix;
}

Es_Matrix* es_matrix_create_rotation_2D(float angle) {
    Es_Matrix* rtrn_matrix;
    rtrn_matrix = es_matrix_create_identity(3, 3);                                       /* Creates a matrix like so : */
    *es_matrix_get_value(rtrn_matrix, 0, 0) = cos(angle);                                           /* [cos(angle) -sin(angle) 0] */
    *es_matrix_get_value(rtrn_matrix, 0, 1) = -sin(angle);                                          /* [sin(angle) cos(angle)  0] */
    *es_matrix_get_value(rtrn_matrix, 1, 0) = sin(angle);                                           /* [    0         0        1] */
    *es_matrix_get_value(rtrn_matrix, 1, 1) = cos(angle);
    return rtrn_matrix;
}

float* es_matrix_get_value(Es_Matrix* matrix, uint16_t i, uint16_t j) { 
    return matrix->matrix + (i * matrix->columns + j);                                              /* calculates the offset needed to find the value at m(i, j) */
}

void es_matrix_get_row(float* array, Es_Matrix* m, uint16_t row) {
    uint16_t i;
    for(i = 0; i < m->columns; i++) {                                                               /* Fills the array with the values from the row */
        array[i] = *(m->matrix + row * m->columns + i);
    }
}

void es_matrix_get_column(float* array, Es_Matrix* m, uint16_t column) {
    uint16_t i;
    for(i = 0; i < m->rows; i++) {                                                                  /* Fills the array with the values from the column */
        array[i] = *(m->matrix + i * m->columns + column);
    }
}

Es_Matrix* es_matrix_mult(Es_Matrix* m1, Es_Matrix* m2) {
    if(m1->columns == m2->rows) {                                                                   /* Checks if the matrixes are the right size */
        Es_Matrix* result;
        uint16_t i, j;
        float *row, *column;

        result = es_matrix_create(m1->rows, m2->columns);                                /* Creates a matrix to store the result */
        row = malloc(sizeof(float) * m1->columns);                                           /* Creates a float array to store the content of the rows of m1 */
        column = malloc(sizeof(float) * m2->rows);                                           /* Creates a float array to store the content of the columns of m2 */
        
        for(i = 0; i < result->rows; i++) {                                                    /* For each cell in the result matrix */
            for(j = 0; j < result->columns; j++) {
                es_matrix_get_row(row, m1, i);                                                      /* Sets the row that needs to be processed to m1(i) */
                es_matrix_get_column(column, m2, j);                                                /* Sets the column that needs to be processed to m2(j) */
                *es_matrix_get_value(result, i, j) = es_dot_product(column, row, m1->columns);      /* Sets the value of result(i, j) to be the dot product of m1(i)row and m2(j)column */
            }
        }
        free(row);                                                                                  /* Frees the memory of the temp variables and returns the result */
        free(column);
        return result;
    } else {
        printf("The two matrixes cannot be multiplied.\n");
        return NULL;
    }
}

void es_matrix_to_vector2(Es_Vector2* v, Es_Matrix* m) {                                            /* Converts a vector of format v(x, y, i) */
    if(m->rows == 3 && m->columns == 1) {                                                           /* to a (3x1)matrix :                     */
        v->x = *es_matrix_get_value(m, 0, 0);                                                       /* [x]                                    */
        v->y = *es_matrix_get_value(m, 1, 0);                                                       /* [y]                                    */
        v->i = *es_matrix_get_value(m, 2, 0);                                                       /* [i]                                    */
    } else {
        printf("The matrix must be 3x1.");
    }
}

void es_matrix_display(Es_Matrix* m) {
    uint16_t i, j;
    for(i = 0; i < m->rows; i++) {
        printf("[");
        for(j = 0; j < m->columns; j++) {
            printf(" %.1f ", *es_matrix_get_value(m, i, j));
        }
        printf("]\n");
    }
}

/* Es_Vector2 */

Es_Vector2* es_vector2_create(float x, float y) {
    Es_Vector2* rtrn_vector;
    rtrn_vector = malloc(sizeof (Es_Vector2));
    rtrn_vector->i = 1;                                                                             /* i has no purpose exept being used in matrix calculations when a vector transformation occurs */
    rtrn_vector->x = x;
    rtrn_vector->y = y;
    return rtrn_vector;
}

void es_vector2_to_matrix(Es_Matrix* m, Es_Vector2* v) {                                            /* Turns a 3x1 matrix into a vector */
    if(m->rows == 3 && m->columns == 1) {
        *es_matrix_get_value(m, 0, 0) = v->x;
        *es_matrix_get_value(m, 1, 0) = v->y;
        *es_matrix_get_value(m, 2, 0) = v->i;
    } else {
        printf("The matrix must be 3x1.\n");
    }
}

void es_vector2_translate(Es_Vector2* v, float x, float y) {
    Es_Matrix* trans_matrix;
    Es_Matrix* vector_matrix;
    Es_Matrix* result_matrix;

    trans_matrix = es_matrix_create_translation_2D(x, y);                                /* Creates a translation matrix based on x and y */
    
    vector_matrix = es_matrix_create(3,1);                                               /* Creates a matrix to store the vector */
    es_vector2_to_matrix(vector_matrix, v);                                                         /* Stores the vector values into a matrix */
    
    result_matrix = es_matrix_mult(trans_matrix, vector_matrix);                         /* Creates a matrix to store the resulting vector and multiplies the transformation matrix by the vector matrix */
    es_matrix_to_vector2(v, result_matrix);                                                         /* Overwrites the vector with the result matrix values */

    free(trans_matrix);                                                                             /* frees the memory duh */
    free(vector_matrix);
    free(result_matrix);
}

void es_vector2_scale(Es_Vector2* v, float x, float y) {
    Es_Matrix* trans_matrix;
    Es_Matrix* vector_matrix;
    Es_Matrix* result_matrix;

    trans_matrix = es_matrix_create_scale_2D(x, y);                                      /* Creates a scale matrix based on x and y */
    
    vector_matrix = es_matrix_create(3, 1);                                              /* Creates a matrix to store the vector */
    es_vector2_to_matrix(vector_matrix, v);                                                         /* Stores the vector values into a matrix */
    
    result_matrix = es_matrix_mult(trans_matrix, vector_matrix);                         /* Creates a matrix to store the resulting vector and multiplies the transformation matrix by the vector matrix */
    es_matrix_to_vector2(v, result_matrix);                                                         /* Overwrites the vector with the result matrix values */

    free(trans_matrix);
    free(vector_matrix);
    free(result_matrix);
}

void es_vector2_rotate(Es_Vector2* v, float angle) {
    Es_Matrix* trans_matrix;
    Es_Matrix* vector_matrix;
    Es_Matrix* result_matrix;
    trans_matrix = es_matrix_create_rotation_2D(angle);                                  /* Creates a rotation matrix based on the angle */
    
    vector_matrix = es_matrix_create(3, 1);                                              /* Creates a matrix to store the vector */
    es_vector2_to_matrix(vector_matrix, v);                                                         /* Stores the vector values into a matrix */
    
    result_matrix = es_matrix_mult(trans_matrix, vector_matrix);                         /* Creates a matrix to store the resulting vector and multiplies the transformation matrix by the vector matrix */
    es_matrix_to_vector2(v, result_matrix);                                                         /* Overwrites the vector with the result matrix values */

    free(trans_matrix);
    free(vector_matrix);
    free(result_matrix);
}

void es_vector2_transform(Es_Vector2* v, Es_Matrix* m) {
    Es_Matrix* vector_matrix;
    Es_Matrix* result_matrix;
    vector_matrix = es_matrix_create(3, 1);                                              /* Creates a matrix to store the vector */
    es_vector2_to_matrix(vector_matrix, v);                                                         /* Stores the vector values into the matrix */
    
    result_matrix = es_matrix_mult(m, vector_matrix);                                    /* Creates a matrix to store the resulting vector and multiplies the transformation matrix by the vector matrix */
    es_matrix_to_vector2(v, result_matrix);                                                         /* Overwrites the vector with the result matrix values */

    free(vector_matrix);
    free(result_matrix);
}

void es_vector2_display(Es_Vector2* v) {
    printf("(%.1f, %.1f)\n", v->x, v->y);
}