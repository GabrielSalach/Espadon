#include "../Espadon.h"

/* Events */

Es_Event* es_event_create(Es_Event_Type type) {
    Es_Event* rtrn_event;
    rtrn_event = malloc(sizeof *rtrn_event);
    rtrn_event->type = type;
    rtrn_event->event_pointer = NULL;
    return rtrn_event;
}

Es_Event_Keyboard* es_event_keyboard_down_create(char keycode) {
    Es_Event_Keyboard* rtrn_event;
    rtrn_event = malloc(sizeof *rtrn_event);
    rtrn_event->event = es_event_create(ES_EVENT_KEYBOARD_DOWN);
    rtrn_event->event->event_pointer = rtrn_event;
    rtrn_event->keycode = keycode;
    return rtrn_event;
}

Es_Event_Keyboard*      es_event_keyboard_up_create(char keycode);                                  /* Creates a keyboard up event with the keycode passed as argument. */

Es_Event_Window*        es_event_window_close_create() {
    Es_Event_Window* rtrn_event;
    rtrn_event = malloc(sizeof *rtrn_event);
    rtrn_event->event = es_event_create(ES_EVENT_WINDOW_CLOSE);
    rtrn_event->event->event_pointer = rtrn_event;
    return rtrn_event;
}


Es_Event_Window*        es_event_window_resize_create(int width, int height);                       /* Creates a window resize event with the width and height passed as argument. */
Es_Event_Window*        es_event_window_focus_create(int focus);                                    /* Creates a window focus event with the state passed as argument. */
Es_Event_Mouse*         es_event_mouse_down_create(char keycode);                                   /* Creates a mouse button down event with the keycode passed as argument. */
Es_Event_Mouse*         es_event_mouse_up_create(char keycode);                                     /* Creates a mouse button up event with the keycode passed as argument. */
Es_Event_Mouse*         es_event_mouse_move_create(double posX, double posY);                       /* Creates a mouse move event with the X and Y positions passed as argument. */

/* Event bus */
Es_Event_Bus* es_event_bus_create() {
    Es_Event_Bus* rtrn_bus;
    rtrn_bus = malloc(sizeof *rtrn_bus);
    rtrn_bus->bus = malloc(ES_MAX_EVENT_BUS_SIZE * sizeof *(rtrn_bus->bus));
    rtrn_bus->event_count = 0;
    return rtrn_bus;
}

void es_event_bus_add(Es_Event_Bus* bus, Es_Event* event) {
    if(bus->event_count < ES_MAX_EVENT_BUS_SIZE) {
        bus->bus[bus->event_count] = event;
        bus->event_count++;
    } else {
        ES_ERROR("The event bus has reached its max size !\n");
    }
}

/* Handlers */
Es_Event_Handler* es_event_handler_create(Es_Event_Type type) {
    Es_Event_Handler* rtrn_handler;
    rtrn_handler = malloc(sizeof *rtrn_handler);
    rtrn_handler->type = type;
    rtrn_handler->event = NULL;
    return rtrn_handler;
}


Es_Event_Handler_Array* es_event_handler_array_create() {
    Es_Event_Handler_Array* rtrn_array;
    rtrn_array = malloc(sizeof *rtrn_array);
    rtrn_array->array = malloc(ES_MAX_HANDLER_ARRAY_SIZE * sizeof *(rtrn_array->array));
    rtrn_array->handler_count = 0;
    return rtrn_array;
}

void es_event_handler_array_add(Es_Event_Handler_Array* array, Es_Event_Handler* handler) {
    if(array->handler_count < ES_MAX_HANDLER_ARRAY_SIZE) {
        array->array[array->handler_count] = handler;
        array->handler_count++;
    } else {
        ES_ERROR("The handler array has reached its max size !\n");
    }
}

void es_event_handler_array_clear(Es_Event_Handler_Array* array) {
    uint16_t i;
    for(i = 0; i < array->handler_count; i++) {
        if(array->array[i]->event != NULL) {
            es_event_destroy(array->array[i]->event);
            array->array[i]->event = NULL;
        }
    }
}

/* Process */
void es_event_process(Es_Event_Bus* bus, Es_Event_Handler_Array* handler_array) {
    Es_Event_Type type;
    uint16_t i;

    if(bus->event_count == 0) {
        ES_LOG("There isn't any event to process.\n");
    } else {
        if(handler_array->handler_count == 0) {
            ES_LOG("There isn't any listeners available.\n");
        } else {
            type = bus->bus[0]->type;
            for(i = 0; i < handler_array->handler_count; i++) {
                if(type == handler_array->array[i]->type) {
                    handler_array->array[i]->event = bus->bus[0];
                }
            }
            bus->event_count--;
            for(i = 0; i < bus->event_count; i++) {
                bus->bus[i] = bus->bus[i+1];
            }
        }
    }
}


/* Destructors */

void es_event_destroy(Es_Event* event) {
    free(event->event_pointer);
    free(event);
}

void es_event_bus_destroy(Es_Event_Bus* event_bus) {
    unsigned int i;
    for(i = 0; i < event_bus->event_count; i++) {
        es_event_destroy(event_bus->bus[i]);
    }
    free(event_bus->bus);
    free(event_bus);
}

void es_event_handler_destroy(Es_Event_Handler* handler) {
    if(handler->event != NULL) {
        es_event_destroy(handler->event);
        handler->event = NULL;
    }
    free(handler);
}

void es_event_handler_array_destroy(Es_Event_Handler_Array* handler_array) {
    unsigned int i;
    for(i = 0; i < handler_array->handler_count; i++) {
        es_event_handler_destroy(handler_array->array[i]);
    }
    free(handler_array->array);
    free(handler_array);
}