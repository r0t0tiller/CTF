#include <stdio.h>

void PauseIt()
{
	printf("%s", "Pausing...\n");
}

int main(int argc, char const *argv[])
{
	printf("%s","This is a test\n");

	char* ptr1 = (char*)malloc(20); // two equal sized chunks
	char* ptr2 = (char*)malloc(100);
	char* ptr3 = (char*)malloc(20); // two equal sized chunks

	strcpy(ptr1, "AAAAAAAA"); // add data to chunks
	strcpy(ptr2, "BORDER");
	strcpy(ptr3, "BBBBBBBB");

	PauseIt();

	printf("%s\n", ":::Before:::");

	printf("String = %s,  Address = %u\n", ptr1, ptr1); // printing chunk data
	printf("String = %s,  Address = %u\n", ptr2, ptr2);
	printf("String = %s,  Address = %u\n", ptr3, ptr3);

	free(ptr1); // free chunks
	free(ptr2);
	free(ptr3);
	free(ptr1);

	printf("%s\n", ":::After:::");

	// ptr1 will leak data as this freed chunk will be populated with a pointer to main_arena
	// ptr1 is not backed by the top chunk and will not get consolidated

	printf("String = %s,  Address = %u\n", ptr1, ptr1); // printing chunk data 
	printf("String = %s,  Address = %u\n", ptr2, ptr2);
	printf("String = %s,  Address = %u\n", ptr3, ptr3);

	PauseIt();

	return 0;
}