#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH (50)

typedef struct _Person* Position;
typedef struct _Person {
    char name[MAX_LENGTH];
    char surname[MAX_LENGTH];
    int  birthYear;
    Position next;
} Person;

int appendList(Position head);
int addToEndOfList(Position head);
int printList(Position first);
int findPerson(Position head, const char* surname);
int deletePerson(Position head, const char* surname);
Position createPerson(void);
Position findLast(Position head);
int printPerson(Position person);
int menu(Position head);

int main(void)
{
    Person Head = { .name = {0}, .surname = {0}, .birthYear = 0, .next = NULL };
    menu(&Head);
    return EXIT_SUCCESS;
}

Position createPerson(void)
{
    Position newPerson = (Position)malloc(sizeof(Person));
    if (!newPerson) {
        perror("Neuspjesna alokacija memorije");
        return NULL;
    }

    printf("Upisi ime:\n");
    scanf("%49s", newPerson->name);
    printf("Upisi prezime:\n");
    scanf("%49s", newPerson->surname);
    printf("Upisi godinu rodenja:\n");
    scanf("%d", &newPerson->birthYear);

    newPerson->next = NULL;
    return newPerson;
}

int appendList(Position head)
{
    Position newPerson = createPerson();
    if (!newPerson) return EXIT_FAILURE;

    newPerson->next = head->next;
    head->next = newPerson;
    return EXIT_SUCCESS;
}

Position findLast(Position head)
{
    while (head->next != NULL)
        head = head->next;
    return head;
}

int addToEndOfList(Position head)
{
    Position newPerson = createPerson();
    if (!newPerson) return EXIT_FAILURE;

    Position last = findLast(head);
    last->next = newPerson;   // newPerson->next je već NULL
    return EXIT_SUCCESS;
}

int printList(Position first)
{
    if (!first) {
        printf("Lista je prazna.\n");
        return EXIT_SUCCESS;
    }
    for (; first != NULL; first = first->next)
        printf("Name: %s\t Surname: %s\t Birth year: %d\n",
            first->name, first->surname, first->birthYear);
    return EXIT_SUCCESS;
}

int findPerson(Position head, const char* surname)
{
    Position cur = head->next;
    while (cur && strcmp(cur->surname, surname) != 0)
        cur = cur->next;

    if (!cur) {
        printf("Nema osobe s prezimenom '%s'.\n", surname);
        return -1;
    }
    printPerson(cur);
    return EXIT_SUCCESS;
}

int deletePerson(Position head, const char* surname)
{
    Position prev = head;
    Position cur = head->next;

    while (cur && strcmp(cur->surname, surname) != 0) {
        prev = cur;
        cur = cur->next;
    }

    if (!cur) {
        printf("Nema osobe s prezimenom '%s'.\n", surname);
        return -1;
    }

    printPerson(cur);
    prev->next = cur->next;
    free(cur);
    printf("Obrisano.\n");
    return EXIT_SUCCESS;
}

int printPerson(Position person)
{
    printf("Name: %s, surname: %s, birth year: %d, address: %p\n",
        person->name, person->surname, person->birthYear, (void*)person);
    return EXIT_SUCCESS;
}

int menu(Position Head)
{
    char choice = '\0';
    char surname[MAX_LENGTH];

    while (1) {
        printf("Enter A(Append), E(End), P(Print), S(Search), D(Delete), X(Exit)\n");
        scanf(" %c", &choice);
        switch (tolower((unsigned char)choice)) {
        case 'a':
            appendList(Head);
            break;
        case 'e':
            addToEndOfList(Head);
            break;
        case 'p':
            printList(Head->next);
            break;
        case 's':
            printf("Unesi prezime za pretragu: ");
            scanf("%49s", surname);
            findPerson(Head, surname);
            break;
        case 'd':
            printf("Unesi prezime za brisanje: ");
            scanf("%49s", surname);
            deletePerson(Head, surname);
            break;
        case 'x':
            return EXIT_SUCCESS;
        default:
            printf("Pogresan unos!\n");
        }
    }
}
