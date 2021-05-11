#include "../Espadon.h"

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

Es_Application* es_application_create(char* path_to_ini) {
	Es_Application* rtrn_application;
	Pe_File* settings_file;

	rtrn_application = malloc(sizeof *rtrn_application);

	settings_file = pe_file_create(path_to_ini);
	pe_file_read(settings_file);
	rtrn_application->width = pe_value_get_as_int(settings_file, "Window", "width");
	rtrn_application->height = pe_value_get_as_int(settings_file, "Window", "height");
	rtrn_application->window_title = malloc(strlen(pe_value_get(settings_file, "Window", "title")) * sizeof *(rtrn_application->window_title));
	strcpy(rtrn_application->window_title, pe_value_get(settings_file, "Window", "title"));

	rtrn_application->time.delta_time.tv_nsec = 0;
	rtrn_application->time.delta_time.tv_sec = 0;
	rtrn_application->time.time.tv_nsec = 0;
	rtrn_application->time.time.tv_sec = 0;

	rtrn_application->main_window = NULL;

	rtrn_application->layer_stack = es_layer_stack_create();
	rtrn_application->global_event_listeners = es_event_listener_array_create();

	pe_file_destroy(settings_file);
	return rtrn_application;
}

int es_init(Es_Application* application) {
	if(!glfwInit()) 
		return -1;

	application->main_window = glfwCreateWindow(application->width, application->height, application->window_title, NULL, NULL);

	if(!application->main_window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(application->main_window);
	/* Add listeners to the global listener array and create default layers */
	return 0;
}

void es_update(Es_Application* application) {
	

	
	while(!glfwWindowShouldClose(application->main_window)) {
		/* Time updating */
		timespec now;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
		application->time.delta_time = es_time_diff(application->time.time, now);
		application->time.time = now;
		/* Event processing */

		/* Game update */

		/* Rendering */
		glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(application->main_window);
        glfwPollEvents();
	}
}

void es_exit(Es_Application* application) {
	glfwTerminate();
	free(application->window_title);
	application->window_title = NULL;
	free(application->main_window);
	application->main_window = NULL;
	free(application);
}

double es_delta_time(Es_Application* application) {
    return (double) application->time.delta_time.tv_sec + (double) application->time.delta_time.tv_nsec / ES_NANOSEC;
}