#include <stdio.h>
#include <string.h>
#include "mymemory.h"


int main() {
    printf ( "shell> start\n");

    initialize() ;

    char * ptr1 = (char *) mymalloc ( 20 ) ; 
    strcpy (ptr1, "this test1111111111"); 
    printf( "shell> content of allocated memory: %s\n", ptr1 ) ; 

    char * ptr2 = (char *) mymalloc ( 20 ) ; 
    strcpy (ptr2, "this test2222222222"); 
    printf( "shell> content of allocated memory: %s\n", ptr2 ) ;

    char* ptr3 = (char*) mymalloc(20);
    strcpy(ptr3,"this test3333333333");
    printf( "shell> content of allocated memory: %s\n", ptr3 ) ;

    printmemory();

    printsegmenttable();

    // myfree(ptr1);

    myfree(ptr2);

    printmemory();

    printsegmenttable();

    // mydefrag(1,ptr1);
    mydefrag(2,ptr1,ptr3);

    printmemory();
    printsegmenttable();

    printf ( "shell> end\n");
    return 0;
}