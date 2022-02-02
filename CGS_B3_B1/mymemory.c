/* mymemory.c
 * 
 * provides interface to memory management
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "mymemory.h"

#define BUFFERLEN 20


// our memory
Byte        mymemory [MAXMEM] ;
Segment_t * segmenttable = NULL;


void initialize ()
{
    printf ( "initialize> start\n");
    // Byte mymemory[MAXMEM];
    for(int i=0; i<MAXMEM;i++) mymemory[i]='\0';

    // create segment table
    // contains one segment description that declares the whole memory
    // as one free segment
    //
    // create a single segment descriptor
    Segment_t* segment = (Segment_t*)malloc(sizeof(Segment_t));
    segment = setSegment(segment,FALSE,mymemory,MAXMEM,NULL);

    // segment->allocated = FALSE;
    // segment->start = mymemory;
    // segment->size = MAXMEM;
    // segment->next = NULL;
	
    segmenttable = segment;

    printf ( "initialize> end\n");
}

Segment_t* setSegment(Segment_t* segment, Byte allocated, void* start, size_t size, Segment_t* next) {
    // Aux setter for Segment_t*
    segment->allocated = (allocated? TRUE: FALSE);segment->start = start;segment->size = size;segment->next = next;
    return segment;
}

void * mymalloc ( size_t size )
{
    printf ( "mymalloc> start\n");

    // Receive the segment
    Segment_t* freeSegment = findFree(segmenttable,size);

    // If segment doesn't exist, return NULL.
    if(freeSegment==NULL) return NULL;

    // If segment size is the same, return the free segment but set it to allocated.
    if(freeSegment->size == size) {freeSegment->allocated=TRUE; return freeSegment->start;}

    // Declaring new variables because it seems like it's using the same pointers otherwise  
    
    freeSegment->size -= size;

    Byte* newStartForNewSegment = (Byte*)((size_t)freeSegment->start + freeSegment->size);
    

    Segment_t* newSegment = (Segment_t*)malloc(sizeof(Segment_t));
    newSegment = setSegment(newSegment,TRUE,newStartForNewSegment,size,NULL);

    insertAfter(freeSegment, newSegment);
    
    printf("my malloc> end\n");
    return newSegment->start;
// 
}

void myfree ( void * ptr )
{
    printf ( "myfree> start\n");
    int i=0;
    Segment_t* segment = findSegment(segmenttable,ptr);
    if(segment!=NULL){
        segment->allocated = FALSE;
        Byte* tmp;
        while(i < segment->size){
            tmp = (Byte*)segment->start + (i++);
            *tmp='\0';
        }
    }
    printf("myfree> end\n");
    return;
}

/*
    - Pass a list of pointers (TODO: Implement a variable number of arguments)
    - Pointers passed will be moved and rearranged in memory while at the same time the contents of any free segments
      will be merged together in one single segment at the end of the moving.
    - Returns nothing. 
*/
void mydefrag(int count, ...)
{
    printf(">mydefrag start\n");
    // Combine all the free segments
    int memOffset = 0;
    // deleteFreeSegments();

    Segment_t* segmentAlias = segmenttable;
    segmentAlias = segmentAlias->next;

    // Declarations for the loop
    va_list varg;
    void* ptr;
    va_start(varg,count);

    for(int i=0;i<count;i++){
        // memOffset=0;
        ptr = va_arg(varg,void*);

        if(ptr==segmentAlias->start) {
            segmentAlias->start = moveSegment(segmenttable, ptr,memOffset);
            memOffset=memOffset+segmentAlias->size;
            if(segmentAlias->next) segmentAlias = segmentAlias->next;
            continue;
        }
        else{
            while(ptr!=segmentAlias->start) {
                if(segmentAlias->allocated ==FALSE) memOffset+=delSegment(segmenttable,segmentAlias);
                if(segmentAlias->next) segmentAlias = segmentAlias->next;
                else return;
            }
        }
    }

    // Pointers will be dealt with inside a loop

    // Pointer X -

    // delSegment(segmenttable,segmenttable);
    va_end(varg);
    printf("mydefrag> end\n");
}

// Receives a linked list of segments and a pointer to look for.
// Returns the size of the deleted node.
int delSegment ( Segment_t* list, Segment_t * ptr ){
    if(ptr==NULL) return 0; //Handling some extremely edge case

    Segment_t* segment = list;
    Segment_t* prev = malloc(sizeof(Segment_t*));
    int ssize;
    
    while(TRUE){
        if(segment==ptr){
            ssize = segment->size;

            // If there is another segment after
            if(segment->next) {
            segment = setSegment(segment,segment->next->allocated,segment->next->start,segment->next->size,segment->next->next);
            }

            // If there is no other segment after
            else{
                prev->next = NULL;
            } 
            // "Delete" the segment
            // ptr = setSegment(ptr,0,NULL,0,NULL);
            return ssize;
        }
        if(segment->next) {
            prev = segment;
            segment=segment->next;
        }
        else return 0;
    }
}

