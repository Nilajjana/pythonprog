#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int sockfd, length;
    char buf[100], buf1[100];
    struct sockaddr_in sa;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_port = htons(6025);

    length = sizeof(sa);

    while (1) {
        printf("Enter a message (type 'exit' to quit): ");
        fgets(buf, sizeof(buf), stdin);

        // Remove newline from fgets
        buf[strcspn(buf, "\n")] = '\0';

        if (strcmp(buf, "exit") == 0) break;

        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&sa, sizeof(sa));

        int k = recvfrom(sockfd, buf1, sizeof(buf1) - 1, 0, (struct sockaddr *)&sa, &length);
        buf1[k] = '\0';
        printf("Server: %s\n", buf1);
    }

    close(sockfd);
    return 0;
}
