#include <stdio.h>

struct Array {
    int a;
    int b;
    int c;	
}

main() {
    struct Array array = {20,30,40};
    printf("%d\n", array.a);
    struct Array *ptr = &array;
    ptr->a = 25;
    printf("%d\n", ptr->a);
}
