#include <stdio.h>
#include <winsock2.h>  
#include <ws2tcpip.h>  
#pragma comment(lib, "ws2_32.lib")  
void func(SOCKET sockfd);

int main()
{
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in servaddr;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

       sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }
    else
    {
        printf("Socket is created\n");

        // Configure server address
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(43451);
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        // Connect to the server
        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        {
            printf("Connection failed\n");
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
        else
        {
            printf("Connected to server\n");
            func(sockfd);  // Call function to interact with server
        }
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

void func(SOCKET sockfd)
{
    int number, factorial;

    printf("Enter a number to calculate its factorial: ");
    scanf("%d", &number);

    // Send the number to the server
    send(sockfd, (char*)&number, sizeof(number), 0);

    // Receive the factorial result from the server
    recv(sockfd, (char*)&factorial, sizeof(factorial), 0);

    printf("The factorial of %d is: %d\n", number, factorial);
}
