	Doubly Circular Linked List - Aashish N. Ayyar

********************** Compilation Steps ****************************
To compile server : 
cl.exe /LD component.cpp guids.cpp component.def /EHsc

To compile client :
cl.exe client.cpp guids.cpp /EHsc

Before testing, make sure the dll file has been copied into the required path [ in this case client directory ].

Compiled and executed successfully on the following platform :
Windows 10 version 1703, Microsoft Visual Studio 2015.

**************************** Design ********************************

Main objective is to bring isolation between client and server. The server provides an interface, which is an 
abstract class. Server internally maintains the List,  and exposes just the methods to manipulate the list.
Keeping in mind the future patters of COM, it has been designed using pure vitual methods. Hence the internal layout 
is not accesible to the client. 
Also the complete project is in complete parity with the framework designed by Dale Rogerson in Inside COM (Chapter05).


