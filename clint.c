#include <stdio.h>
#include <stdlib.h>
#include "winsock2.h"
#include "string.h"

int openSocket (void);
void accountMenu (void);
void registeration (void);
void login (void);
void mainMenu (char * authToken);
void createChannel (char * authToken);
void joinChannel (char * authToken);
void logout (char * authToken);
void chatMenu (char * authToken);
void message (char * authToken);
void refresh (char * authToken);
void members (char * authToken);
void leave (char * authToken);
void req (char *request, int size);
void searchMember (char * authToken);
void searchMessage (char * authToken);

int smile()
{
 char s[] = { 0xf0, 0x9f, 0x98, 0x8e, 0};

 printf("%s\n", s);
}
int main()
{
    printf("Hi\nWell come to my first project\nEnjoy it\n");

    accountMenu();
    return 0;
}

int openSocket (void)
{
    int client_socket, server_socket;
	struct sockaddr_in servaddr, cli;
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;
	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return -1;
    }
	// Create and verify socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		return -1;
	}
	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(12345);
    // Connect the client socket to server socket
	if (connect(client_socket, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n Sorry");
		return -1;
	}
    return client_socket;
}

void accountMenu(void)
{
    printf("At the first you might register \nAnd if you have an account please log in\n");
    printf ("Account menu:\n1: Register\n2: Log in\n");
    int option;
    scanf ("%d", &option);
    if (option == 1)
        registeration ();
    else if (option == 2)
        login ();
    else
    {
        puts ("Please enter a number between 1 and 2");
        accountMenu();
    }
    return;
}
void registeration (void)
{
    char user[200], pass[65];
    printf ("Register:\nEnter your username\n");
    scanf ("%s", user);
    printf ("Enter your password\n");
    scanf ("%s", pass);
    char request [300] = "register ";
    strcat (request, user);
    strcat (request, ", ");
    strcat (request, pass);
    strcat (request, "\n");
    req (request, 300);
    int tool = strlen (request) - 2;
    if (request[9] == 'E')
    {
        for (int i = 27; i < tool; i++)
            printf ("%c", request [i]);
        printf ("\n");
        registeration();
    }
    else
    {
        puts ("Your registeration is successfully\nWell come");
        login ();
    }
    return;
}

void login (void)
{
    char user[200], pass[65];
    printf ("Log in:\nEnter your username\n");
    scanf ("%s", user);
    printf ("Enter your password\n");
    scanf ("%s", pass);
    char request[300] = "login ";
    strcat (request, user);
    strcat (request, ", ");
    strcat (request, pass);
    strcat (request, "\n");
    req (request, 300);
    int tool = strlen (request) - 2;
    if (request[9] == 'E')
    {
        for (int i = 27; i < tool; i++)
            printf ("%c", request [i]);
        printf ("\n");
        login ();
    }
    else
    {
        puts ("You logged in successfully");
        char authToken[21] = {0};
        for (int i = 31; i < tool; i++)
            authToken [i - 31] = request [i];
        mainMenu (authToken);
    }
    return;
}

void mainMenu (char * authToken)
{
    printf ("Main menu:\n1: Create channel\n2: Join channel\n3: Log out\n");
    int option;
    scanf ("%d", &option);
    if (option == 1)
        createChannel (authToken);
    else if (option == 2)
        joinChannel (authToken);
    else if (option == 3)
        logout (authToken);
    else
    {
        puts ("Please enter a valid number");
        mainMenu (authToken);
    }
    return;
}

void logout (char * authToken)
{
    char request[50] = "logout ";
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 50);
    puts ("good bye \nSee you soon\n");
    return;
}

void createChannel (char * authToken)
{
    char request[160] = "create channel ";
    char channelName[100];
    puts ("What is your channels name:\n");
    scanf ("%s", channelName);
    strcat (request, channelName);
    strcat (request, ", ");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 160);
    int tool = strlen (request) - 2;
    if (request[9] == 'E')
    {
        for (int i = 27; i < tool; i++)
            printf ("%c", request [i]);
        printf ("\n");
        createChannel (authToken);
    }
    else
    {
        puts ("You created the channel successfully");
        chatMenu (authToken);
    }
    return;
}
void joinChannel (char * authToken)
{
    char request[160] = "join channel ";
    char channelName[100];
    puts ("Please enter name of channel");
    scanf ("%s", channelName);
    strcat (request, channelName);
    strcat (request, ", ");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 160);
    int tool = strlen (request) - 2;
    if (request[9] == 'E')
    {
        for (int i = 27; i < tool; i++)
            printf ("%c", request [i]);
        printf ("\n");
        joinChannel (authToken);
    }
    else
    {
        puts ("You joined the channel \n Wellcome\n");
        chatMenu (authToken);
    }
    return;
}

