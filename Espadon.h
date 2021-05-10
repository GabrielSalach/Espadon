#ifndef ESPADON_ENGINE 
#define ESPADON_ENGINE _

/* ---------- INCLUDES ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Percemaille.h>
#include <GLFW/glfw3.h>

/* ---------- MACROS ---------- */
/* Logging  */
#define ES_DEBUG 0
#define ES_ERROR(x)     if(ES_DEBUG) fprintf(stderr, x)
#define ES_LOG(x)       if(ES_DEBUG) fprintf(stdout, x)

/* Maths */
#define ES_PI acos(-1.0)

/* Time */
#define ES_NANOSEC 1000000000
typedef struct timespec timespec;                                                       /* typedef of the timespec struct from time.h */

/* Events */
#define ES_MAX_EVENT_BUS_SIZE 8 
#define ES_MAX_HANDLER_ARRAY_SIZE 8

/* Layers */
#define ES_DEFAULT_LAYER_STACK_SIZE 3

/* ---------- STRUCTS AND ENUMS ---------- */
/* Es_math.c */

typedef struct {
    uint16_t rows, columns;                                                             /* Number of rows and columns. */
    float matrix[1];                                                                    /* Uses only one malloc(), keep matrix as the last element of Es_Matrix. */
} Es_Matrix;

typedef struct {
    float x, y;                                                                         /* Coordinates of the vector. */
    uint16_t i; /* turn to const l8ter */                                               /* Const used for matrix transformations. */
} Es_Vector2;

/* Es_profiler.c */

typedef struct {
    struct timespec starting_time;                                                      /* Timestamp set when using es_profiler_start() method. */
    struct timespec ending_time;                                                        /* Timestamp set when using es_profiler_stop() method. */
    double elapsed_time;                                                                /* Difference between ending_time and starting_time. */
} Es_Profiler;


/* Es_events.c */

typedef enum Es_Event_Type {
    ES_EVENT_NONE = 0,
    ES_EVENT_KEYBOARD_DOWN, ES_EVENT_KEYBOARD_UP,                                       /* Keyboard events. */
    ES_EVENT_WINDOW_CLOSE, ES_EVENT_WINDOW_RESIZE, ES_EVENT_WINDOW_FOCUS,               /* Window events. */
    ES_EVENT_MOUSE_MOVE, ES_EVENT_MOUSE_BUTTON_DOWN, ES_EVENT_MOUSE_BUTTON_UP           /* Mouse events. */
}Es_Event_Type;

typedef struct {                                                                        /* Base struct for all events. */
    Es_Event_Type type;                                                             
    void* event_pointer;
}Es_Event;

typedef struct {
    Es_Event* event;
    char keycode; /* Replace with real keycode later */                                 /* Keycode pressed/released. */
}Es_Event_Keyboard;

typedef struct {
    Es_Event* event;
    char keycode;                                                                       /* Keycode pressed/released. */
    double posX, posY;                                                                  /* X and Y positions of the mouse. */
}Es_Event_Mouse;

typedef struct {
    Es_Event* event;
    double posX, posY;                                                                  /* X and Y positions of the window */
    int width, height;                                                                  /* Width and height of the window */
    int focus;                                                                          /* Is the window the main focus, 0 = NO, 1 = YES. */
}Es_Event_Window;

typedef struct {
    Es_Event_Type type;                                                                 /* Type of events the listener listens to. */
    Es_Event* event;                                                                    /* Current event. */
} Es_Event_Listener;

typedef struct {
    Es_Event** bus;                                                                     /* Stores all the events. */
    uint16_t event_count;                                                               /* Ammount of events currently stored in the bus. */
} Es_Event_Bus;

typedef struct {
    Es_Event_Listener** array;                                                          /* Listener array */
    uint16_t listener_count;                                                            /* Ammount of listeners currently stored in the array. */
} Es_Event_Listener_Array;

/* Es_layers.c */

typedef enum Es_Layer_Type {                                                                          /* Layers types. Overlays are drawn on top. */
    ES_LAYER_NONE = 0,
    ES_LAYER_OVERLAY, ES_LAYER_NORMAL                                                  
} Es_Layer_Type;

