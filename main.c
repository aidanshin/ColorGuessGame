#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>

//int time = 30;
int correct = 0;
int wrong = 0;
int score = 1;

int randomColor;
int randomWordIndex;

char *colors[] = {"GREY", "RED", "GREEN", "YELLOW", "PURPLE", "PINK", "CYAN", "WHITE"};
char color[12];

struct Leaderboard{
    int position;
    char name[16];
    int playerScore;
};

struct Leaderboard topPlayers[5];

void displayScore(){
    score = correct - wrong;
    printf("\033[37;1mScore: %d ", score);
    printf("\033[32;1mCorrect: %d \033[31;1mWrong: %d\n",correct, wrong);
}

void displayColors(){
    printf("\tCOLORS: ");
    for(int i = 30; i <= 37; i++){
        char s[12];
        sprintf(s, "\033[%d;1m", i);
        printf("%s%s ", s, colors[i%10]);
    }
    printf("\n");
}

void displayWord(){
    srand(time(0));
    randomWordIndex = rand() % 8;
    randomColor = rand() % 8 + 30;
    displayScore();
    sprintf(color, "\033[%d;1m", randomColor);
    //printf("%s", color);
    printf("\n\t\033[37;1mCOLOR: %s%s\n\n\t", color, colors[randomWordIndex]);
    printf("\033[1;0m");
}

char *strupr(char *str){
    unsigned char *p = (unsigned char *)str;
    while (*p) {
        *p = toupper((unsigned char)*p);
        p++;
    }
    return str;
}

void countdown(){
    for(int i = 3; i > 0; i--){
        system("clear");
        printf("\t----->\033[37;1m %d <-----", i);
        fflush(stdout);
        sleep(1);
        
    }
    system("clear");
    printf("\t----->GO!<-----");
    fflush(stdout);
    sleep(1);
    system("clear");
}

void game(){
    printf("\033[37;1mInput: ");
    printf("%s", color);
    char input[32];
    scanf("%s", input);
    strupr(input);
    if(strcmp(input, colors[randomColor % 10]) == 0){
        correct++;
        // printf("\033[32;1m");
        // printf("\n\nCorrect!\n");
        // printf("\033[1;0m");
    } else {
        wrong++;
        // printf("\033[31;1m");
        // printf("\n\nWrong!\n");
        // printf("\033[1;0m");
    }
}

void getLeaderboard(){
    FILE *ptr;
    if((ptr = fopen("/Users/aidanshinfeld/Desktop/C/Aidan BANK/leaderboard.txt", "r")) == NULL){
        printf("File not found!");
        exit(1);
    }

    for(int i = 0; i < 5; i++){
        struct Leaderboard leader;
        fscanf(ptr, "%d %s %d", &leader.position, leader.name, &leader.playerScore);
        topPlayers[i] = leader;
    }

    fclose(ptr);
}

void sortLeaderboard(){
    int j; 
    struct Leaderboard key;
    for(int i = 1; i < 5; i++){
        key = topPlayers[i];
        j = i - 1;

        while(j >= 0 && topPlayers[j].playerScore < key.playerScore){
            topPlayers[j+1] = topPlayers[j];
            j = j - 1;
        }
        topPlayers[j+1] = key;
    }

    FILE *ptr;
    if((ptr = fopen("leaderboard.txt", "w")) == NULL){
        printf("File not found!");
        exit(1);
    }

    for(int i = 0; i < 5; i++){
        fprintf(ptr, "%d %s %d\n", (i+1), topPlayers[i].name, topPlayers[i].playerScore);
        topPlayers[i].position = i+1;
    }
    fclose(ptr);
}

void displayLeaderboard(){
    printf("\n\033[33;1mLeaderboard: Position ~ Name ~ Score\n\n");
    for(int i = 0; i < 5; i++){
        if(i == 0){
            printf("\t\033[31;1m%d. %s %d\n", topPlayers[i].position, topPlayers[i].name, topPlayers[i].playerScore);
        } else if (i == 1){
            printf("\t\033[34;1m%d. %s %d\n", topPlayers[i].position, topPlayers[i].name, topPlayers[i].playerScore);
        } else if (i == 2){
            printf("\t\033[32;1m%d. %s %d\n", topPlayers[i].position, topPlayers[i].name, topPlayers[i].playerScore);
        } else {
            printf("\t\033[30;1m%d. %s %d\n", topPlayers[i].position, topPlayers[i].name, topPlayers[i].playerScore);
        }
    }
}

void checkScore(){
    getLeaderboard();
    if(score > topPlayers[4].playerScore){
        printf("\n\033[37;1mCongratulations! You placed on the Leaderboard!\n\n");
        struct Leaderboard newPlayer;
        printf("\t\033[37;1mEnter Name: ");
        scanf("%s", newPlayer.name);
        newPlayer.playerScore = score; 
        newPlayer.position = 0;
        topPlayers[4] = newPlayer;
        sortLeaderboard();
        displayLeaderboard();
    } else {
        displayLeaderboard();
    }
}

int main(){
    time_t start, current;
    printf("\033[37;1m");
    printf("~~~~ You have 30 seconds to state the color of the text that is printed ~~~~\n");
    displayColors();
    printf("\t\t-----> Press \033[31;1mENTER\033[37;1m to start the game <-----\n");
    getchar();
    
    countdown();
    start = time(NULL);
    for(;;){
        current = time(NULL);
        if(current - start >= 30){
            break;
        }
        displayWord();
        game();
        system("clear");
        fflush(stdout);
    }
    system("clear");
    fflush(stdout);
    printf("\033[37;1m");
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("@                                @\n");
    printf("@           GAME OVER!           @\n");
    printf("@                                @\n");
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n\n");
    displayScore();
    checkScore();
}
