#include <iostream>
#include <stdio.h>

#include <objbase.h>
#include <assert.h>

#include "Registry.h"

using namespace std;

BOOL setKeyAndValue (const char *pszPath,
					 const char *szSubkey,
					 const char *szValue);

void CLSIDtochar (const CLSID &clsid, 
				  char *szCLSID,
				  int   length);

LONG recursiveDeleteKey(HKEY hKeyParent, const char *szKeyChild);

const int CLSID_STRING_SIZE = 39;

HRESULT RegisterServer(HMODULE hModule,
					  const CLSID &clsid,
					  const char *szFriendlyName,
					  const char *szVerIndProgID,
					  const char *szProgID)
{

	char szModule[512];
	DWORD dwResult = ::GetModuleFileName(hModule, 
										 szModule,
										 sizeof(szModule)/sizeof(char));
	assert(dwResult != 0);

	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	char szKey[64];
	strcpy(szKey, "WOW6432Node\\CLSID\\");
	strcat(szKey, szCLSID);

	setKeyAndValue(szKey, NULL, szFriendlyName);

	setKeyAndValue(szKey, "InprocServer32", szModule);

	setKeyAndValue(szKey, "ProgID", szProgID);

	setKeyAndValue(szKey, "VersionIndepedentProgID", szVerIndProgID);

	setKeyAndValue(szVerIndProgID,  NULL   , szFriendlyName);
	setKeyAndValue(szVerIndProgID, "CLSID" , szCLSID);
	setKeyAndValue(szVerIndProgID, "CurVer", szProgID);

	setKeyAndValue(szProgID,  NULL  , szFriendlyName);
	setKeyAndValue(szProgID, "CLSID", szCLSID);

	return S_OK;
}

LONG UnregisterServer(const CLSID &clsid, 
					  const char *szVerIndProgID,
					  const char *szProgID)
{
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND));

	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND));

	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND));

	return S_OK;
}

BOOL setKeyAndValue(const char *szKey, 
					const char *szSubkey,
					const char *szValue)
{
	HKEY hKey;
	char szKeyBuf[1024];

	strcpy(szKeyBuf, szKey);

	if (szSubkey != NULL)
	{
		strcat (szKeyBuf , "\\");
		strcat (szKeyBuf, szSubkey);
	}
	long lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT,
								  szKeyBuf,
								  0, NULL, REG_OPTION_NON_VOLATILE,
								  KEY_ALL_ACCESS, NULL,
								  &hKey, NULL);
	if (lResult != ERROR_SUCCESS)
		return FALSE;

	if (szValue != NULL)
		RegSetValueEx(hKey, NULL, 0, REG_SZ,
					(BYTE *)szValue,
					strlen(szValue) + 1);

	RegCloseKey(hKey);

	return TRUE;
}

void CLSIDtochar (const CLSID &clsid, 
				  char *szCLSID,
				  int length)
{

	assert(length >= CLSID_STRING_SIZE);

	LPOLESTR wszCLSID = NULL;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
	assert(SUCCEEDED(hr));

	wcstombs(szCLSID, wszCLSID, length);

	CoTaskMemFree(wszCLSID);
}

LONG recursiveDeleteKey(HKEY hKeyParent,
						const char *lpszKeyChild)
{
	HKEY hKeyChild;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0, 
							 KEY_ALL_ACCESS, &hKeyChild);
	if (lRes != ERROR_SUCCESS)
		return lRes;

	FILETIME time;
	char szBuffer[256];
	DWORD dwSize = 256;
	while (RegEnumKeyEx(hKeyChild, 0, szBuffer, &dwSize, NULL, 
						NULL, NULL, &time) == S_OK)
	{
		lRes = recursiveDeleteKey(hKeyChild, szBuffer);
		if (lRes != ERROR_SUCCESS)
		{
			RegCloseKey(hKeyChild);
			return lRes;
		}
		dwSize = 256;
	}
	RegCloseKey(hKeyChild);

	return RegDeleteKey(hKeyParent, lpszKeyChild);
}