/*
    C socket server example, handles multiple clients using threads
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread


const int  NumberOfLobbyPerServer = 20;
const char NumberOfPlayerPerLobby = 10;
const int  NameLenghtOfLobby = 10;
const int  NameLenghtOfPlayer = 10;


//the thread function
void *connection_handler(void *);



struct Player
{
        Player()
        {
                ID = 0;
                strncpy(Name, "Empty     ", sizeof(Name));
        }
        char Name[NameLenghtOfPlayer];
        char ID;
};

struct Lobby
{
        char Name[NameLenghtOfLobby];
        char ID;
        Player PlayerList[NumberOfPlayerPerLobby];
};
Lobby LobbyList[NumberOfLobbyPerServer];
int main(int argc , char *argv[])
{

    for (int i = 0; i < NumberOfLobbyPerServer; i++)
    {
        //      LobbyList[i].Name = (char *)"Empty";
                strncpy(LobbyList[i].Name, "Empty     ", sizeof(LobbyList[i].Nam                                                                                        e));
                LobbyList[i].ID = i + 1;
                for (char j = 0; j < NumberOfPlayerPerLobby; j++)
                {
                //      LobbyList[i].PlayerList[j].ID = 0;
            //  strncpy(LobbyList[i].PlayerList[j].Name, "NoPlayer  ", sizeof(Lo                                                                                        bbyList[i].PlayerList[j].Name));
                        LobbyList[i].PlayerList[j] = Player();

                }
        }


    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;


    //Create socket
    socket_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        //socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 4444 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (sockl                                                                                        en_t*)&c)) )
    {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = (int * )malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void                                                                                        *) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];

        char LobbyID = 0;
        char PlayerID = 0;

    //Send some messages to the client
    message = (char *)"Connected to Cloneserver!\n";
    write(sock , message , strlen(message));

    message = (char *)"What is your name?\n";
    write(sock , message , strlen(message));

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
                puts(client_message);
                if (strcmp(client_message, "Hey! Who is connected?") == 0)
                {
                        puts(client_message);
                        char LobbyMessage[NumberOfLobbyPerServer * (NameLenghtOf                                                                                        Lobby + (NumberOfPlayerPerLobby * (NameLenghtOfPlayer + 1)) + 1) + 1];
                        LobbyMessage[NumberOfLobbyPerServer * (NameLenghtOfLobby                                                                                         + (NumberOfPlayerPerLobby * (NameLenghtOfPlayer + 1)) + 1)] = '\0';
                        int Index = 0;
                        for (int i = 0; i < NumberOfLobbyPerServer; i++)
                {
                                LobbyMessage[Index++] = LobbyList[i].ID;
                                for (int j = 0; j < NameLenghtOfLobby; j++)
                                {
                                        if (LobbyList[i].Name[j] != '\0')
                                                LobbyMessage[Index++] = LobbyLis                                                                                        t[i].Name[j];
                                        else
                                                LobbyMessage[Index++] = ' ';
                                }
                        for (int k = 0; k < NumberOfPlayerPerLobby; k++)
                                {
                                        LobbyMessage[Index++] = LobbyList[i].Pla                                                                                        yerList[k].ID;
                                        for (int l = 0; l < NameLenghtOfPlayer;                                                                                         l++)
                                        {
                                                if (LobbyList[i].PlayerList[k].N                                                                                        ame[l] != '\0')
                                                        LobbyMessage[Index++] =                                                                                         LobbyList[i].PlayerList[k].Name[l];
                                                else
                                                        LobbyMessage[Index++] =                                                                                         ' ';
                                        }
                                }
                        }
        //      for (int i = 0; i < NumberOfLobbyPerServer * (NameLenghtOfLobby                                                                                         + (NumberOfPlayerPerLobby * (NameLenghtOfPlayer + 1))); i++)
         //       {
          //              if (LobbyMessage[i] == '\0')
           //                     LobbyMessage[i] = ' ';
            //    }
                puts(LobbyMessage);
            write(sock , LobbyMessage , NumberOfLobbyPerServer * (NameLenghtOfLo                                                                                        bby + (NumberOfPlayerPerLobby * (NameLenghtOfPlayer + 1))) + 1);
//              send(sock, LobbyMessage, NumberOfLobbyPerServer * (NameLenghtOfL                                                                                        obby + (NumberOfPlayerPerLobby * (NameLenghtOfPlayer + 1))) + 1, 0);
        }
        else
                if(strstr(client_message, "I'm ") && strstr(client_message, " an                                                                                        d I'd join lobby "))
        {
            char* LobbyName = strstr(client_message, " and I'd join lobby ");
                        LobbyID = LobbyName[20];
                        //puts(LobbyID);
            //strncpy(Name, , sizeof(Name));
                        //puts(PlayerID);
                }
    }
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

        if (PlayerID != 0)
        {
                LobbyList[LobbyID].PlayerList[PlayerID - 1] = Player();
        }
    //Free the socket pointer
    free(socket_desc);

    return 0;
}
