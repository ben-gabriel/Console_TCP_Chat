# Console_TCP_Chat
 
 A low level TCP/IP chat app made using WINSOCK API
-----
How to use:
-----
The program will ask to choose between being a client or a server.
First start a server side instance, the server side instance must have the port indicated in the code open (Forwarded).
Then, the client instance must enter the public ip of the machine running the server (or the local ip if it's running in a local network).

-----
Things to know:
-----
* The server instance will tell you to give the local ip to the client instance, this is only valid if running on local network.
The idea was to get the public ip showing there (Work in progress)
* I tried using threads without being too familiar with them yet, so the implementation is a little rough around the edges to say the least.
* All the functions are in one big file because the idea was to make a quick project, which the ngot out of hand, I will update and clean them in the future.

-----
About the project:
-----
A while back I took on the challenge of building a TCP/IP Chat because I wanted to practice C++, and also wanted to learn a little bit about networking.
After some research I found the Windows Socket API (or Winsock), which handles the sockets management at the OS level.

-----
The main things I learned with this project:
-----
* Ports
* Berkeley sockets
* Windows APIs
* Networking Protocols
* Practiced c++ 
