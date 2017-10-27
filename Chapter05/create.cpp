// Aashish N. Ayyar : CREATE.cpp : Implements the CallCreateInstance
#include <iostream>
#include <unknwn.h>

#include "create.h"
using namespace std;

typedef IUnknown * (*CREATEFUNCPOINTER)();

IUnknown * CallCreateInstance(char * name)
{
	// Loading dynamic link library into process
	HINSTANCE hComponent = ::LoadLibrary(name);
	if (hComponent == NULL)
	{
		cout << "CallCreateInstance :\tError : Cannot load component." << endl;
		return NULL;
	}

	CREATEFUNCPOINTER CreateInstance = (CREATEFUNCPOINTER)::GetProcAddress(hComponent, "CreateInstance");
	if (CreateInstance == NULL)
	{
		cout << "CallCreateinstance:\tError: " 
			 << "Cannot find CreateInstance function."
			 << endl;
		return NULL;	 
	}

	return CreateInstance();
}	
