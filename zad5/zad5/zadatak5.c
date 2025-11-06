/*5. Napisati program koji iz datoteke èita postfiks izraz i zatim korištenjem stoga raèuna rezultat. Stog je potrebno realizirati preko vezane liste.*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOCATION_FAILURE -1
#define FILE_OPEN_FAILURE -2
#define MAX 1024

struct _List;
typedef struct _List* Position;
typedef struct _List {
    double number;
    Position next;
} List;

int pushToStart(Position Head, double number);
int readFromFile(char fileName[], Position Stack);
int pushAfter(Position List, Position newElement);
Position createNewElement(double number);
int deleteAllElements(Position Stack);
int deleteAfter(Position Head);
int calculatePostfix(Position Stack, char operator);
int pop(Position Stack, double* operator);
int printList(Position Head);

int main() {
    char fileName[50];
    List Stack = { .number = 0, .next = NULL };

    printf("Enter the file name to read the postfix expression from: \n");
    scanf(" %49s", fileName);

    if (readFromFile(fileName, &Stack) != 0)
        printf("An error occurred while processing the file.\n");

    // If the result exists, print it nicely
    if (Stack.next != NULL)
        printf("\nFinal result: %.2f\n", Stack.next->number);

    deleteAllElements(&Stack);
    return 0;
}

int readFromFile(char fileName[], Position Stack) {
    FILE* fp = NULL;
    double readNumber = 0;
    int byteCount = 0;
    int functionValue = 0;
    char operator = 0;
    char line[MAX] = { 0 };
    char* ptr = NULL;

    fp = fopen(fileName, "r");
    if (!fp) {
        printf("Unable to open the file!\n");
        return FILE_OPEN_FAILURE;
    }

    if (!fgets(line, MAX, fp)) {
        printf("Failed to read from file.\n");
        fclose(fp);
        return FILE_OPEN_FAILURE;
    }

    printf("\nYour postfix expression: %s\n", line);

    ptr = line;

    while (*ptr != '\0') {
        // Try to read a number
        functionValue = sscanf(ptr, "%lf %n", &readNumber, &byteCount);
        if (functionValue == 1) {
            if (pushToStart(Stack, readNumber) != 0) {
                deleteAllElements(Stack);
                fclose(fp);
                return ALLOCATION_FAILURE;
            }
        }
        else {
            // Try to read an operator
            if (sscanf(ptr, " %c %n", &operator, &byteCount) == 1) {
                if (calculatePostfix(Stack, operator) != 0) {
                    deleteAllElements(Stack);
                    fclose(fp);
                    return EXIT_FAILURE;
                }
            }
            else break;
        }
        ptr += byteCount;
    }

    printf("\nStack content after evaluation:\n");
    printList(Stack);

    fclose(fp);
    return 0;
}

Position createNewElement(double number) {
    Position newElement = (Position)malloc(sizeof(List));
    if (!newElement) {
        printf("Unable to allocate memory!\n");
        return NULL;
    }

    newElement->number = number;
    newElement->next = NULL;
    return newElement;
}

int pushToStart(Position Head, double number) {
    Position newElement = createNewElement(number);
    if (!newElement)
        return EXIT_FAILURE;

    pushAfter(Head, newElement);
    return 0;
}

int pushAfter(Position List, Position newElement) {
    newElement->next = List->next;
    List->next = newElement;
    return 0;
}

int deleteAfter(Position Head) {
    Position temp = Head->next;

    if (!temp)
        return EXIT_FAILURE;

    Head->next = temp->next;
    free(temp);
    return EXIT_SUCCESS;
}

int deleteAllElements(Position Stack) {
    while (Stack->next != NULL)
        deleteAfter(Stack);
    return EXIT_SUCCESS;
}

int pop(Position Stack, double* value) {
    if (Stack->next == NULL) {
        printf("Invalid postfix expression (stack empty)!\n");
        return EXIT_FAILURE;
    }

    *value = Stack->next->number;
    deleteAfter(Stack);
    return EXIT_SUCCESS;
}

int printList(Position Head) {
    while (Head->next != NULL) {
        Head = Head->next;
        printf("%.2f\n", Head->number);
    }
    return 0;
}

int calculatePostfix(Position Stack, char operator) {
    double operator1 = 0;
    double operator2 = 0;
    double result = 0;

    if (pop(Stack, &operator2) != 0)
        return EXIT_FAILURE;

    if (pop(Stack, &operator1) != 0)
        return EXIT_FAILURE;

    switch (operator) {
    case '+':
        result = operator1 + operator2;
        break;
    case '-':
        result = operator1 - operator2;
        break;
    case '*':
        result = operator1 * operator2;
        break;
    case '/':
        if (operator2 == 0) {
            printf("Cannot divide by 0!\n");
            return EXIT_FAILURE;
        }
        result = operator1 / operator2;
        break;
    default:
        printf("Unknown operator: %c\n", operator);
        return EXIT_FAILURE;
    }

    pushToStart(Stack, result);
    return EXIT_SUCCESS;
}
