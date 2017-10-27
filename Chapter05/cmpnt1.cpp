// Aashish N. Ayyar : CMPNT1.cpp
#include <iostream>
#include <objbase.h>

using namespace std;

#include "iface.h"

void trace (const char *pMsg){ cout << pMsg << endl; }

class CA : public IX
{
	virtual HRESULT __stdcall QueryInterface(const IID &refIID, void ** ppv);
	virtual ULONG 	__stdcall AddRef();
	virtual ULONG 	__stdcall Release();

	virtual void __stdcall Fx() {cout << "Fx" << endl; }

	public:
		CA() : m_cRef(0){}
	   ~CA() { trace ("Destroy Self."); }

	private:
		long m_cRef;   
};

HRESULT __stdcall CA::QueryInterface(const IID &refIID, void ** ppv)
{
	if (refIID == IID_IUnknown)
	{
		trace ("Return pointer to IUnknown");
		*ppv = static_cast<IX*>(this);
	}
	else if (refIID == IID_IX)
	{
		trace ("Return pointer to IX");
		*ppv = static_cast<IX*>(this);
	}
	else
	{
		trace ("Requested Interface not supported.");
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppv) -> AddRef();
	return S_OK;
}

ULONG __stdcall CA::AddRef()
{
	return (InterlockedIncrement(&m_cRef));

}

ULONG __stdcall CA::Release()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

extern "C" IUnknown * CreateInstance()
{
	IUnknown *pI = static_cast<IX*>(new CA);
	pI -> AddRef();
	return pI;
}
