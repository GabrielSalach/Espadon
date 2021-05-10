#include "../Espadon.h"

Es_Layer* es_layer_create(Es_Layer_Type type) {
	Es_Layer* rtrn_layer;
	rtrn_layer = malloc(sizeof *rtrn_layer);

	rtrn_layer->layer_type = type;
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

	rtrn_stack = malloc(sizeof *rtrn_stack);
	rtrn_stack->layer_count_total = 0;
	rtrn_stack->layer_count_normal = 0;
	rtrn_stack->layer_count_overlay = 0;
	rtrn_stack->layer_overlay_start_index = ES_DEFAULT_LAYER_STACK_SIZE - 1;
	rtrn_stack->current_size = ES_DEFAULT_LAYER_STACK_SIZE;

	rtrn_stack->array = malloc(ES_DEFAULT_LAYER_STACK_SIZE * sizeof *(rtrn_stack->array));

	return rtrn_stack;
}

void es_layer_stack_destroy(Es_Layer_Stack* stack) {
	int i;
	for(i = 0; i < stack->layer_count_total; i++) {
		es_layer_destroy(stack->array[i]);
		stack->array[i] = NULL;
	}
	free(stack->array);
	stack->array = NULL;
	free(stack);
}

void es_layer_stack_push(Es_Layer_Stack* stack, Es_Layer* layer) {
	if(stack->current_size == stack->layer_count_total) {
		/* Realloc */
		/*
			Push all overlays to the end and don't forget to set indexes correctly
		*/
	} else {
		/*
			push normal layers at layer_count_normal and overlays at overlay_index
			if the layer is a normal layer and the current index is overlay, push overlays to the next index to leave space
		*/
	}
}