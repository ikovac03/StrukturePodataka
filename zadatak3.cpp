#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct _osoba* Position;
typedef struct _osoba {
    char ime[MAX];
    char prezime[MAX];
    int godina;
    Position next;
} Osoba;

// Kreiranje novog čvora i alokacija memorije
Position kreiraj(char* ime, char* prezime, int godina) {
    Position nova = (Position)malloc(sizeof(Osoba));
    if (!nova) return NULL;
    strcpy(nova->ime, ime);
    strcpy(nova->prezime, prezime);
    nova->godina = godina;
    nova->next = NULL;
    return nova;
}

// A. Umetanje čvora na početak liste
int dodajNaPocetak(Position head, char* i, char* p, int g) {
    Position nova = kreiraj(i, p, g);
    if (!nova) return -1;
    nova->next = head->next;
    head->next = nova;
    return 0;
}

// Dodavanje na kraj liste
int dodajNaKraj(Position head, char* i, char* p, int g) {
    while (head->next) head = head->next;
    return dodajNaPocetak(head, i, p, g);
}

// Traženje osobe po prezimenu
Position nadji(Position first, char* prezime) {
    while (first && strcmp(first->prezime, prezime) != 0) first = first->next;
    return first;
}

// Traženje PRETHODNIKA (nužno za brisanje i dodavanje ispred)
Position nadjiPrethodni(Position head, char* prezime) {
    while (head->next && strcmp(head->next->prezime, prezime) != 0) head = head->next;
    return head->next ? head : NULL;
}

// Brisanje elementa iz liste preko prethodnika
int obrisi(Position head, char* prezime) {
    Position prev = nadjiPrethodni(head, prezime);
    if (!prev) return -1;
    Position temp = prev->next;
    prev->next = temp->next;
    free(temp);
    return 0;
}

// Čišćenje cijele liste iz memorije
int ocisti(Position head) {
    while (head->next) obrisi(head, head->next->prezime);
    return 0;
}

// A. Dodavanje iza određenog elementa
int dodajIza(Position head, char* trazeno, char* i, char* p, int g) {
    Position target = nadji(head->next, trazeno);
    return target ? dodajNaPocetak(target, i, p, g) : -1;
}

// B. Dodavanje ispred određenog elementa
int dodajIspred(Position head, char* trazeno, char* i, char* p, int g) {
    Position prev = nadjiPrethodni(head, trazeno);
    return prev ? dodajNaPocetak(prev, i, p, g) : -1;
}

// C. Sortiranje liste po prezimenima (Bubble sort)
int sortiraj(Position head) {
    Position p, end = NULL;
    int swapped;
    if (!head->next) return 0;
    do {
        swapped = 0;
        p = head;
        while (p->next->next != end) {
            if (strcmp(p->next->prezime, p->next->next->prezime) > 0) {
                Osoba temp = *p->next;
                *p->next = *p->next->next;
                *p->next->next = temp;

                Position t = p->next->next;
                p->next->next = temp.next;
                temp.next = t;
                swapped = 1;
            }
            p = p->next;
        }
        end = p->next;
    } while (swapped);
    return 0;
}

// D. Upisivanje liste u datoteku
int zapisi(Position p,const char* file) {
    FILE* fp = fopen(file, "w");
    if (!fp) return -1;
    while (p) {
        fprintf(fp, "%s %s %d\n", p->ime, p->prezime, p->godina);
        p = p->next;
    }
    fclose(fp);
    return 0;
}

// E. Čitanje liste iz datoteke
int ucitaj(Position head,const char* file) {
    FILE* fp = fopen(file, "r");
    if (!fp) return -1;
    ocisti(head);
    char i[MAX], p[MAX];
    int g;
    while (fscanf(fp, "%s %s %d", i, p, &g) == 3)
        dodajNaKraj(head, i, p, g);
    fclose(fp);
    return 0;
}

// Ispis svih elemenata
void ispisi(Position p) {
    if (!p) printf("\t[Prazna lista]\n");
    while (p) {
        printf("\t%s %s (%d.)\n", p->ime, p->prezime, p->godina);
        p = p->next;
    }
}

int main() {
    Osoba head = { "", "", 0, NULL }, headF = { "", "", 0, NULL };
    char ch = 0, p[MAX] = { 0 }, p2[MAX] = { 0 }, i[MAX] = { 0 };
    int g = 0;

    do {
        printf("\n[f-pocetak, e-kraj, a-iza, b-ispred, s-sort, p-ispis, d-brisi, w-pisi, r-citaj, 0-izlaz]: ");
        if (scanf(" %c", &ch) != 1) break;
        switch (ch) {
        case 'f':
            printf("Ime Prezime Godina: ");
            scanf("%s %s %d", i, p, &g);
            dodajNaPocetak(&head, i, p, g);
            break;
        case 'e':
            printf("Ime Prezime Godina: ");
            scanf("%s %s %d", i, p, &g);
            dodajNaKraj(&head, i, p, g);
            break;
        case 'a':
            printf("Iza koga (prezime)? ");
            scanf("%s", p2);
            printf("Novo Ime Prezime Godina: ");
            scanf("%s %s %d", i, p, &g);
            dodajIza(&head, p2, i, p, g);
            break;
        case 'b':
            printf("Ispred koga (prezime)? ");
            scanf("%s", p2);
            printf("Novo Ime Prezime Godina: ");
            scanf("%s %s %d", i, p, &g);
            dodajIspred(&head, p2, i, p, g);
            break;
        case 's':
            sortiraj(&head);
            printf("Sortirano!\n");
            break;
        case 'p':
            ispisi(head.next);
            break;
        case 'd':
            printf("Prezime za brisanje: ");
            scanf("%s", p);
            obrisi(&head, p);
            break;
        case 'w':
            zapisi(head.next, "osobe.txt");
            printf("Spremljeno u datoteku.\n");
            break;
        case 'r':
            ucitaj(&headF, "osobe.txt");
            printf("Ucitano:\n");
            ispisi(headF.next);
            break;
        }
    } while (ch != '0');

    ocisti(&head);
    ocisti(&headF);
    return 0;
}