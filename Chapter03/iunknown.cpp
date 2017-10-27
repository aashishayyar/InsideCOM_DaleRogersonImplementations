// Aashish N. Ayyar : IUNKNOWN.cpp
#include <iostream>
#include <objbase.h>

using namespace std;

void trace (const char *pMsg)
{
	cout << pMsg << endl;
}

interface IX : IUnknown
{
	virtual void __stdcall Fx() = 0;
};

interface IY : IUnknown
{
	virtual void __stdcall Fy() = 0;
};

interface IZ : IUnknown 
{
	virtual void __stdcall Fz() = 0;
};

extern const IID IID_IX;
extern const IID IID_IY;
extern const IID IID_IZ;

/************ COMPONENT *****************/

class CA : public IX,
		   public IY
{
	public : 
		virtual HRESULT __stdcall QueryInterface(const IID &refIID, void **ppv);
		virtual ULONG 	__stdcall AddRef()  {return 0; }
		virtual ULONG 	__stdcall Release() {return 0; }

		// Implementing IX interface methods
		virtual void __stdcall Fx() { cout << "Fx" << endl; }

		// Implementing IY interface methods
		virtual void __stdcall Fy() { cout << "Fy" << endl; } 
};

HRESULT __stdcall CA::QueryInterface(const IID &refIID, void **ppv)
{
	if (refIID == IID_IUnknown)
	{
		trace ("Query Interface : Return pointer to IUnknown.");
		*ppv = static_cast<IX*>(this);
	}
	else if (refIID == IID_IX)
	{
		trace ("Query Interface : Return pointer to IX.");
		*ppv = static_cast<IX*>(this);
	}
	else if (refIID == IID_IY)
	{
		trace ("Query Interface : Return pointer to IY.");
		*ppv = static_cast<IY*>(this);	
	}
	else 
	{
		trace ("QueryInterface : Interface not supported.");
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	static_cast<IUnknown *>(*ppv) -> AddRef();
	return (S_OK);
}	

IUnknown * CreateInstance()
{
	IUnknown *pI = static_cast<IX*>(new CA);
	pI -> AddRef();
	return pI;
}

// {5E715628-36F0-4A73-9161-39B5F2EEE2C1}
static const IID IID_IX = { 0x5e715628, 0x36f0, 0x4a73, { 0x91, 0x61, 0x39, 0xb5, 0xf2, 0xee, 0xe2, 0xc1 } };

// {CCA92625-08AE-4400-A0F2-16F174BF2341}
static const IID IID_IY = { 0xcca92625, 0x8ae, 0x4400, { 0xa0, 0xf2, 0x16, 0xf1, 0x74, 0xbf, 0x23, 0x41 } };

// {3E373608-EAAC-49F1-8B9E-2D7A9844638B}
static const IID IID_IZ = { 0x3e373608, 0xeaac, 0x49f1, { 0x8b, 0x9e, 0x2d, 0x7a, 0x98, 0x44, 0x63, 0x8b } };


/***************** CLIENT **************/

int main()
{
	HRESULT hr;

	trace ("Client : Get an IUnknown pointer. ");
	IUnknown *pIUnknown = CreateInstance();

	trace ("Client : Get interface IX. ");

	IX *pIX = NULL;
	hr = pIUnknown -> QueryInterface(IID_IX, (void **)&pIX);
	if (SUCCEEDED(hr))
	{
		trace ("Client : Succedded in getting IX");
		pIX -> Fx();
	}

	trace ("Client : Get interface IY. ");
	IY *pIY = NULL;
	hr = pIUnknown -> QueryInterface(IID_IY, (void **)&pIY);
	if (SUCCEEDED(hr))
	{
		trace ("Client : Succedded in getting IY");
		pIY -> Fy();
	}

	trace ("Client : Get interface IZ.");
	IZ *pIZ = NULL;
	hr = pIUnknown -> QueryInterface(IID_IZ, (void **)&pIZ);
	if (SUCCEEDED(hr))
	{
		trace ("Client : Succedded in getting IZ");
		pIZ -> Fz();
	}
	else 
	{
		trace ("Client : Failed to get interface IZ");
	}

	trace ("Client : Get interface IY from IZ");
	IY * pIYFromIX = NULL;
	hr = pIX -> QueryInterface(IID_IY, (void **)&pIYFromIX);
	if (SUCCEEDED(hr))
	{
		trace ("Client : Succedde in getting IY from IX");
		pIYFromIX -> Fy();
	}

	trace ("Client : Get interface IUnknown from IY.");
	IUnknown * pIUnknownFromIY = NULL;
	hr = pIY -> QueryInterface(IID_IUnknown, (void **)&pIUnknownFromIY);
	if (SUCCEEDED(hr))
	{
		cout << "Are the IUnknown pointers equal ?? ";
		if (pIUnknownFromIY == pIUnknown)
			cout << "Yup, pIUnknownFromIY == pIUnknown." << endl; 
		else 
			cout << "No, pIUnknownFromIY != pIUnknown." << endl;
	}

	delete pIUnknown;
	return 0;
}
