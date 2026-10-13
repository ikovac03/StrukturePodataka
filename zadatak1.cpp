#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _student{
    char ime[20];
    char prezime[30];
    float bodovi;
} stud;

int main() {
    int i = 0;
    char buffer[50] = { 0 };
    int brojac = 0;

    FILE* fp = fopen("studenti.txt", "r");
    if (!fp) {
        printf("Greska! Datoteka studenti.txt nije pronadena.\n");
        return -1;
    }

    while (fgets(buffer,50,fp)!=NULL) {
        brojac++;
    }
    rewind(fp);

    stud* studenti = NULL;
    studenti = (stud*)malloc(brojac * sizeof(stud));

    if (!studenti) {
        printf("Greska pri alokaciji memorije!\n");
        fclose(fp);
        return -1;
    }

    for (i = 0; i < brojac; i++) {
        (void)fscanf(fp, "%s %s %f", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
    }
    
    fclose(fp);
    
    float max_bodovi = studenti[0].bodovi;
    for (i = 1; i < brojac; i++) {
        if (studenti[i].bodovi > max_bodovi) {
            max_bodovi = studenti[i].bodovi;
        }
    }

    printf("\n REZULTATI STUDENATA \n");
    for (i = 0; i < brojac; i++) {
        
        float relativni_bodovi = 0;
        if (max_bodovi > 0) {
            relativni_bodovi = (studenti[i].bodovi / max_bodovi) * 100.0f;
        }

        printf("Ime: %s | Prezime: %s | Apsolutno: %f | Relativno: %.2f%%\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            relativni_bodovi);
    }

    free(studenti);

    return 0;
}