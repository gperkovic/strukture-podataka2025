#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct article Article;
typedef Article* Position_A;
struct article {
    char name[50];
    int quantity;
    double price;
    Position_A next;
};

typedef struct {
    int year;
    int month;
    int day;
} date;

typedef struct invoice Invoice;
typedef Invoice* Position_I;
struct invoice {
    date invoice_date;
    Position_A head_A;  // Dummy head za listu artikala
    Position_I next;
};

typedef struct summary Summary;
typedef Summary* Position_S;
struct summary {
    char name[50];
    int total_quantity;
    double total_price;
    Position_S next;
};

int read_from_file(Position_I head_I);
int add_invoice_sorted(Position_I head_I, char file_name[]);
int add_article_sorted(Position_A head_A, char name[], int quantity, double price);
int request(Position_I head_I);
int cmp_date(date a, date b);
int best_and_worst_article(Position_I head_I);
void free_all(Position_I head_I);

int main(void)
{
    Invoice head_I = { 0 };
    head_I.head_A = NULL;
    head_I.next = NULL;

    if (read_from_file(&head_I)) {
        printf("Program neuspjesan.\n");
        free_all(&head_I);
        return 1;
    }

    if (request(&head_I)) {
        printf("Zahtjev nauspjesan.\n");
    }

    if (best_and_worst_article(&head_I)) {
        printf("Neuspjelo trazenje najprodavanijeg / najmanje prodanog artikla.\n");
    }

    free_all(&head_I);
    return 0;
}

// Usporedba datuma: neg(-) ako je a<b, 0 ako su jednaki, poz(+) ako je a>b
int cmp_date(date a, date b)
{
    if (a.year != b.year)
        return a.year - b.year;
    if (a.month != b.month)
        return a.month - b.month;
    return a.day - b.day;
}

int read_from_file(Position_I head_I)
{
    char file_name[50];

    FILE* file_pointer = fopen("racuni.txt", "r");
    if (file_pointer == NULL) {
        printf("ERROR: Neuspjesno otvaranje racuni.txt\n");
        return 1;
    }

    // Citanje imena datoteka racuna
    while (fscanf(file_pointer, " %49s", file_name) == 1) {
        if (add_invoice_sorted(head_I, file_name)) {
            printf("ERROR: Neuspjesno dodavanje racuna.\n");
            fclose(file_pointer);
            return 1;
        }
    }

    fclose(file_pointer);
    return 0;
}

int add_invoice_sorted(Position_I head_I, char file_name[])
{
    char article_name[50];
    int quantity;
    double price;

    Position_I temp = head_I;

    Position_I new_invoice = (Position_I)malloc(sizeof(Invoice));
    if (new_invoice == NULL) {
        printf("ERROR: Neuspjesna alokacija memorije za racun.\n");
        return 1;
    }

    new_invoice->head_A = (Position_A)malloc(sizeof(Article));
    if (new_invoice->head_A == NULL) {
        printf("ERROR: Neuspjesna alokacija memorije za head_A.\n");
        free(new_invoice);
        return 1;
    }

    // Inicijalizacija dummy head-a za artikle
    new_invoice->head_A->name[0] = '\0';
    new_invoice->head_A->quantity = 0;
    new_invoice->head_A->price = 0.0;
    new_invoice->head_A->next = NULL;

    new_invoice->next = NULL;
    new_invoice->invoice_date.year = 0;
    new_invoice->invoice_date.month = 0;
    new_invoice->invoice_date.day = 0;

    FILE* file_pointer = fopen(file_name, "r");
    if (file_pointer == NULL) {
        printf("ERROR: Neuspjesno otvaranje datoteke racuna: %s.\n", file_name);
        free(new_invoice->head_A);
        free(new_invoice);
        return 1;
    }

    // Citanje datuma rauna 
    if (fscanf(file_pointer, " %d-%d-%d\n",
        &new_invoice->invoice_date.year,
        &new_invoice->invoice_date.month,
        &new_invoice->invoice_date.day) != 3) {
        printf("ERROR: Neuspjesno citanje datuma racuna %s.\n", file_name);
        fclose(file_pointer);
        free(new_invoice->head_A);
        free(new_invoice);
        return 1;
    }

    // Citanje artikala
    while (fscanf(file_pointer, " %49[^,], %d, %lf\n", article_name, &quantity, &price) == 3) {
        if (add_article_sorted(new_invoice->head_A, article_name, quantity, price)) {
            printf("ERROR: Neuspjesno dodavanje artikla.\n");
            fclose(file_pointer);
            free(new_invoice->head_A);
            free(new_invoice);
            return 1;
        }
    }

    fclose(file_pointer);

    // Umetanje racuna u sortiranu listu po datumu-uzlazno
    if (head_I->next != NULL) {
        while (temp->next != NULL &&
            cmp_date(temp->next->invoice_date, new_invoice->invoice_date) <= 0) {
            temp = temp->next;
        }
    }

    new_invoice->next = temp->next;
    temp->next = new_invoice;

    return 0;
}

