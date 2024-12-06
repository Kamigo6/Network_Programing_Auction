#include <bits/stdc++.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "database/mysql_connector.h"
#include "enums.h"
#include "model.h"

#define MAXLINE 4096   // max text line length
#define SERV_PORT 3000 // port
#define LISTENQ 8      // maximum number of client connections

using namespace std;

int socketfd;
int child_process_running = 1;

void sig_chld(int signo);
void initServer();
void log_recv_msg(string client_ip, int client_port, string buf, UserRequest req);
void log_send_msg(int connfd, string client_ip, int client_port, char response[], ServerResponse res);
int _register(MySQLOperations *mysqlOps, string username, string password);
int login(MySQLOperations *mysqlOps, string username, string password, int &user_id);
int logout(MySQLOperations *mysqlOps, int user_id);
int createRoom(MySQLOperations *mysqlOps, int user_id, string room_name);
void viewRooms(int connfd, string client_ip, int client_port, MySQLOperations *mysqlOps);
void viewRoomsOwned(int connfd, string client_ip, int client_port, MySQLOperations *mysqlOps, int user_id);

int main(int argc, char **argv)
{
    int listenfd, connfd, n;
    pid_t childpid;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;
    initServer();
    for (;;)
    {
        clilen = sizeof(cliaddr);
        // accept a connection
        connfd = accept(socketfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0)
        {
            exit(1);
        }
        cout << "\n[+]" << inet_ntoa(cliaddr.sin_addr) << ":" << ntohs(cliaddr.sin_port) << " - Connection accepted. Received request..." << endl;

        if ((childpid = fork()) == 0) // if it’s 0, it’s child process
        {
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(cliaddr.sin_addr), client_ip, INET_ADDRSTRLEN); // convert ip number from binary to string
            int client_port = ntohs(cliaddr.sin_port);                           // network order to host short

            printf("\n[+]%s\n", "Child created for dealing with client requests");

            // close listening socket
            close(socketfd);

            // Initialize MySQL operations
            MySQLOperations mysqlOps;
            if (!mysqlOps.connect(ipAddress, username, password, database))
            {
                cout << "[-]Failed to connect to MySQL database\n"
                     << endl;
                exit(1);
            }
            else
                cout << "[+]Connected to Database!\n";

            while (child_process_running)
            {
                n = recv(connfd, buf, MAXLINE, 0);
                if (n < 0)
                {
                    perror("Read error");
                    exit(1);
                }
                if (n == 0)
                {
                    cout << "[+]" << client_ip << ":" << client_port << " - Disconnected" << endl;
                    exit(0);
                }

                int cmd = 0;

                for (int i = 0; i < strlen(buf); i++)
                {
                    char digit = buf[i];
                    if (digit == '\n')
                        break;
                    if (!(digit >= '0' && digit <= '9'))
                    {
                        cmd = -1;
                        break;
                    }
                    cmd = cmd * 10 + (int)(digit - '0');
                }

                log_recv_msg(client_ip, client_port, buf, static_cast<UserRequest>(cmd));

                switch (cmd)
                {
                case REGISTER_REQ:
                {
                    char username[20], password[20], response[50];
                    int noargs = sscanf(buf, "%d\n%s %s\n", &cmd, username, password);
                    if (noargs == 3)
                    {
                        int result = _register(&mysqlOps, username, password);
                        response[0] = '0' + result;
                        response[1] = '\0';
                        log_send_msg(connfd, client_ip, client_port, response, REGISTER_RES);
                    }
                    else
                    {
                        printf("[-]Invalid register protocol! %s\n", buf);
                        sprintf(response, "Invalid register protocol!\n");
                        log_send_msg(connfd, client_ip, client_port, response, REGISTER_RES);
                    }
                    break;
                }
                case LOGIN_REQ:
                {
                    char username[20], password[20], response[50];
                    int noargs = sscanf(buf, "%d\n%s %s\n", &cmd, username, password);
                    if (noargs == 3)
                    {
                        int user_id;
                        int result = login(&mysqlOps, username, password, user_id);
                        sprintf(response, "%d %d", result, user_id);
                        log_send_msg(connfd, client_ip, client_port, response, LOGIN_RES);
                    }
                    else
                    {
                        printf("[-]Invalid login protocol! %s\n", buf);
                        sprintf(response, "Invalid login protocol!\n");
                        log_send_msg(connfd, client_ip, client_port, response, LOGIN_RES);
                    }
                    break;
                }
                case LOGOUT_REQ:
                {
                    int user_id;
                    char response[50];
                    int noargs = sscanf(buf, "%d\n%d\n", &cmd, &user_id);
                    if (noargs == 2)
                    {
                        int result = logout(&mysqlOps, user_id);
                        response[0] = '0' + result;
                        response[1] = '\0';
                        log_send_msg(connfd, client_ip, client_port, response, LOGOUT_RES);
                    }
                    else
                    {
                        printf("[-]Invalid logout protocol! %s\n", buf);
                        sprintf(response, "Invalid logout protocol!\n");
                        log_send_msg(connfd, client_ip, client_port, response, LOGOUT_RES);
                    }
                    break;
                }
                case CREATE_ROOM_REQ:
                {
                    char room_name[255], response[50];
                    int user_id;
                    int noargs = sscanf(buf, "%d\n%d %s\n", &cmd, &user_id, room_name);
                    if (noargs == 3)
                    {
                        int result = createRoom(&mysqlOps, user_id, room_name);
                        response[0] = '0' + result;
                        response[1] = '\0';
                        log_send_msg(connfd, client_ip, client_port, response, CREATE_ROOM_RES);
                    }
                    else
                    {
                        printf("[-]Invalid create room protocol! %s\n", buf);
                        sprintf(response, "Invalid create room protocol!\n");
                        log_send_msg(connfd, client_ip, client_port, response, CREATE_ROOM_RES);
                    }
                    break;
                }
                case VIEW_ROOMS_REQ:
                {
                    char response[50];
                    int noargs = sscanf(buf, "%d\n", &cmd);
                    if (noargs == 1)
                    {
                        viewRooms(connfd, client_ip, client_port, &mysqlOps);
                    }
                    else
                    {
                        printf("[-]Invalid view rooms protocol! %s\n", buf);
                        sprintf(response, "Invalid view rooms protocol!\n");
                        log_send_msg(connfd, client_ip, client_port, response, VIEW_ROOMS_RES);
                    }
                    break;
                }
                case VIEW_ROOMS_OWNED_REQ:
                {
                    char response[50];
                    int user_id;
                    int noargs = sscanf(buf, "%d\n%d\n", &cmd, &user_id);
                    if (noargs == 2)
                    {
                        viewRoomsOwned(connfd, client_ip, client_port, &mysqlOps, user_id);
                    }
                    else
                    {
                        printf("[-]Invalid view rooms owned protocol! %s\n", buf);
                        sprintf(response, "Invalid view rooms owned protocol!\n");
                        log_send_msg(connfd, client_ip, client_port, response, VIEW_ROOMS_OWNED_RES);
                    }
                    break;
                }
                default:
                {
                    char response[50];
                    printf("[-]Invalid protocol: wrong command code\n\n");
                    sprintf(response, "Invalid  protocol: wrong command code\n");
                    log_send_msg(connfd, client_ip, client_port, response, static_cast<ServerResponse>(-1));
                }
                }

                int i;
                for (i = 0; i < MAXLINE; i++)
                    buf[i] = '\0';
            }
            mysqlOps.disconnect();
        }
    }
    signal(SIGCHLD, sig_chld); // signal sent when child process ends
    // close socket of the server
    close(connfd);
}

