/******************* Command to build Server ***************************/

cl /LD component.cpp component.def registry.cpp guids.cpp /EHsc advapi32.lib ole32.lib


VVIMP : Run Cmd with Admin rights. THen go to SysWow64 and then run the following command 
regsvr32 <path>\component.dll

/******************* Command to build client ***************************/

cl client.cpp guids.cpp /EHsc ole32.lib