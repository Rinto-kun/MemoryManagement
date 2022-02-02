### Introduction
Name: Matey Krastev | Student ID: 51987311

### How-to use
In the terminal, run the following command: "make all".
Then, in the terminal, run the following command to see the results: "./shell".

### Explanation for printmemory and printsegmenttable
The function printmemory prints to the terminal the contents of the memory array used to imitate actual program memory.
The function printsegmenttable prints to the terminal the contents of a segmentation table implemented as a linked list where each segment is a node in the list. It prints the allocation status of the segment, the byte of memory it corresponds to, as well as the size of the chunk.

### Explanation for mymalloc
Mymalloc is a function which allocates a segment from the program memory and returns it to the user.

### Explanation for myfree
Myfree deallocates the memory that is occupied when mymalloc has been invoked. 

### Explanation for mydefrag
Due to time constraints, the function is incomplete, although the helper functions are perfectly functional by themselves. Specified here are the helper functions implemented as well as the basic algorithm for mydefrag. 

int delSegment ( Segment_t *, Segment_t * );
- The function takes a list and a pointer of type Segment_t as arguments and removes the latter from the list.
- Returns the size of the removed segment, or 0 if nothing is deleted.

void * moveSegment ( Segment_t *, void *, int );
- The function takes a pointer to a list of segments as well as a location in the memory (void pointer) and an arbitrary offset, and moves the contents of the latter to the first free segment in memory.
- Returns the new location of the segment in the memory array.

void mydefrag(int count, ...)
- The function takes a variable number of arguments (which are expected to be pointers to a place in the memory), and, while/after deleting all deallocated segments, moves the received pointers up in the table.
- Returns nothing.

### Requirements and considerations
The software was tested in the Codio environment, as well as locally on a Windows machine using MINGW64.
The program was compiled using gcc. Please make sure you have at least one of these.