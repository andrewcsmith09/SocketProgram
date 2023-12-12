# Webpage Fetcher - Client-Server Project

## Overview

This project consists of a simple client-server architecture for fetching webpage content. 
The client sends a command to the server, which fetches the content of a specified webpage using libcurl and sends it back to the client.
This project was created for a Network & Internet Applications class at SUNY Brockport. Hello, Dr. Yu!

## Network Environment & Testing

This project was tested successfully in 2 different network environments (Screenshots of the client command line are in this repository):  
- The first test was performed while running the server as localhost. The loopback address (127.0.0.1) was used to communicate with the server. Both the client and server were running on the same machine for the first test.
- The second test was performed over wireless LAN using the private IP address (192.168.1.174) of the machine running the server. Both the client and server were running on different machines but connected to the same network during the second test.

## Prerequisites

- C++ compiler
- libcurl library
- CMake (for building)

## Getting Started

1. Clone the repository:

   ```bash
   git clone https://github.com/andrewcsmith09/socketprogram.git
   ```

2. Build the project:

   ```bash
   cd socketprogram
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Run the server

   ```bash
   ./server
   ```

4. Run the client (Use 127.0.0.1 for localhost or IP of server machine for LAN)

   ```bash
   ./client <server_ip>
   ```

## Usage

- The client sends a command to the server with a specified URL.  
- The server fetches the webpage content using libcurl.  
- The server sends the webpage content back to the client.  
- The client prints and saves the webpage content.

## Optional Functionality

- The server handles multiple clients concurrently using multi-threading.


## Project Structure

- client.cpp: Implementation of the client program.  
- server.cpp: Implementation of the server program.  
- CMakeLists.txt: CMake configuration file for building the project.  

## Dependencies

- C++11  
- libcurl (Make sure it's installed on your system)

## Contributing

- Contributions are welcome! Feel free to open issues or submit pull requests.

## License

- This project is licensed under the MIT License.



