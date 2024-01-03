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
Node* insertAtPosition(Node* head, int data, int position) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (position == 1) {
        newNode->next = head;
        return newNode;
    }
    Node* current = head;
    for (int i = 1; i < position - 1 && current != NULL; i++) {
        current = current->next;
    }
    if (current == NULL) {
        return insertAtEnd(head, data);
    }
    newNode->next = current->next;
    current->next = newNode;

    return head;
}



int getInputFromUser(char message[50]) {
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
        DrawText(message, 310, 425, 25, WHITE);

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

Rectangle userMenu = {0, 0, 1000, 78};
Rectangle addLIFO = { 4, 4, 200, 70 };
Rectangle addFIFO = { 210, 4, 200, 70 };
Rectangle addAtPos = { 416, 4, 270, 70 };
Rectangle deleteButton = { 692, 4, 120, 70 };
Rectangle sortButton = { 818, 4, 100, 70 };
Rectangle logo = {918, 4, 82, 70};
bool sortPressed = false;

void main() {
    
    const int screenHeigt = 1000 ;
    const int screenWidth =1000;
    InitWindow(screenHeigt, screenWidth, "Tri-par-Insertion APP");
    IsWindowResized();
    MaximizeWindow();

    Node* head = NULL;
    

    Node** steps;
    int stepCount;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        Vector2 mousePosition = GetMousePosition();

        if (CheckCollisionPointRec(mousePosition, sortButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            sortPressed = !sortPressed;
            if (sortPressed) {
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

        if (sortPressed) {
            for (int i = 0; i < stepCount; i++) {
                displayList(steps[i], 100, 150 + i * 100);
            }
            DrawText("Tri en cours...", 10, 100, 20, RED);
        } else {
            displayList(head, 100, 150);
        }

        
         if(CheckCollisionPointRec(mousePosition, addLIFO) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int value = getInputFromUser("Ajouter au début");
            head = insertAtBeginning(head, value);
        }
    

        if(CheckCollisionPointRec(mousePosition, deleteButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int value = getInputFromUser("Entrez élément que vous voulez supprimer");
            deleteNodeWithValue(&head, value);
            
        }


        if(CheckCollisionPointRec(mousePosition, addFIFO) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int value = getInputFromUser("Ajoute a la fin");
            head = insertAtEnd(head,value);
        }

        if(CheckCollisionPointRec(mousePosition, addAtPos) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int value = getInputFromUser("Entrer la valeur");
            int position = getInputFromUser("Entrez la position de la valeur");
            head = insertAtPosition(head,value,position);
        }


//user menu
        DrawRectangleRec(userMenu, DARKGRAY);
        DrawRectangleLinesEx(userMenu, 4, BLACK);         

//button "Tri"
        DrawRectangleRec(sortButton, GRAY);
        DrawRectangleLines(sortButton.x , sortButton.y, sortButton.width, sortButton.height, BLACK);
        DrawText("Tri",
         (int)(sortButton.x + sortButton.width / 2 - MeasureText("Tri", 20) / 2),
          (int)(sortButton.y + sortButton.height / 2 - 10),
           20,
            WHITE //Color
            );
// button "ajouter un element"
        DrawRectangleRec(addLIFO, GRAY);
        DrawRectangleLines(addLIFO.x , addLIFO.y, addLIFO.width, addLIFO.height, BLACK);
        DrawText("Ajouter au début",
         (int)(addLIFO.x + addLIFO.width / 2 - MeasureText("Ajouter au début", 20) / 2),
          (int)(addLIFO.y + addLIFO.height / 2 - 10),
           20,
            WHITE //Color
            );
// button "Supprimer"
        DrawRectangleRec(deleteButton, GRAY);
        DrawRectangleLines(deleteButton.x , deleteButton.y, deleteButton.width, deleteButton.height, BLACK);
        DrawText("Supprimer",
         (int)(deleteButton.x + deleteButton.width / 2 - MeasureText("Supprimer", 20) / 2),
          (int)(deleteButton.y + deleteButton.height / 2 - 10),
           20,
            WHITE //Color
            );

// button "ajouter a la fin "
        DrawRectangleRec(addFIFO, GRAY);
        DrawRectangleLines(addFIFO.x , addFIFO.y, addFIFO.width, addFIFO.height, BLACK);
        DrawText("Ajoute a la fin",
         (int)(addFIFO.x + addFIFO.width / 2 - MeasureText("Ajoute a la fin", 20) / 2),
          (int)(addFIFO.y + addFIFO.height / 2 - 10),
           20,
            WHITE //Color
            );

        DrawRectangleRec(addAtPos, GRAY);
        DrawRectangleLines(addAtPos.x , addAtPos.y, addAtPos.width, addAtPos.height, BLACK);
        DrawText("Ajoute a position donnée",
         (int)(addAtPos.x + addAtPos.width / 2 - MeasureText("Ajoute a position donnée", 20) / 2),
          (int)(addAtPos.y + addAtPos.height / 2 - 10),
           20,
            WHITE //Color
            );

// small logo
        DrawRectangleRec(logo, BLACK);
        DrawText("</>", 935, 10, 30, WHITE);
        DrawText("<\\>",935, 40, 30, WHITE);


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