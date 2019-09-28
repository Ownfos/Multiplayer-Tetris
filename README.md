This project implements server and client for 2 player tetris.
I used custom DirectX11 and WinSock2 wrapping library(directx11 playground, winsock playground) which are stored under "library" folder.

Note that server's ip address is hard-coded as "127.0.0.1". That's because I didn't have extra computer to use as server...
If you run the tetris server on fixed server, you'll have to change the ip address located on System::System().
