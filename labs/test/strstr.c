#include <stdio.h>
#include <stdlib.h>
char * strstr (const char *s1, const char *s2) {
	if (s1 == NULL || s2 == NULL) {
		return NULL;
	}
	
	int i, j;
	for (i = 0; *(s1 + i) != '\0'; i++) {
		
		for (j = 0; *(s2 + j) != '\0'; j++) {
			if (*(s1 + i +j) != *(s2 + j)) {
				break;
			}
		}		
		
		if (*(s2 + j) == '\0') {
			return (char*)(s1 + i);
		}

	}
	
	return NULL;
}

void check(const char *s) {
	if (*s) {
		printf("%s\n", s);
	}else {
		printf("%s\n", "NULL");
	}	
}

char* concat(char *s1, char *s2) {
	int i = 0;
	int j = 0;
    while(*(s1 + i) != '\0') {
		i++;
	}
			
    while(*(s2 + j) != '\0') {
		j++;
	}
	
	int length = i + j + 1;
	char* result = malloc(length);
	if (result == NULL) {
		printf("\nMemory is FULL\n");
		exit(1);
	}
	int w;
	for (w = 0; w < i; w++) {
		*(result + w) = *(s1 + w);
	}
	for (w = i;w < i + j; w++) {
		*(result + w) = *(s2 + w - i);
	}
	
	*(result + length - 1) = '\0';
	return result;

}






main () {
	char* s1 = "hello,worldGO BEARS!";
	char* s2 = "GO BEARS!";
	char* s3 = "world";
    char* test1 = strstr(s1, s2);
    char* test2 = strstr(s1, s3);
    char* test3 = strstr(s1, "llo");

	check(test1);
	check(test2);
	check(test3);
	

	char*s = concat("hello", "world");
	printf("%s\n", s);
	free(s);
}
