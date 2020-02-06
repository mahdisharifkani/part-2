#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "time.h"
#include "ctype.h"
#define MAX 10000
#define PORT 12345
#define SA struct sockaddr
struct karbar
{
    char username[21];
    char password[21];
    char authToken[21];
    char channel[21];
    int lastMessage;
};

char buffer[MAX];
struct karbar users[100];
int n = 0;

void openServer();
void connecttoClinet(int client_socket);
void regs();
void logins();
void logouts();
void makechannel();
void joinch();
void refres();
void leaveChannel();
void sendmes();
void chMembers();
void searchMembers ();
void searchMessages ();


int main ()
{
    while (true)
        openServer();
        printf("Hi \n what can I do for you?\n I'm listening\n")
}
void openServer ()
{
    int server_socket, client_socket;
    struct sockaddr_in server, client;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        printf("I'm sorry 🐀򙐀🐀򙐀 \n" );
        return;
    }

    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        printf("I'm sorry 🐀򙐀🐀򙐀 \n" );
        return;
    }

    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        printf("I'm sorry 🐀򙐀🐀򙐀 \n" );
        return;
    }

    // Accept the data packet from client and verify
    int len = sizeof(client);
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        printf("I'm sorry 🐀򙐀🐀򙐀 \n" );
        return;
    }

    // Function for chatting between client and server
    connecttoClinet(client_socket);

    // Close the socket
    closesocket(server_socket);
}

//ye tabe mikham ke be client vasl she va payame client ro bekhoone
void connecttoClinet(int client_socket)
{
        memset(buffer, 0, sizeof(buffer));

        // Read the message from client and copy it to buffer
        recv(client_socket, buffer, sizeof(buffer), 0);
        // Call function
        puts (buffer);
        if (!memcmp (buffer, "register", 8))
            regs ();
        else if (!memcmp (buffer, "login", 5))
            logins ();
        else if (!memcmp (buffer, "create channel", 14))
            makechannel ();
        else if (!memcmp (buffer, "join channel", 12))
            joinch ();
        else if (!memcmp (buffer, "send", 4))
            sendmes ();
        else if (!memcmp (buffer, "refresh", 7))
            refres ();
        else if (!memcmp (buffer, "channel members", 15))
            chMembers ();
        else if (!memcmp (buffer, "leave", 5))
            leaveChannel ();
        else if (!memcmp (buffer, "logout", 6))
            logouts ();
        else if (!memcmp (buffer, "search member", 13))
            searchMembers ();
        else if (!memcmp (buffer, "search message", 14))
            searchMessages ();
        puts (buffer);
        // Send the buffer to client
        send(client_socket, buffer, sizeof(buffer), 0);
    return;
}
void regs ()
{
    char username[40], password[40], command[10];
    sscanf (buffer, "%s %s %s", command, username, password);
    memset (buffer, 0, sizeof(buffer));
    username [strlen(username) - 1] = 0;
    //file address
    char address[200] = "Resources/Users/";
    strcat (address, username);
    strcat (address, ".user.json");
    FILE *fp;
    //check it is used or no
    fp = fopen (address, "r");
    if (fp != NULL)
    {
        strcpy (buffer, "{\"type\":\"Error\",\"content\":\"The username has been used\"}");
        return;
    }
    fclose (fp);
    //write in file
    fp = fopen(address, "w");
    strcpy (buffer, "{\"password\":\"");
    strcat (buffer, password);
    strcat (buffer, "\"}");
    fputs (buffer, fp);
    fclose (fp);
    //answer to client
    strcpy (buffer, "{\"type\":\"Successful\",\"content\":\"\"}");
    return;
}

void logins ()
{
    char command[6], username[40], password[40];
    sscanf (buffer, "%s %s %s", command, username, password);
    memset (buffer, 0, sizeof(buffer));
    username [strlen(username) - 1] = 0;
    //file address
    for (int i = 0; i < n; i++)
    {
        if (!strcmp (username, users[i].username))
        {
            strcpy (buffer, "{\"type\":\"Error\",\"content\":\"The user has already logged in\"}");
            return;
        }
    }
    char address[200] = "Resources/Users/";
    strcat (address, username);
    strcat (address, ".user.json");
    FILE *fp;
    fp = fopen (address, "r");
    //check it is available or no
    if (fp == NULL)
    {
        strcpy (buffer, "{\"type\":\"Error\",\"content\":\"The username has not been found\"}");
        return;
    }
    //password check
    fscanf(fp, "%[^EOF]s", buffer);
    char pass [21] = {0};
    int tool = strlen (buffer) - 2;
    for (int c = 13; c < tool; c++)
        pass [c - 13] = buffer [c];
    if (strcmp (pass, password))
    {
        strcpy (buffer, "{\"type\":\"Error\",\"content\":\"The password is wrong\"}");
        fclose(fp);
        return;
    }
    fclose (fp);
    //making authentication token
    srand(time(NULL));
    int i = 0;
    while (i < 20)
    {
        int kind = rand() % 3;
        if (kind == 0)
            users[n].authToken[i] = rand() % 10 + '0';
        else if (kind == 1)
            users[n].authToken[i] = rand() % 26 + 'a';
        else
            users[n].authToken[i] = rand() % 26 + 'A';
        i++;
    }
    //add to online users
    strcpy (users[n].username, username);
    strcpy (users[n].password, password);
    //answer to client
    strcpy (buffer, "{\"type\":\"AuthToken\",\"content\":\"");
    strcat (buffer, users[n].authToken);
    strcat (buffer, "\"}");
    n++;
    return;
}

