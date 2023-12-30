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



void main(){
    InitWindow(800,800,"Project");
    IsWindowResized();
    MaximizeWindow();
Node* head = NULL;
    head = insertAtBeginning(head, 10);
    head = insertAtBeginning(head, 20);
    head = insertAtBeginning(head, 30);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        displayList(head);
        EndDrawing();
        
    }
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
    CloseWindow();
}
