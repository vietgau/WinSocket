//For Server
#include<WinSock2.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")
# pragma warning(disable:4996)
using namespace std;
static SOCKET TCPServerSocket;
static SOCKET sAcceptSocket;

void Connect() {
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
	TCPServerAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
	TCPServerAdd.sin_port = htons(8000);

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
	iListen = listen(TCPServerSocket, 2);
	if (iListen == SOCKET_ERROR)
	{
		cout << "Listen fun failed &error No->" << WSAGetLastError();
	}
	cout << "Listen fun success" << endl;

	// STEP-6 Accept---------------------------------------------

	struct sockaddr_in TCPClientAdd;
	int iTCPClientAdd = sizeof(TCPClientAdd);
	
	sAcceptSocket = accept(TCPServerSocket, (SOCKADDR*)&TCPClientAdd, &iTCPClientAdd);
	if (sAcceptSocket == INVALID_SOCKET)
	{
		cout << "Accept failed & Error No ->" << WSAGetLastError() << endl;
	}
	cout << "Accept fun success" << endl;
}
void Send() {

	// STEP-7 Send Data to the client

	int iSend;
	char SenderBuffer[512] =  "Hello from server";
	int iSenderBuffer = strlen(SenderBuffer) + 1;

	iSend = send(sAcceptSocket, SenderBuffer, iSenderBuffer, 0);
	if (iSend == SOCKET_ERROR)
	{
		cout << "Sending Failed & Error No->" << WSAGetLastError() << endl;
	}
	cout << "Send fun success" << endl;
	
}

void Recv() {
	// STEP -8 Recv Data from Client

	int iRecv;
	char RecvBuffer[512];
	int iRecvBuffer = strlen(RecvBuffer) + 1;
	iRecv = recv(sAcceptSocket, RecvBuffer, iRecvBuffer, 0);
	if (iRecv == SOCKET_ERROR)
	{
		cout << "Receiving Failed & Error No->" << WSAGetLastError() << endl;
	}
	cout << "Receive fun success" << endl;
	cout << "Data Received -> " << RecvBuffer << endl;

}
void Disconnect() {
	//STEP - 9 Close Socket
	int iCloseSocket;
	iCloseSocket = closesocket(TCPServerSocket);

	if (iCloseSocket == SOCKET_ERROR)
	{
		cout << "Closing Failed & Error No->" << WSAGetLastError() << endl;
	}
	cout << "Cleanup fun success" << endl;
	system("PAUSE");
}

int main()
{

	cout << "\t\t------TCP Server-------" << endl;
	cout << endl;
	Connect();
	Send();
	Recv();
	Disconnect();
}
