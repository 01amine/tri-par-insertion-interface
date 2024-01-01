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
        ClearBackground(RAYWHITE);
        DrawRectangle(10, 40, 380, 30, LIGHTGRAY);
        DrawText(inputText, 20, 45, 20, MAROON);

        EndDrawing();
    }

    

    return userInput;
}

void displayList(Node *l, int x, int y){
    Node *tmp=l;
    int i=0;
    while(tmp !=NULL){

            Rectangle rect = {.x=x+i*100, .y=y,.height=60, .width=70};
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

Rectangle button = { 350, 600, 100, 70 };
Rectangle button2 = { 300, 700, 200, 70 };
bool buttonPressed = false;
bool button2Pressed = false;

void main() {
    int val;
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
        if(CheckCollisionPointRec(mousePosition, button2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int val = getInputFromUser();
            head = insertAtBeginning(head, val);
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

        DrawRectangleRec(button, buttonPressed ? DARKGRAY : GRAY);
        DrawRectangleLines(button.x , button.y, button.width, button.height, BLACK);
        DrawText("Tri",
         (int)(button.x + button.width / 2 - MeasureText("Tri", 20) / 2),
          (int)(button.y + button.height / 2 - 10),
           20,
            WHITE //Color
            );
        DrawRectangleRec(button2, button2Pressed ? DARKGRAY : GRAY);
        DrawRectangleLines(button2.x , button2.y, button2.width, button2.height, BLACK);
        DrawText("Ajouter un element",
         (int)(button2.x + button2.width / 2 - MeasureText("Ajouter un element", 20) / 2),
          (int)(button2.y + button2.height / 2 - 10),
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