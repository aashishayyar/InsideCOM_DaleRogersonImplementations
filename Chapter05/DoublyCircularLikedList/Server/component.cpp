#include <iostream>
#include <objbase.h>

#include "interface_header.h"
#include "component.h"
/************************* CoClass *****************************/
class CList : public IList
{
	public:
		CList()
		{
			main_list = getnode(0);
			main_list -> next = main_list;
			main_list -> prev = main_list; 
		}
		~CList()
		{
			node_t *tmp, *run = main_list -> next;
			while(run != main_list)
			{
				tmp = run -> next;
				free(run);
				run = tmp;
			}
			free(main_list);
		}

		virtual HRESULT __stdcall QueryInterface(const IID &refIID, void ** ppv);
		virtual ULONG 	__stdcall AddRef();
		virtual ULONG 	__stdcall Release();

		result_t insert_beginning(data_t n_data);
		result_t insert_end		 (data_t n_data);
		result_t insert_before 	 (data_t e_data, data_t n_data);
		result_t insert_after 	 (data_t e_data, data_t n_data); 
		result_t delete_beignning();
		result_t delete_end		 ();	
		result_t delete_data 	 (data_t e_data);
		result_t find			 (data_t s_data);
		result_t is_empty		 ();
		result_t display		 ();

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

/********************* IUnknwon Methods ******************************/
HRESULT __stdcall CList::QueryInterface(const IID &refIID, void ** ppv)
{
	if (refIID == IID_IUnknown)
		*ppv = static_cast<IList *>(this);
	else if (refIID == IID_IList)
		*ppv = static_cast<IList *>(this);
	else 
	{
		cout << "Requested interface not supported." << endl;
		*ppv = NULL;
		return E_NOINTERFACE;
	}	 

	reinterpret_cast<IUnknown *>(*ppv) -> AddRef();
	
	return (S_OK);	
}

ULONG __stdcall CList::AddRef()
{
	InterlockedIncrement(&m_cRef);
	return (S_OK);
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

/******************** Global Function to create an instance *********************/
extern "C" IUnknown *CreateInstance()
{
	IUnknown *pI = static_cast<IList*>(new CList);
	pI -> AddRef();
	return pI;
}