void log_recv_msg(string client_ip, int client_port, string buf, UserRequest req)
{
    cout << "[+]Received message from " << client_ip << ":" << client_port << "\n"
         << UserRequestToString(req) << "\n"
         << buf.substr(2) << endl;
}

// Send header and response to client
void log_send_msg(int connfd, string client_ip, int client_port, char response[], ServerResponse res)
{
    int n = send(connfd, response, MAXLINE, 0);
    if (n < 0)
    {
        perror("Send error");
        exit(1);
    }
    cout << "[+]Sent message to " << client_ip << ":" << client_port << "\n"
         << ServerResponseToString(res) << "\n"
         << response << '\n';
}

// Send only response to client without header
void log_send_msg(int connfd, string client_ip, int client_port, char response[])
{
    int n = send(connfd, response, MAXLINE, 0);
    if (n < 0)
    {
        perror("Send error");
        exit(1);
    }
    cout << "[+]Sent message to " << client_ip << ":" << client_port << "\n"
         << response << '\n';
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        cout << "[+]Child " << pid << " terminated with status " << stat << endl;
    return;
}

void initServer()
{
    struct sockaddr_in servaddr;
    // creation of the socket
    // If socketfd<0 there was an error in the creation of the socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        cout << "[-]Error in creating the socket" << endl;
        exit(1);
    }
    cout << "[+]Server Socket is created." << endl;

    // preparation of the socket address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    // bind the socket
    if (bind(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        cout << "[-]Error in binding." << endl;
        exit(1);
    }
    // listen to the socket by creating a connection queue, then wait for clients
    if (listen(socketfd, LISTENQ) == 0)
    {
        cout << "[+]Listening...." << endl;
    }
    else
    {
        cout << "[-]Error in binding." << endl;
    }
}

