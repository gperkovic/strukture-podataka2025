﻿﻿/*2. Definirati strukturu osoba(ime, prezime, godina rođenja) i napisati program koji :
A.dinamički dodaje novi element na početak liste,
B.ispisuje listu,
C.dinamički dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH (50)
;
typedef struct _Person* Position;
typedef struct _Person {
	
	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	int birthyear;
	Position next;
}Person;

int appendList(Position head);
int addToEndOfList(Position head);
int printList(Position first);
int findPerson(Position head);
int deletePerson(Position head);
Position createPerson();
Position findLast(Position head);
int printPerson(Position head);
int menu(Position head);
char* wishedSurname();

int main(int argc, char** argv){
	
	Person Head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };

	menu(&Head);

	return EXIT_SUCCESS;
}

Position createPerson() {

	Position newPerson = NULL;
	char name[MAX_LENGTH] = { 0 };
	char surname[MAX_LENGTH] = { 0 };
	int birthYear = 0;

	newPerson = (Position)malloc(sizeof(Person));
	if (!newPerson)
	{
		perror("Neuspjesna alokacija memorije\n");
		return NULL;
	}

	printf("Upisi ime:\n");
	scanf("%s", name);
	printf("Upisi prezime:\n");
	scanf("%s", surname);
	printf("Upisi godinu rodenja:\n");
	scanf("%d", &birthYear);

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthyear = birthYear;

	return newPerson;
}

int appendList(Position head) {

	Position newPerson = NULL;

	newPerson = createPerson();

	if (newPerson)
	{
		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}