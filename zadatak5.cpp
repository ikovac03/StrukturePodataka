#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024

typedef struct Stack* Position;

typedef struct Stack {
    int broj;
    Position next;
} Stack;

Position createNode(int broj);
int Push(Position head, int broj);
int PopElement(Position head, int* result);
int PerformOperation(Position head, char operation);
int DeleteAll(Position head);
int CalculatePostfixFromFile(Position head, char* fileName);

int main() {
    Stack head = { 0, NULL };
    char fileName[] = "postfiks.txt";

    if (CalculatePostfixFromFile(&head, fileName) == EXIT_SUCCESS) {
        printf("Rezultat: %d\n", head.next ? head.next->broj : 0);
        DeleteAll(&head);
    }
    else {
        printf("Greska!\n");
    }

    return 0;
}

Position createNode(int broj) {
    Position newP = (Position)malloc(sizeof(Stack));
    if (!newP) {
        return NULL;
    }
    newP->broj = broj;
    newP->next = NULL;
    return newP;
}

// A) Dinamički dodaje novi element na početak stoga (Push)
int Push(Position head, int broj) {
    // - alokacija
    Position newP = createNode(broj);
    if (!newP) {
        return EXIT_FAILURE;
    }

    // - unos podataka i insertBegin
    newP->next = head->next;
    head->next = newP;

    return EXIT_SUCCESS;
}

int PopElement(Position head, int* result) {
    Position temp = head->next;

    if (!temp) {
        return EXIT_FAILURE;
    }

    *result = temp->broj;
    head->next = temp->next;
    free(temp);

    return EXIT_SUCCESS;
}

// B) Skida elemente sa stoga, računa rezultat i vraća ga natrag (Pop + operacija)
int PerformOperation(Position head, char operation) {
    int q = 0;
    int p = 0;
    int rezultat = 0;

    // - operacija (izračunati rezultat skidanjem operanda)
    if (PopElement(head, &q) != EXIT_SUCCESS || PopElement(head, &p) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    switch (operation) {
    case '+':
        rezultat = p + q;
        break;
    case '-':
        rezultat = p - q;
        break;
    case '*':
        rezultat = p * q;
        break;
    case '/':
        if (q == 0) return EXIT_FAILURE;
        rezultat = p / q;
        break;
    default:
        return EXIT_FAILURE;
    }

    // - Push (vraćanje rezultata na stog)
    return Push(head, rezultat);
}

// C) Oslobađanje memorije liste i obavijest o grešci
int DeleteAll(Position head) {
    Position temp = NULL;

    // - osloboditi listu
    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }

    return EXIT_SUCCESS;
}

int CalculatePostfixFromFile(Position head, char* fileName) {
    FILE* fp = NULL;
    char buffer[MAX_SIZE] = { 0 };
    char* currentBuffer = NULL;
    int bytesRead = 0;
    int number = 0;
    char operation = 0;

    // - otvorit datoteku
    fp = fopen("postfiks.txt", "r");
    if (!fp) return EXIT_FAILURE;

    // - čitat podatke
    if (!fgets(buffer, MAX_SIZE, fp)) {
        fclose(fp);
        return EXIT_FAILURE;
    }
    // - zatvorit datoteku
    fclose(fp);

    currentBuffer = buffer;

    while (strlen(currentBuffer) > 0) {
        // a) broj -> poziv funkcije Push
        if (sscanf(currentBuffer, " %d %n", &number, &bytesRead) == 1) {
            Push(head, number);
            currentBuffer += bytesRead;
        }
        // b) operacija -> poziv funkcije PerformOperation (Pop + račun + Push)
        else if (sscanf(currentBuffer, " %c %n", &operation, &bytesRead) == 1) {
            if (PerformOperation(head, operation) != EXIT_SUCCESS) {
                // c) krivi unos -> osloboditi listu i javiti grešku
                DeleteAll(head);
                return EXIT_FAILURE;
            }
            currentBuffer += bytesRead;
        }
        else {
            break;
        }
    }

    if (head->next == NULL || head->next->next != NULL) {
        // c) krivi unos -> osloboditi listu i javiti grešku
        DeleteAll(head);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}