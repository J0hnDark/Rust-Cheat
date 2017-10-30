#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
void initalizeSocket();
void sendReceivePacket(char* packet, char* addr, void * out);
void encrypt(const char * decrypted, char * encrypted);
void decrypt(char * encrypted, char * decrypted);
//(c) Apflmus