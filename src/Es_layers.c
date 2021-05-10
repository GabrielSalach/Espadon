#include "../Espadon.h"

Es_Layer* es_layer_create(Es_Layer_Type type, char* layer_name) {
	Es_Layer* rtrn_layer;
	rtrn_layer = malloc(sizeof *rtrn_layer);

	rtrn_layer->layer_type = type;
	rtrn_layer->layer_name = malloc(strlen(layer_name) * sizeof *(rtrn_layer->layer_name));
	strcpy(rtrn_layer->layer_name, layer_name);
	rtrn_layer->listener_array = es_event_listener_array_create();

	return rtrn_layer;
}

void es_layer_destroy(Es_Layer* layer) {
	es_event_listener_array_destroy(layer->listener_array);
	layer->listener_array = NULL;
	free(layer);
}

Es_Layer_Stack* es_layer_stack_create() {
	Es_Layer_Stack* rtrn_stack;
	int i;

	rtrn_stack = malloc(sizeof *rtrn_stack);
	rtrn_stack->layer_count_total = 0;
	rtrn_stack->layer_count_normal = 0;
	rtrn_stack->layer_count_overlay = 0;
	rtrn_stack->layer_overlay_start_index = 0;
	rtrn_stack->current_size = ES_DEFAULT_LAYER_STACK_SIZE;

	rtrn_stack->array = malloc(ES_DEFAULT_LAYER_STACK_SIZE * sizeof *(rtrn_stack->array));
	for(i = 0; i < ES_DEFAULT_LAYER_STACK_SIZE; i++) {
		rtrn_stack->array[i] = NULL;
	}

	return rtrn_stack;
}

void es_layer_stack_destroy(Es_Layer_Stack* stack) {
	int i;
	for(i = 0; i < stack->layer_count_total; i++) {
		es_layer_destroy(stack->array[i]);
	}
	free(stack->array);
	stack->array = NULL;
	free(stack);
}

void es_layer_stack_push(Es_Layer_Stack* stack, Es_Layer* layer) {
	int i;

	if(stack->current_size == stack->layer_count_total) {
		stack->current_size += ES_DEFAULT_LAYER_STACK_SIZE;
		stack->array = realloc(stack->array, stack->current_size);
		
	} 
	if(layer->layer_type == ES_LAYER_NORMAL) {
		if(stack->layer_count_normal >= stack->layer_overlay_start_index) {
			for(i = stack->layer_count_total; i >= stack->layer_overlay_start_index; i--) {
				stack->array[i+1] = stack->array[i];
			}
			stack->layer_overlay_start_index++;
		} 
		stack->array[stack->layer_count_normal] = layer;
		stack->layer_count_normal++;
	} else if(layer->layer_type == ES_LAYER_OVERLAY) {
		if(stack->layer_overlay_start_index < stack->layer_count_normal) {
			stack->layer_overlay_start_index = stack->layer_count_normal;
		}
		stack->array[stack->layer_overlay_start_index + stack->layer_count_overlay] = layer;
		stack->layer_count_overlay++;
	} else {
		ES_ERROR("The layer cannot be pushed into the stack because the type is not recognized.");
		return;
	}
	
	stack->layer_count_total++;
}