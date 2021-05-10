#include "../Espadon.h"

struct Es_Matrix{
    uint16_t rows, columns;                                                             /* Number of rows and columns. */
    float matrix[1];                                                                    /* Uses only one malloc(), keep matrix as the last element of Es_Matrix. */
};

struct Es_Vector2 {
    float x, y;                                                                         /* Coordinates of the vector. */
    uint16_t i; /* turn to const l8ter */                                               /* Const used for matrix transformations. */
};