typedef struct {
    Es_Event_Listener_Array* listener_array;                                            /* Array of listeners. */
    char* layer_name;
    Es_Layer_Type layer_type;                                                           /* Type of the layer. */
} Es_Layer;

typedef struct {
    int layer_count_total;                                                              /* Total ammount of layers in the array. */
    uint16_t layer_count_normal;                                                        /* Ammount of normal layers. */
    uint16_t layer_count_overlay;                                                       /* Ammount of overlay layers. */
    int layer_overlay_start_index;                                                      /* Index of the first overlay layer. */
    int current_size;                                                                   /* Current size of the array. */
    Es_Layer** array;                                                                   /* Array that stores all the layers in an application. */
} Es_Layer_Stack;

/* Es_application.c */

typedef struct {
    timespec delta_time;                                                                /* Time spent since last frame (in ms). */
    timespec time;                                                                      /* Time spent since the beginning of the program (in ms). */
} Es_Time;

typedef struct {
	char* window_title;
	Es_Layer_Stack* layer_stack;
	GLFWwindow* main_window;
	Es_Time time;
    uint16_t width, height;
} Es_Application;

/* ---------- ES_MATH ---------- */

float               es_dot_product(float* array1, float* array2, uint16_t length);      /* Returns the dot product of 2 float arrays. */
float               es_radians(float angle);                                            /* Returns the conversion of angle in radians. */

/* Es_Matrix */


Es_Matrix*          es_matrix_create(uint16_t rows, uint16_t columns);                  /* Creates and returns an empty matrix of size (rows, columns). */
Es_Matrix*          es_matrix_create_identity(uint16_t rows, uint16_t columns);         /* Creates and return a new identity matrix of size (rows, columns). */
Es_Matrix*          es_matrix_create_translation_2D(float x, float y);                  /* Creates and return a new 2D translation matrix. */
Es_Matrix*          es_matrix_create_scale_2D(float x, float y);                        /* Creates and return a new 2D scale matrix. */
Es_Matrix*          es_matrix_create_rotation_2D(float angle);                          /* Creates and return a new 2D rotation matrix. */
float*              es_matrix_get_value(Es_Matrix* matrix, uint16_t i, uint16_t j);     /* Returns a pointer to the value held in matrix[i][j]. */
void                es_matrix_get_row(float* array, Es_Matrix* m, uint16_t row);        /* Stores the row specified of the matrix m in the array. */
void                es_matrix_get_column(float* array, Es_Matrix* m, uint16_t column);  /* Stores the column specified of the matrix m in the array. */
Es_Matrix*          es_matrix_mult(Es_Matrix* m1, Es_Matrix* m2);                       /* Returns the result of the multiplication of m1 and m2. */
void                es_matrix_to_vector2(Es_Vector2* v, Es_Matrix* m);                  /* Stores a 3x1 Matrix inside the vector v. */  
void                es_matrix_display(Es_Matrix* m);                                    /* Displays the matrix m */

/* Es_Vector2 */

Es_Vector2*         es_vector2_create(float x, float y);                                /* Returns a Vector2(x, y). */
void                es_vector2_to_matrix(Es_Matrix* m, Es_Vector2* v);                  /* Stores the vector v in the 3x1 matrix m */
void                es_vector2_translate(Es_Vector2* v, float x, float y);              /* Translates the vector v by (x,y). */
void                es_vector2_scale(Es_Vector2* v, float x, float y);                  /* Scales the vector v by (x,y). */    
void                es_vector2_rotate(Es_Vector2* v, float angle);                      /* Rotates the vector v arround the Z axis by angle. */  
void                es_vector2_transform(Es_Vector2* v, Es_Matrix* m);                  /* Transforms the vector v accordingly to the transformation matrix m. */  
void                es_vector2_display(Es_Vector2* v);                                  /* Displays the vector v. */


/* ---------- ES_PROFILER ---------- */

Es_Profiler*        es_profiler_create();                                               /* Creates and returns a profiler. */
void                es_profiler_start(Es_Profiler* p);                                  /* Starts the profiler and stores the starting time inside it. */
void                es_profiler_stop(Es_Profiler* p);                                   /* Stops the profiler and stores the ending time inside it. */
double              es_profiler_get_elapsed_time(Es_Profiler* p);                       /* Returns the elapsed time. */
extern uint16_t     es_get_fps();                                                       /* Returns the number of frames per second. */

