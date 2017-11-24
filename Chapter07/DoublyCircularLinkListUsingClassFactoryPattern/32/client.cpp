// Aashish N. Ayyar : CLIENT.cpp
#include <iostream>
#include <objbase.h>

#include "interface_header.h"

void trace (const char *pMsg) { cout << "Client :\t" << pMsg << endl; }

int main (void)
{
	CoInitialize(NULL);

	IList *pIList = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_CList, 
								  NULL, 
								  CLSCTX_INPROC_SERVER,
								  IID_IList,
								  (void **)&pIList);
	if (FAILED(hr))
	{
		cout << "Failed to create instance of IList. hr = " << hex << hr << endl;
		CoUninitialize();
		return 0;
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

	pIList -> Release();
	CoUninitialize();

	return 0;
}