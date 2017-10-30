#include "Socket.h"
#include <iostream>
#include "Blowfish.h"
#pragma comment (lib, "Ws2_32.lib")



struct sockaddr_in myaddr;
struct hostent *host = NULL;



SOCKET s;
struct addrinfo *result = NULL,
	*ptr = NULL,
	hints;



char e[513];
char recvbuf[513];
char d[513];
void initalizeSocket()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	s = socket(hints.ai_family, hints.ai_socktype,
		hints.ai_protocol);
}


CBlowFish oBlowFish((unsigned char*)"xdlmao1231231231", 16);


void encrypt(const char * decrypted, char * encrypted)
{
	char szDataIn[513];
	char szDataOut[513];
	memset(szDataIn, 0, 513);
	memset(szDataOut, 0, 513);

	strcpy(szDataIn, decrypted);
	memset(szDataOut, 0, 513);
	oBlowFish.Encrypt((unsigned char*)szDataIn,
		(unsigned char*)szDataOut, 64, CBlowFish::ECB);
	memcpy(encrypted, szDataOut, sizeof(szDataOut));

}
void decrypt(char * encrypted, char * decrypted)
{
	char szDataIn[513];
	char szDataOut[513];
	memset(szDataIn, 0, 513);
	memset(szDataOut, 0, 513);
	oBlowFish.ResetChain();
	strcpy(szDataIn, encrypted);
	memset(szDataOut, 0, 513);
	oBlowFish.Decrypt((unsigned char*)szDataIn,
		(unsigned char*)szDataOut,64, CBlowFish::ECB);

	memcpy(decrypted, szDataOut,strlen(szDataOut));

}

int numOfErrors;

int modifiedLen(char* packet)
{
	int testNum = strlen(packet);
recheck:
	__try
	{
		if (packet[testNum + 1] != 0)
		{
			testNum++;
			goto recheck;
		}
		else
		{
			return testNum;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return testNum;
	}
}



void sendReceivePacket(char* packet, char* addr, void * out) {
	int iResult = getaddrinfo(addr, "9999", &hints, &result);
	int length = modifiedLen(packet);
	
	s = socket(hints.ai_family, hints.ai_socktype,
		hints.ai_protocol);
	iResult = connect(s, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == -1)
	{

		numOfErrors++;
		if (numOfErrors == 3){
		Sleep(2000);
		exit(0);
		}
	}
	ZeroMemory(e, sizeof(e));
	encrypt(packet, e);
	iResult = send(s, e, 512, 0);
	if (iResult == -1)
	{

		numOfErrors++;
		if (numOfErrors == 3) {
			Sleep(2000);
			exit(0);
		}
	}
	ZeroMemory(recvbuf, 512);
	ZeroMemory(d, sizeof(d));
	iResult = recv(s, recvbuf, 512, 0);
	if (iResult == -1)
	{
		numOfErrors++;
		if (numOfErrors == 3) {
			Sleep(2000);
			exit(0);
		}
	}
	closesocket(s);
	decrypt(recvbuf, d);
	memcpy(out, d, strlen(d));
	
}


//(c) Apflmus