#include "common_functions.h"
#include "server_side.h"



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
        printf("Listening Failed with Code Error: %d \n", WSAGetLastError());
    }
    else {
        printf("Socket Listening For Connections... \n");
    }

    SOCKET clientSocket = INVALID_SOCKET; // Creates a temporary socket to hold the connection with the client in.
    clientSocket = accept(listenerSocket, NULL, NULL); // Accepts and returns a socket in which the actual connection is handled. 

    if (clientSocket == INVALID_SOCKET) {
        printf("Error accepting connection, code: %d \n", WSAGetLastError());
    }
    else {
        printf("Connections Established. \n\n\n");
    }

    //experiment

    std::thread first(recv_message, clientSocket);
    std::thread second(send_message, clientSocket);

    first.join();
    second.join();

    char cmdHolder;
    std::cin >> cmdHolder;

    return 0;
}