void chatMenu (char * authToken)
{
    printf ("Chat menu:\n1: Send messsage\n2: Refresh\n3: Channel members\n4: Search in channel members\n5: Search in channel messages\n6: Leave channel\n");
    int option;
    scanf ("%d", &option);
    if (option == 1)
        message (authToken);
    else if (option == 2)
        refresh (authToken);
    else if (option == 3)
        members (authToken);
    else if (option == 4)
        searchMember (authToken);
    else if (option == 5)
        searchMessage (authToken);
    else if (option == 6)
        leave (authToken);
    else if (option == 7)
        logout (authToken);
    else
    {
        puts ("come on 1 or 2 or 3 or 4 or 5 or 6 or 7");
        chatMenu (authToken);
    }
    return;
}

void message (char * authToken)
{
    char request[300] = "send ";
    puts ("Talk with me");
    char payam[250] = {0}, buffer;
    scanf("%c%[^\n]s", &buffer, payam);
    strcat (request, payam);
    strcat (request, ", ");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 300);
    chatMenu (authToken);
    return;
}

void refresh (char * authToken)
{
    char request[10000] = "refresh ";
    puts ("New messages are:");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 10000);
    int tool = strlen (request) - 4;
    int i = 28;
    while (1)
    {
        if (!memcmp ("sender", request + i, 6))
        {
            i += 9;
            while (memcmp (request + i, "\",\"content\":\"", 13))
            {
                printf ("%c", request[i]);
                i++;
            }
            printf (": ");
            i += 3;
        }
        if (!memcmp ("content", request + i, 7))
        {
            i += 10;
            while (memcmp (request + i, "\"},{\"sender\":\"", 14) && memcmp (request + i, "\"}]}\0", 5))
            {
                printf ("%c", request[i]);
                i++;
            }
            printf ("\n");
            i += 5;
        }
        if (*(request + i) == 0)
            break;
    }
    chatMenu(authToken);
    return;
}

void members (char * authToken)
{
    char request[10000] = "channel members ";
    puts ("Channel members are:");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 10000);
    int tool = strlen (request) - 2;
    for (int i = 25; i < tool; i++)
    {
        if (request [i] == '\"')
        {
            i++;
            while (memcmp (request + i, "\",\"", 3) && memcmp (request + i, "\"]}\0", 4))
            {
                printf ("%c", request[i]);
                i++;
            }
            printf ("\n");
        }
    }
    chatMenu(authToken);
    return;
}

void leave (char * authToken)
{
    char request[300] = "leave ";
    puts ("good bay");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 50);
    mainMenu (authToken);
    return;
}

void req (char *request, int size)
{
    int client_socket = openSocket();
    if (client_socket == -1)
        return;
    send(client_socket, request, size, 0);
    shutdown(client_socket, 1);
    memset(request, 0, size);
    recv(client_socket, request, size, 0);
    shutdown (client_socket, 0);
    closesocket (client_socket);
    return;
}

void searchMember (char * authToken)
{
    char request [150] = "search member ";
    char input [100] = {0};
    scanf ("%s", input);
    strcat (request, input);
    strcat (request, ", ");
    strcat (request, authToken);
    req (request, 150);
    int tool = strlen (request) - 2;
    if (request[9] == 'E')
    {
        for (int i = 27; i < tool; i++)
            printf ("%c", request [i]);
        printf ("\n");
    }
    else
        puts ("That user is a member of this channel");
    chatMenu (authToken);
    return;
}

void searchMessage (char * authToken)
{
    char request [1000] = "search message ";
    char input [950] = {0};
    scanf ("%s", input);
    strcat (request, input);
    strcat (request, ", ");
    strcat (request, authToken);
    req (request, 1000);
    int tool = strlen (request) - 4;
    int i = 28;
    if (!strcmp (request, "{\"type\":\"List\",\"content\":[]}"))
        puts ("Not found sorry");
    while (1)
    {
        if (!memcmp ("sender", request + i, 6))
        {
            i += 9;
            while (memcmp (request + i, "\",\"content\":\"", 13))
            {
                printf ("%c", request[i]);
                i++;
            }
            printf (": ");
            i += 3;
        }
        if (!memcmp ("content", request + i, 7))
        {
            i += 10;
            while (memcmp (request + i, "\"},{\"sender\":\"", 14) && memcmp (request + i, "\"}]}\0", 5))
            {
                printf ("%c", request[i]);
                i++;
            }
            printf ("\n");
            i += 5;
        }
        if (*(request + i) == 0)
            break;
    }
    chatMenu(authToken);
    return;
}
