// CODE FOR CLIENT - ANDREW SMITH
#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 60000;
const unsigned char SPECIAL_FLAG[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xFE, 0xFE, 0xFE, 0xFE};

void printAndSaveWebpage(const char* webpageContent, size_t contentSize) {
    // Print the webpage content
    std::cout << "Received webpage content:\n" << webpageContent << std::endl;

    // Save the webpage content to a local file
    std::ofstream outputFile("webpage_content.html", std::ios::binary);
    outputFile.write(webpageContent, contentSize);
    outputFile.close();

    std::cout << "Webpage content saved to 'webpage_content.html'" << std::endl;
}

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // Set up server address
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error connecting to server");
        close(clientSocket);
        return -1;
    }

    // Assemble the command string
    const char* url = "https://www2.brockport.edu/";
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
        // Call the function to print and save the webpage content
        printAndSaveWebpage(buffer, static_cast<size_t>(bytesRead));
    }

    // Close the socket
    close(clientSocket);

    return 0;
}

