/*3. Prethodnom zadatku dodati funkcije : A.dinamièki dodaje novi element iza odreðenog elementa, 
B.dinamièki dodaje novi element ispred odreðenog elementa, C.sortira listu po prezimenima osoba,
D.upisuje listu u datoteku, E.èita listu iz datoteke.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH (50)
#define NO_PERSON_FOUND (NULL)
#define EMPTY_LIST (-1)
#define NO_PERSON_FOUND_INT (-2)
#define SINGLE_PERSON_IN_LIST (-3)

struct _Person;
typedef struct _Person* Position;
typedef struct _Person {
	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	int birthYear;
	Position next;
} Person;

int menu(Position head);
Position createPerson();
Position findLast(Position head);
char* enterSurname();
int printPerson(Position person);
Position findPrevious(Position head);
int swapItems(Position first, Position second);
int insertAtFrontOfTheList(Position head);
int insertAtEndOfTheList(Position head);
int printList(Position firstItem);
Position findPerson(Position firstItem);
int deletePerson(Position head);
int insertAfterPerson(Position person);
int bubbleSort(Position firstItem);
int writeToFile(Position firstItem, const char* filename);
int readFromFile(Position head, const char* filename);
static void freeList(Position head);


int main() {
	Person head = { .next = NULL, .name = {0},
				.surname = {0}, .birthYear = 0 };
	menu(&head);

	return EXIT_SUCCESS;
}

int menu(Position head) {
	char choice = '\0';
	Position person = NULL;
	Position previous = NULL;
	while (1) {
		printf("Enter:  A(Add to the front of the list)\n\tE(Add to the end of list)\n\tP(Print list)\n\tS(Search)\n\tD(Delete)\n\tF(Add after item)\n\tB(Add before item)\n\tO(Sort)\n\tW(Write to file)\n\tR(Read from file)\n\tX(Exit)\n");
		scanf(" %c", &choice);
		switch (choice) {
			// add item to the front of the list
		case 'A':
		case 'a':
			insertAtFrontOfTheList(head);
			continue;
			// add item to the end of the list
		case 'E':
		case 'e':
			insertAtEndOfTheList(head);
			continue;
			// print list items
		case 'P':
		case 'p':
			printList(head->next);
			continue;
			// search item in list by surname
		case 'S':
		case 's':
			person = findPerson(head->next);
			person ? printPerson(person) : printf("Can't find person with that surname!\n");
			continue;
			// delete a item from list
		case 'D':
		case 'd':
			switch (deletePerson(head)) {
			case EXIT_SUCCESS:
				printf("Deleted!\n");
				break;
			case EMPTY_LIST:
				printf("List is empty!\n");
				break;
			case NO_PERSON_FOUND_INT:
				printf("Can't find person with that surname!\n");
				break;
			default:
				break;
			}
			continue;
			// add item to the list after some specified item
		case 'F':
		case 'f':
			person = findPerson(head->next);
			person ? insertAfterPerson(person) : printf("Can't find person with that surname!\n");
			continue;
			// add item to the list before some specified item
		case 'B':
		case 'b':
			previous = findPrevious(head);
			previous ? insertAfterPerson(previous) : printf("Can't find person with that surname!\n");
			continue;
			// sort items in list by surname
		case 'O':
		case 'o':
			switch (bubbleSort(head->next)) {
			case EXIT_SUCCESS:
				printf("Sorted!\n");
				printList(head->next);
				break;
			case EMPTY_LIST:
				printf("List is empty!\n");
				break;
			case SINGLE_PERSON_IN_LIST:
				printf("Single person in list, no use sorting!\n");
				break;
			default:
				break;
			}
			continue;
			// write items in list to the file
		case 'W':
		case 'w': {
			char filename[128];
			printf("Enter filename to write to (e.g. people.txt): ");
			scanf(" %127s", filename);
			if (writeToFile(head->next, filename) == EXIT_SUCCESS)
				printf("Written to '%s'.\n", filename);
			else
				printf("Write failed.\n");
			continue;
		}

		case 'R':
		case 'r': {
			char filename[128];
			printf("Enter filename to read from (e.g. people.txt): ");
			scanf(" %127s", filename);
			if (readFromFile(head, filename) == EXIT_SUCCESS) {
				printf("Read from '%s'.\n", filename);
				printList(head->next);
			}
			else {
				printf("Read failed.\n");
			}
			continue;
		}

		case 'X':
		case 'x':
			break;
		default:
			printf("Wrong letter!\n");
			continue;
		}
		break;
	}
	return EXIT_SUCCESS;
}

int insertAtFrontOfTheList(Position head) {
	Position newPerson = NULL;

	newPerson = createPerson();

	if (newPerson) {
		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int insertAtEndOfTheList(Position head) {
	Position newPerson = NULL;
	Position last = NULL;

	newPerson = createPerson();

	if (newPerson) {
		last = findLast(head);
		newPerson->next = last->next;
		last->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int printList(Position firstItem) {
	Position current = firstItem;

	if (!firstItem) {
		printf("Empty list!\n");
	}

	while (current) {
		printPerson(current);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

Position findPerson(Position firstItem)
{
	Position current = firstItem;
	char surname[MAX_LENGTH] = { 0 };

	if (!firstItem) {
		printf("Empty list!\n");
		return NO_PERSON_FOUND;
	}

	strcpy(surname, enterSurname());
	do {
		if (strcmp(current->surname, surname) == 0) {
			return current;
		}
		else {
			current = current->next;
		}
	} while (current != NULL);

	return NO_PERSON_FOUND;
}

int deletePerson(Position head)
{
	Position current = head;
	char surname[MAX_LENGTH] = { 0 };

	strcpy(surname, enterSurname());
	if (head->next) {
		Position previous = NULL;

		while (current->next && strcmp(current->surname, surname) != 0) {
			previous = current;
			current = current->next;
		}
		//we have to have check for "previous" so intellisense doesn't show warning
		if (previous && previous->next && strcmp(current->surname, surname) == 0) {
			printPerson(current);
			previous->next = current->next;
			free(current);
		}
		else {
			return NO_PERSON_FOUND_INT;
		}
	}
	else {
		return EMPTY_LIST;
	}

	return EXIT_SUCCESS;
}

int insertAfterPerson(Position person) {
	Position newPerson = NULL;

	newPerson = createPerson();

	if (newPerson) {
		newPerson->next = person->next;
		person->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int bubbleSort(Position firstItem) {
	int swapped = 0;
	Position lastPersonRead = NULL;
	Position start = firstItem;

	// Return if the list is empty or has only one element
	if (!firstItem) {
		printf("Empty list!\n");
		return NO_PERSON_FOUND_INT;
	}
	else if (!firstItem->next) {
		printf("Only single element in list!\n");
		return SINGLE_PERSON_IN_LIST;
	}

	do {
		swapped = 0;
		Position current = start;

		while (current->next != lastPersonRead) {
			if (strcmp(current->surname, current->next->surname) > 0) {
				swapItems(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
		lastPersonRead = current;
	} while (swapped);

	return EXIT_SUCCESS;
}

Position createPerson() {
	Position newPerson = NULL;
	char name[MAX_LENGTH] = { 0 };
	char surname[MAX_LENGTH] = { 0 };
	int birthYear = 0;

	newPerson = (Position)malloc(sizeof(Person));
	if (!newPerson) {
		printf("Can't allocate memory!\n");
		return NULL;
	}

	printf("Enter name:\n");
	scanf(" %s", name);
	printf("Enter surname:\n");
	scanf(" %s", surname);
	printf("Enter birth year:\n");
	scanf(" %d", &birthYear);


	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;

	return newPerson;
}

Position findLast(Position head) {
	Position current = head;

	while (current->next) {
		current = current->next;
	}

	return current;
}

char* enterSurname() {
	char surname[MAX_LENGTH] = { 0 };
	printf("Enter surname of the wanted person:\n");
	scanf(" %s", surname);

	return surname;
}

int printPerson(Position person) {
	printf("\033[0;32mName: %s\t Surname: %s\t Birth year: %d\t\n\033[0m", person->name, person->surname, person->birthYear);
	return EXIT_SUCCESS;
}

Position findPrevious(Position head) {
	Position current = head;
	char surname[MAX_LENGTH] = { 0 };

	if (!head->next) {
		printf("Empty list!\n");
		return NO_PERSON_FOUND;
	}

	strcpy(surname, enterSurname());
	do {
		if (strcmp(current->next->surname, surname) == 0) {
			return current;
		}
		else {
			current = current->next;
		}
	} while (current->next != NULL);

	return NO_PERSON_FOUND;
}

int swapItems(Position first, Position second) {
	char tempName[MAX_LENGTH];
	char tempSurname[MAX_LENGTH];
	int tempBirthYear;

	strcpy(tempName, first->name);
	strcpy(tempSurname, first->surname);
	tempBirthYear = first->birthYear;

	strcpy(first->name, second->name);
	strcpy(first->surname, second->surname);
	first->birthYear = second->birthYear;

	strcpy(second->name, tempName);
	strcpy(second->surname, tempSurname);
	second->birthYear = tempBirthYear;

	return EXIT_SUCCESS;
}

int writeToFile(Position firstItem, const char* filename) {
	FILE* f = fopen(filename, "w");
	if (!f) {
		perror("File open failed");
		return EXIT_FAILURE;
	}
	for (Position p = firstItem; p; p = p->next) {
		// format: name surname birthYear
		fprintf(f, "%s %s %d\n", p->name, p->surname, p->birthYear);
	}
	fclose(f);
	return EXIT_SUCCESS;
}

static void freeList(Position head) {
	Position curr = head->next;
	while (curr) {
		Position tmp = curr->next;
		free(curr);
		curr = tmp;
	}
	head->next = NULL;
}

int readFromFile(Position head, const char* filename) {
	FILE* f = fopen(filename, "r");
	if (!f) {
		perror("File open failed");
		return EXIT_FAILURE;
	}

	// oèisti staru listu
	freeList(head);

	char name[MAX_LENGTH], surname[MAX_LENGTH];
	int year;

	while (fscanf(f, " %49s %49s %d", name, surname, &year) == 3) {
		Position np = (Position)malloc(sizeof(Person));
		if (!np) {
			fclose(f);
			printf("Can't allocate memory!\n");
			return EXIT_FAILURE;
		}
		strcpy(np->name, name);
		strcpy(np->surname, surname);
		np->birthYear = year;
		np->next = NULL;

		// ubaci na kraj
		Position last = findLast(head);
		last->next = np;
	}

	fclose(f);
	return EXIT_SUCCESS;
}
