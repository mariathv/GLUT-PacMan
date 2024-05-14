#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
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

// Define ANSI escape codes for text color
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, ANSI_COLOR_RED "Usage: %s <client_id>\n" ANSI_COLOR_RESET, argv[0]);
        exit(EXIT_FAILURE);
    }
    int totalClients = atoi(argv[2]);
    int client_id = atoi(argv[1]);

    // Access shared memory segment
    key_t key = ftok("server.c", 'R');
    int shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1)
    {
        perror(ANSI_COLOR_RED "shmget" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    shared_data *shm_ptr = (shared_data *)shmat(shmid, NULL, 0);
    if ((void *)shm_ptr == (void *)-1)
    {
        perror(ANSI_COLOR_RED "shmat" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    char chat[10000];

    char chatFifo[10] = "chatFifo";
    int fd;
    sprintf(chatFifo, "%d", client_id - 1);
    fd = open(chatFifo, O_RDONLY);
    if (fd == -1)
    {
        printf(ANSI_COLOR_RED "Unable to open fifo chatFifo\n" ANSI_COLOR_RESET);
        exit(1);
    }

    bool isShownChat = false;
    bool isInGroup = false;
    bool isG1 = false;
    bool isG2 = false;

    int groupJoined = 0;
    bool isExit = false;

    while (1)
    {
        printf(ANSI_COLOR_CYAN "Client %d options:\n" ANSI_COLOR_RESET, client_id);
        printf(ANSI_COLOR_GREEN "1. Send a message directly to a client\n");
        printf("2. Send a message in group\n");
        printf("3. Join a group\n");
        printf("4. Leave a group\n");
        printf("5. Show the chat\n");
        printf("6. Leave the server\n" ANSI_COLOR_RESET);
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the client number where you want to send your message\n");
            printf("Enter your choice: ");
            int reciever;
            scanf("%d", &reciever);
            while (1)
            {
                if (reciever == client_id)
                {
                    printf(ANSI_COLOR_RED "You can't send a message to yourself\n" ANSI_COLOR_RESET);
                    scanf("%d", &reciever);
                    continue;
                }
                else if (reciever > totalClients)
                {
                    printf(ANSI_COLOR_RED "Client does not exist, Enter Valid input\n" ANSI_COLOR_RESET);
                    scanf("%d", &reciever);
                    continue;
                }
                else
                {
                    break;
                }
            }
            printf("Enter message to send: ");
            getchar(); // Clear input buffer
            fgets(shm_ptr->message, sizeof(shm_ptr->message), stdin);
            // Remove trailing newline character
            shm_ptr->message[strcspn(shm_ptr->message, "\n")] = '\0';
            shm_ptr->client_id = client_id;
            shm_ptr->reciever = reciever;
            shm_ptr->command = 1;
            isShownChat = false;
            break;
        case 2:
            if (!isInGroup)
            {
                printf(ANSI_COLOR_YELLOW "------------------------------\n");
                printf("You are not in any group yet\n");
                printf("------------------------------\n" ANSI_COLOR_RESET);
                break;
            }
            if (groupJoined == 1)
            {
                int gnum;
                char groupName[20];
                if (isG1)
                {
                    strcpy(groupName, "Study Group");
                    gnum = 1;
                }
                else
                {
                    strcpy(groupName, "Discussion Group");
                    gnum = 2;
                }
                printf("Enter message to send: ");
                getchar(); // Clear input buffer
                fgets(shm_ptr->message, sizeof(shm_ptr->message), stdin);
                // Remove trailing newline character
                shm_ptr->message[strcspn(shm_ptr->message, "\n")] = '\0';
                shm_ptr->client_id = client_id;
                shm_ptr->reciever = 0;
                shm_ptr->command = 100;
                shm_ptr->gNum = gnum;
                isShownChat = false;
            }
            else
            {
                int groupChoice = 0;
                int gnum;
                printf("In which group do you want to send?\n");
                printf("1: Study Group\n");
                printf("2: Discussion Group\n");
                while (groupChoice < 1 || groupChoice > 2)
                {
                    printf("Enter your choice: ");
                    scanf("%d", &groupChoice);
                }
                char groupName[20];
                if (groupChoice == 1)
                {
                    strcpy(groupName, "Study Group");
                    gnum = 1;
                }
                else
                {
                    strcpy(groupName, "Discussion Group");
                    gnum = 2;
                }

                printf("Enter message to send: ");
                getchar(); // Clear input buffer
                fgets(shm_ptr->message, sizeof(shm_ptr->message), stdin);
                // Remove trailing newline character
                shm_ptr->message[strcspn(shm_ptr->message, "\n")] = '\0';
                shm_ptr->client_id = client_id;
                shm_ptr->reciever = 0;
                shm_ptr->command = 100;
                shm_ptr->gNum = gnum;
                isShownChat = false;
            }
            break;
        case 3:
            int groupChoice = 0;
            printf("Which group do you want to join?\n");
            printf("1: Study Group\n");
            printf("2: Discussion Group\n");
            while (groupChoice < 1 || groupChoice > 2)
            {
                printf("Enter your choice: ");
                scanf("%d", &groupChoice);
            }
            if (groupChoice == 1 && isG1)
            {
                printf(ANSI_COLOR_YELLOW "-------------------------------------\n");
                printf("You have already joined that group\n");
                printf("-------------------------------------\n" ANSI_COLOR_RESET);
                break;
            }
            if (groupChoice == 2 && isG2)
            {
                printf(ANSI_COLOR_YELLOW "-------------------------------------\n");
                printf("You have already joined that group\n");
                printf("-------------------------------------\n" ANSI_COLOR_RESET);
                break;
            }
            char groupName[20];
            int gnum;
            if (groupChoice == 1)
            {
                strcpy(groupName, "Study Group");
                isG1 = true;
                gnum = 1;
            }
            else
            {
                strcpy(groupName, "Discussion Group");
                gnum = 2;
                isG2 = true;
            }

            sprintf(shm_ptr->message, "Client%d has joined Group \"%s\"", client_id, groupName);
            printf(ANSI_COLOR_GREEN "-------------------------------------------\n");
            printf("You have joined the group \"%s\"\n", groupName);
            printf("-------------------------------------------\n" ANSI_COLOR_RESET);
            shm_ptr->client_id = client_id;
            shm_ptr->reciever = 0;
            shm_ptr->command = 10;
            shm_ptr->gNum = gnum;
            isShownChat = false;
            isInGroup = true;
            groupJoined++;
            break;
        case 4:
            if (!isInGroup)
            {
                printf(ANSI_COLOR_YELLOW "------------------------------\n");
                printf("You are not in any group yet\n");
                printf("------------------------------\n" ANSI_COLOR_RESET);
                break;
            }
            else
            {
                int groupChoice = 0;
                printf("Which group do you want to leave?\n");
                printf("1: Study Group\n");
                printf("2: Discussion Group\n");
                while (groupChoice < 1 || groupChoice > 2)
                {
                    printf("Enter your choice: ");
                    scanf("%d", &groupChoice);
                }
                if (groupChoice == 1 && !isG1)
                {
                    printf(ANSI_COLOR_RED "-------------------------------------\n");
                    printf("You haven't joined that group\n");
                    printf("-------------------------------------\n" ANSI_COLOR_RESET);
                    break;
                }
                if (groupChoice == 2 && !isG2)
                {
                    printf(ANSI_COLOR_RED "-------------------------------------\n");
                    printf("You haven't joined that group\n");
                    printf("-------------------------------------\n" ANSI_COLOR_RESET);
                    break;
                }
                int gnum;
                if (groupChoice == 1)
                {
                    isG1 = false;
                    gnum = 1;
                }
                else
                {
                    isG2 = false;
                    gnum = 2;
                }

                sprintf(shm_ptr->message, "Client%d has left Group \"%s\"", client_id, groupName);
                printf(ANSI_COLOR_RED "-------------------------------------------\n");
                printf("You have left the group \"%s\"\n", groupName);
                printf("-------------------------------------------\n" ANSI_COLOR_RESET);
                shm_ptr->client_id = client_id;
                shm_ptr->reciever = 0;
                shm_ptr->command = 1000;
                shm_ptr->gNum = gnum;
                isShownChat = false;
                groupJoined--;
                if (groupJoined == 0)
                {
                    isInGroup = false;
                }
            }
            break;
        case 5:
            memset(chat, 0, sizeof(chat)); // Initialize chat buffer
            if (isShownChat)
            {
                printf(ANSI_COLOR_YELLOW "-------------------------------------------------\n");
                printf("Chat already Shown and haven't been updated\n");
                printf("-------------------------------------------------\n" ANSI_COLOR_RESET);
                break;
            }
            read(fd, chat, sizeof(chat) - 1); // Read from FIFO
            isShownChat = true;
            char *lines[20];
            for (int i = 0; i < 20; i++)
            {
                lines[i] = malloc(SHM_SIZE * 2); // Allocate memory for each line
            }

            int lines_count = 0;

            // Tokenize the large string into individual lines
            char *token = strtok(chat, "\n");
            while (token != NULL && lines_count < 20)
            {
                // Check if the line is unique
                int duplicate = 0;
                for (int i = 0; i < lines_count; i++)
                {
                    if (strcmp(lines[i], token) == 0)
                    {
                        duplicate = 1;
                        break;
                    }
                }
                // Add the line to the array if it's not a duplicate
                if (!duplicate)
                {
                    lines[lines_count] = token;
                    lines_count++;
                }
                token = strtok(NULL, "\n");
            }

            // Construct the new string with unique lines
            char new_chat[sizeof(chat)];
            strcpy(new_chat, lines[0]);
            for (int i = 1; i < lines_count; i++)
            {
                strcat(new_chat, "\n");
                strcat(new_chat, lines[i]);
            }
            strcpy(chat, new_chat);
            printf(ANSI_COLOR_CYAN "-----------------------------------------------------\n");
            printf("Chat History:\n%s\n", new_chat);
            printf("-----------------------------------------------------\n" ANSI_COLOR_RESET);

            break;
        case 6:
            shm_ptr->command = 10000;
            shm_ptr->client_id = client_id;
            isExit = true;
            break;
        default:
            printf(ANSI_COLOR_RED "Invalid choice. Please enter a number between 1 and 4.\n" ANSI_COLOR_RESET);
        }
        if (isExit)
        {
            break;
        }
    }

    unlink(chatFifo);
    return 0;
}
