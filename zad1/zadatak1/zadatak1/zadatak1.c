#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE (128)
#define MAX_LEN (1024)
#define MAX_POINTS (15)

typedef struct _student {
	
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	double points;

}student;

int readNoRowsInFile();
student* allocateMemoryAndReadStudents(int noStudents);
double calculateRelativePoints(double points);
int showStudents(int noStudents, student* students);

int main()
{
	int noRows = 0;
	student* students = NULL;

	noRows = readNoRowsInFile();
	if (noRows > 0)
	{
		students = allocateMemoryAndReadStudents(noRows);
		showStudents(noRows, students);

		free(students);
	}

	return 0;
}

int readNoRowsInFile() {

	int counter = 0;
	FILE* filepointer = NULL;
	char buffer[MAX_SIZE] = { 0 };

	filepointer = fopen("student.txt", "r");
	if (!filepointer) {
		printf("Neuspjesno otvaranje datoteke!\n");
		return -1;
	}

	while (!feof(filepointer)) {
		fgets(buffer, MAX_SIZE, filepointer);
		counter++;
	}

	fclose(filepointer);

	return counter;
}

student* allocateMemoryAndReadStudents(int noStudents) {
	
	int counter = 0;
	FILE* filepointer = NULL;
	student* students = NULL;


	students = (student*)malloc(noStudents * sizeof(student));
	if (!students) {
		printf("Neuspjesna alokacija memorije!\n");
		return NULL;
	}

	filepointer = fopen("student.txt", "r");
	if (!filepointer) {
		printf("Neuspjesno otvaranje datoteke!\n");
		return NULL;
	}

	while (!feof(filepointer))
	{
		fscanf(filepointer, " %s %s %lf", students[counter].name, students[counter].surname, &students[counter].points);
		counter++;
	}

	fclose(filepointer);

	return students;
}

double calculateRelativePoints(double points)
{
	return ((points / MAX_POINTS) * 100);
}

int showStudents(int noStudents, student* students)
{
	int counter = 0;

	for (counter; counter < noStudents; counter++)
	{
		printf("Name: %s\t Surname: %s\t Absolute points: %.2lf\t Relative points: %.2lf%%\t\n", students[counter].name,
			students[counter].surname, students[counter].points, calculateRelativePoints(students[counter].points));
	}

	return 0;
}