void makechannel ()
{
    char channel[40], auth[21];
    int len = strlen (buffer);
    for (int c = len - 21; c < len - 1; c++)
        auth[c - len + 21] = buffer [c];
    auth[20] = 0;
    int c = 15;
    for (; c < len - 23; c++)
        channel [c - 15] = buffer [c];
    channel [c - 15] = 0;
    memset (buffer, 0, sizeof(buffer));
    //check the token
    int i = 0;
    for (; i < n; i++)
    {
        if (!strcmp(auth, users[i].authToken))
            break;
    }
    //file address
    char address[200] = "Resources/Channels/";
    strcat (address, channel);
    strcat (address, ".channel.json");
    FILE *fp;
    fp = fopen(address, "r");
    //check duplication
    if (fp != NULL)
    {
        strcpy (buffer, "{\"type\":\"Error\",\"content\":\"This name is not available\"}");
        return;
    }
    fclose (fp);
    //make channel file
    fp = fopen (address, "w");
    char start[21] = {0};
    strcpy (buffer, "{\"messages\":[{\"sender\":\"server\",\"content\":\"");
    strcat (buffer, users[i].username);
    strcat (buffer, " created the channel\"}]}");
    fputs(buffer, fp);
    fclose (fp);
    //answer to client
    strcpy (buffer, "{\"type\":\"Successful\",\"content\":\"\"}");
    //online user data
    strcpy (users[i].channel, channel);
    users[i].lastMessage = 0;
    return;
}

void joinch ()
{
    char channel[40], auth[21];
    int len = strlen (buffer);
    for (int c = len - 21; c < len - 1; c++)
        auth[c - len + 21] = buffer [c];
    auth [20] = 0;
    int c = 13;
    for (; c < len - 23; c++)
        channel [c - 13] = buffer [c];
    channel [c - 13] = 0;
    memset (buffer, 0, sizeof(buffer));
    //checking token
    int i = 0;
    for (; i < n; i++)
    {
        if (!strcmp(auth, users[i].authToken))
            break;
    }
    //file address
    char address[200] = "Resources/Channels/";
    strcat (address, channel);
    strcat (address, ".channel.json");
    FILE *fp;
    fp = fopen(address, "r");
    //check it is available or no
    if (fp == NULL)
    {
        strcpy (buffer, "{\"type\":\"Error\",\"content\":\"The channel is not found\"}");
        return;
    }

    fscanf (fp, "%[^EOF]s", buffer);
    int tool = strlen (buffer);
    buffer [tool - 1] = 0;
    buffer [tool - 2] = 0;
    fclose (fp);
    //sending join message
    fp = fopen (address, "w");

    char join[21] = {0};
    strcpy (join, users[i].username);
    strcat (join, " joined the channel");

    strcat (buffer, ",{\"sender\":\"server\",\"content\":\"");
    strcat (buffer, join);
    strcat (buffer, "\"}]}");
    fputs(buffer, fp);

    fclose (fp);
    //answer to client
    strcpy (buffer, "{\"type\":\"Successful\",\"content\":\"\"}");
    //updating data
    strcpy (users[i].channel, channel);
    users[i].lastMessage = 0;

    return;
}

