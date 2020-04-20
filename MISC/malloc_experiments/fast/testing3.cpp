#include <iostream>
#include <stdio.h>

void PauseIt()
{
	printf("%s\n", "Pausing...");
}

class Object
{
public:
	virtual void CreateObject(char *Pdata)
	{
		char* data = (char*)Pdata;
	}
};


int main(int argc, char const *argv[])
{
	Object *obj = new Object;
	obj->CreateObject((char*)"AAAAAAAAAAAAAAAA");

	delete obj;

	PauseIt();

	return 0;
}