
// Abusing fast bin list to leak FD of previously freed chunks

#include <stdio.h>

void PauseIt()
{
	printf("%s", "Pausing...\n");
}

int main(int argc, char const *argv[])
{
	printf("%s","This is a test\n");

	char* ptr1 = (char*)malloc(10); // allocate chunk 1

	strcpy(ptr1, "AAAAAAAA"); // add data to chunk 1;

	free(ptr1); // free chunk 1

	PauseIt();

	char* ptr2 = (char*)malloc(10); // allocate chunk 2

	strcpy(ptr2, "BBBBBBBB"); // add data to chunk 2

	char* ptr3 = (char*)malloc(10); // allocate chunk 3

	strcpy(ptr3, "CCCCCCCC"); // add data to chunk 3

	free(ptr3); // free chunk 3

	ptr3 = NULL; // set malloc ptr3 to null

	PauseIt();

	free(ptr2); // free chunk 2

	ptr2 = NULL; // set malloc ptr2 to null

	PauseIt();

	printf("String = %s,  Address = %u\n", ptr1, ptr1); // printing chunk 1 data 
	printf("String = %s,  Address = %u\n", ptr2, ptr2); // printing chunk 2 data 
	printf("String = %s,  Address = %u\n", ptr3, ptr3); // printing chunk 3 data 

	PauseIt();

	return 0;
}