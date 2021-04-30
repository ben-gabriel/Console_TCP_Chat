#include <iostream>
#include <string>
#include <thread>
#include "winsock2.h"
#include "ws2tcpip.h"

#pragma comment(lib, "Ws2_32.lib")

int run_server_version();
int run_client_version();

void send_message(SOCKET connectorSocket);
void recv_message(SOCKET clientSocket);

bool isNumber(const std::string& strToCheck);

// \ -> (alt+92) self-remainder

int main() {
    //-------------------------------------------------------------------------------------------------------------------------------------//
    //                                              TCP Client-Server app Based off:                                                       //
    //                            docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock                              //
    //                                                *With some minor adjustments*                                                         //
    //-------------------------------------------------------------------------------------------------------------------------------------//

//_--------------------------------------------------------------------_
    std::string strOption;
    int reset = 0;
    do {
        printf("Do you want to run as a client(1) or as a server(2)\n");
        std::cin >> strOption;
        
        if(strOption == "1"){
            run_client_version();
        }else if (strOption == "2"){
            run_server_version(); 
        }else{
            reset = 1;
        }
    } while (reset == 1);
//_--------------------------------------------------------------------_

}

//-----------------------------------------------------------------------------------------------------------------------------------------//


int run_server_version() {
    std::cout << " \n \nWinSock Server-Practice \n \n";

    WSADATA wsaData;

    int inspecResult;

    inspecResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // Starts the use of Ws2_32.lib 
    if (inspecResult != 0) {
        printf("Windows Socket API Startup failed with error: %d \n", WSAGetLastError());
    }
    else {
        printf("Windows Socket API Startup Successful. \n");
    }

    struct addrinfo* result = NULL, * addrInfoHolder = NULL, * addrInfoHolder2 = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE; // Indicates the returned address structure is to be used in bind() 

  //- Host Name -
    char hostname[1024];
    hostname[1023] = '\0';
    inspecResult = gethostname(hostname, 1023); //Gets the local hostname and Stores it in a char array.
    if (inspecResult == 0) {
        printf("Hostname: %s\n", hostname);
    }
    else {
        printf("ERROR.................. %d  \n", WSAGetLastError());
    }

    inspecResult = getaddrinfo(hostname, "27500", &hints, &result);

    struct sockaddr_in* addr_in = (struct sockaddr_in*)result->ai_addr;
    char addrCh[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(addr_in->sin_addr), addrCh, INET_ADDRSTRLEN);

    printf("Send this IP to the Client-Side: %s \n\n", addrCh);

  //- Host Name -

    inspecResult = getaddrinfo(NULL, "27500", &hints, &result); // Sets the values for the server socket in &result
    if (inspecResult != 0) {
        printf("Get Address Info failed with error: %d \n", WSAGetLastError());
    }
    else {
        printf("Get Adrress Info Call Successful \n");
    }

    SOCKET listenerSocket = INVALID_SOCKET; // Creates a pointer to a empty (invalid) socket.
    listenerSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // Fills the soket with pertinent data.

    if (listenerSocket != INVALID_SOCKET) {
        printf("Listener Socket Created Successfully. \n");
    }
    else {
        printf("Unable to create Listener Socket, error code: %d \n", WSAGetLastError());
    }

    inspecResult = bind(listenerSocket, result->ai_addr, (int)result->ai_addrlen); // Binds the listenerSocket to the ip/port given.

    if (inspecResult != 0) {
        printf("Socket binding failed, error code: %d \n", WSAGetLastError());
    }
    else {
        printf("Socket binding successful. \n\n\n");
    }
    
    freeaddrinfo(result); // Once the socket has been bound, &result has no longer use, and can be freed.
    // Once bound, listenerSocket is ready to start listening for incoming client connect()'s


    inspecResult = listen(listenerSocket, SOMAXCONN); // Sets ListenerSocket in listening state, with a MAX CONNection number set by the OS
    if (inspecResult != 0) {
        printf("Listening Failed with Code Error: %d \n", WSAGetLastError() );
    }
    else {
        printf("Socket Listening For Connections... \n");
    }

    SOCKET clientSocket = INVALID_SOCKET; // Creates a temporary socket to hold the connection with the client in.
    clientSocket = accept(listenerSocket,NULL,NULL); // Accepts and returns a socket in which the actual connection is handled. 

    if (clientSocket == INVALID_SOCKET) {
        printf("Error accepting connection, code: %d \n", WSAGetLastError());
    }
    else {
        printf("Connections Established. \n\n\n");
    }

    //experiment
    
        std::thread first (recv_message,clientSocket);
        std::thread second (send_message,clientSocket);
        
        first.join();
        second.join();

    char cmdHolder;
    std::cin >> cmdHolder;

    return 0;
}


//-----------------------------------------------------------------------------------------------------------------------------------------//