/* ---------- ES_APPLICATION ---------- */

Es_Application*     es_application_create(char* path_to_ini);
int                 es_init(Es_Application* application);                               /* Needs to be run at the start of the program */
void                es_update(Es_Application* application);                             /* Needs to be run each frame */
/*void              es_late_update();*/                                                 /* Runs at the end of each frame */
void                es_exit(Es_Application* application);                               /* Frees all the memory and exits the app. */
double              es_delta_time(Es_Application* application);                         /* Returns the time elapsed since last frame */
timespec            es_time_diff(timespec start, timespec end);                         /* Returns the difference between a starting time and an ending time. */

/* ---------- ES_EVENTS ---------- */

/* Events */
Es_Event*               es_event_create(Es_Event_Type type);                                        /* Creates an event with the type passed as argument. */
Es_Event_Keyboard*      es_event_keyboard_down_create(char keycode);                                /* Creates a keyboard down event with the keycode passed as argument. */
Es_Event_Keyboard*      es_event_keyboard_up_create(char keycode);                                  /* Creates a keyboard up event with the keycode passed as argument. */
Es_Event_Window*        es_event_window_close_create();                                             /* Creates a window close event. */
Es_Event_Window*        es_event_window_resize_create(int width, int height);                       /* Creates a window resize event with the width and height passed as argument. */
Es_Event_Window*        es_event_window_focus_create(int focus);                                    /* Creates a window focus event with the state passed as argument. */
Es_Event_Mouse*         es_event_mouse_down_create(char keycode);                                   /* Creates a mouse button down event with the keycode passed as argument. */
Es_Event_Mouse*         es_event_mouse_up_create(char keycode);                                     /* Creates a mouse button up event with the keycode passed as argument. */
Es_Event_Mouse*         es_event_mouse_move_create(double posX, double posY);                       /* Creates a mouse move event with the X and Y positions passed as argument. */

/* Event bus */
Es_Event_Bus*           es_event_bus_create();                                                      /* Creates an event bus. */
void        es_event_bus_add(Es_Event_Bus* bus, Es_Event* event);                                   /* Adds the event to the event bus. */

/* Listeners */
Es_Event_Listener*       es_event_listener_create(Es_Event_Type type);                                  /* Creates an event listener that listens to the event type passed as argument. */
Es_Event_Listener_Array* es_event_listener_array_create();                                              /* Creates an event listener array. */
void        es_event_listener_array_add(Es_Event_Listener_Array* array, Es_Event_Listener* listener);   /* Adds the listener to the listener array */
void        es_event_listener_array_clear(Es_Event_Listener_Array* array);                              /* Removes all the events from a listener array. */

/* Process */
void        es_event_process(Es_Event_Bus* bus, Es_Event_Listener_Array* listener_array);               /* Processes one event from the bus and calls the right listener from the listener list. */

int         es_get_keyboard_down(char keycode);                                                         /* TESTING ONLY !!!!! Returns if the key associated with the keycode has been pressed. */
int         es_get_window_close();

/* Destructor */
void        es_event_destroy(Es_Event* event);
void        es_event_bus_destroy(Es_Event_Bus* event_bus);
void        es_event_listener_destroy(Es_Event_Listener* listener);
void        es_event_listener_array_destroy(Es_Event_Listener_Array* listener_array);

/* ---------- ES_LAYERS ---------- */

Es_Layer*           es_layer_create(Es_Layer_Type type, char* layer_name);                                            /* Creates and returns a layer of the specified type. */

Es_Layer_Stack*     es_layer_stack_create();                                                        /* Creates and returns a layer stack. */
void                es_layer_stack_push(Es_Layer_Stack* stack, Es_Layer* layer);                 /* Adds a layer to the stack, and placing it regarding its type.*/

void                es_layer_destroy(Es_Layer* layer);                                              /* Destroys a layer. */
void                es_layer_stack_destroy(Es_Layer_Stack* stack);                                  /* Destroys a layer stack. */

#endif