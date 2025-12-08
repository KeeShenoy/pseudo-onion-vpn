// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "onion_builder.h"

#define PORT_NODE1 8001

int main() {
    char message[1024];

    printf("Enter message to send: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;

    size_t onion_len;
    uint8_t *onion = build_onion(message, &onion_len);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NODE1);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Connection error");
        free(onion);
        return 1;
    }

    send(sock, onion, onion_len, 0);
    printf("Message sent through onion network!\n");

    free(onion);
    close(sock);

    return 0;
}
