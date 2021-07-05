#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <malloc.h>


typedef struct{
    char *word; //word and corresponding hint
    char *clue;
    int x; //Starting x and y positions
    int y;
    char direction; //H for horizontal, V for vertical
    int f; //solved or not
} Word_t;

Word_t* loadTextFile( FILE* myFile, int nr );
void displayBoard(int rows, int cols, char** myBoard);
int isBoardFilled(int rows, int cols, char** myBoard);
char** createArray(int rows, int cols);
int findIndex(int arr[], int size, int val);
void playGame(char** myBoard, int words, Word_t *word, int x, int y);
char** updateBoard(char** myBoard, Word_t *words, int solve);

int main(void)
{
    FILE *fp;
    char fnamer[50];
    char **board;
    Word_t *dynarray;
    int nrof_rows, nrof_cols, nrof_words;

    printf("Enter the name of the text file:\n");
    scanf("%s",fnamer);
    fp = fopen(fnamer,"r");
    if(fp == NULL)
        {
        printf("FILE NOT FOUND\n");
    }


    fscanf(fp,"%d %d %d", &nrof_rows, &nrof_cols, &nrof_words);
    printf("Game is %d rows x %d cols with %d words\n", nrof_rows, nrof_cols, nrof_words);

    dynarray = (Word_t*)malloc(nrof_words * sizeof(Word_t));
    dynarray = loadTextFile(fp,nrof_words);

    board = createArray(nrof_rows,nrof_cols);

    playGame(board, nrof_words, dynarray, nrof_cols, nrof_rows);

    free(fp);
    free(board);
    free(dynarray);

    return 0;
}

Word_t* loadTextFile( FILE* myFile, int nrWords )
{
    Word_t *arr = (Word_t *)malloc(nrWords * sizeof(Word_t));
    char wordtmp[20];
    char cluetmp[150];

    int i = 0;
    while(i < nrWords)
    {
        fscanf(myFile, " %c %d %d %s %[^\n]", &arr[i].direction, &arr[i].x, &arr[i].y, wordtmp, cluetmp);
        arr[i].f = 0;

        arr[i].clue = (char *)malloc(strlen(cluetmp) + 1);
        arr[i].word = (char *)malloc(strlen(wordtmp) + 1);

        strcpy(arr[i].word,     wordtmp);
        strcpy(arr[i].clue, cluetmp);

        i++;
    }
    printf("Words and clues are read!\n");
    return arr;
}

char** createArray(int rows, int cols){

    char** tmpboard;
    int i,j;
    tmpboard = (char**)calloc(rows,sizeof(char*));

    for(i=0; i<rows; i++)
    {
        tmpboard[i] = (char*)calloc(cols, sizeof(char));
        for(j=0; j<cols; j++)
        {
            tmpboard[i][j] = '#';
        }
    }
    return tmpboard;
}

void displayBoard(int rows, int cols, char** myBoard)
{
    int i,j;
    printf("\n\t ");
    for(i = 1; i <= cols; i++)
    {
        printf(" %d ",i);
    }
    printf("\n\t ");
    for(i = 1; i <= cols; i++)
    {
        printf("-- ");
    }
    printf("\n");
    for(i = 0; i < cols; i++)
    {
        printf("%d\t|",i+1);
        for(j = 0; j < rows; j++)
        {
            printf(" %c ",myBoard[i][j]);
        }
        printf("\n");
    }
    printf("\t");
    for(i = 0; i < rows; i++)
        printf("***");
    printf("\n\n");
}

int isBoardFilled(int rows, int cols, char** myBoard)
{
    int i,j;
    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            if(myBoard[i][j] == '_')
            {
                return 0;
            }
        }
    }
    return 1;
}

char** updateBoard(char** myBoard, Word_t *words,int solve)
{
    int tmp = 0;
    int i;
    if(words[solve].f == 0)
    {
        if(words[solve].direction=='V')
        {
            for(i=0; i<strlen(words[solve].word); i++){
                myBoard[i+words[solve].x-1][words[solve].y-1]='_';
            }
        }
        else if(words[solve].direction=='H')
        {
            for(i=0; i<strlen(words[solve].word); i++){
                myBoard[words[solve].x-1][i+words[solve].y-1]='_';
            }
        }

    }
    else
    {
        if(words[solve].direction == 'V')
        {
            for(i=0; i < strlen(words[solve].word); i++){
                myBoard[i+words[solve].x-1][words[solve].y-1]=words[solve].word[tmp++];
            }
        }
        else if(words[solve].direction == 'H')
        {
            for(i=0; i< strlen(words[solve].word); i++){
                myBoard[words[solve].x-1][i + words[solve].y-1]=words[solve].word[tmp++];
            }
        }

    }
    return myBoard;
}

void playGame(char** myBoard, int nrWords, Word_t *words, int x,int y)
{
    int word_number;
    int count = 0;
    while(count < nrWords){
        updateBoard(myBoard, words, count++);
    }
    printf("Current puzzle:\n");
    displayBoard(x, y, myBoard);
    while(1)
    {
        printf("Ask for hint:\n#  Direction\trow   col\n----------------------------\n");
        count = 0;

        while(!isBoardFilled(x, y, myBoard) && count < nrWords)
        {
            if(!words[count++].f)
            {
                if(words[count-1].direction =='V'){
                    printf("%d: Vertical\t%d\t%d\n", count, words[count-1].x, words[count-1].y);
                }

                else if(words[count-1].direction == 'H'){
                    printf("%d: Horizontal\t%d\t%d\n", count, words[count-1].x, words[count-1].y);
                }

            }

        }

        do{
            printf("\nEnter -1 to exit\nWhich word to solve next?: ");
            scanf(" %d", &word_number);
            if(words[word_number-1].f == 1)
                printf("That word has been solved before\n");
            if(word_number == -1)
                return 0;
        }
        while(words[word_number-1].f == 1);

        printf("Current hint: %s\n", words[word_number-1].clue);
        printf("Enter your solution: ");

        char solution[10];
        scanf(" %s", solution);

        if(!strcasecmp(words[word_number-1].word, solution))
        {
            printf("\nCorrect!\n\n");
            words[word_number-1].f = 1;
            updateBoard(myBoard, words, word_number-1);
            printf("Current puzzle:\n");
            displayBoard(x, y, myBoard);
        }
        else{
            printf("WRONG ANSWER\n");
        }

        if(isBoardFilled(x, y, myBoard))
        {
            printf("Congratulations! You beat the puzzle!\n");
            return 0;
        }

    }
}
