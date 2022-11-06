# IA_hw1

## Getting Started
* After compiling, there will be two executables: server and client.
* Run the server by doing: `./server [port]` or just `./server`.
    * The default port is 9999
* Run the client by doing: `./client  <username> <host> <port>`
    * Exmample: `./client user localhost 9999\n`
    * Note that multiple clients can be exsisted at the same time.
* The client is mostly the same except for the existence of a static 
    global variable (sorry).
* The client's socket fd to the server is global, 
    so the client's interrupt handler can send a message to the server
    telling it that the client has disconnected.

## References
1. https://github.com/eugeneli/Multithreaded-Chat-in-C