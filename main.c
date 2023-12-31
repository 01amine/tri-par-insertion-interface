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

Rectangle button = { 300, 600, 100, 70 };
bool buttonPressed = false;

void main() {
    int val;
    InitWindow(1000, 1000, "Tri-par-Insertion-Interface");
    IsWindowResized();
    MaximizeWindow();

    Node* head = NULL;
    head = insertAtBeginning(head, 10);
    head = insertAtBeginning(head, 40);
    head = insertAtBeginning(head, 30);
    head = insertAtBeginning(head, 60);
    head = insertAtBeginning(head, -10);
    head = insertAtBeginning(head, -9);
    head = insertAtBeginning(head, 70);
    head = insertAtBeginning(head, -100);
    head = insertAtBeginning(head, 68);

    Node** steps;
    int stepCount;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        Vector2 mousePosition = GetMousePosition();

        if (CheckCollisionPointRec(mousePosition, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            buttonPressed = !buttonPressed;
            if (buttonPressed) {
                // Compter le nombre d'éléments dans la liste
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

        /
        if (buttonPressed) {
            for (int i = 0; i < stepCount; i++) {
                displayList(steps[i], 100, 100 + i * 100);
            }
            DrawText("Tri en cours...", 10, 70, 20, RED);
        } else {
            displayList(head, 100, 100);
        }

        DrawRectangleRec(button, buttonPressed ? DARKGRAY : GRAY);
        DrawText("Tri", (int)(button.x + button.width / 2 - MeasureText("Tri", 20) / 2), (int)(button.y + button.height / 2 - 10), 20, WHITE);

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
