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

void displayList(Node* head, int x, int y) {
    int rowHeight = 50;

    while (head != NULL) {
        char text[10];
        sprintf(text, "%d", head->data);
        DrawEllipseLines(x, y, 35, 20, BLACK);
        DrawText(text, x - 5, y - 8, 20, BLACK);
        head = head->next;
        x = x + rowHeight + 50;
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
        ClearBackground(WHITE);
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
        DrawText("Tri",
         (int)(button.x + button.width / 2 - MeasureText("Tri", 20) / 2),
          (int)(button.y + button.height / 2 - 10),
           20,
            WHITE //Color
            );
        DrawRectangleRec(button2, button2Pressed ? DARKGRAY : GRAY);
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