#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct monom {
    int k; // koeficijent
    int e; // eksponent
    struct monom* next;
} Monom;

// ubacuje i sortira da radi i kad nisu sortirani u datoteci
void ubaci(Monom* head, int koef, int eks) {
    if (koef == 0) return;

    Monom* novi = (Monom*)malloc(sizeof(Monom));
    novi->k = koef;
    novi->e = eks;
    novi->next = NULL;

    Monom* p = head;
    while (p->next != NULL && p->next->e > eks) {
        p = p->next;
    }

    if (p->next != NULL && p->next->e == eks) {
        p->next->k += koef;
        free(novi);
        if (p->next->k == 0) {
            Monom* temp = p->next;
            p->next = temp->next;
            free(temp);
        }
    }
    else {
        novi->next = p->next;
        p->next = novi;
    }
}

void ucitaj(Monom* head, FILE* f) {
    int n, koef, eks;
    if (fscanf(f, "%d", &n) != 1) return;
    for (int i = 0; i < n; i++) {
        fscanf(f, "%d %d", &koef, &eks);
        ubaci(head, koef, eks);
    }
}

void ispisi(Monom* head) {
    Monom* curr = head->next;
    if (curr == NULL) {
        printf("0\n");
        return;
    }
    int prvi = 1;
    while (curr != NULL) {
        if (!prvi && curr->k > 0) printf(" + ");
        else if (curr->k < 0) printf(" - ");

        int val = curr->k < 0 ? -curr->k : curr->k;
        if (curr->e == 0) printf("%d", val);
        else if (curr->e == 1) printf("%dx", val);
        else printf("%dx^%d", val, curr->e);

        prvi = 0;
        curr = curr->next;
    }
    printf("\n");
}

void zbroji(Monom* res, Monom* p1, Monom* p2) {
    Monom* curr = p1->next;
    while (curr != NULL) {
        ubaci(res, curr->k, curr->e);
        curr = curr->next;
    }
    curr = p2->next;
    while (curr != NULL) {
        ubaci(res, curr->k, curr->e);
        curr = curr->next;
    }
}

void pomnozi(Monom* res, Monom* p1, Monom* p2) {
    Monom* i = p1->next;
    while (i != NULL) {
        Monom* j = p2->next;
        while (j != NULL) {
            ubaci(res, i->k * j->k, i->e + j->e);
            j = j->next;
        }
        i = i->next;
    }
}

void oslobodi(Monom* head) {
    while (head->next != NULL) {
        Monom* t = head->next;
        head->next = t->next;
        free(t);
    }
}

int main() {
    FILE* f = fopen("polynomials.txt", "r");
    if (f == NULL) {
        printf("greska pri otvaranju datoteke\n");
        return 1;
    }

    Monom p1 = { 0, 0, NULL };
    Monom p2 = { 0, 0, NULL };
    Monom zbroj = { 0, 0, NULL };
    Monom umnozak = { 0, 0, NULL };

    ucitaj(&p1, f);
    ucitaj(&p2, f);
    fclose(f);

    printf("P1: "); ispisi(&p1);
    printf("P2: "); ispisi(&p2);

    zbroji(&zbroj, &p1, &p2);
    printf("Zbroj: "); ispisi(&zbroj);

    pomnozi(&umnozak, &p1, &p2);
    printf("Umnozak: "); ispisi(&umnozak);

    oslobodi(&p1);
    oslobodi(&p2);
    oslobodi(&zbroj);
    oslobodi(&umnozak);

    return 0;
}
