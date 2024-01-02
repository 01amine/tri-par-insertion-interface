#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* insertAtBeginning(Node* head, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = head;
    return newNode;
}
Node* insertAtEnd(Node* head, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    if (head == NULL) {
        return newNode;
    }
    Node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
    return head;
}


int getInputFromUser() {
    bool mouseOnText ;
    Rectangle textBox = { 1000/2.0f - 100, 180, 225, 50 };
    if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;
    if(mouseOnText)SetMouseCursor(MOUSE_CURSOR_IBEAM);
    int userInput = 0;
    char inputText[9] = "\0"; 
    int letterCount = 0;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
            userInput = atoi(inputText); // Convert input string to integer
            break; 
        }

        int key = GetKeyPressed();

        
        if ((key >= 32) && (key <= 125) && (letterCount < 9)) {
            inputText[letterCount] = (char)key;
            letterCount++;
        } else if ((key == KEY_BACKSPACE) && (letterCount > 0)) {
            letterCount--;
            inputText[letterCount] = '\0';
        }

        BeginDrawing();
        ClearBackground(GRAY);
        DrawRectangle(310, 460, 150, 60, BLACK);
        DrawRectangleLines(310, 460, 150, 60, LIGHTGRAY);
        DrawText(inputText, 330, 478, 25, WHITE);

        EndDrawing();
    }

    

    return userInput;
}

void displayList(Node *l, int x, int y){
    Node *tmp=l; 

    if(tmp!=NULL){
        Rectangle r ={.x=x, .y=y, .height =60, .width= 80};
        DrawRectangleRec(r, LIGHTGRAY);
        DrawRectangleLinesEx(r, 2, BLACK);
        DrawText("Head", x + 10, y + 13, 25, BLACK);
        DrawLine(x+80, y+30, x+110, y+30, BLACK);
    }

    int i=1;
    while(tmp !=NULL){

            Rectangle rect = {.x=x+i*110, .y=y,.height=60, .width=70};
            char str[20];
            sprintf(str, "%d", tmp->data);
            DrawRectangleRec(rect, LIGHTGRAY);
            DrawRectangleLinesEx(rect, 2, BLACK);
            DrawText(str, rect.x + 15, rect.y + 13, 25, BLACK);
            if(tmp->next!=NULL){
                DrawLine(rect.x+70, rect.y+35, rect.x+160, rect.y+35, BLACK);
            }
        
        i++;
        tmp=tmp->next;
    }
}


Node* copyList(Node* head) {
    Node* newHead = NULL;
    Node* tail = NULL;

    while (head != NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = head->data;
        newNode->next = NULL;

        if (newHead == NULL) {
            newHead = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        head = head->next;
    }

    return newHead;
}

void insertionSort(Node* head, Node*** steps, int* stepCount) {
    Node* sorted = NULL;
    Node* current = head;

    *stepCount = 0;

    while (current != NULL) {
        Node* next = current->next;
        if (sorted == NULL || sorted->data >= current->data) {
            current->next = sorted;
            sorted = current;
        } else {
            Node* temp = sorted;
            while (temp->next != NULL && temp->next->data < current->data) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }

        (*steps)[*stepCount] = copyList(sorted);
        (*stepCount)++;

        current = next;
    }
}

void freeSteps(Node** steps, int stepCount) {
    for (int i = 0; i < stepCount; i++) {
        Node* current = steps[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(steps);
}
void deleteNodeWithValue(Node** head, int value) {
    Node* current = *head;
    Node* previous = NULL;

    while (current != NULL && current->data != value) {
        previous = current;
        current = current->next;
    }

    if (current != NULL) {
        if (previous == NULL) {
            *head = current->next;
        } else {
            previous->next = current->next;
        }

        free(current);
    }
}

Rectangle button = { 350, 600, 100, 70 };
Rectangle button2 = { 300, 700, 200, 70 };
Rectangle button3 = { 350, 800, 100, 70 };
Rectangle button4 = { 350, 900, 100, 70 };
bool buttonPressed = false;
bool button2Pressed = false;
bool button3Pressed = false;
bool button4Pressed = false;

void main() {
    
    const int screenHeigt = 1000 ;
    const int screenWidth =1000;
    InitWindow(screenHeigt, screenWidth, "Tri-par-Insertion-Interface");
    IsWindowResized();
    MaximizeWindow();

    Node* head = NULL;
    

    Node** steps;
    int stepCount;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        Vector2 mousePosition = GetMousePosition();

        if (CheckCollisionPointRec(mousePosition, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            buttonPressed = !buttonPressed;
            if (buttonPressed) {
                // Compter le nombre d elements dans la liste
                int count = 0;
                Node* temp = head;
                while (temp != NULL) {
                    count++;
                    temp = temp->next;
                }

                // Allouer de la memoire pour stocker chaque etape du tri
                steps = (Node**)malloc(count * sizeof(Node*));

                // Trier les elements
                insertionSort(head, &steps, &stepCount);
            }
        }
        DrawText("Cliquez sur le bouton pour trier la liste par insertion", 10, 10, 20, RED);

        if (buttonPressed) {
            for (int i = 0; i < stepCount; i++) {
                displayList(steps[i], 100, 100 + i * 100);
            }
            DrawText("Tri en cours...", 10, 70, 20, RED);
        } else {
            displayList(head, 100, 100);
        }

        
         if(CheckCollisionPointRec(mousePosition, button2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int value = getInputFromUser();
            head = insertAtBeginning(head, value);
        }
    

        if(CheckCollisionPointRec(mousePosition, button3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int value = getInputFromUser();
            deleteNodeWithValue(&head, value);
            
        }


if(CheckCollisionPointRec(mousePosition, button4) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int value = getInputFromUser();
            head = insertAtEnd(head,value);
        }


        DrawRectangleRec(button, buttonPressed ? DARKGRAY : GRAY);
        DrawRectangleLines(button.x , button.y, button.width, button.height, BLACK);
        DrawText("Tri",
         (int)(button.x + button.width / 2 - MeasureText("Tri", 20) / 2),
          (int)(button.y + button.height / 2 - 10),
           20,
            WHITE //Color
            );
// button "ajouter un element"
        DrawRectangleRec(button2, button2Pressed ? DARKGRAY : GRAY);
        DrawRectangleLines(button2.x , button2.y, button2.width, button2.height, BLACK);
        DrawText("Ajouter un element",
         (int)(button2.x + button2.width / 2 - MeasureText("Ajouter un element", 20) / 2),
          (int)(button2.y + button2.height / 2 - 10),
           20,
            WHITE //Color
            );
// button "Supprimer"
             DrawRectangleRec(button3, button3Pressed ? DARKGRAY : GRAY);
        DrawRectangleLines(button3.x , button3.y, button3.width, button3.height, BLACK);
        DrawText("Supprimer",
         (int)(button3.x + button3.width / 2 - MeasureText("Supprimer", 20) / 2),
          (int)(button3.y + button3.height / 2 - 10),
           20,
            WHITE //Color
            );

// button "ajouter a la fin "
            DrawRectangleRec(button4, button4Pressed ? DARKGRAY : GRAY);
        DrawRectangleLines(button4.x , button4.y, button4.width, button4.height, BLACK);
        DrawText("ajoute a la fin",
         (int)(button4.x + button4.width / 2 - MeasureText("ajoute a la fin", 20) / 2),
          (int)(button4.y + button4.height / 2 - 10),
           20,
            WHITE //Color
            );


        EndDrawing();
    }

    
    freeSteps(steps, stepCount);

    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    CloseWindow();
}