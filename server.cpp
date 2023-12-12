// CODE FOR SERVER - ANDREW SMITH
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <csignal>
#include <cstdio>
#include <curl/curl.h>

const int PORT = 60000;  // Server listening port
const unsigned char SPECIAL_FLAG[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xFE, 0xFE, 0xFE, 0xFE}; // Special flag sent in front of URL

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string fetchWebpage(const std::string& url) {
    CURL* curl;
    CURLcode res;

    std::string webpageContent;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to write the received data to webpageContent
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &webpageContent);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return webpageContent;
}

void handleClient(int clientSocket) {
    // Receive command string from the client
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
        perror("Error receiving data");
        close(clientSocket);
        return;
    }

    // Check for the special flag
    if (memcmp(buffer, SPECIAL_FLAG, sizeof(SPECIAL_FLAG)) == 0) {
        // Extract URL from the command string
        size_t urlLength;
        memcpy(&urlLength, buffer + sizeof(SPECIAL_FLAG), sizeof(urlLength));
        std::string url(buffer + sizeof(SPECIAL_FLAG) + sizeof(urlLength), urlLength);

        // Fetch the webpage content
        std::string webpageContent = fetchWebpage(url);

        // Send the webpage content back to the client
        send(clientSocket, webpageContent.c_str(), webpageContent.size(), 0);
    } else {
        std::cerr << "Invalid command string" << std::endl;
    }

    // Close the client socket
    close(clientSocket);
}

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // Bind the socket to an IP address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error binding socket");
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        perror("Error listening");
        close(serverSocket);
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept connections and handle communication in separate threads
    while (true) {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);

        // Accept a connection
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            perror("Error accepting connection");
            continue;
        }

        std::cout << "Client connected" << std::endl;

        // Create a new thread for each client (or use a thread pool)
        std::thread(handleClient, clientSocket).detach();
    }

    // Close the server socket (this part is unreachable in the current code)
    close(serverSocket);

    return 0;
}

