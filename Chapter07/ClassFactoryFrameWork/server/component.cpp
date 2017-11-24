#include <iostream>
#include <objbase.h>

#include "iface.h"
#include "registry.h"

using namespace std;

static HMODULE g_hModule = NULL;

static long g_cComponents  = 0;
static long g_cServerLocks = 0;

const char g_szFriendlyName[] = "Aashish N. Ayyar : InsidCOM : Chapter07";
const char g_szVerIndProgId[] = "AashishAyyar.InsideCOM07";

const char g_szProgId[] = "AashishAyyar.InsideCOM07.1";

void trace(const char *pMsg)
{
	cout << pMsg << endl;
}

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  dwReason,
					  void  *lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = hModule;
	}
	return TRUE;
}

class CA : public IX, IY
{
	public:

		virtual	HRESULT __stdcall QueryInterface(REFIID refIID, void ** ppv);
		virtual ULONG 	__stdcall AddRef();
		virtual ULONG 	__stdcall Release();

		 CA();
		~CA();

		virtual void __stdcall Fx() { cout << "Fx" << endl; }
		virtual	void __stdcall Fy() { cout << "Fy" << endl; }

	private:

		long m_cRef;
};

CA::CA() : m_cRef(1)
{
	InterlockedIncrement(&g_cComponents);
}

CA::~CA()
{
	InterlockedDecrement(&g_cComponents);
	trace ("Destroying self");
}

HRESULT __stdcall CA::QueryInterface(REFIID refIID, void ** ppv)
{
	if (refIID == IID_IUnknown)
	{
		trace ("IUnknown pointer is returned here.");
		*ppv = static_cast<IX *>(this);
	}
	else if (refIID == IID_IX)
	{
		trace ("IX pointer is returned now.");
		*ppv = static_cast<IX *>(this);
	}
	else if (refIID == IID_IY)
	{
		trace ("IY pointer is returned now.");
		*ppv = static_cast<IY *>(this);
	}
	else 
	{
		trace ("Requested interface not supported.");
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown *>(*ppv) -> AddRef();
	return S_OK;
}

ULONG __stdcall CA::AddRef()
{
	return 	InterlockedIncrement(&m_cRef);
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

class ClassFactory : public IClassFactory
{
public:
	 ClassFactory() : m_cRef(1){}
	~ClassFactory() { trace ("ClassFactory : Destroy Self."); }

	virtual HRESULT __stdcall QueryInterface(REFIID refIID, void **ppv);
	virtual ULONG 	__stdcall AddRef();
	virtual ULONG 	__stdcall Release();

	virtual HRESULT __stdcall CreateInstance(IUnknown * pUnkOuter,
											 REFIID refIID,
											 void ** ppv);

	virtual HRESULT __stdcall LockServer(BOOL bLock);

private:
	long m_cRef;	
};

HRESULT __stdcall ClassFactory::QueryInterface(REFIID refIID, void **ppv)
{
	if (refIID == IID_IUnknown)
	{
		trace ("Returning IUnknown pointer.");
		*ppv = static_cast<IClassFactory *>(this);
	}
	else if (refIID == IID_IClassFactory)
	{
		trace ("Returning IClassFactory pointer.");
		*ppv = static_cast<IClassFactory *>(this);		
	}
	else 
	{
		trace ("ClassFactory : Requested interface not found");
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown *>(*ppv) -> AddRef();

	return S_OK;
}

ULONG __stdcall ClassFactory::AddRef()
{
	return InterlockedIncrement (&m_cRef);
}

ULONG __stdcall ClassFactory::Release()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;	
	}
	return m_cRef;
}

HRESULT __stdcall ClassFactory::CreateInstance (IUnknown * pUnkOuter, REFIID refIID, void ** ppv)
{
	trace ("ClassFactory : CreateInstance.");
	if (pUnkOuter != NULL)
	{
		trace ("return CLASS_E_NOAGGREGATION");
	}

	CA *pA = new CA;
	if (pA == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pA -> QueryInterface(refIID, ppv);
	pA -> Release();

	return hr;
}

HRESULT __stdcall ClassFactory::LockServer(BOOL bLock)
{
	if (bLock)
		InterlockedIncrement(&g_cServerLocks);
	else 
		InterlockedDecrement(&g_cServerLocks);
	return S_OK;
}

STDAPI DllCanUnloadNow()
{
	if ((g_cServerLocks == 0) && (g_cComponents == 0))
	{
		return S_OK;
	}
	else 
		return S_FALSE;
}

STDAPI DllGetClassObject(const CLSID &clsid, REFIID refIID, void ** ppv)
{
	trace ("DllGetClassObject : Create Class Factory");

	if (clsid != CLSID_Component1)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	ClassFactory *pClassFactory = new ClassFactory;
	if (pClassFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pClassFactory -> QueryInterface(refIID, ppv);
	pClassFactory -> Release();

	return hr;
}

STDAPI DllRegisterServer()
{
 	cout << "Inside DllRegister Server " << endl;
	return RegisterServer(g_hModule,
						  CLSID_Component1,
						  g_szFriendlyName,
						  g_szVerIndProgId,
						  g_szProgId);
}

STDAPI DllUnregisterServer()
{
	cout << "Inside DllUnregisterServer" << endl;
	return UnregisterServer(CLSID_Component1,
							g_szVerIndProgId,
							g_szProgId);
}