void deleteFreeSegments(){

    // If there have been no segmentations.
    if(segmenttable->size==MAXMEM) return;
    

    Segment_t* segment = segmenttable;
    Segment_t* prev = malloc(sizeof(Segment_t*));

    // Advance once so as not to consume the "base" free segment
    segment = segment->next;

    while(TRUE){
        if(segment->allocated==FALSE){
            if(segment->next){
                segmenttable->size += segment->size;
                segment = setSegment(segment,segment->next->allocated,segment->next->start,segment->next->size,segment->next->next);
            }
            else{
                prev->next = NULL;
            }
        // There could be certain issues with this approach
        // but proper use shoudn't lead to them.
        }
        if(segment->next) {
            prev = segment;
            segment = segment->next;
        }
        else
            break;
    }
    

    if(segment->size!=0&&segment->allocated==FALSE){
        // prev = NULL;
        // prev->next->next = NULL;
        segmenttable->size += segment->size;

        // prev = setSegment(prev->next,0,NULL,0,NULL);

        segment = setSegment(segment,0,NULL,0,NULL);
    }
}


/*
- Moves a segment within your memory array (the content in the memory has to be moved as well) 
// - Returns the new "start" pointer of the segment, or NULL in case of a failure situation
- Returns 
- Tbh this expains next to nothing
*/
void* moveSegment(Segment_t* list, void* ptr_start, int memOffset){

    // Find the corresponding segment to the location in memory
    Segment_t* ptr = findSegment(list, ptr_start);

    // Find a new start location
    Byte* newStart = (Byte*)list->start+list->size+memOffset;

    // Copy the contents of the previous location (ptr->start)

    int i=0;Byte* src=ptr_start; Byte* dest=newStart;
    while(i< ptr->size){
        *dest = *src;
        *src = '\0';
        src++;dest++;
        i++;
    }

    // Remove the contents of the previous location

    // Return the new location (and set ptr->start to new location)
    ptr->start = newStart;

    return ptr->start;
}


// helper functions for management segmentation table
Segment_t * findFree ( Segment_t * list, size_t size )
{
    Segment_t* segmentdescriptor = list;
	while(TRUE){
        // Go through the list until a sufficient chunk is found;
        if(segmentdescriptor->size>=size && segmentdescriptor->allocated==FALSE){
            // printsegmentdescriptor(segmentdescriptor);
            return segmentdescriptor;
        }
		
		if(!segmentdescriptor->next) break;
		segmentdescriptor = segmentdescriptor->next;
	}
    return NULL;
}

void insertAfter ( Segment_t * oldSegment, Segment_t * newSegment )
{
    printf(">insertAfter start\n");
    // printsegmentdescriptor(oldSegment);
    // printsegmentdescriptor(newSegment);
    if(oldSegment==NULL && newSegment != NULL) {oldSegment=newSegment;return;}
    if(newSegment==NULL && oldSegment != NULL) {newSegment=oldSegment;return;}
    
    if(oldSegment->next == NULL){//End of the list
        oldSegment->next = newSegment;
    }
    else{
        Segment_t* oldNextSegment = oldSegment->next;
        oldSegment->next = newSegment;
        // What do we do if newSegment has its own descendants?
        newSegment->next = oldNextSegment;
    }
    // printsegmenttable();
    return;
}

Segment_t * findSegment ( Segment_t * list, void * ptr )
{
    Segment_t* segmentdescriptor=list;
    while(TRUE){
        // Go through the list until a sufficient chunk is found;
        if(segmentdescriptor->start == ptr){
            // printsegmentdescriptor(segmentdescriptor);
            return segmentdescriptor;
        }
		
		if(!segmentdescriptor->next) break;
		segmentdescriptor = segmentdescriptor->next;
    }
}

int isPrintable ( int c )
{
    if ( c >= 0x20 && c <= 0x7e ) return c ;

    return 0 ;
}

void printmemory(){
    int i;
    unsigned char buff[BUFFERLEN+1];
    const Byte * pc = (const Byte*) mymemory;

    // Process every byte in the data.

    for (i = 0; i < sizeof(mymemory); i++) {
        // Multiple of BUFFERLEN means new line (with line offset).

        if ((i % BUFFERLEN) == 0) {
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
            buff[i % BUFFERLEN] = pc[i];
        else
            buff[i % BUFFERLEN] = '.';
        buff[(i % BUFFERLEN) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.

    while ((i % BUFFERLEN) != 0) {
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

	Segment_t* segmentdescriptor = segmenttable;

	while(TRUE){
        // if(i>5) break;
		printf("Segment %d\n", i++);
		printsegmentdescriptor(segmentdescriptor);
		
        // if(!segmentdescriptor->next) break;
        // segmentdescriptor++;
		if(!segmentdescriptor->next||segmentdescriptor->next->size==0) break;
		segmentdescriptor = segmentdescriptor->next;
	}
}