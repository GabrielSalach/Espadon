#include "../Espadon.h"

struct Es_Event {                                                                        /* Base struct for all events. */
    Es_Event_Type type;                                                             
    void* event_pointer;
};

struct Es_Event_Keyboard {
    Es_Event* event;
    char keycode; /* Replace with real keycode later */                                 /* Keycode pressed/released. */
};

struct Es_Event_Mouse {
    Es_Event* event;
    char keycode;                                                                       /* Keycode pressed/released. */
    double posX, posY;                                                                  /* X and Y positions of the mouse. */
};

struct Es_Event_Window {
    Es_Event* event;
    double posX, posY;                                                                  /* X and Y positions of the window */
    int width, height;                                                                  /* Width and height of the window */
    int focus;                                                                          /* Is the window the main focus, 0 = NO, 1 = YES. */
};

struct Es_Event_Handler {
    Es_Event_Type type;                                                                 /* Type of events the handler listens to. */
    Es_Event* event;                                                                    /* Current event. */
};

struct Es_Event_Bus {
    Es_Event** bus;                                                                     /* Stores all the events. */
    uint16_t event_count;                                                               /* Ammount of events currently stored in the bus. */
};

struct Es_Event_Handler_Array {
    Es_Event_Handler** array;                                                            /* Handler array */
    uint16_t handler_count;                                                             /* Ammount of handlers currently stored in the array. */
};