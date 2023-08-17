#include "../include/webserv.hpp"

//const char* SERVER_HOST = "www.example.com";
//const char* SERVER_PATH = "/";
//const int SERVER_PORT = 80;
//
//int main() {
//    int sockfd;
//    struct sockaddr_in serv_addr;
//    struct hostent *server;
//    const char* request_template =
//            "GET %s HTTP/1.0\r\n"
//            "Host: %s\r\n"
//            "User-Agent: SimpleHTTPClient\r\n"
//            "Connection: close\r\n"
//            "\r\n";
//
//    // Create a socket
//    sockfd = socket(AF_INET, SOCK_STREAM, 0);
//    if (sockfd < 0) {
//        perror("ERROR opening socket");
//        return 1;
//    }
//
//    // Get server IP address
//    server = gethostbyname(SERVER_HOST);
//    if (server == NULL) {
//        perror("ERROR getting host by name");
//        return 1;
//    }
//
//    // Prepare the server address structure
//    memset(&serv_addr, 0, sizeof(serv_addr));
//    serv_addr.sin_family = AF_INET;
//    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
//    serv_addr.sin_port = htons(SERVER_PORT);
//
//    // Connect to the server
//    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//        perror("ERROR connecting");
//        return 1;
//    }
//
//    // Create the HTTP request
//    char request[1024];
//    snprintf(request, sizeof(request), request_template, SERVER_PATH, SERVER_HOST);
//
//    // Send the request
//    if (send(sockfd, request, strlen(request), 0) < 0) {
//        perror("ERROR sending request");
//        return 1;
//    }
//
//    // Receive and print the response
//    char buffer[1024];
//    int bytes_received;
//    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
//        buffer[bytes_received] = '\0';
//        std::cout << buffer;
//    }
//    if (bytes_received < 0) {
//        perror("ERROR receiving response");
//        return 1;
//    }
//
//    // Close the socket
//    close(sockfd);
//
//    return 0;
//}


const int SERVER_PORT = 8080;

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[1024];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return 1;
    }

    // Prepare the server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        return 1;
    }

    // Start listening for incoming connections
    listen(sockfd, 5);

    while (true) {
        clilen = sizeof(cli_addr);

        // Accept a new connection
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;
        }

        // Read the request from the client
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("ERROR reading from socket");
            close(newsockfd);
            continue;
        }

        // Send a simple HTTP response
        const char* response = "HTTP/1.0 200 OK\r\n"
                               "Content-Type: text/html\r\n"
                               "\r\n"
                               "<html><body><h1>WRONG! Start again!</h1></body></html>";
        send(newsockfd, response, strlen(response), 0);

        // Close the client socket
        close(newsockfd);
    }

    // Close the server socket
    close(sockfd);

    return 0;
}
