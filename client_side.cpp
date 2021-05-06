#include "common_functions.h"
#include "client_side.h"


int run_client_version() {
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
        std::cin >> charAddress;

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