int _register(MySQLOperations *mysqlOps, string username, string password)
{
    // Check if the username already exists
    string checkSql = "SELECT * FROM user WHERE name = '" + username + "';";
    cout << "SQL query: " << checkSql << '\n';
    int checkRes = (*mysqlOps).checkRecord(checkSql);

    if (checkRes == 1)
    {
        cout << "[-]Username already exists." << endl;
        return 2;
        // Already existed
    }

    string sql = "INSERT INTO user(name, password) VALUES ('" + username + "','" + password + "');";
    bool res = (*mysqlOps).insertRecords(sql);
    cout << "SQL query: " << sql << '\n';
    if (res)
        return 1; // SUCCESS
    else
        return 0; // FAIL
}

int login(MySQLOperations *mysqlOps, string username, string password, int &user_id)
{
    // Check if the username already exists
    string checkSql = "SELECT * FROM user WHERE name = '" + username + "' AND loggin = 1;";
    cout << "SQL query: " << checkSql << '\n';
    int checkRes = mysqlOps->checkRecord(checkSql);

    if (checkRes == 1)
    {
        cout << "[-]User already logged in." << endl;
        return 2;
        // Already existed
    }

    string sql = "SELECT user_id FROM user WHERE name = '" + username + "' AND password = '" + password + "';";
    cout << "SQL query: " << sql << '\n';
    sql::ResultSet *res = mysqlOps->executeQuery(sql);
    if (res->next())
    {
        user_id = res->getInt("user_id");
        string updateSql = "UPDATE user SET loggin = 1 WHERE name = '" + username + "';";
        mysqlOps->insertRecords(updateSql);
        return 1; // SUCCESS
    }
    else
        return 0; // Wrong username or password
}

int logout(MySQLOperations *mysqlOps, int user_id)
{
    string sql = "SELECT * FROM user WHERE user_id = " + to_string(user_id) + " AND loggin = 1;";
    cout << "SQL query: " << sql << '\n';
    int res = (*mysqlOps).checkRecord(sql);
    if (res == SUCCESS)
    {
        string updateSql = "UPDATE user SET loggin = 0 WHERE user_id = " + to_string(user_id) + ";";
        (*mysqlOps).insertRecords(updateSql);
        return SUCCESS;
    }
    else
        return FAIL;
}

int createRoom(MySQLOperations *mysqlOps, int user_id, string room_name)
{
    // Check if the room already exists
    string checkSql = "SELECT * FROM room WHERE name = '" + room_name + "';";
    cout << "SQL query: " << checkSql << '\n';
    int checkRes = mysqlOps->checkRecord(checkSql);

    if (checkRes == 1)
    {
        cout << "[-]Room already existed." << endl;
        return 2;
        // Already existed
    }
    string sql = "INSERT INTO room(name, owner_id) VALUES ('" + room_name + "'," + to_string(user_id) + ");";
    bool res = mysqlOps->insertRecords(sql);
    cout << "SQL query: " << sql << '\n';
    if (res)
        return 1; // SUCCESS
    else
        return 0; // FAIL
}

