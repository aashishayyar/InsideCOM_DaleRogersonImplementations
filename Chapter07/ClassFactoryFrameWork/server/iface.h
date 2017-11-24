interface IX : public IUnknown 
{
	virtual void __stdcall Fx() = 0;
};

interface IY : public IUnknown 
{
	virtual void __stdcall Fy() = 0;
};

interface IZ : public IUnknown 
{
	virtual void __stdcall Fz() = 0;
};

extern "C" extern CLSID CLSID_Component1;

extern "C" extern IID IID_IX;
extern "C" extern IID IID_IY;
extern "C" extern IID IID_IZ;
