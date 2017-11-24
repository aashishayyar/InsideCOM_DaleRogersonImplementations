#include <iostream>
#include <objbase.h>

#include "interface_header.h"
#include "registry.h"
#include "component.h"

using namespace std;

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

class CList : public IList 
{
public: 
	virtual HRESULT __stdcall QueryInterface(REFIID refIID, void **ppv);
	virtual ULONG   __stdcall AddRef();
	virtual ULONG   __stdcall Release();

	 CList();
	~CList();
	
	result_t __stdcall insert_beginning(data_t n_data);
	result_t __stdcall insert_end		 (data_t n_data);
	result_t __stdcall insert_before 	 (data_t e_data, data_t n_data);
	result_t __stdcall insert_after 	 (data_t e_data, data_t n_data); 
	result_t __stdcall delete_beignning();
	result_t __stdcall delete_end		 ();	
	result_t __stdcall delete_data 	 (data_t e_data);
	result_t __stdcall find			 (data_t s_data);
	result_t __stdcall is_empty		 ();
	result_t __stdcall display		 ();

private:
	struct _node; 
	typedef struct _node node_t;
	typedef struct _node list_t;

	struct _node
	{
		node_t *next;	
		node_t *prev;
		data_t  data;
	};

	node_t  *getnode 	(data_t n_data);
	void    *getblock	(int nr_elements, int size_per_element);
	node_t  *getlastnode ();
	void  	 g_insert	(node_t *target, node_t *new_node);
	node_t  *search_node (data_t s_data);
 	result_t g_delete(node_t *target_node);

	list_t  *main_list;
	long 	 m_cRef;
};

CList::CList() : m_cRef(1)
{
	InterlockedIncrement(&g_cComponents);
}

CList::~CList()
{
	InterlockedDecrement(&g_cComponents);
	trace("Destroying Self");
}

/********************* IUnknwon Methods ******************************/

