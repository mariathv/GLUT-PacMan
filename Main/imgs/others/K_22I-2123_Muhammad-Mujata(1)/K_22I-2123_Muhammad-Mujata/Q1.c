#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define RESET "\033[0m"
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#define WHITE "\033[37m"   /* White */

char **board;
int playerScores[4] = {0};

void printBoard(int size)
{
    for (int i = 0; i < 4; ++i)
    {
        if (i == 1)
            printf(RED);
        if (i == 2)
            printf(BLUE);
        if (i == 3)
            printf(YELLOW);
        if (i == 0)
            printf(GREEN);
        printf("Player %d Score: %d\n", i + 1, playerScores[i]);
    }
    printf(RESET "\n");

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (board[i][j] == '2')
                printf(RED);
            else if (board[i][j] == '3')
                printf(BLUE);
            else if (board[i][j] == '4')
                printf(YELLOW);
            else if (board[i][j] == '1')
                printf(GREEN);
            else
                printf(RESET);
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf(RESET);
}

void updatePlayer(int turn, char key, int *playerPosX, int *playerPosY, int size)
{
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    int x = playerPosX[turn - 1];
    int y = playerPosY[turn - 1];

    switch (key)
    {
    case 'w':
        x += dx[0];
        y += dy[0];
        break;
    case 's':
        x += dx[1];
        y += dy[1];
        break;
    case 'a':
        x += dx[2];
        y += dy[2];
        break;
    case 'd':
        x += dx[3];
        y += dy[3];
        break;
    default:
        printf(RED "Invalid key. Please use WASD keys.\n" RESET);
        printf(GREEN "Press any key to Continue\n" RESET);
        char k;
        scanf(" %c", &k);
        return;
    }

    if (x >= 0 && x < size && y >= 0 && y < size)
    {
        if (board[x][y] == '*' || board[x][y] == ' ')
        {
            board[playerPosX[turn - 1]][playerPosY[turn - 1]] = ' ';
            board[x][y] = turn + '0';

            playerPosX[turn - 1] = x;
            playerPosY[turn - 1] = y;
        }
        else
        {
            printf(RED "Invalid move. Player cannot move to an occupied position.\n" RESET);
            printf(GREEN "Press any key to Continue\n" RESET);
            char k;
            scanf(" %c", &k);
        }
    }
    else
    {
        printf(RED "Invalid move. Player cannot move outside the board.\n" RESET);
        printf(GREEN "Press any key to Continue\n" RESET);
        char k;
        scanf(" %c", &k);
    }
}

int checkGameOver(int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (board[i][j] == '*')
                return 0;
        }
    }
    return 1;
}

void displayWinners()
{
    int maxScore = 0;
    int winners[4] = {0};

    for (int i = 0; i < 4; ++i)
    {
        if (playerScores[i] > maxScore)
            maxScore = playerScores[i];
    }

    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (playerScores[i] == maxScore)
            winners[count++] = i + 1;
    }

    printf(YELLOW "Winners: ");
    for (int i = 0; i < count; ++i)
    {
        printf("Player %d ", winners[i]);
    }
    printf(RESET "\n");
}

int main()
{
    system("clear");
    srand(time(0));
    int randomNum = 10 + rand() % 90;
    int grid = randomNum * 9;
    grid = 879 / grid;
    grid %= 25;
    if (grid < 10)
        grid += 15;

    board = (char **)malloc(grid * sizeof(char *));
    for (int i = 0; i < grid; ++i)
    {
        board[i] = (char *)malloc(grid * sizeof(char));
    }

    for (int i = 0; i < grid; ++i)
    {
        for (int j = 0; j < grid; ++j)
        {
            board[i][j] = ' ';
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        int row = (i < 2) ? 0 : grid - 1;
        int col = (i % 2 == 0) ? 0 : grid - 1;
        board[row][col] = i + '1';
    }

    for (int i = 0; i < grid; ++i)
    {
        for (int j = 0; j < grid; ++j)
        {
            if (board[i][j] != '1' && board[i][j] != '2' && board[i][j] != '3' && board[i][j] != '4')
            {
                if (rand() % 25 == 1)
                {
                    board[i][j] = '*';
                }
                else
                {
                    board[i][j] = ' ';
                }
            }
        }
    }

    printBoard(grid);

    int turn = 1;
    int playerPosX[] = {0, 0, grid - 1, grid - 1};
    int playerPosY[] = {0, grid - 1, 0, grid - 1};

    while (1)
    {
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            return 1;
        }
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            close(pipefd[0]);

            printf("Player %d - Enter a key: ", turn);
            char key;
            scanf(" %c", &key);

            if (write(pipefd[1], &key, sizeof(char)) == -1)
            {
                perror("write");
                return 1;
            }

            close(pipefd[1]);
            return 0;
        }
        else
        {
            close(pipefd[1]);

            char key;
            if (read(pipefd[0], &key, sizeof(char)) == -1)
            {
                perror("read");
                return 1;
            }

            updatePlayer(turn, key, playerPosX, playerPosY, grid);

            close(pipefd[0]);

            if (checkGameOver(grid))
            {
                displayWinners();
                break;
            }

            turn = (turn % 4) + 1;

            system("clear");
            printBoard(grid);
        }
    }

    for (int i = 0; i < grid; ++i)
    {
        free(board[i]);
    }
    free(board);

    return 0;
}
