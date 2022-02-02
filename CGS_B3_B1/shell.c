#include <stdio.h>
#include <string.h>
#include "mymemory.h"


int main() {
    printf ( "shell> start\n");

    initialize() ;

    char * ptr1 = (char *) mymalloc ( 101 ) ; 
    strcpy (ptr1, "this test1"); 
    printf( "shell> content of allocated memory: %s\n", ptr1 ) ; 

    char * ptr2 = (char *) mymalloc ( 102 ) ; 
    strcpy (ptr2, "this test2"); 
    printf( "shell> content of allocated memory: %s\n", ptr2 ) ;

    printmemory();

    printsegmenttable();

    myfree(ptr1);

    myfree(ptr2);

    printmemory();
    printsegmenttable();

    printf ( "shell> end\n");
    return 0;
}