HRESULT __stdcall CList::QueryInterface(REFIID refIID, void ** ppv)
{
	if (refIID == IID_IUnknown)
	{
		trace ("IUnknown pointer is returned here.");
		*ppv = static_cast<IList *>(this);
	}
	else if (refIID == IID_IList)
	{
		trace ("IList pointer is returned now.");
		*ppv = static_cast<IList *>(this);
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

ULONG __stdcall CList::AddRef()
{
	return 	InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CList::Release()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

/********************** AUXILIARY METHODS *************************/

CList::node_t *CList::getnode(data_t n_data)
{
	CList::node_t *node = (CList::node_t *)getblock(1, sizeof(CList::node_t));
	if (node != NULL)
		node -> data = n_data;
	return node;	 
}

void *CList::getblock(int nr_elements, int size_per_element)
{
	void *ptr = calloc (nr_elements, size_per_element);
	return ptr;
}

CList::node_t *CList::getlastnode()
{
	return main_list -> prev;
}

void CList::g_insert(CList::node_t *target, CList::node_t *new_node)
{
	new_node -> next  			= target -> next;
	target   -> next  -> prev 	= new_node;
	new_node -> prev 			= target;
	target	 -> next 			= new_node; 
}

CList::node_t *CList::search_node(data_t s_data)
{
	CList::node_t *run = main_list -> next;
	while (run != main_list)
	{
		if (run -> data == s_data)
			return run;
		else 
			run = run -> next;
	}
	return (NULL);
}

result_t CList::g_delete(CList::node_t *target_node)
{
	if (find(target_node -> data) == FALSE)
	{
		cout << "g_delete : Cannot delete : Requested node or data is not present." << endl;
		return FAILURE;
	}
	target_node -> prev -> next = target_node -> next;
	target_node -> next -> prev = target_node -> prev;
	return SUCCESS;
}

/********************** INTERFACE METHODS **************************/

result_t CList::insert_beginning(data_t n_data)
{
	CList::node_t *new_node = getnode(n_data);
	if (new_node == NULL)
	{
		cout << "insert_beginning : Cannot get a new node : E_OUTOFMEMORY." << endl;
		return FAILURE;
	}
	cout << "HERE : " << n_data << endl;
	g_insert(main_list, new_node);
	return SUCCESS;
}

result_t CList::insert_end(data_t n_data)
{
	CList::node_t *new_node  = getnode(n_data);
	if (new_node == NULL)
	{
		cout << "insert_end : Cannot get a new node : E_OUTOFMEMORY." << endl;
		return FAILURE;
	}
	CList::node_t *last_node = getlastnode();
	g_insert(last_node, new_node);

	return SUCCESS;
}

result_t CList::insert_before(data_t e_data, data_t n_data)
{
	if (find(e_data) == FALSE)
	{
		cout << "insert_after : Cannot insert : Requested data is not present." << endl;
		return FAILURE;
	}

	CList::node_t *new_node  = getnode(n_data);
	if (new_node == NULL)
	{
		cout << "insert_before : Cannot get a new node : E_OUTOFMEMORY." << endl;
		return FAILURE;
	}

	CList::node_t *e_node 	 = search_node(e_data);
	g_insert(e_node -> prev, new_node);

	return SUCCESS;
}

result_t CList::insert_after(data_t e_data, data_t n_data)
{
	if (find(e_data) == FALSE)
	{
		cout << "insert_after : Cannot insert : Requested data is not present." << endl;
		return FAILURE;
	}

	CList::node_t *new_node = getnode(n_data);
	CList::node_t *e_node 	 = search_node(e_data);
	g_insert(e_node, new_node);

	return SUCCESS;
}

result_t CList::delete_beignning()
{
	if (is_empty() == TRUE)
	{
		cout << "delete_beignning : Cannot Delete : List is empty" << endl;
		return FAILURE;
	}
	g_delete(main_list -> next);
	return SUCCESS;
}

result_t CList::delete_end ()
{
	if (is_empty() == TRUE)
	{
		cout << "delete_beignning : Cannot Delete : List is empty" << endl;
		return FAILURE;
	}
	g_delete(main_list -> prev);
	return SUCCESS;
}	

result_t CList::delete_data(data_t target_data)
{
	if (find(target_data) == FALSE)
	{
		cout << "delete_data : Cannot Delete : Requested data not present." << endl;
		return FAILURE;
	}
	CList::node_t *target_node = search_node(target_data);
	g_delete(target_node);
	return SUCCESS; 
}

result_t CList::is_empty()
{
	if ((main_list -> next == main_list) && (main_list -> prev == main_list))
		return TRUE;
	else 
		return FALSE;
}

result_t CList::find(data_t s_data)
{
	CList::node_t *node = search_node(s_data);
	if (node == NULL)
		return FALSE;
	else 
		return TRUE;
}

result_t CList::display ()
{
	CList::node_t *run = main_list -> next;
	cout << "[HEAD] <-> ";
	while (run != main_list)
	{
		cout << "[" << run -> data << "] <-> ";
		run = run -> next;
	}
	cout << "[HEAD]" << endl;
	return SUCCESS;
}

/**************************** IClassFactory ******************************/
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

/********************* IUnknown methods of ClassFactory *******************/
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
/**************** ClassFactory method to create CoClass instance *************/
HRESULT __stdcall ClassFactory::CreateInstance (IUnknown * pUnkOuter, REFIID refIID, void ** ppv)
{
	trace ("ClassFactory : CreateInstance.");
	if (pUnkOuter != NULL)
	{
		trace ("return CLASS_E_NOAGGREGATION");
	}

	CList *pCList = new CList;
	if (pCList == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pCList -> QueryInterface(refIID, ppv);
	pCList -> Release();

	return hr;
}

/**************** Miscellaneous Methods *********************************/
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

/*************** Global method to create ClassFactory instance *************/
STDAPI DllGetClassObject(const CLSID &clsid, REFIID refIID, void ** ppv)
{
	trace ("DllGetClassObject : Create Class Factory");

	if (clsid != CLSID_CList)
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

/****************** Methods called by regsvr32 **************************/
STDAPI DllRegisterServer()
{
 	cout << "Inside DllRegister Server " << endl;
	return RegisterServer(g_hModule,
						  CLSID_CList,
						  g_szFriendlyName,
						  g_szVerIndProgId,
						  g_szProgId);
}

STDAPI DllUnregisterServer()
{
	cout << "Inside DllUnregisterServer" << endl;
	return UnregisterServer(CLSID_CList,
							g_szVerIndProgId,
							g_szProgId);
}

