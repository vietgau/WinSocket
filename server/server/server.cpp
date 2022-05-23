//For Server
//#include<WinSock2.h>
#include<iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <process.h>

#pragma comment(lib,"ws2_32.lib")


using namespace std;
static SOCKET TCPServerSocket;
FILE* f;
const char fileName[] = "C:\\Users\\vuhon\\OneDrive - actvn.edu.vn\\works\\Socket\\WinSocket\\log.txt";

UINT Recv(VOID* pParam);
UINT Send(VOID* pParam);
UINT RecvAndSend(VOID* pParam);
VOID Connect() {
	//Step-1 WSAStartup Fun------------------------------------
	WSADATA Winsockdata;
	int iWsaStartup;
	int iWsaCleanup;

	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);
	if (iWsaStartup != 0)
	{
		cout << "WSAStartup Failed" << endl;

	}
	cout << "WSAStartup Success" << endl;

	// STEP -2 Fill the structure-------------------------------

	struct sockaddr_in TCPServerAdd;
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.S_un.S_addr = INADDR_ANY;
	TCPServerAdd.sin_port = htons(54000);

	//Step -3 Socket Creation------------------------------------


	TCPServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPServerSocket == INVALID_SOCKET)
	{
		cout << "TCP Server Socket Creation failed" << WSAGetLastError() << endl;

	}


	//Step -4 bind fun------------------------------------------

	int iBind;
	iBind = bind(TCPServerSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iBind == SOCKET_ERROR)
	{
		cout << "Binding Failed &Error No->" << WSAGetLastError() << endl;


	}
	cout << "Binding success" << endl;

	//STEP-5 Listen fun-----------------------------------------
	int iListen;
	iListen = listen(TCPServerSocket, 10);
	if (iListen == SOCKET_ERROR)
	{
		cout << "Listen fun failed &error No->" << WSAGetLastError();

	}
	cout << "Listen fun success" << endl;

	// STEP-6 Accept---------------------------------------------
	SOCKET sAcceptSocket;
	struct sockaddr_in TCPClientAdd;
	int iTCPClientAdd = sizeof(TCPClientAdd);
	fopen_s(&f, fileName, "a+");
	while (true)
	{
		sAcceptSocket = accept(TCPServerSocket, (SOCKADDR*)&TCPClientAdd, &iTCPClientAdd);
		if (sAcceptSocket == INVALID_SOCKET)
		{
			cout << "Accept failed & Error No ->" << WSAGetLastError() << endl;

		}
		cout << "Accept fun success" << endl;

		char ip[INET_ADDRSTRLEN];
		int port = ntohs(TCPClientAdd.sin_port) ;
		ZeroMemory(ip, INET_ADDRSTRLEN);
		inet_ntop(AF_INET, &TCPClientAdd.sin_addr, ip, INET_ADDRSTRLEN);
		cout << ip << " connected on port " << port << endl;
		fprintf(f, "ip: %s, port: %d, ",ip, port);
		_beginthreadex(0, 0, Recv, (LPVOID)&sAcceptSocket, 0, 0);
	}

}


UINT Send(VOID* pParam) {

	// STEP-7 Send Data to the client
	SOCKET* client = (SOCKET*)pParam;
	SOCKET sAcceptSocket = *client;
	int iSend;
	char* SenderBuffer;
	SenderBuffer = (char*)(malloc(1 * sizeof(char)));
	cout << "Mess to client: " << endl;
	cin >> SenderBuffer;
	int iSenderBuffer = strlen(SenderBuffer) + 1;
	iSend = send(sAcceptSocket, SenderBuffer, iSenderBuffer, 0);
	if (iSend == SOCKET_ERROR)
	{
		cout << "Sending Failed & Error No->" << WSAGetLastError() << endl;
		return 1;
	}
	cout << "Send fun success" << endl;
	return 0;
}

UINT Recv(VOID* pParam) {
	// STEP -8 Recv Data from Client
	SOCKET* client = (SOCKET*)pParam;
	SOCKET sAcceptSocket = *client;
	int iRecv;
	char* RecvBuffer;
	RecvBuffer = (char*)(malloc(1 * sizeof(char)));
	int iRecvBuffer = strlen(RecvBuffer) + 1;
	iRecv = recv(sAcceptSocket, RecvBuffer, iRecvBuffer, 0);
	fprintf(f, "long: %d \n", iRecvBuffer - 1);
	if (iRecv == SOCKET_ERROR)
	{
		cout << "Receiving Failed & Error No->" << WSAGetLastError() << endl;
		return 1;
	}
	cout << "Receive fun success" << endl;
	cout << "Data Received -> " << RecvBuffer << " leng " << iRecvBuffer - 1 << endl;
	return 0;

}
UINT RecvAndSend(VOID* pParam) {
	// STEP -8 Recv Data from Client
	SOCKET* client = (SOCKET*)pParam;
	SOCKET sAcceptSocket = *client;

	// Recv
	int iRecv;
	char* RecvBuffer;
	RecvBuffer = (char*)(malloc(1 * sizeof(char)));
	int iRecvBuffer = strlen(RecvBuffer) + 1;
	iRecv = recv(sAcceptSocket, RecvBuffer, iRecvBuffer, 0);
	if (iRecv == SOCKET_ERROR)
	{
		cout << "Receiving Failed & Error No->" << WSAGetLastError() << endl;
		return 1;
	}
	cout << "Receive fun success" << endl;
	cout << "Data Received -> " << RecvBuffer <<" leng " << iRecvBuffer -1 << endl;

	// Send
	int iSend;
	char* SenderBuffer;
	SenderBuffer = (char*)(malloc(1 * sizeof(char)));
	cout << "Mess to client: " << endl;
	cin >> SenderBuffer;
	int iSenderBuffer = strlen(SenderBuffer) + 1;
	iSend = send(sAcceptSocket, SenderBuffer, iSenderBuffer, 0);
	if (iSend == SOCKET_ERROR)
	{
		cout << "Sending Failed & Error No->" << WSAGetLastError() << endl;
		return 1;
	}
	cout << "Send fun success" << endl;
	return 0;

}

BOOL Disconnect() {
	//STEP - 9 Close Socket

		if (FALSE)
		{
			int iCloseSocket;
			iCloseSocket = closesocket(TCPServerSocket);

			if (iCloseSocket == SOCKET_ERROR)
			{
				cout << "Closing Failed & Error No->" << WSAGetLastError() << endl;
			}
			cout << "Cleanup fun success" << endl;
			system("PAUSE");
			return FALSE;
		}
	return TRUE;
}

int main()
{

	Connect();
	closesocket(TCPServerSocket);
	WSACleanup();

}
