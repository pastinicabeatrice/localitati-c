#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* denumire;
	char* judet;
	unsigned int populatie;
	char* tipLocalitate;
	float buget;
} Localitate;

typedef struct Nod {
	Localitate inf;
	struct Nod* next;
	struct Nod* prev;
} Nod;

char* copiereSir(const char* s) {
	char* r = (char*)malloc(strlen(s) + 1);
	strcpy(r, s);
	return r;
}

Nod* inserare(Nod* cap, Nod** coada, Localitate l) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->inf.denumire = copiereSir(l.denumire);
	nou->inf.judet = copiereSir(l.judet);
	nou->inf.tipLocalitate = copiereSir(l.tipLocalitate);
	nou->inf.populatie = l.populatie;
	nou->inf.buget = l.buget;
	nou->next = NULL;
	nou->prev = *coada;
	if (cap == NULL) {
		cap = nou;
	}
	else {
		(*coada)->next = nou;
	}
	*coada = nou;
	return cap;
}

void afisareLocalitate(Localitate l) {
	printf("%-12s | judet %-8s | %9u locuitori | %-9s | buget %10.2f mii lei\n",
		l.denumire, l.judet, l.populatie, l.tipLocalitate, l.buget);
}

void traversare(Nod* cap) {
	for (Nod* t = cap; t != NULL; t = t->next) {
		afisareLocalitate(t->inf);
	}
}

void traversareInversa(Nod* coada) {
	for (Nod* t = coada; t != NULL; t = t->prev) {
		afisareLocalitate(t->inf);
	}
}

unsigned int populatieTotala(Nod* cap, const char* tipLocalitate) {
	unsigned int total = 0;
	for (Nod* t = cap; t != NULL; t = t->next) {
		if (strcmp(t->inf.tipLocalitate, tipLocalitate) == 0) {
			total += t->inf.populatie;
		}
	}
	return total;
}

int actualizarePopulatieJudet(Nod* cap, Nod* coada, const char* judet, float procent) {
	int actualizate = 0;
	Nod* st = cap;
	Nod* dr = coada;
	while (st != NULL && dr != NULL) {
		if (strcmp(st->inf.judet, judet) == 0) {
			st->inf.populatie = (unsigned int)(st->inf.populatie * (1 + procent / 100));
			actualizate++;
		}
		if (st == dr) {
			break;
		}
		if (strcmp(dr->inf.judet, judet) == 0) {
			dr->inf.populatie = (unsigned int)(dr->inf.populatie * (1 + procent / 100));
			actualizate++;
		}
		if (st->next == dr) {
			break;
		}
		st = st->next;
		dr = dr->prev;
	}
	return actualizate;
}

void dezalocare(Nod** cap, Nod** coada) {
	Nod* t = *cap;
	while (t != NULL) {
		Nod* urm = t->next;
		free(t->inf.denumire);
		free(t->inf.judet);
		free(t->inf.tipLocalitate);
		free(t);
		t = urm;
	}
	*cap = NULL;
	*coada = NULL;
}

int main() {
	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul fisier.txt nu a putut fi deschis.\n");
		return 1;
	}

	Nod* cap = NULL;
	Nod* coada = NULL;
	char denumire[50];
	char judet[50];
	char tip[20];
	Localitate l;

	while (fscanf(f, "%49s %49s %u %19s %f", denumire, judet, &l.populatie, tip, &l.buget) == 5) {
		l.denumire = denumire;
		l.judet = judet;
		l.tipLocalitate = tip;
		cap = inserare(cap, &coada, l);
	}
	fclose(f);

	printf("Localitati:\n");
	traversare(cap);

	printf("\nLocalitati in ordine inversa:\n");
	traversareInversa(coada);

	const char* tipuri[] = { "sat", "comuna", "oras", "municipiu" };
	printf("\nPopulatie totala pe tip de localitate:\n");
	for (int i = 0; i < 4; i++) {
		printf("%-10s %u\n", tipuri[i], populatieTotala(cap, tipuri[i]));
	}

	int n = actualizarePopulatieJudet(cap, coada, "Cluj", 5.0f);
	printf("\nPopulatia a crescut cu 5%% in %d localitati din judetul Cluj:\n", n);
	traversare(cap);

	dezalocare(&cap, &coada);
	return 0;
}
