#include <stdio.h>
#include <winsock2.h> 
#include <ws2tcpip.h> 

#pragma comment(lib, "ws2_32.lib") 
void func(int confd);
int factorial(int n);

int main()
{
    WSADATA wsaData;
    SOCKET sockfd, confd;
    struct sockaddr_in servaddr, cliaddr;
    int len;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Create socket
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

        // Bind the socket
        if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        {
            printf("Bind failed\n");
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
        else
        {
            printf("Bind successful\n");

            // Listen for connections
            if (listen(sockfd, 5) == SOCKET_ERROR)
            {
                printf("Listen failed\n");
                closesocket(sockfd);
                WSACleanup();
                return 1;
            }
            else
            {
                printf("Listening for connections...\n");
                len = sizeof(cliaddr);

                // Accept a client connection
                confd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
                if (confd == INVALID_SOCKET)
                {
                    printf("Accept failed\n");
                    closesocket(sockfd);
                    WSACleanup();
                    return 1;
                }
                else
                {
                    printf("Client connected\n");
                    func(confd);  // Handle client
                }
            }
        }
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

void func(int confd)
{
    int number, result;

    // Read the number sent by the client
    recv(confd, (char*)&number, sizeof(number), 0);

    // Calculate factorial
    result = factorial(number);

    // Send the result back to the client
    send(confd, (char*)&result, sizeof(result), 0);
}

int factorial(int n)
{
    if (n == 0 || n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}