void sendmes ()
{
    char auth[21], payam[80];
    int len = strlen (buffer);
    for (int c = len - 21; c < len - 1; c++)
        auth[c - len + 21] = buffer [c];
    auth [20] = 0;
    int c = 5;
    for (; c < len - 23; c++)
        payam [c - 5] = buffer [c];
    payam [c - 5] = 0;
    memset (buffer, 0, sizeof(buffer));
    //checking token
    int i = 0;
    for (; i < n; i++)
    {
        if (!strcmp(auth, users[i].authToken))
            break;
    }
    //file address
    char address[200] = "Resources/Channels/";
    strcat (address, users[i].channel);
    strcat (address, ".channel.json");
    FILE *fp;
    fp = fopen(address, "r");
    fscanf (fp, "%[^EOF]s", buffer);
    int tool = strlen (buffer);
    buffer [tool - 1] = 0;
    buffer [tool - 2] = 0;
    fclose (fp);
    //sending join message
    fp = fopen (address, "w");

    strcat (buffer, ",{\"sender\":\"");
    strcat (buffer, users[i].username);
    strcat (buffer, "\",\"content\":\"");
    strcat (buffer, payam);
    strcat (buffer, "\"}]}");
    fputs(buffer, fp);

    fclose (fp);
    //answer to client
    strcpy (buffer, "{\"type\":\"Successful\",\"content\":\"\"}");

    return;
}

void refres ()
{
    char auth[21];
    int len = strlen (buffer);
    for (int c = 8; c < 28; c++)
        auth[c - 8] = buffer [c];
    auth [20] = 0;
    memset (buffer, 0, sizeof(buffer));
    //Token checking
    int i = 0;
    for (; i < n; i++)
    {
        if (!strcmp(auth, users[i].authToken))
            break;
    }
    //file address
    char address[200] = "Resources/Channels/";
    strcat (address, users[i].channel);
    strcat (address, ".channel.json");
    FILE *fp;
    fp = fopen(address, "r");
    char matn [10000] = {0};
    fscanf (fp, "%[^EOF]s", matn);
    int tool = strlen (matn);
    for (int c = 13; c < tool; c++)
        matn [c - 13] = matn [c];
    matn [tool - 13] = 0;
    tool -= 13;
    fclose (fp);
    int messageCounter = 0;
    int cursor = tool - 2;
    //answer to client
    strcpy (buffer, "{\"type\":\"List\",\"content\":[");
    for (int c = 0; c < tool; c++)
    {
        if (matn [c] == '{')
            messageCounter ++;
    }
    int temp = messageCounter;
    messageCounter -= users[i].lastMessage;
    users[i].lastMessage = temp;
    for (int c = tool; c >= 0 && messageCounter > 0; c--)
    {
        if (matn [c] == '{')
        {
            cursor = c;
            messageCounter--;
        }
    }
    tool -= cursor;
    for (int c = 0; c < tool; c++)
        matn [c] = matn [c + cursor];
    matn [tool] = 0;
    strcat (buffer, matn);
    return;
}

void chMembers ()
{
    char auth[21];
    for (int c = 0; c < 20; c++)
        auth [c] = buffer [c + 16];
    auth [20] = 0;
    memset (buffer, 0, sizeof(buffer));
    //Token check
    int i = 0;
    for (; i < n; i++)
    {
        if (!strcmp(auth, users[i].authToken))
            break;
    }
    //finding channel members
    strcpy (buffer, "{\"type\":\"List\",\"content\":[");
    for (int j = 0; j < n; j++)
    {
        if (!strcmp(users[j].channel, users[i].channel))
        {
            strcat (buffer, "\"");
            strcat (buffer, users[j].username);
            strcat (buffer, "\",");
        }
    }
    int tool = strlen (buffer);
    buffer [tool - 1] = ']';
    buffer [tool] = '}';
    buffer [tool + 1] = 0;
    return;
}

void leaveChannel ()
{
    char auth[21];
    for (int c = 0; c < 20; c++)
        auth [c] = buffer [c + 6];
    auth [20] = 0;
    memset (buffer, 0, sizeof(buffer));
    //token check
    int i = 0;
    for (; i < n; i++)
    {
        if (!strcmp(auth, users[i].authToken))
            break;
    }
    //file address
    char address[200] = "Resources/Channels/";
    strcat (address, users[i].channel);
    strcat (address, ".channel.json");
    FILE *fp;
    fp = fopen(address, "r");
    fscanf (fp, "%[^EOF]s", buffer);
    int tool = strlen (buffer);
    buffer [tool - 1] = 0;
    buffer [tool - 2] = 0;
    fclose (fp);
    //sending join message
    fp = fopen (address, "w");

    char left[21] = {0};
    strcpy (left, users[i].username);
    strcat (left, " left the channel");

    strcat (buffer, ",{\"sender\":\"server\",\"content\":\"");
    strcat (buffer, left);
    strcat (buffer, "\"}]}");
    fputs(buffer, fp);

    fclose (fp);
    //answer to client
    strcpy (buffer, "{\"type\":\"Successful\",\"content\":\"\"}");
    //updating data
    strcpy (users[i].channel, "");

    return;
}

