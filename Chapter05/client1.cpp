// Aashish N. Ayyar : CLIENT1.cpp
#include <iostream>
#include <objbase.h>

using namespace std;

#include "iface.h"
#include "create.h"

void trace (const char *pMsg){ cout << pMsg << endl; }

int main ()
{
	HRESULT hr;
	char name[40];
	cout << "Enter the filename of the component to use [Cmpnt?.dll] : ";
	cin  >> name;
	cout << endl;

	trace ("Get an IUnknown pointer.");
	IUnknown *pIUnknown = CallCreateInstance(name);
	if (pIUnknown == NULL)
	{
		trace ("CallCreateInstance Failed");
		return 1;
	}

	trace ("Get interface IX");
	IX *pIX = NULL;
	hr = pIUnknown -> QueryInterface (IID_IX, (void **)&pIX);
	if (SUCCEEDED(hr))
	{
		trace ("Succeeded in getting IX");
		pIX -> Fx();
		pIX -> Release();
	}
	else 
	{
		trace ("Could not get interface IX");
	}

	trace ("Release IUnknown interface");
	pIUnknown -> Release();

	return 0;
}