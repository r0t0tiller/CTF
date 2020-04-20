#include <stdio.h>

void PauseIt()
{
	printf("%s", "Pausing...\n");
}

int main(int argc, char const *argv[])
{
	printf("%s","This is a test\n");

	char* ptr1 = (char*)malloc(1024); // two equal sized chunks
	char* ptr2 = (char*)malloc(1024);

	strcpy(ptr1, "AAAAAAAA"); // add data to chunks
	strcpy(ptr2, "BBBBBBBB");

	PauseIt();

	printf("String = %s,  Address = %u\n", ptr1, ptr1); // printing chunk data
	printf("String = %s,  Address = %u\n", ptr2, ptr2);

	free(ptr1); // free chunks
	PauseIt();
	free(ptr2); 
	PauseIt();

	// ptr1 will leak data as this freed chunk will be populated with a pointer to main_arena
	// ptr1 is not backed by the top chunk and will not get consolidated

	printf("String = %s,  Address = %u\n", ptr1, ptr1); // printing chunk data 
	printf("String = %s,  Address = %u\n", ptr2, ptr2);

	PauseIt();

	return 0;
}