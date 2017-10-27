#include <iostream>
#include <objbase.h>

using namespace std;

void trace (const char *pMsg) { cout << pMsg << endl; }

extern const IID IID_IX;
extern const IID IID_IY;
extern const IID IID_IZ;

interface IX : public IUnknown
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

/******************** COMPONENT ******************/

class CA : public IX,
		   public IY
{
	virtual HRESULT __stdcall QueryInterface(const IID &refIID, void ** ppv);
	virtual ULONG 	__stdcall AddRef();
	virtual ULONG 	__stdcall Release();

	virtual void __stdcall Fx() { cout << "Fx" << endl; }
	virtual void __stdcall Fy() { cout << "Fy" << endl; }

	public :
		 CA() : m_cRef(0){}
		~CA() { trace ("CA : Destroy self."); }

	private :
		long m_cRef;	 	
};

HRESULT __stdcall CA::QueryInterface(const IID &refIID, void ** ppv)
{
	if (refIID == IID_IUnknown)
	{
		trace ("CA : QI : Return pointer to IUnknown");
		*ppv = static_cast<IX*>(this);
	}
	else if (refIID == IID_IX)
	{
		trace ("CA : QI : Return pointer to IX");
		*ppv = static_cast<IX*>(this);
	}
	else if (refIID == IID_IY)
	{
		trace ("CA : QI : Return pointer to IY");
		*ppv = static_cast<IY*>(this);
	}
	else 
	{
		trace ("CA : QI : Interface not supported ");
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown *>(*ppv) -> AddRef();
	return (S_OK);
}

ULONG __stdcall CA::AddRef()
{
	cout << "CA : AddRef = " << m_cRef + 1 << "." << endl;
	return (InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CA::Release()
{
	cout << "CA : Release = " << m_cRef - 1 << "." << endl;
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

IUnknown * CreateInstance ()
{
	IUnknown *pI = static_cast<IX*>(new CA);
	pI -> AddRef();
	return pI;
}

// {E9FBD3A6-AAC8-4F4F-8845-E04CA36BB959}
static const IID IID_IX = { 0xe9fbd3a6, 0xaac8, 0x4f4f, { 0x88, 0x45, 0xe0, 0x4c, 0xa3, 0x6b, 0xb9, 0x59 } };

// {67063A88-FBA1-4421-A923-0A03C96CC3E0}
static const IID IID_IY = { 0x67063a88, 0xfba1, 0x4421, { 0xa9, 0x23, 0xa, 0x3, 0xc9, 0x6c, 0xc3, 0xe0 } };

// {017EDE65-2D7F-4471-A6A5-05E4E4FDD0AD}
static const IID IID_IZ = { 0x17ede65, 0x2d7f, 0x4471, { 0xa6, 0xa5, 0x5, 0xe4, 0xe4, 0xfd, 0xd0, 0xad } };

int main (void)
{
	HRESULT hr;

	trace ("Client : Get an IUnknown pointer.");
	IUnknown *pIUnknown = CreateInstance ();

	trace ("Client : Get interface IX");
	IX *pIX = NULL;
	hr = pIUnknown -> QueryInterface(IID_IX, (void **)&pIX);
	if (SUCCEEDED(hr))
	{
		trace ("Client : Succeeded in getting IX");
		pIX -> Fx();
		pIX -> Release();
	}

	trace ("Client : Get interface IY");
	IY *pIY = NULL;
	hr = pIUnknown -> QueryInterface(IID_IY, (void **)&pIY);
	if (SUCCEEDED(hr))
	{
		trace ("Client : Succeeded in getting IY");
		pIY -> Fy();
		pIX -> Release();
	}

	trace ("Client : Ask for an unsupported interface.");
	IZ *pIZ = NULL;
	hr = pIUnknown -> QueryInterface(IID_IZ, (void **)&pIZ);
	if (SUCCEEDED(hr))
	{
		trace ("Client : Succeeded in getting interface IZ");
		pIZ -> Fz();
		pIZ -> Release();
	}
	else 
	{
		trace ("Client : Couls not get interface IZ");
	}

	trace ("Client : Release IUnknown interface.");
	pIUnknown -> Release();

	return 0;
}




