#pragma once
#include "winsock2.h"
#include "ws2tcpip.h"
#include <string>
#include <iostream>
#include <thread>

void send_message(SOCKET connectorSocket);

void recv_message(SOCKET clientSocket);

bool isNumber(const std::string& strToCheck);