// CODE FOR CLIENT - ANDREW SMITH
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

const int SERVER_PORT = 60000;  // Server's listening port
const int CLIENT_PORT = 60001;  // Client's source port
const unsigned char SPECIAL_FLAG[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xFE, 0xFE, 0xFE, 0xFE}; // Special flag sent before URL

// Check for correct # of arguments
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <server_ip>" << std::endl;
        return -1;
    }

    // Set server address to entered argument
    const char *serverIP = argv[1];

    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // Set up client address
    sockaddr_in clientAddress{};
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(CLIENT_PORT);  // Client's source port
    inet_pton(AF_INET, "127.0.0.1", &clientAddress.sin_addr); // This can be any valid local IP address

    // Set up server address
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, serverIP, &serverAddress.sin_addr);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error connecting to server");
        close(clientSocket);
        return -1;
    }

    // Assemble the command string
    const char* url = "https://www.wikipedia.org/"; // Website to be retrieved
    char commandString[1024];
    memcpy(commandString, SPECIAL_FLAG, sizeof(SPECIAL_FLAG));
    size_t urlLength = strlen(url);
    memcpy(commandString + sizeof(SPECIAL_FLAG), &urlLength, sizeof(urlLength));
    memcpy(commandString + sizeof(SPECIAL_FLAG) + sizeof(urlLength), url, urlLength);

    // Send the command string to the server
    send(clientSocket, commandString, sizeof(SPECIAL_FLAG) + sizeof(urlLength) + urlLength, 0);

    // Receive and print/save the server's response (webpage content)
    char buffer[4096];  // Adjust buffer size based on expected webpage content size
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
        perror("Error receiving data");
    } else {
        // Print the received webpage content to the console
        std::cout << "Received webpage content:\n" << buffer << std::endl;

        // Save the received webpage content to a local file
        std::ofstream outputFile("received_webpage.html", std::ios::out | std::ios::binary);
        outputFile.write(buffer, bytesRead);
        outputFile.close();
        std::cout << "Webpage content saved to 'received_webpage.html'" << std::endl;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
