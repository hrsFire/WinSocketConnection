#include "stdafx.h"
#include "WinSocketConnection.h"

using namespace std;
using namespace WinSocketConnection;

int _tmain(int argc, _TCHAR* argv[])
{
	
	string message = "";
	char* buffer;

	WinSocketServer a( "192.168.1.101", 8888);

	
	if( a.createWinSocket())
		cout << "connection accepted!\r\n";
	for( ; ; )
	{
		cout << "message: ";
		cin >> message;

		if( !message.empty())
		{
			if( message == "closesocket")
			{
				a.closeWinSocket();
			}
			if( message != "exit")
			{
				buffer =  new char( message.length());
				strcpy( buffer, message.c_str());
				
				// send and check if the packet has arrived
				if( a.sendMessage( buffer))
					cout << "\r\nreached\r\n";
				else
				{
					system("PAUSE");
					return 0;
				}
			}
			else
			{
				a.closeWinSocket();
				a.~WinSocketServer();
				return 0;
			}
		}
	}
	return 0;
}