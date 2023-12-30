#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>
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
void displayList(Node* head) {
    int x = 100;
    int y = 100;
    int rowHeight = 50;

    while (head != NULL) {
        char text[10];
        sprintf(text, "%d", head->data);
     DrawEllipseLines(x, y, 35, 15, BLACK);
        DrawText(text, x-5, y-8, 20, BLACK);
        head = head->next;
        y += rowHeight;
    }
}
void insertionSort(Node** head) {
    Node* sorted = NULL;
    Node* current = *head;

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
        current = next;
    }

    *head = sorted;
}

Rectangle button = { 300, 600, 100, 70 };
    bool buttonPressed = false;

void main(){
    InitWindow(800,800,"Project");
    IsWindowResized();
    MaximizeWindow();
Node* head = NULL;
    head = insertAtBeginning(head, 10);
    head = insertAtBeginning(head, 40);
    head = insertAtBeginning(head, 30);



    while (!WindowShouldClose())
{
    BeginDrawing();
    ClearBackground(WHITE);
    Vector2 mousePosition = GetMousePosition();

    if (CheckCollisionPointRec(mousePosition, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        buttonPressed = !buttonPressed; // Inverser l'état du bouton
        if (buttonPressed) {
            insertionSort(&head); // Tri seulement lorsque le bouton est pressé
        }
    }

  displayList(head);

    DrawRectangleRec(button, buttonPressed ? DARKGRAY : GRAY);
    DrawText("trie", (int)(button.x + button.width / 2 - MeasureText("trie", 20) / 2), (int)(button.y + button.height / 2 - 10), 20, WHITE);

    EndDrawing();
}

    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
    CloseWindow();
}