void viewRooms(int connfd, string client_ip, int client_port, MySQLOperations *mysqlOps)
{
    string sql = "SELECT * FROM room;";
    cout << "SQL query: " << sql << '\n';

    struct RoomList roomList;
    initRoomList(&roomList);
    (*mysqlOps).getRoomList(&roomList, sql);

    char sendline[MAXLINE], response[10];
    char END[10] = "End";

    if (!roomList.size)
    {
        response[0] = '0' + FAIL;
        response[1] = '\0';
        log_send_msg(connfd, client_ip, client_port, response, VIEW_ROOMS_RES);
        return;
    }
    else
    {
        printf("[+]Begin send response...\n");
        response[0] = '0' + SUCCESS;
        response[1] = '\0';
        log_send_msg(connfd, client_ip, client_port, response, VIEW_ROOMS_RES);
        // sprintf(sendline, "Number of rooms matched: %d\n", roomList.size);
        // log_send_msg(connfd, client_ip, client_port, sendline);

        send(connfd, "================================================\n", MAXLINE, 0);
        sprintf(sendline, "%-10s | %-20s | %-10s |\n", "Room id", "Room name", "Owner id");
        log_send_msg(connfd, client_ip, client_port, sendline);
        memset(sendline, 0, strlen(sendline));
        send(connfd, "-------------------------------------------------\n", MAXLINE, 0);
        for (int i = 0; i < roomList.size; i++)
        {
            struct Room room = roomList.rooms[i];
            sprintf(sendline, "%-10d | %-20s | %-10d |\n", room.room_id, room.name, room.owner_id);
            log_send_msg(connfd, client_ip, client_port, sendline);
            memset(sendline, 0, MAXLINE);
        }
        send(connfd, "================================================\n", MAXLINE, 0);
        usleep(500000);
        sprintf(sendline, "%s", END);
        log_send_msg(connfd, client_ip, client_port, sendline);
        memset(sendline, 0, strlen(sendline));
        printf("[+]Send completely!\n");
    }
    freeRoomList(&roomList);
}

void viewRoomsOwned(int connfd, string client_ip, int client_port, MySQLOperations *mysqlOps, int user_id)
{
    string sql = "SELECT * FROM room WHERE owner_id = " + to_string(user_id) + ";";
    cout << "SQL query: " << sql << '\n';

    struct RoomList roomList;
    initRoomList(&roomList);
    mysqlOps->getRoomList(&roomList, sql);

    char sendline[MAXLINE], response[10];
    char END[10] = "End";

    if (!roomList.size)
    {
        response[0] = '0' + FAIL;
        response[1] = '\0';
        log_send_msg(connfd, client_ip, client_port, response, VIEW_ROOMS_OWNED_RES);
        return;
    }
    else
    {
        printf("[+]Begin send response...\n");
        response[0] = '0' + SUCCESS;
        response[1] = '\0';
        log_send_msg(connfd, client_ip, client_port, response, VIEW_ROOMS_OWNED_RES);

        send(connfd, "================================================\n", MAXLINE, 0);
        sprintf(sendline, "%-10s | %-20s | %-10s |\n", "Room id", "Room name", "Owner id");
        log_send_msg(connfd, client_ip, client_port, sendline);
        memset(sendline, 0, strlen(sendline));
        send(connfd, "-------------------------------------------------\n", MAXLINE, 0);
        for (int i = 0; i < roomList.size; i++)
        {
            struct Room room = roomList.rooms[i];
            sprintf(sendline, "%-10d | %-20s | %-10d |\n", room.room_id, room.name, room.owner_id);
            log_send_msg(connfd, client_ip, client_port, sendline);
            memset(sendline, 0, MAXLINE);
        }
        send(connfd, "================================================\n", MAXLINE, 0);
        usleep(500000);
        sprintf(sendline, "%s", END);
        log_send_msg(connfd, client_ip, client_port, sendline);
        memset(sendline, 0, strlen(sendline));
        printf("[+]Send completely!\n");
    }
    freeRoomList(&roomList);
}