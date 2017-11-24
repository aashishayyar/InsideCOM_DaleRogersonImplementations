typedef int result_t;
typedef int data_t;

using namespace std;

interface IList : public IUnknown
{
	virtual result_t __stdcall insert_beginning	(data_t n_data) 				= 0;
	virtual result_t __stdcall insert_end		(data_t n_data)					= 0;
	virtual result_t __stdcall insert_before 	(data_t e_data, data_t n_data) 	= 0;
	virtual result_t __stdcall insert_after 	(data_t e_data, data_t n_data) 	= 0; 
	virtual result_t __stdcall delete_beignning	()								= 0;
	virtual result_t __stdcall delete_end		()								= 0;	
	virtual result_t __stdcall delete_data 	 	(data_t e_data)					= 0;
	virtual result_t __stdcall find			 	(data_t s_data)					= 0;
	virtual result_t __stdcall is_empty		 	()								= 0;
	virtual result_t __stdcall display		 	()								= 0;
};

extern "C" extern const CLSID CLSID_CList;

extern "C" extern const IID IID_IList;
	

