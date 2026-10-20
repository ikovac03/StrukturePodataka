#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Person* Position;
typedef struct _Person {
    char ime[50];
    char prezime[50];
    int godina;
    Position next;
} Person;

// Pomo?na funkcija za stvaranje novog ?vora i alokaciju memorije
Position createPerson(const char* ime,const char* prezime, int godina) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }
    strcpy(newPerson->ime, ime);
    strcpy(newPerson->prezime, prezime);
    newPerson->godina = godina;
    newPerson->next = NULL;

    return newPerson;
}

// Pomo?na funkcija za umetanje ?vora iza zadanog ?vora p
int insertAfter(Position p, Position newPerson) {
    newPerson->next = p->next;
    p->next = newPerson;
    return 0;
}

// A. Dinami?ko dodavanje na po?etak liste (iza head elementa)
int insertBegin(Position head,const char* ime,const char* prezime, int godina) {
    Position newPerson = createPerson(ime, prezime, godina);
    if (newPerson == NULL) return -1;

    insertAfter(head, newPerson);
    return 0;
}

// B. Ispis liste
int printList(Position p) {
    printf("\nLISTA OSOBA\n");
    if (p == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    while (p != NULL) {
        printf("Ime: %s  Prezime: %s  Godina: %d\n", p->ime, p->prezime, p->godina);
        p = p->next;
    }
    printf("---------\n");
    return 0;
}

// C. Dinami?ko dodavanje na kraj liste
int insertEnd(Position head,const char* ime,const char* prezime, int godina) {
    Position newPerson = createPerson(ime, prezime, godina);
    if (newPerson == NULL) return -1;

    Position p = head;
    
    while (p->next != NULL) {
        p = p->next;
    }

    insertAfter(p, newPerson);
    return 0;
}

// D. Pronalaženje elementa po prezimenu
Position findLastName(Position head,const char* prezime) {
    Position p = head->next; // Preska?emo prazno zaglavlje

    while (p != NULL) {
        if (strcmp(p->prezime, prezime) == 0) {
            return p; // Vra?amo pokaziva? na prona?enu osobu
        }
        p = p->next;
    }
    return NULL;
}

// Pomo?na funkcija za pronalaženje PRETHODNIKA
Position findPrevious(Position head,const char* prezime) {
    Position p = head;

    while (p->next != NULL) {
        if (strcmp(p->next->prezime, prezime) == 0) {
            return p; 
        }
        p = p->next;
    }
    return NULL;
}

// E. Brisanje odre?enog elementa iz liste po prezimenu
int deletePerson(Position head,const char* prezime) {
    Position prev = findPrevious(head, prezime);

    if (prev == NULL) {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", prezime);
        return -1;
    }

    Position curr = prev->next;
    prev->next = curr->next; 
    free(curr);              

    printf("Osoba s prezimenom '%s' je uspjesno obrisana.\n", prezime);
    return 0;
}


int main() {
   
    Person head = { "","",0,NULL };

    
    insertBegin(&head, "Ana", "Anic", 2002);
    insertBegin(&head, "Ivo", "Ivic", 1999);

    insertEnd(&head, "Marko", "Markic", 2001);
   
    printList(head.next);

    // Testiranje D: Traženje po prezimenu
    Position nadjen = findLastName(&head, "Anic");
    if (nadjen != NULL) {
        printf("\n[Trazenje] Prona?ena osoba: %s %s (%d.)\n", nadjen->ime, nadjen->prezime, nadjen->godina);
    }
    else {
        printf("\n[Trazenje] Osoba nije prona?ena.\n");
    }

    // Testiranje E: Brisanje
    deletePerson(&head, "Anic");

    // Ispis nakon brisanja
    printList(head.next);

    // ?iš?enje memorije prije izlaza
    Position p = head.next;
    while (p != NULL) {
        Position temp = p;
        p = p->next;
        free(temp);
    }
    head.next = NULL;

    return 0;
}