#include "../Espadon.h"

Es_Time es_time = {{0,0}, {0,0}};

Es_Event_Bus* event_bus_main;
Es_Event_Handler_Array* handler_array_main;
Es_Event_Handler* window_close_listener;

GLFWwindow* es_main_window;

Es_Event* buffer;

void es_render();

/* Entry point */

void es_init() {
    int width, height;
    Pe_File* settings_file;

    ES_LOG("Espadon Engine Started !\n");

    ES_LOG("Events creation\n");
    event_bus_main = es_event_bus_create();
    handler_array_main = es_event_handler_array_create();
    window_close_listener = es_event_handler_create(ES_EVENT_WINDOW_CLOSE);
    es_event_handler_array_add(handler_array_main, window_close_listener);

    ES_LOG("Getting settings\n");
    settings_file = pe_file_create("Espadon Settings/es_settings.ini");
    pe_file_read(settings_file);
    width = pe_value_get_as_int(settings_file, "Window", "width");
    height = pe_value_get_as_int(settings_file, "Window", "height");
    pe_file_destroy(settings_file);

    if (!glfwInit())
        return;

    es_main_window = glfwCreateWindow(width, height, "Espadon Engine", NULL, NULL);
    if(!es_main_window) {
        return;
    }

    glfwMakeContextCurrent(es_main_window);
}

void es_update() {
    /* UPDATE ES_TIME */
    timespec now;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
    es_time.delta_time = es_time_diff(es_time.time, now);
    es_time.time = now;

    /* Events */
    es_event_handler_array_clear(handler_array_main);
    es_event_process(event_bus_main, handler_array_main);

    /* Render */
    es_render();
    
    
}

void es_late_update() {
    glfwPollEvents();
    if(glfwWindowShouldClose(es_main_window)) {
        buffer = es_event_create(ES_EVENT_WINDOW_CLOSE);
        es_event_bus_add(event_bus_main, buffer);
    }
}

void es_exit() {
    es_event_bus_destroy(event_bus_main);
    es_event_handler_array_destroy(handler_array_main);

    glfwTerminate();
}

/* Rendering */
void es_render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(es_main_window);
}

/* Timescale */

double es_delta_time() {
    return (double) es_time.delta_time.tv_sec + (double) es_time.delta_time.tv_nsec / ES_NANOSEC;
}

timespec es_time_diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = ES_NANOSEC+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

/* Event getters */

int es_get_keyboard_down(char keycode) {
    uint16_t i;
    int rtrn_value;
    Es_Event_Keyboard* key_event;
        
    rtrn_value = 0;
    for(i = 0; i < handler_array_main->handler_count; i++) {
        if(handler_array_main->array[i]->type == ES_EVENT_KEYBOARD_DOWN && handler_array_main->array[i]->event != NULL) {
            key_event = handler_array_main->array[i]->event->event_pointer;
            if(key_event->keycode == keycode)
                rtrn_value = 1;
        }
    }
    return rtrn_value;
}

int es_get_window_close() {
    int rtrn_value;
    unsigned int i;
    rtrn_value = 0;
    for(i = 0; i < handler_array_main->handler_count; i++) {
        if(handler_array_main->array[i]->type == ES_EVENT_WINDOW_CLOSE && handler_array_main->array[i]->event != NULL) {
            rtrn_value = 1;
        }
    }
    return rtrn_value;
}