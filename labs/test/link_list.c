#include <stdio.h>
#include <stdlib.h>

struct ll_node {
	int value;
	struct ll_node* next;
}

ll_node* create_node(int value; ll_node* next) {
	ll_node* new = (ll_node*) malloc(sizeof(ll_node));
	if (new == NULL) {
		printf("Memory exhausted!\n");
		exit(1);
	}else {
		new->value = value;
		new->next = next;
		return new;
	}
}


void prepend(struct ll_node** list, int value) {
	if (*list == NULL) {
		*list = create_node(value, NULL);
		return;		
	}else{
		struct ll_node *ptr = create_node(value, NULL);
		ptr->next = *list;
		*list = ptr;
	}
	 
}
