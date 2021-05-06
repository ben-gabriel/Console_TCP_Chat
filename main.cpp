#pragma comment(lib, "Ws2_32.lib") //Tells VS To link to the WS2_32 library
#include "common_functions.h"
#include "client_side.h"
#include"server_side.h"

//-------------------------------------------------------------------------------------------------------------------------------------//
//                                              TCP Client-Server app Based off:                                                       //
//                            docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock                              //
//                                                *With some minor adjustments*                                                        //
//-------------------------------------------------------------------------------------------------------------------------------------//

int main() {
    
    // Main menu
    std::string strOption;
    int reset = 0;
    
    do {
        printf("Do you want to run as a client(1) or as a server(2)\n");
        printf("Please enter a numeric value \n");
        std::cin >> strOption;
        
        if(strOption == "1"){
            run_client_version();
        }else if (strOption == "2"){
            run_server_version(); 
        }else{
            reset = 1;
        }
    } while (reset == 1);

}