int add_article_sorted(Position_A head_A, char name[], int quantity, double price)
{
    Position_A temp = head_A;
    Position_A new_article = (Position_A)malloc(sizeof(Article));
    if (new_article == NULL) {
        printf("ERROR: Neuspjesna alokacija memorije za novi artikal.\n");
        return 1;
    }

    strncpy(new_article->name, name, 49);
    new_article->name[49] = '\0';
    new_article->quantity = quantity;
    new_article->price = price;
    new_article->next = NULL;

    // Umetanje sortirano po nazivu artikla
    if (head_A->next != NULL) {
        while (temp->next != NULL && strcmp(new_article->name, temp->next->name) > 0) {
            temp = temp->next;
        }
    }

    new_article->next = temp->next;
    temp->next = new_article;

    return 0;
}

int request(Position_I head_I)
{
    char name[50];
    date start;
    date finish;

    int total_quantity = 0;
    double total_price = 0.0;
    int found = 0;

    Position_I temp = head_I->next;

    printf("\nUnesite ime artikla: ");
    // Citanje cijele linije 
    scanf(" %49[^\n]", name);

    printf("\nUnesite pocetni datum (dan mjesec godina): ");
    scanf("%d %d %d", &start.day, &start.month, &start.year);

    printf("\nUnesite konacni datum (dan mjesec godina): ");
    scanf("%d %d %d", &finish.day, &finish.month, &finish.year);

    if (cmp_date(start, finish) > 0) {
        date tmp = start;
        start = finish;
        finish = tmp;
    }

    // Lista racuna je sortirana po datumu uzlazno
    while (temp != NULL && cmp_date(temp->invoice_date, finish) <= 0) {

        if (cmp_date(temp->invoice_date, start) >= 0) {

            Position_A temp_A = temp->head_A->next;

            while (temp_A != NULL) {
                if (strcmp(temp_A->name, name) == 0) {
                    found = 1;
                    total_quantity += temp_A->quantity;
                    total_price += (temp_A->price * temp_A->quantity);
                }
                temp_A = temp_A->next;
            }
        }

        temp = temp->next;
    }

    if (!found) {
        printf("Trazeni proizvod nije pronaden u zadanom razdoblju.\n");
        return 1;
    }

    printf("Ukupno kupljena kolicina: %d\n", total_quantity);
    printf("Ukupno potroseni iznos: %.2lf\n", total_price);

    return 0;
}

int best_and_worst_article(Position_I head_I)
{
    Summary head_S = { 0 };
    head_S.next = NULL;

    Position_I current_inv = head_I->next;
    Position_S tempS = NULL;
    Position_S newS = NULL;

    // Skupljanje svih artikala u pomocnu listu summary
    while (current_inv != NULL) {
        Position_A current_art = current_inv->head_A->next;

        while (current_art != NULL) {

            tempS = &head_S;
            while (tempS->next != NULL && strcmp(tempS->next->name, current_art->name) != 0) {
                tempS = tempS->next;
            }

            if (tempS->next == NULL) {
                newS = (Position_S)malloc(sizeof(Summary));
                if (newS == NULL) {
                    printf("ERROR: Neuspjesna alokacija za summary.\n");

                    // Oslobodanje sve dosad napravljene summary cvorove
                    Position_S del = head_S.next;
                    while (del != NULL) {
                        Position_S nextDel = del->next;
                        free(del);
                        del = nextDel;
                    }
                    return 1;
                }

                strncpy(newS->name, current_art->name, 49);
                newS->name[49] = '\0';
                newS->total_quantity = 0;
                newS->total_price = 0.0;
                newS->next = NULL;

                tempS->next = newS;
            }

            tempS = tempS->next;
            tempS->total_quantity += current_art->quantity;
            tempS->total_price += current_art->price * current_art->quantity;

            current_art = current_art->next;
        }

        current_inv = current_inv->next;
    }

    // Provjera ako nema artikala
    if (head_S.next == NULL) {
        printf("Nema artikala u racunima.\n");
        return 1;
    }

    // Trazenje najprodavanijeg i najmanje prodanog
    Position_S best = head_S.next;
    Position_S worst = head_S.next;
    Position_S current = head_S.next->next;

    while (current != NULL) {
        if (current->total_quantity > best->total_quantity) {
            best = current;
        }
        if (current->total_quantity < worst->total_quantity) {
            worst = current;
        }
        current = current->next;
    }

    // Ispis rezultata
    printf("\nNajprodavaniji artikl:\n");
    printf("Naziv: %s\n", best->name);
    printf("Ukupna kolicina: %d\n", best->total_quantity);
    printf("Ukupno potroseno: %.2lf\n", best->total_price);

    printf("\nNajmanje prodani artikl:\n");
    printf("Naziv: %s\n", worst->name);
    printf("Ukupna kolicina: %d\n", worst->total_quantity);
    printf("Ukupno potroseno: %.2lf\n", worst->total_price);

    // Oslobodanje summary liste
    current = head_S.next;
    while (current != NULL) {
        Position_S nextS = current->next;
        free(current);
        current = nextS;
    }

    return 0;
}

void free_all(Position_I head_I)
{
    Position_I current_inv = head_I->next;
    while (current_inv != NULL) {
        Position_I next_inv = current_inv->next;

        Position_A current_art = current_inv->head_A;
        while (current_art != NULL) {
            Position_A next_art = current_art->next;
            free(current_art);
            current_art = next_art;
        }

        free(current_inv);
        current_inv = next_inv;
    }

    head_I->next = NULL;
}
