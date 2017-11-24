// Aashish N. Ayyar : CLIENT.cpp
#include <iostream>
#include <objbase.h>

#include "interface_header.h"

void trace (const char *pMsg) { cout << "Client :\t" << pMsg << endl; }

typedef IUnknown *(*CREATEFUNCPOINTER)();

IUnknown *CallCreateInstance(char *name);

int main (void)
{
	HRESULT hr;

	trace ("Get an IUnknown pointer.");
	IUnknown *pIUnknown = CallCreateInstance("component.dll");
	if (pIUnknown == NULL)
	{
		trace ("CallCreateInstance Failed");
		return 1;
	}

	trace ("Get interface IList");

	IList * pIList = NULL;
	hr = pIUnknown -> QueryInterface(IID_IList, (void **)&pIList);
	if (FAILED(hr))
	{
		trace("Failed to get IList");
		return 1;
	}

	pIList -> insert_beginning (10);
	pIList -> display();
	pIList -> insert_end(20);
	pIList -> display();
	pIList -> insert_before(20, 15);
	pIList -> display();
	pIList -> insert_after(20, 25);
	pIList -> display();
	cout << "find(25)  : " << pIList -> find(25) << endl;
	cout << "is_empty(): " << pIList -> is_empty() << endl;
	pIList -> delete_beignning ();
	pIList -> display();
	pIList -> delete_end();
	pIList -> display();
	pIList -> delete_data(20);
	pIList -> display();

	trace ("Releasing pIList ...");
	pIList -> Release();

	trace ("Releasing pIUnknown ...");
	pIUnknown -> Release();

	return 0;
}

IUnknown *CallCreateInstance(char *name)
{
	HINSTANCE hComponent = ::LoadLibrary(name);
	if (hComponent == NULL)
	{
		cout << "CallCreateInstance:\tError:Cannot load component." << endl;
		return NULL;
	}

	CREATEFUNCPOINTER CreateInstance = (CREATEFUNCPOINTER)::GetProcAddress(hComponent, "CreateInstance");
	if (CreateInstance == NULL)
	{
		cout << "CallCreateInstance:\tError: "
			 << "Cannot find CreateInstance function."
			 << endl;
		return NULL;	 
	}
	return CreateInstance();
}