int run_client_version(){
    std::cout << " \n \nWinSock Client-Practice \n \n";

    int inspecResult; // Variable to check for errors in the return value of initialization of data structures and objects.

    WSADATA winSockApiData; // "data structure to receive details of the Windows Sockets implementation" on WSAStartup()
    inspecResult = WSAStartup(MAKEWORD(2, 2), &winSockApiData); // Initializes the use of the Ws2_32.lib 

    if (inspecResult == 0) {
        // Checks the initialization result.
        printf("WinSock API Startup called successfully\n");
    }
    else {
        printf("WinSsock API Startup Failed with error code: %d\n", inspecResult);
    }
    struct addrinfo* result = NULL; // data structure to hold a list of data structures from the address intended to connect to.
    struct addrinfo* addrInfoHolder = NULL; // data structure to act as a placeholder for logical comparisons.
    struct addrinfo* addrInfoHolder2 = NULL;
    struct addrinfo hints; // data structure to hold the data hints of the type of sockets the caller supports. 

    ZeroMemory(&hints, sizeof(hints)); // Initializes in 0 the memory of &hints, necessary for getaddrinfo() to work correctly 
    hints.ai_family = AF_INET; // Address Family to hint as supported by the caller. AF_INET = IPv4.
    hints.ai_socktype = SOCK_STREAM; // Socket Type to hint as supported by the caller. (sock_stream = TCP in this example)
    hints.ai_protocol = IPPROTO_TCP; // Protocol to hint as supported by the caller in the connection. TCP.

  //-Host Name-
    char hostname[1024];
    hostname[1023] = '\0';
    inspecResult = gethostname(hostname, 1023); //Gets the local hostname and Stores it in a char array.
    if (inspecResult == 0) {
        printf("Hostname: %s\n", hostname);
    }
    else {
        printf("ERR0R.................. %d  \n", WSAGetLastError());
    }

    inspecResult = getaddrinfo(hostname, "27500", &hints, &result);

    struct sockaddr_in* addr_in = (struct sockaddr_in*)result->ai_addr; //Cast ai_addr: 'sockaddr' type to 'sockaddr_in' type
                                                                       //So we can turn binary address in direct (char) address
    char addrCh[INET_ADDRSTRLEN]; //Char array of lenght INET_ADDRSTRLEN to hold the literal IP Address

    inet_ntop(AF_INET, &(addr_in->sin_addr), addrCh, INET_ADDRSTRLEN);

    printf("IP: %s \n\n\n", addrCh);
  //- Host Name -

    int tryAddress = 1;
    SOCKET connectorSocket = INVALID_SOCKET; // Creates a socket to connect to a server, 'invalid_socket' initializes it empty.
    do {
        char  charAddress[50];
        std::cout << "Enter the Server-Side IP Address:  \n ";
        std::cin>>charAddress;

        PCSTR nodeName, port = "27500"; //Variables of type Pointer Constant STRing defined in winnt.h.
        nodeName = charAddress;

        inspecResult = getaddrinfo(nodeName, port, &hints, &result); // Will get the address info of nodeName:port based on what the &hints says 
                                                                    // that the caller support, and store it as linked list in &result.
                                                                   // (A given nodeName can have multiple addrinfo structures).
        if (inspecResult == 0) {
            // Checks the result of getaddrinfo().
            printf("\n\nGet Address Info called successfully \n");
        }
        else {
            printf("Get Address Info failed with error: %d \n", inspecResult);
            WSACleanup(); // Terminates the socket and closes Ws2_32.dll freeing the resources.
        }

        addrInfoHolder = result; // Sets the address info holder equal to the info in the 'result' from getaddrinfo().
        addrInfoHolder2 = result;

        
        while (addrInfoHolder != NULL) {

            connectorSocket = socket(addrInfoHolder->ai_family, addrInfoHolder->ai_socktype, addrInfoHolder->ai_protocol);
            if (connectorSocket != INVALID_SOCKET) {
                printf("Socket Created successfully. \n");
            }
            else {
                printf("Invalid Socket. Error code: %ld \n...Trying next address. \n", WSAGetLastError());
            }

            while (addrInfoHolder2 != NULL) {
                inspecResult = connect(connectorSocket, addrInfoHolder->ai_addr, addrInfoHolder->ai_addrlen);
                if (inspecResult != SOCKET_ERROR) {
                    printf("Socket Connection successful. \n\n");
                    break;
                }
                else {
                    printf("Connection Unsuccessful. Error code: %ld ...Trying next address. \n\n", WSAGetLastError());
                    addrInfoHolder2 = addrInfoHolder2->ai_next;
                }

            }
            //addrInfoHolder2 = result;

            if (addrInfoHolder2 == NULL) {
                addrInfoHolder = addrInfoHolder->ai_next; // If reached, the end of the list will return null.
            }
            else {
                break;
            }

        }// Sets the connectorSocket info equal to the first compatible address result from getaddrinfo()

        if (addrInfoHolder == NULL) {
            printf("Unable to conenct. Try a new IP. \n");
            tryAddress = 1;
        }
        else {
            tryAddress = 0;
        }

    } while (tryAddress == 1);

    if (addrInfoHolder == NULL) {
        printf("Connection Error, unable to create and connect Socket. Freeing resources. \n");
        freeaddrinfo(result);
        WSACleanup();

        char cdmHolder;
        std::cin >> cdmHolder;
        return 1;
    }

    // 
        //experimenting
    
        std::thread third(recv_message, connectorSocket);
        std::thread fourth(send_message, connectorSocket);
    
        third.join();
        fourth.join();
    //


    char cdmHolder;
    std::cin >> cdmHolder;
    return 0;
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
