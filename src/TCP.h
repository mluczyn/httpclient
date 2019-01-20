#pragma once
#ifndef TCP_SOC_H
#define TCP_SOC_H
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<Windows.h>
#include<string>
#include<cstdio>
#pragma comment (lib, "Ws2_32.lib")
 

struct addrinfo defaultHints;

struct serverCom
{
public:
	SOCKET comSocket;
	struct addrinfo* addrProperties = NULL;
	std::string id;
	serverCom(std::string name) : id{ name }
	{}
	int setup(std::string addr, std::string port)
	{
		ZeroMemory(&defaultHints, sizeof(defaultHints));
		ZeroMemory(&addrProperties, sizeof(addrProperties));
		defaultHints.ai_family = AF_UNSPEC;
		defaultHints.ai_socktype = SOCK_STREAM;
		defaultHints.ai_protocol = IPPROTO_TCP;

		if (getaddrinfo(addr.c_str(), port.c_str(), &defaultHints, &addrProperties) != 0)
		{
			printf("%s:Invalid Host: %s/%s\n", id.c_str(), addr.c_str(), port.c_str());
			return 1;
		}

		comSocket = socket(addrProperties->ai_family, addrProperties->ai_socktype, addrProperties->ai_protocol);
		if (comSocket == INVALID_SOCKET)
		{
			printf("%s:Invalid Socket: %d %d %d\n", id.c_str(), &addrProperties->ai_family, &addrProperties->ai_socktype, &addrProperties->ai_protocol);
			return 2;
		}
		else return 0;
	}

	int openSoc()
	{
		int result = connect(comSocket, addrProperties->ai_addr, (int)addrProperties->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			closesocket(comSocket);
			printf("%s:Error Opening Connection\n", id.c_str());
			return 1;
		}
		return 0;
	}
	int sendData(void* data, size_t lenght)
	{
		//if (openSoc())
		//	return 1;
		int result = send(comSocket, (const char*)data, lenght, 0);
		if (result == SOCKET_ERROR)
		{
			closesocket(comSocket);
			printf("%s:Error Sending Data with Error: %d\n", id.c_str(), WSAGetLastError());
			return 2;
		}
		result = shutdown(comSocket, SD_SEND);
		if (result == SOCKET_ERROR)
		{
			closesocket(comSocket);
			printf("%s:Connection Shutdown Failed with Error: %d\n", id.c_str(), WSAGetLastError());
			return 3;
		}

		return 0;
	}
	int recvData(void* &data, size_t lenght)
	{
		//if (openSoc())
		//	return 1;
		int result = recv(comSocket, (char*)data, lenght, 0);
		if (result == 0)
			printf("%s:No Data Recieved\n", id.c_str());
		if (result < 0)
		{
			printf("&s:Data Recieve Failed with Error: %d\n", id.c_str(), WSAGetLastError());
			closesocket(comSocket);
			return 2;
		}

		return 0;
	}
	int requestFile(std::string request, std::string &data)
	{
		//if (openSoc())
		//	return 1;
		int result = send(comSocket, request.c_str(), request.length(), 0);
		if (result == SOCKET_ERROR)
		{
			closesocket(comSocket);
			printf("%s:Error Sending Request with Error: %d\n", id.c_str(), WSAGetLastError());
			return 2;
		}
		result = shutdown(comSocket, SD_SEND);
		if (result == SOCKET_ERROR)
		{
			closesocket(comSocket);
			printf("%s:Connection Shutdown Failed with Error: %d\n", id.c_str(), WSAGetLastError());
			return 3;
		}

		char temp[9999];
		memset((void*)temp, (int)'\0', 9999);

		result = recv(comSocket, temp, 9999, 0);
		data = std::string(temp);
		if (result < 0)
			printf("%s:Recieve Failed with Error: %d\n", id.c_str(), WSAGetLastError());



		return 0;
	}
	void closeSoc() { closesocket(comSocket); }
	~serverCom()
	{
		closesocket(comSocket);
		freeaddrinfo(addrProperties);
	}

};
#endif;