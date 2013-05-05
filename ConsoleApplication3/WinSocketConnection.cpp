#include "stdafx.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include "WinSocketConnection.h"

using namespace std;
using namespace WinSocketConnection;


PCTSTR ipAddr;
u_long interfaceAddress;
int port;
WSADATA WsaDat; // structure for the windows socket implementation
SOCKET Socket;
SOCKET TempSocket;
SOCKADDR_IN serverInf; // stores required informations such as ip, port, protocol type
byte inetType; // 2 -> AF_INET ; 23 -> AF_INET6
u_long connectionMode; // 0 -> blocking mode: wait until the request is finished ; != 0 -> 1 -> non-blocking mode
char* ServerMessage;
int nError;
char* buffer;
wchar_t c;
string ipBuffer;
		

WinSocketServer::WinSocketServer( string ipAddress, int portNumber)
{
	//c = &ipAddress;
	//strcat( buffer, ipAddress.c_str());
	//char Narrow = 'a'; 
//wchar_t Widen = _USE(cout.getloc(), ctype<wchar_t> ).widen(Narrow);
	//ipAddr = _USE(cout.getloc(), ctype<wchar_t>).widen(buffer);
	//ipAddr = ipAddress.w_str(); //Marshal::String.ToHGlobalAnsi( ipAddress);
	//struct sockaddr_in sa;
			

	/*if( InetPton( AF_INET, ipAddr, &(sa.sin_addr)) == 1)
	{
		interfaceAddress = inet_addr( ipAddress.c_str()); // convert ipAddress in interfaceAddress
		inetType = 2;
	}
	else if( InetPton( AF_INET6, ipAddr, &(sa.sin_addr)) == 1)
	{
		interfaceAddress = inet_addr( ipAddress.c_str()); // convert ipAddress in interfaceAddress
		inetType = 23;
	}
	else
		interfaceAddress = INADDR_ANY; // use all connections
*/


	// validate the ipAddress
	if( count( ipAddress.begin(), ipAddress.end(), '.') == 3) // IPv4
	{
		inetType = AF_INET;
		interfaceAddress = inet_addr( ipAddress.c_str()); // convert ipAddress in interfaceAddress
	}
	else if( count( ipAddress.begin(), ipAddress.end(), ':') == 7) // IPv6
	{
		inetType = AF_INET6;
		interfaceAddress = inet_addr( ipAddress.c_str()); // convert ipAddress in interfaceAddress
	}
	else
	{
		inetType = AF_INET;
		interfaceAddress = INADDR_ANY;
	}

	// set port number when it is in a valid range
	if( port <= 65535)
		port = portNumber;
	else
		port = 0;
}

WinSocketServer::~WinSocketServer() {
	shutdown( Socket, SD_SEND);
	closesocket( Socket);
};

bool WinSocketServer::createWinSocket() {
	if( WSAStartup( MAKEWORD( 2, 2), &WsaDat) != 0)
	{
		cout << "WSA initialisation failed!\r\n";
		WSACleanup();
		system( "PAUSE");
		return false;
	}

	cout << "WSA initialisation successful!\r\n";

	// use Interet Address Protocoll, stream and TCP
	Socket = socket( inetType, SOCK_STREAM, IPPROTO_TCP);
	if( Socket == INVALID_SOCKET)
	{
		cout << "Socket creation failed!\r\n";
		WSACleanup();
		system( "PAUSE");
		return false;
	}
			
	cout << "Socket creation successful!\r\n";

	// initialization of SOCKADDR_IN
	serverInf.sin_family = AF_INET;
	serverInf.sin_addr.s_addr = interfaceAddress;
	serverInf.sin_port = htons( port);

	if( bind( Socket, (SOCKADDR*) (&serverInf), sizeof( serverInf)) == SOCKET_ERROR)
	{
		cout << "Unable to bind socket!\r\n";
		WSACleanup();
		system( "PAUSE");
		return false;
	}

	listen( Socket, 1); //backlog set to 1 -> no queue

	TempSocket = SOCKET_ERROR;

	while( TempSocket == SOCKET_ERROR)
	{
		cout << "Watiting for incoming connections...\r\n";
		TempSocket = accept( Socket, NULL, NULL);
	}

	ioctlsocket( Socket, FIONBIO, &connectionMode);

	Socket = TempSocket;

	cout << "\r\n\r\nConnection established with Client!\r\n\r\n";
	return true;
}

bool WinSocketServer::sendMessage( char* s)
{
	ServerMessage = s;
	send( Socket, ServerMessage, strlen( ServerMessage), 0);


	nError = WSAGetLastError();
	if( (nError != WSAEWOULDBLOCK) && (nError != 0))
	{
		cout << "Winsock error code: "<<nError<<"\r\nShutdown Socket:...";

		shutdown( Socket, SD_SEND);

		closesocket( Socket);
		cout << "OK";
		return false;
	}
	return true;
}

void WinSocketServer::closeWinSocket() {
	shutdown( Socket, SD_SEND);
	closesocket( Socket);
}