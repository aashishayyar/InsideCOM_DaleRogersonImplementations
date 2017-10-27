// Aashish N. Ayyar : IFACE.cpp

#include <iostream>
#include <objbase.h>

using namespace std;

void trace(const char *pMsg) {cout << pMsg << endl; }

interface IX
{
	virtual void __stdcall Fx1() = 0;
	virtual void __stdcall Fx2() = 0;
};

interface IY
{
	virtual void __stdcall Fy1() = 0;
	virtual void __stdcall Fy2() = 0;
};

class CA :  public IX,
			public IY
{
	public :
		virtual void __stdcall Fx1() {cout << "CA::Fx1" << endl; }
		virtual void __stdcall Fx2() {cout << "CA::Fx2" << endl; }

		virtual void __stdcall Fy1() {cout << "CA::Fy1" << endl; }
		virtual void __stdcall Fy2() {cout << "CA::Fy2" << endl; }			
};

int main()
{
	trace ("Client : Create an instance of the compnent. ");
	CA *pA = new CA;

	IX * pIX = pA;

	trace ("Client : Use the IX interface. ");
	pIX -> Fx1();
	pIX -> Fx2();

	IY * pIY = pA;

	trace ("Client : Use the IY interface. ");
	pIY -> Fy1();
	pIY -> Fy2();

	trace ("Client : Delete the component. ");
	delete pA;

	return 0;
}

