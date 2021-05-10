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

	rtrn_application = malloc(sizeof &rtrn_application);

	settings_file = pe_file_create(path_to_ini);
	pe_file_read(settings_file);
	rtrn_application->width = pe_value_get_as_int(settings_file, "Window", "width");
	rtrn_application->height = pe_value_get_as_int(settings_file, "Window", "height");
	rtrn_application->window_title = malloc(strlen(pe_value_get(settings_file, "Window", "title")) * sizeof &rtrn_application->window_title);
	strcpy(rtrn_application->window_title, pe_value_get(settings_file, "Window", "title"));

	rtrn_application->time.delta_time.tv_nsec = 0;
	rtrn_application->time.delta_time.tv_sec = 0;
	rtrn_application->time.time.tv_nsec = 0;
	rtrn_application->time.time.tv_sec = 0;

	rtrn_application->main_window = NULL;

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
	return 0;
}

void es_update(Es_Application* application) {
	while(!glfwWindowShouldClose(application->main_window)) {
		glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(application->main_window);

        /* Poll for and process events */
        glfwPollEvents();
	}
}

void es_exit(Es_Application* application) {
	glfwTerminate();
}