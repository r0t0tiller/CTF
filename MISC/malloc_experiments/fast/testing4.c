
// Abusing fast bin list to leak FD of previously freed chunks

#include <stdio.h>

void PauseIt()
{
	printf("%s", "Pausing...\n");
}

void Test1()
{
	printf("%s","This is a test\n");

	char* ptr1 = (char*)malloc(32); // allocate chunk 1
	char* ptr2 = (char*)malloc(32); // allocate chunk 1
	char* ptr3 = (char*)malloc(32); // allocate chunk 1

	strcpy(ptr1, "AAAAAAAAA"); // add data to chunk 1;
	strcpy(ptr2, "BBBBBBBB"); // add data to chunk 1;
	strcpy(ptr3, "CCCCCCCC"); // add data to chunk 1;

	PauseIt();

	free(ptr1);
	free(ptr2);
	free(ptr3);

	printf("String = %s,  Address = %u\n", ptr1, ptr1); // printing chunk 1 data 
	printf("String = %s,  Address = %u\n", ptr2, ptr2); // printing chunk 2 data 
	printf("String = %s,  Address = %u\n", ptr3, ptr3); // printing chunk 3 data 

	PauseIt();
}

void Test2()
{
	printf("%s","This is a test\n");

	char* ptr0 = (char*)malloc(1024); // allocate chunk 1
	char* ptr2 = (char*)malloc(1024); // allocate chunk 1
	char* ptr3 = (char*)malloc(1024); // allocate chunk 1
	char* ptr4 = (char*)malloc(1024); // allocate chunk 1

	strcpy(ptr0, "AAAAAAAA"); // add data to chunk 1;
	strcpy(ptr2, "BBBBBBBB"); // add data to chunk 1;
	strcpy(ptr3, "CCCCCCCC"); // add data to chunk 1;
	strcpy(ptr4, "DDDDDDDD"); // add data to chunk 1;

	// Free out of order to avoid chunk consolidation

	PauseIt();
	free(ptr3);
	PauseIt();
	free(ptr0);
	PauseIt();
	free(ptr2);
	PauseIt();
	free(ptr4);

	/*

	+gef>  heap chunks
	Chunk(addr=0x804b008, size=0x408, flags=PREV_INUSE)
	    [0x0804b008     50 61 75 73 69 6e 67 2e 2e 2e 0a 65 73 74 0a 00    Pausing....est..]
	Chunk(addr=0x804b410, size=0x408, flags=PREV_INUSE)
	    [0x0804b410     18 bc 04 08 b0 37 fc f7 00 00 00 00 00 00 00 00    .....7..........]
	Chunk(addr=0x804b818, size=0x408, flags=)
	    [0x0804b818     42 42 42 42 42 42 42 42 00 00 00 00 00 00 00 00    BBBBBBBB........]
	Chunk(addr=0x804bc20, size=0x408, flags=PREV_INUSE)
	    [0x0804bc20     b0 37 fc f7 08 b4 04 08 00 00 00 00 00 00 00 00    .7..............]
	Chunk(addr=0x804c028, size=0x408, flags=)
	    [0x0804c028     00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................]
	Chunk(addr=0x804c430, size=0x1fbd8, flags=PREV_INUSE)  ←  top chunk
	+gef➤  x/20wx 0x804b410
	0x804b410:      0x0804bc18      0xf7fc37b0      0x00000000      0x00000000
	0x804b420:      0x00000000      0x00000000      0x00000000      0x00000000
	0x804b430:      0x00000000      0x00000000      0x00000000      0x00000000
	0x804b440:      0x00000000      0x00000000      0x00000000      0x00000000
	0x804b450:      0x00000000      0x00000000      0x00000000      0x00000000
	+gef➤  x/20wx 0x804bc20
	0x804bc20:      0xf7fc37b0      0x0804b408      0x00000000      0x00000000
	0x804bc30:      0x00000000      0x00000000      0x00000000      0x00000000
	0x804bc40:      0x00000000      0x00000000      0x00000000      0x00000000
	0x804bc50:      0x00000000      0x00000000      0x00000000      0x00000000
	0x804bc60:      0x00000000      0x00000000      0x00000000      0x00000000
	+gef>

	chunk3 FD = main_arena
	chunk3 BK = chunk0

	chunk0 FD = chunk3
	chunk0 BK = main_arena

	doubly-linked chunks

	*/

	// PauseIt();
	// ptr2 = (char*)malloc(1024); // allocate chunk 1
	// PauseIt();
	// ptr3 = (char*)malloc(1024); // allocate chunk 1
	// PauseIt();
	// ptr4 = (char*)malloc(1024); // allocate chunk 1

	// strcpy(ptr2, "DDDDDDDD"); // add data to chunk 1;
	// strcpy(ptr3, "EEEEEEEE"); // add data to chunk 1;
	// strcpy(ptr3, "FFFFFFFF"); // add data to chunk 1;

	// free(ptr2);
	// free(ptr3);
	// free(ptr4);

	// PauseIt();

	printf("String = %s,  Address = %u\n", ptr2, ptr2); // printing chunk 2 data 
}

int main(int argc, char const *argv[])
{
	Test2();

	return 0;
}