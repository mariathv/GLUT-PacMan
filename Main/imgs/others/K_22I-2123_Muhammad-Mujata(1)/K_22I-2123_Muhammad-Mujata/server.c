#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define SHM_SIZE 1024

typedef struct
{
    char message[SHM_SIZE];
    int client_id;
    int reciever;
    int command;
    int gNum;
    char groupName[20];
} shared_data;

int main()
{

    int totalClients;
    printf("Enter the number of clients: ");
    scanf("%d", &totalClients);

    // Create shared memory segment
    key_t key = ftok("server.c", 'R');
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("\x1b[31mshmget\x1b[0m"); // ANSI escape code for red color
        exit(EXIT_FAILURE);
    }

    shared_data *shm_ptr = (shared_data *)shmat(shmid, NULL, 0);
    if ((void *)shm_ptr == (void *)-1)
    {
        perror("\x1b[31mshmat\x1b[0m"); // ANSI escape code for red color
        exit(EXIT_FAILURE);
    }

    printf("Server is ready to receive messages...\n");

    // Creating clients
    for (int i = 0; i < totalClients; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("\x1b[31mfork\x1b[0m"); // ANSI escape code for red color
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        { // Child process (client)
            char client_id_str[5];
            sprintf(client_id_str, "%d", i + 1);
            char terminal_command[100];
            sprintf(terminal_command, "gnome-terminal -- ./client %s %d", client_id_str, totalClients);
            system(terminal_command);
            exit(EXIT_SUCCESS);
        }
    }

    char chatFifo[10] = "chatFifo";
    for (int i = 0; i < totalClients; i++)
    {
        sprintf(chatFifo, "%d", i);
        int status = mkfifo(chatFifo, 0666); // Read and write permissions for everyone
        if (status == -1)
        {
            // Handle error
            printf("\x1b[31merror creating serverfifo\x1b[0m\n"); // ANSI escape code for red color
            return -1;
        }
    }
    int fd[totalClients];
    for (int i = 0; i < totalClients; i++)
    {
        sprintf(chatFifo, "%d", i);
        fd[i] = open(chatFifo, O_WRONLY);
        if (fd[i] == -1)
        {
            printf("\x1b[31munable to open fifo chatFifo\x1b[0m\n"); // ANSI escape code for red color
            exit(1);
        }
    }

    char chats[totalClients][SHM_SIZE * 2];
    for (int i = 0; i < totalClients; i++)
    {
        memset(chats[i], 0, sizeof(chats[i]));
    }

    bool clientGroup1[totalClients];
    bool clientGroup2[totalClients];
    for (int i = 0; i < totalClients; i++)
    {
        clientGroup1[i] = false;
        clientGroup2[i] = false;
    }

    int currentClients = totalClients;

    // Server receiving messages
    while (1)
    {
        if (shm_ptr->command != 0)
        {
            if (shm_ptr->command == 1)
            {
                char msg[SHM_SIZE * 2];
                char msg2[SHM_SIZE * 2];
                sprintf(msg, "Client%d(IN): %s\n", shm_ptr->client_id, shm_ptr->message);
                sprintf(msg2, "Client%d(OUT): %s\n", shm_ptr->reciever, shm_ptr->message);
                printf("\x1b[36mClient%d sent \"%s\" to Client%d\x1b[0m\n", shm_ptr->client_id, shm_ptr->message, shm_ptr->reciever); // ANSI escape code for cyan color
                strcat(chats[shm_ptr->reciever - 1], msg);
                strcat(chats[shm_ptr->client_id - 1], msg2);
                write(fd[shm_ptr->client_id - 1], chats[shm_ptr->client_id - 1], strlen(chats[shm_ptr->client_id - 1]));
                write(fd[shm_ptr->reciever - 1], chats[shm_ptr->reciever - 1], strlen(chats[shm_ptr->reciever - 1]));
                shm_ptr->command = 0;
            }
            else if (shm_ptr->command == 10)
            {
                if (shm_ptr->gNum == 1)
                    clientGroup1[shm_ptr->client_id - 1] = true;
                else
                    clientGroup2[shm_ptr->client_id - 1] = true;
                char msg[SHM_SIZE * 2];
                sprintf(msg, "%s\n", shm_ptr->message);
                printf("%s", msg);
                shm_ptr->command = 0;
            }
            else if (shm_ptr->command == 100)
            {
                char msg[SHM_SIZE * 2];
                char msg2[SHM_SIZE * 2];
                char groupName[20];
                if (shm_ptr->gNum == 1)
                {
                    strcpy(groupName, "Study Group");
                }
                else
                {
                    strcpy(groupName, "Discussion Group");
                }
                sprintf(msg, "Client%d(OUT-%s): %s\n", shm_ptr->client_id, groupName, shm_ptr->message);
                sprintf(msg2, "Client%d(IN-%s): %s\n", shm_ptr->client_id, groupName, shm_ptr->message);
                strcat(chats[shm_ptr->client_id - 1], msg);
                write(fd[shm_ptr->client_id - 1], chats[shm_ptr->client_id - 1], strlen(chats[shm_ptr->client_id - 1]));
                for (int i = 0; i < totalClients; i++)
                {
                    if (shm_ptr->gNum == 1)
                    {
                        if (clientGroup1[i] == true && i != shm_ptr->client_id - 1)
                        {
                            strcat(chats[i], msg2);
                            write(fd[i], chats[i], strlen(chats[i]));
                        }
                    }
                }
                printf("\x1b[36mClient%d sent a group message\x1b[0m\n", shm_ptr->client_id); // ANSI escape code for cyan color
                printf("%s: %s\n", groupName, shm_ptr->message);
                shm_ptr->command = 0;
            }
            else if (shm_ptr->command == 10000)
            {
                printf("\x1b[36mClient%d has left the server\x1b[0m\n", shm_ptr->client_id); // ANSI escape code for cyan color
                clientGroup1[shm_ptr->client_id - 1] = false;
                clientGroup2[shm_ptr->client_id - 1] = false;
                currentClients--;
                if (currentClients == 0)
                {
                    break;
                }
                shm_ptr->command = 0;
            }
        }
        else
        {
            usleep(100000);
        }
    }

    // Detach shared memory
    if (shmdt(shm_ptr) == -1)
    {
        perror("\x1b[31mshmdt\x1b[0m"); // ANSI escape code for red color
        exit(EXIT_FAILURE);
    }

    // Remove shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("\x1b[31mshmctl\x1b[0m"); // ANSI escape code for red color
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < totalClients; i++)
    {
        sprintf(chatFifo, "%d", i);
        unlink(chatFifo);
    }

    return 0;
}
