#include <iostream>
#include <SDL.h>
#include <SDL_net.h>

int main(int argc, char* argv[]) {
    // Initialize SDL_net
    if (SDL_Init(0) == -1 || SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        return -1;
    }

    IPaddress ip;
    TCPsocket client;

    // Resolve server IP and port
    if (SDLNet_ResolveHost(&ip, "192.168.0.10", 12345) == -1) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        return -1;
    }

    // Open a connection to the server
    client = SDLNet_TCP_Open(&ip);
    if (!client) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return -1;
    }

    // Send a message to the server
    const char* message = "Hello from client!";
    if (SDLNet_TCP_Send(client, message, strlen(message) + 1) < strlen(message) + 1) {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
        SDLNet_TCP_Close(client);
        return -1;
    }

    // Wait for response
    char response[512];
    int result = SDLNet_TCP_Recv(client, response, 512);
    if (result > 0) {
        response[result] = '\0';
        std::cout << "Received response: " << response << std::endl;
    }
    else {
        std::cerr << "SDLNet_TCP_Recv: " << SDLNet_GetError() << std::endl;
    }

    // Close connection and quit SDL_net
    SDLNet_TCP_Close(client);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}
