#ifndef __Registry_H__
#define __Registry_H__

HRESULT RegisterServer(HMODULE hModule, 
					   const CLSID &clsid,
					   const char *szFriendlyname,
					   const char *szVerIndProgID,
					   const char *szProgID);

HRESULT UnregisterServer(const CLSID &clsid, 
						 const char *szVerIndProgID,
						 const char *szProgID);

#endif