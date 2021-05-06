#include "common_functions.h"

void send_message(SOCKET connectorSocket) {

    int bitesSent = -1;

    do {
        printf(" - ");
        std::string strBuffer = " ";
        std::getline(std::cin, strBuffer);
        strBuffer.push_back('\n');
        const char* buffer = strBuffer.c_str();

        bitesSent = send(connectorSocket, buffer, strlen(buffer), 0);

        if (bitesSent == SOCKET_ERROR) {
            printf("Error sending buffer, code: %d \n", WSAGetLastError());
        }


    } while (true);

}

void recv_message(SOCKET clientSocket) {
    int bitesSent;
    int inspecResult;

    // do{
    do {
        char receiverBuffer[100] = " ";
        inspecResult = recv(clientSocket, receiverBuffer, 100, 0);

        printf(" + %s", receiverBuffer);


    } while (inspecResult > 0);
    //printf(" closing connect sv");
    //} while (true);

}

bool isNumber(const std::string& strToCheck) {
    int charAmount = strToCheck.length();

    if (charAmount < 1) return false;

    for (int currentChar = 0; currentChar < charAmount; currentChar++) {
        if (!isdigit(strToCheck[currentChar])) {
            return false;
        }
    }

    return true;
}