void logouts ()
{
    char auth[21];
    for (int c = 0; c < 20; c++)
        auth [c] = buffer [c + 7];
    memset (buffer, 0, sizeof(buffer));
    //token check
    int i = 0;
    for (; i < n; i++)
    {
        if (!strcmp(auth, users[i].authToken))
            break;
    }
    //remove from online users
    n--;
    for (; i < n; i++)
    {
        strcpy (users[i].username, users[i + 1].username);
        strcpy (users[i].password, users[i + 1].password);
        users[i].lastMessage = users[i + 1].lastMessage;
        strcpy (users[i].channel, users[i + 1].channel);
        strcpy (users[i].authToken, users[i + 1].authToken);
    }

    strcpy (users[n].username, "");
    strcpy (users[n].password, "");
    users[n].lastMessage = 0;
    strcpy (users[n].channel, "");
    strcpy (users[n].authToken, "");
    //answer to client
    strcpy (buffer, "{\"type\":\"Successful\",\"content\":\"\"}");
    return;
}

void searchMembers ()
{
    char auth[21], username[100];
    int len = strlen (buffer);
    for (int c = len - 20; c < len; c++)
        auth[c - len + 20] = buffer [c];
    auth [20] = 0;
    int c = 14;
    for (; c < len - 22; c++)
        username [c - 14] = buffer [c];
    username [c - 14] = 0;
    puts (username);
    //Token check
    int i = 0;
    for (; i < n; i++)
    {
        if (!strcmp(auth, users[i].authToken))
            break;
    }
    memset (buffer, 0, sizeof (buffer));
    //finding channel members
    int flag = 0;
    for (int j = 0; j < n; j++)
    {
        if (!strcmp(users[j].channel, users[i].channel) && !strcmp (users[j].username, username))
        {
            flag = 1;
            break;
        }
    }
    if (flag)
        strcpy (buffer, "{\"type\":\"Successful\",\"content\":\"\"}");
    else
        strcpy (buffer, "{\"type\":\"Error\",\"content\":\"That user is not a member of this channel\"}");
    return;
}

void searchMessages ()
{
    char auth[21], payam[950] = {0};
    int len = strlen (buffer);
    for (int c = len - 20; c < len; c++)
        auth[c - len + 20] = buffer [c];
    auth [20] = 0;
    for (int c = 15; c < len - 22; c++)
        payam [c - 15] = buffer [c];
    //Token check
    int i = 0;
    for (; i < n; i++)
    {
        if (!strcmp(auth, users[i].authToken))
            break;
    }
    memset (buffer, 0, sizeof (buffer));
    //file address
    char address[200] = "Resources/Channels/";
    strcat (address, users[i].channel);
    strcat (address, ".channel.json");
    FILE *fp;
    fp = fopen(address, "r");
    char matn [10000] = {0};
    fscanf (fp, "%[^EOF]s", matn);
    int tool = strlen (matn);
    for (int c = 13; c < tool; c++)
        matn [c - 13] = matn [c];
    matn [tool - 13] = 0;
    tool -= 13;
    fclose (fp);
    int cursor = tool - 2;
    int counter = 0;
    int toolePayam = strlen (payam);
    strcpy (buffer, "{\"type\":\"List\",\"content\":[");
    for (int c = 0; c < tool; c++)
    {
        if (!strncmp ("\",\"content\":\"", matn + c - 13, 13))
        {
            if (!strncmp (payam, matn + c, toolePayam) && (isspace (*(matn + c + toolePayam)) || *(matn + c + toolePayam) == '\"'))
            {
                cursor = c - 1;
                for (; matn [cursor] != '{' && cursor; cursor--);
                for (; matn [c] != '}' && c < tool; c++);
                c++;
                if (counter)
                    strcat (buffer, ",");
                strncat (buffer, matn + cursor, c - cursor);
                counter++;
            }
            else
            {
                for (; matn [c] != '\"' && c < tool; c++)
                {
                    printf ("%d\n", c);
                    if (isspace(matn [c - 1]) && !strncmp (payam, matn + c, toolePayam) && (isspace (*(matn + c + toolePayam)) || *(matn + c + toolePayam) == '\"'))
                    {
                        cursor = c - 1;
                        puts (matn + c);
                        for (; matn [cursor] != '{' && cursor; cursor--);
                        for (; matn [c] != '}' && c < tool; c++);
                        c++;
                        if (counter)
                            strcat (buffer, ",");
                        strncat (buffer, matn + cursor, c - cursor);
                        counter++;
                    }
                }
            }
        }
    }
    strcat (buffer, "]}");
    return;
}
