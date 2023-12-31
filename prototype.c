#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//defining a type position to use it for shapes positioning
typedef struct pos
{
    int x;
    int y;
}pos;

//defining the nodes of the linked list
typedef struct {
    int info;
    struct node *next;
    bool isActive;
} node;


//to create a node
node *createNode(int num){
    node *n = malloc(sizeof(node));
    n->info = num;
    n->next = NULL;
    n->isActive = false;

    return n;
}

//to create a list
void createList(node* c, node **headList){
    if(*headList==NULL){
        *headList=c;
    }else{
        node *temp=*headList;
        while(temp->next!=NULL){
            temp = temp->next;
        }
        temp->next = c;
    }
}


void drawList(node *l, pos initialPosition){
    node *tmp=l;
    int i=0;
    while(tmp !=NULL){
        if(tmp->isActive){
            Rectangle rect = {.x=initialPosition.x+i*100, .y=initialPosition.y,.height=60, .width=70};
            char str[20];
            sprintf(str, "%d", tmp->info);
            DrawRectangleRec(rect, LIGHTGRAY);
            DrawRectangleLinesEx(rect, 2, BLACK);
            DrawText(str, rect.x + 15, rect.y + 13, 25, BLACK);
        }
        i++;
        tmp=tmp->next;
    }
}

//to free memory occupied by the list
void freeList(node* head) {
    node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}





void main(){

    char input[21] = "\0";
    int index=0;

    node *l = NULL, *t = NULL;

    pos intialPosition1 = {.x=0, .y=80};

    InitWindow(1280,720, "prototype");

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GRAY);

        //drawing the input box
        Rectangle inputRect = {0, 0, 200, 60};
        DrawRectangleRec(inputRect, BLACK);
        DrawRectangleLinesEx(inputRect, 4, LIGHTGRAY);
        DrawText(input,inputRect.x+15, inputRect.y+13, 25, WHITE);

        //drawing a "sort" button
        DrawRectangle(250, 0, 85, 50, RED);
        DrawText("Sort", 265, 13, 25, WHITE);

        //takes the input 
        int key = GetKeyPressed();
        if (key != 0) {
            if ((key >= 32) && (key <= 125) && (index < 20)) {
                input[index] = (char)key;
                index++;
            }
        }

        // Delete characters when backspace is pressed
        if (IsKeyPressed(KEY_BACKSPACE) && index > 0) {
            index--;
            input[index] = '\0';
        }   

        
        // Check if Enter key is pressed and create a node and add it to the list
        
        for(int i =0; i<4; i++){
            if (IsKeyPressed(KEY_ENTER)) {
                t = createNode(atoi(input));
                if (!t->isActive) {
                    t->isActive = true;
                    createList(t,&l);
                    break;
                
                }
                
            }
            
        }  

        //draws the list
        pos p ={.x=0, .y=70};
        if(l!=NULL){
            drawList(l, p);
        }  



        EndDrawing();

    }
    CloseWindow();

    //freeing the list
    freeList(l);
    l=NULL;




}