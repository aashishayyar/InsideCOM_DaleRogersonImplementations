/* Aashish N. Ayyar : INTERFACE_HEADER.H

 This file is shared between the client and server. It contains the pure virtual interfaces that are exposed to client. 
 The client can call these methods, using the interface pointer. Neither the class name nor the its internal private methods 
 and members are exposed to the client.

*/

typedef int result_t;
typedef int data_t;

using namespace std;

interface IList : public IUnknown
{
	virtual result_t insert_beginning(data_t n_data) 				= 0;
	virtual result_t insert_end		 (data_t n_data)				= 0;
	virtual result_t insert_before 	 (data_t e_data, data_t n_data) = 0;
	virtual result_t insert_after 	 (data_t e_data, data_t n_data) = 0; 
	virtual result_t delete_beignning()								= 0;
	virtual result_t delete_end		 ()								= 0;	
	virtual result_t delete_data 	 (data_t e_data)				= 0;
	virtual result_t find			 (data_t s_data)				= 0;
	virtual result_t is_empty		 ()								= 0;
	virtual result_t display		 ()								= 0;
};

extern "C" extern const IID IID_IList;
