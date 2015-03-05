#include <stdio.h>
#include <stdlib.h>

void get_next(char* id, int n) {
	if (id[n - 1] == '6') { 
		id[n - 1] = '1';
	}
	else if (id[n - 1] == '1') {
		id[n - 1] = 'c';
	}
	else if (id[n - 1] == 'c') {
		id[n - 1] = '6';
		get_next(id, n - 1);
	}
		
}

void next_id(char* id, int n) {
	int i;
	char new_id[n];
	for (i = 0; *(id + i) != '\0'; i++) {
		new_id[i] = *(id + i);
	}
	printf("%s\n", new_id);
	get_next(new_id, n);
	printf("%s\n", new_id);
	id = new_id;
}

int get_length(char* id) {
	int length = 0;
    while(id[length] != '\0') {
        length++;
    }
	return length;
}

main() {
	char id[] = "6666";
	get_next(id, 4);
	get_next(id, 4);
	get_next(id, 4);
	get_next(id, 4);
	printf("%s\n", id);
 	int length = get_length(id);
	printf("%d\n",length);   
}
