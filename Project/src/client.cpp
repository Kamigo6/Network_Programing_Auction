#include <bits/stdc++.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "enums.h"

#define MAXLINE 4096   // max text line length
#define SERV_PORT 3000 // port

using namespace std;

int socketfd, loggedIn;

void connectToServer(char *ip);
void displayMenu();
void displayUserMenu(string *username);
int login(string *username);
void _register();
void displayReceiveMessage(int *socketfd);
void searchMovie();
void browseMovie();
void bookTicket();
void reserve(int showtimeId, int movieId);
bool isInteger(const string &s);
bool isValidTicketFormat(string tickets);
void logout();

int main(int argc, char **argv)
{
    loggedIn = 0;
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <server_address>\n", argv[0]);
        exit(1);
    }

    connectToServer(argv[1]);

    displayMenu();
    return 0;
}

void connectToServer(char *ip)
{
    struct sockaddr_in servaddr;
    // Create a socket for the client
    // If socketfd<0 there was an error in the creation of the socket
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("[-]Problem in creating the socket");
        exit(2);
    }
    cout << "[+]Client Socket is created." << endl;
    // Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(SERV_PORT); // convert to big-endian order
    // Connection of the client to the socket
    if (connect(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("[-]Problem in connecting to the server");
        exit(3);
    }
    cout << "[+]Connected to Server." << endl;
}

void displayMenu()
{
    string username;
    while (1)
    {
        if (!loggedIn)
        {
            printf("\n====================================\n");
            printf("Welcome to Online Auction Application\n");
            printf("1. Register\n");
            printf("2. Login\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");

            string choice;
            cin >> choice;

            switch (choice[0])
            {
            case '1':
                _register();
                break;

            case '2':
                loggedIn = login(&username);
                break;

            case '3':
                printf("***Application Ended***\n");
                exit(0);
                break;
            default:
                printf("Invalid choice. Try again.\n");
                break;
            }
            printf("-----------------------------\n");
        }
        else
        {
            // display menu coresponsing to each user mode
            switch (loggedIn)
            {
            default:
                displayUserMenu(&username);
            }
            printf("-----------------------------\n");
        }
    }
}

void displayUserMenu(string *username)
{
    cout << "Account: " << *username << "\n ============Main Menu============\n";
    printf("1. View rooms\n");
    printf("2. View items\n");
    printf("3. View log\n");
    printf("4. Logout\n");
    printf("Enter your choice: ");

    string choice;
    cin >> choice;

    switch (choice[0])
    {
    case '1':
        break;
    case '2':
        break;
    case '3':
        break;
    case '4':
        logout();
        break;
    default:
        printf("Invalid choice. Try again.\n");
        break;
    }
}

void _register()
{
    char username[30], password[30];
    char sendline[MAXLINE], recvline[MAXLINE];

    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    // store values in sendline
    sprintf(sendline, "%d\n%s %s\n", REGISTER_REQ, username, password);
    // send request to server with protocol: "REGISTER\n<username> <password>\n"
    send(socketfd, sendline, strlen(sendline), 0);

    recv(socketfd, recvline, MAXLINE, 0);
    int status = recvline[0] - '0';
    if (status == SUCCESS)
    {
        printf("Register successfully!\nNow you can login with this new account.\n");
    }
    else if (status == FAIL)
    {
        printf("This username already existed!!!\n");
    }
    else
    {
        perror(recvline);
        exit(4);
    }
}

int login(string *user)
{
    char username[30], password[30];
    // string username, password;
    char sendline[MAXLINE], recvline[MAXLINE];

    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    // store values in sendline
    sprintf(sendline, "%d\n%s %s\n", LOGIN_REQ, username, password);
    // send request to server with protocol: "LOGIN\n<username> <password>\n"
    send(socketfd, sendline, strlen(sendline), 0);

    recv(socketfd, recvline, MAXLINE, 0);
    int status = recvline[0] - '0';
    if (status == SUCCESS)
    {
        printf("You have logged in successfully!\n");
        *user = username;
    }
    else if (status == FAIL)
    {
        printf("Wrong username or password!!!\n");
    }
    else
    {
        perror(recvline);
        exit(4);
    }
    return status;
}

void logout()
{
    loggedIn = 0;
    printf("Logged out successfully.\n\n");
}

void displayReceiveMessage(int *socketfd)
{
    char recvline[MAXLINE];
    int n;
    while ((n = recv(*socketfd, recvline, MAXLINE, 0)) > 0)
    {
        if (strcmp(recvline, "End"))
        {
            printf("%s", recvline);
            memset(recvline, 0, sizeof(recvline));
        }
        else
        {
            memset(recvline, 0, sizeof(recvline));
            break;
        }
    }
}