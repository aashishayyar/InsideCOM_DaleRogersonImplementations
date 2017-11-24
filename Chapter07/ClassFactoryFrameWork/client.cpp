// Aashish N. Ayyar : CLIENT.cpp
#include <iostream>
#include <objbase.h>

#include "iface.h"

using namespace std;

void trace (const char *pMsg)
{
	cout << "Client : " << pMsg << endl;
}

int main (void)
{
	trace("****************** START **************************");

	CoInitialize(NULL);

	trace ("Calling CoCreateInstance to create compnent and get interface IX.");

	IX *pIX = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_Component1, 
								  NULL, 
								  CLSCTX_INPROC_SERVER,
								  IID_IX,
								  (void **)&pIX);
	if (FAILED(hr))
	{
		cout << "Failed to create instance of IX. hr = " << hex << hr << endl;
		CoUninitialize();
		return 0;
	}

	trace ("Succeeded in calling CoCreateInstance.");
	
	pIX -> Fx();

	trace ("Ask for IY using IX.");

	IY *pIY = NULL;
	hr = pIX -> QueryInterface(IID_IY, (void **)&pIY);
	if (FAILED (hr))
	{
		cout << "Failed to get instance of IY. hr = " << hex << hr << endl;
	}
	else 
	{

		trace ("Succeeded in getting instance of IY from IX.");
		pIY -> Fy();
		pIY -> Release();
		trace ("IY instance released.");
	}

	trace ("Ask for instance of IZ from IX ");

	IZ *pIZ =NULL;
	hr = pIX -> QueryInterface(IID_IZ, (void **)&pIZ);
	if (FAILED(hr))
	{
		cout << "Failed to get instance of IZ. hr = " << hex << hr << endl;		
	}
	else 
	{
		trace ("Succeeded in getting an instance of IZ.");
		pIZ -> Fz();
		pIZ -> Release();
		trace ("Released IZ successfully.");
	}

	trace("All Operations performed successfully. Calling CoUninitialize()");
	CoUninitialize();

	trace("********************** STOP *****************************");
	return 0;
}
