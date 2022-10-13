/*
	Simple udp client
*/
#include<stdio.h>
#include<winsock2.h>
#include "ImplementModuleMsg.h"
#include "ImplementMsg.h"


#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 8080	//The port on which to listen for incoming data

DWORD WINAPI ThreadFunc(void* data) 
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	int iResult;
	u_long iMode = 1;

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the 
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.

	iResult = ioctlsocket(s, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
		printf("ioctlsocket failed with error: %ld\n", iResult);


	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(7777);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	while (true)
	{
		static int fuelLevel = 999999;
		ImpMsg msg(enumImpMsgId::IMP_FUEL_MSG, fuelLevel--);
		//send the message
		if (sendto(s, (char*)&msg, sizeof(msg), 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen) >= 0)
		{

		}

		Sleep(500);
	}


	return 0;
}


int main(void)
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
	if (!thread) 
	{
		// Optionally do stuff, such as wait on the thread.
		printf("CreateThread() failed with error\n");
	}

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	
	// Set non-blocking socket
	u_long iMode = 1;

	int iResult = ioctlsocket(s, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
		printf("ioctlsocket failed with error: %ld\n", iResult);

	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	//start communication
	while (1)
	{
		char option;
		int id, cmd;
		printf("Choose implement (1-4)\n");
		option = getchar();
		id = atoi(&option);
		getchar();

		printf("Enter option : "
				"1 - Activate implement\n"
				"2 - Deactivate implement\n"
				"3 - Query Fuel\n"
				"4 - Query activation status\n");
		option = getchar();
		cmd = atoi(&option);
		enumImpModuleMsgId msgId;

		switch (cmd)
		{
		case 1:
			msgId = enumImpModuleMsgId::ACTIVATE_IMP;
			break;
		case 2:
			msgId = enumImpModuleMsgId::DEACTIVATE_IMP;
			break;
		case 3:
			msgId = enumImpModuleMsgId::QUERY_IMP_FUEL;
			break;
		case 4:
			msgId = enumImpModuleMsgId::QUERY_IMP_STATUS;
			break;
		default:
			printf("Invalid option!!!\n");
			continue;
		}

		ImpModuleMsg msg(msgId, enumImplement(id));

		//send the message
		if (sendto(s, (char*) & msg, sizeof(msg), 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a non blocking call
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen) > 0)
		{
			ImpModuleMsg* moduleMsg = (ImpModuleMsg*)buf;
			printf("Got message %d from=%d param = %d\n", moduleMsg->m_msgId, moduleMsg->m_id, moduleMsg->m_param);
		}

		
	}

	closesocket(s);
	WSACleanup();

	return 0;
}