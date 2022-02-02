/* mymemory.c
 * 
 * provides interface to memory management
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "mymemory.h"


// our memory
Byte        mymemory [MAXMEM] ;
Segment_t * segmenttable = NULL;

void initialize ()
{
    printf ( "initialize> start\n");

    // for(int i=0; i<MAXMEM;i++) mymemory[i]='\0';
    Byte mymemory[MAXMEM];

    // create segment table
    // contains one segment description that declares the whole memory
    // as one free segment
    //
    // create a single segment descriptor
    Segment_t* segment = malloc(sizeof(Segment_t));
    segment->allocated = FALSE;
    segment->start = 0;
    segment->size = MAXMEM;
    segment->next = NULL;
	
    // initialise the segment

    // Segment_t * segment_2 = malloc(sizeof(Segment_t));
    // segment_2->allocated=TRUE;
    // segment_2->start = 1;
    // segment_2->size= 3;
    // segment_2->next=NULL;
    // segment->next=segment_2;


	segmenttable = segment;

    printf ( "initialize> end\n");
}

void * mymalloc ( size_t size )
{
    printf ( "mymalloc> start\n");

    // implement the mymalloc functionality
}

void myfree ( void * ptr )
{
    printf ( "myfree> start\n");
}

void mydefrag ( void ** ptrlist)
{
    printf ( "mydefrag> start\n");
}


// helper functions for management segmentation table
Segment_t * findFree ( Segment_t * list, size_t size )
{
    printf ( "findFree> start\n");
}

void insertAfter ( Segment_t * oldSegment, Segment_t * newSegment )
{
}

Segment_t * findSegment ( Segment_t * list, void * ptr )
{
}

int isPrintable ( int c )
{
    if ( c >= 0x20 && c <= 0x7e ) return c ;

    return 0 ;
}

void printmemory(){
    int bufferlen = 20;
    int i;
    unsigned char buff[bufferlen+1];
    const Byte * pc = (const Byte*) mymemory;

    // Process every byte in the data.

    for (i = 0; i < sizeof(mymemory); i++) {
        // Multiple of bufferlen means new line (with line offset).

        if ((i % bufferlen) == 0) {
            // Don't print ASCII buffer for the "zeroth" line.

            if (i != 0)
                printf (" |%s\n", buff);

            // Output the offset.

            printf ("[%4d]", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And buffer a printable ASCII character for later.

        if (isPrintable(pc[i]))
            buff[i % bufferlen] = pc[i];
        else
            buff[i % bufferlen] = '.';
        buff[(i % bufferlen) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.

    while ((i % bufferlen) != 0) {
        printf ("   ");
        i++;
    }
    // And print the final ASCII buffer.
    printf (" |%s\n", buff);
}

void printsegmentdescriptor ( Segment_t * descriptor )
{
    printf ( "\tallocated = %s\n" , (descriptor->allocated == FALSE ? "FALSE" : "TRUE" ) ) ;
    printf ( "\tstart     = %p\n" , descriptor->start ) ;
    printf ( "\tsize      = %zu\n", descriptor->size  ) ;
}

void printsegmenttable()
{
    int i = 0; 

	Segment_t segmentdescriptor = *segmenttable;

	while(TRUE){
		printf("Segment %d\n", i++);
		printsegmentdescriptor(&segmentdescriptor);
		
		if(!segmentdescriptor.next) break;
		segmentdescriptor = *segmentdescriptor.next;
	}
}

// int main(){
//     initialize();
//     printsegmenttable();
// }