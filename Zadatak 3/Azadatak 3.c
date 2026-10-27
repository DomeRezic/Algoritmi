#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure representing a person's information
typedef struct {
    char firstName[50];
    char lastName[50];
    int birthYear;
} Person;

// Structure representing a node in the linked list
typedef struct Node {
    Person person;
    struct Node* next;
} Node;

// Function Prototypes
Node* createNode(const char* firstName, const char* lastName, int birthYear);
void addAtBeginning(Node** headRef, const char* firstName, const char* lastName, int birthYear);
void addAtEnd(Node** headRef, const char* firstName, const char* lastName, int birthYear);
void printList(const Node* head);
Node* findByLastName(const Node* head, const char* lastName);
void deleteByLastName(Node** headRef, const char* lastName);
void freeList(Node** headRef);

// New functions
int addAfter(Node* head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear);
int addBefore(Node** headRef, const char* targetLastName, const char* firstName, const char* lastName, int birthYear);
void sortByLastName(Node** headRef);
int writeListToFile(const Node* head, const char* filename);
int readListFromFile(Node** headRef, const char* filename);

int main() {
    Node* head = NULL;

    printf("=== Demonstration of Linked List Operations ===\n\n");

    // a) Dynamically add elements to the beginning of the list
    printf("[a] Adding elements to the beginning:\n");
    addAtBeginning(&head, "Ante", "Cash", 1990);
    addAtBeginning(&head, "Tina", "Nejašmić", 1985);

    // b) Print the list
    printList(head);
    printf("\n");

    // c) Dynamically add elements to the end of the list
    printf("[c] Adding elements to the end:\n");
    addAtEnd(&head, "Anastazija", "Kovalov", 1995);
    addAtEnd(&head, "Robert", "Drašković", 1980);
    printList(head);
    printf("\n");

    // d) Find an element in the list by last name
    printf("[d] Finding an element by last name ('Cash'):\n");
    Node* foundNode = findByLastName(head, "Cash");
    if (foundNode != NULL) {
        printf("  -> Found: %s %s, Born: %d\n",
               foundNode->person.firstName,
               foundNode->person.lastName,
               foundNode->person.birthYear);
    } else {
        printf("  -> Person not found.\n");
    }
    printf("\n");

    // e) Delete a specific element from the list by last name
    printf("[e] Deleting a specific element ('Nejašmić'):\n");
    deleteByLastName(&head, "Nejašmić");
    printList(head);
    printf("\n");

    // f) Add a new element AFTER a certain element
    printf("[f] Adding 'Marko Horvat' after 'Cash':\n");
    if (!addAfter(head, "Cash", "Marko", "Horvat", 2000)) {
        printf("  -> Target 'Cash' not found.\n");
    }
    printList(head);
    printf("\n");

    // g) Add a new element BEFORE a certain element
    printf("[g] Adding 'Ivana Babić' before 'Cash' (which is the head):\n");
    if (!addBefore(&head, "Cash", "Ivana", "Babić", 1992)) {
        printf("  -> Target 'Cash' not found.\n");
    }
    printf("    Adding 'Petar Perić' before 'Kovalov':\n");
    if (!addBefore(&head, "Kovalov", "Petar", "Perić", 1988)) {
        printf("  -> Target 'Kovalov' not found.\n");
    }
    printList(head);
    printf("\n");

    // h) Sort the list by last name
    printf("[h] Sorting the list by last name:\n");
    sortByLastName(&head);
    printList(head);
    printf("\n");

    // i) Write the list to a file
    const char* filename = "people.txt";
    printf("[i] Writing the list to file '%s':\n", filename);
    if (writeListToFile(head, filename)) {
        printf("  -> Success.\n");
    }
    printf("\n");

    // j) Read the list from the file (into a fresh list)
    printf("[j] Reading the list back from file '%s':\n", filename);
    freeList(&head);
    if (readListFromFile(&head, filename)) {
        printList(head);
    }

    // Free all unneeded memory before program termination
    freeList(&head);

    return 0;
}

/**
 * Helper function to allocate and initialize a new node.
 */
Node* createNode(const char* firstName, const char* lastName, int birthYear) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed!\n");
        exit(1);
    }

    // Safely copy string data into the person structure
    snprintf(newNode->person.firstName, sizeof(newNode->person.firstName), "%s", firstName);
    snprintf(newNode->person.lastName, sizeof(newNode->person.lastName), "%s", lastName);
    newNode->person.birthYear = birthYear;
    newNode->next = NULL;

    return newNode;
}

/**
 * Dynamically adds a new element to the beginning of the list.
 */
void addAtBeginning(Node** headRef, const char* firstName, const char* lastName, int birthYear) {
    Node* newNode = createNode(firstName, lastName, birthYear);
    newNode->next = *headRef;
    *headRef = newNode;
}

/**
 * Dynamically adds a new element to the end of the list.
 */
void addAtEnd(Node** headRef, const char* firstName, const char* lastName, int birthYear) {
    Node* newNode = createNode(firstName, lastName, birthYear);

    // If the list is empty, make the new node the head
    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }

    // Otherwise, traverse to the last node
    Node* current = *headRef;
    while (current->next != NULL) {
        current = current->next;
    }

    // Attach the new node at the end
    current->next = newNode;
}

/**
 * Prints all elements in the list.
 */
void printList(const Node* head) {
    if (head == NULL) {
        printf("  -> The list is currently empty.\n");
        return;
    }

    printf("  -> Current Person List:\n");
    const Node* current = head;
    int index = 1;
    while (current != NULL) {
        printf("     %d. %s %s (Born: %d)\n",
               index++,
               current->person.firstName,
               current->person.lastName,
               current->person.birthYear);
        current = current->next;
    }
}

/**
 * Finds an element in the list by last name.
 * Returns a pointer to the matching node, or NULL if not found.
 */
Node* findByLastName(const Node* head, const char* lastName) {
    const Node* current = head;
    while (current != NULL) {
        if (strcmp(current->person.lastName, lastName) == 0) {
            return (Node*)current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Deletes a specific element from the list based on the last name.
 */
void deleteByLastName(Node** headRef, const char* lastName) {
    if (*headRef == NULL) {
        printf("  -> List is empty. Nothing to delete.\n");
        return;
    }

    Node* current = *headRef;
    Node* previous = NULL;

    // If the target node is the head node itself
    if (strcmp(current->person.lastName, lastName) == 0) {
        *headRef = current->next; // Bypass the head
        free(current);           // Free allocated memory
        printf("  -> Successfully deleted person with last name '%s'.\n", lastName);
        return;
    }

    // Traverse the list to find the target node while keeping track of the previous node
    while (current != NULL && strcmp(current->person.lastName, lastName) != 0) {
        previous = current;
        current = current->next;
    }

    // If the element was not found in the list
    if (current == NULL) {
        printf("  -> Person with last name '%s' not found.\n", lastName);
        return;
    }

    // Unlink the node from the chain and free its memory
    previous->next = current->next;
    free(current);
    printf("  -> Successfully deleted person with last name '%s'.\n", lastName);
}

/**
 * Frees all allocated nodes in the list to prevent memory leaks.
 */
void freeList(Node** headRef) {
    Node* current = *headRef;
    Node* nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    *headRef = NULL;
}

/**
 * a) Dynamically adds a new element AFTER the first element whose last
 * name equals targetLastName.
 * Returns 1 on success, 0 if the target was not found.
 */
int addAfter(Node* head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    Node* target = findByLastName(head, targetLastName);
    if (target == NULL) {
        return 0;
    }

    Node* newNode = createNode(firstName, lastName, birthYear);
    newNode->next = target->next;
    target->next = newNode;
    return 1;
}

/**
 * b) Dynamically adds a new element BEFORE the first element whose last
 * name equals targetLastName.
 * Returns 1 on success, 0 if the target was not found.
 */
int addBefore(Node** headRef, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    if (*headRef == NULL) {
        return 0;
    }

    // Target is the head: the new node becomes the new head
    if (strcmp((*headRef)->person.lastName, targetLastName) == 0) {
        addAtBeginning(headRef, firstName, lastName, birthYear);
        return 1;
    }

    // Otherwise look for the node whose NEXT is the target
    Node* previous = *headRef;
    while (previous->next != NULL &&
           strcmp(previous->next->person.lastName, targetLastName) != 0) {
        previous = previous->next;
    }

    if (previous->next == NULL) {
        return 0; // target not found
    }

    Node* newNode = createNode(firstName, lastName, birthYear);
    newNode->next = previous->next;
    previous->next = newNode;
    return 1;
}

/**
 * c) Sorts the list by last name (ascending) using insertion sort.
 * The nodes are relinked (data is not copied), and the sort is stable:
 * people with equal last names keep their relative order.
 * Note: strcmp compares bytes, so characters like 'Š' or 'Ć' (UTF-8)
 * sort after plain ASCII letters.
 */
void sortByLastName(Node** headRef) {
    Node* sorted = NULL;
    Node* current = *headRef;

    while (current != NULL) {
        Node* next = current->next;

        if (sorted == NULL || strcmp(current->person.lastName, sorted->person.lastName) < 0) {
            // Insert at the front of the sorted list
            current->next = sorted;
            sorted = current;
        } else {
            // Find the last node that is <= current (keeps the sort stable)
            Node* s = sorted;
            while (s->next != NULL &&
                   strcmp(s->next->person.lastName, current->person.lastName) <= 0) {
                s = s->next;
            }
            current->next = s->next;
            s->next = current;
        }
        current = next;
    }

    *headRef = sorted;
}

/**
 * d) Writes the list to a text file, one person per line:
 *    firstName;lastName;birthYear
 * Returns 1 on success, 0 on failure.
 */
int writeListToFile(const Node* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open '%s' for writing.\n", filename);
        return 0;
    }

    for (const Node* current = head; current != NULL; current = current->next) {
        fprintf(file, "%s;%s;%d\n",
                current->person.firstName,
                current->person.lastName,
                current->person.birthYear);
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Error: Failed to finish writing '%s'.\n", filename);
        return 0;
    }
    return 1;
}

/**
 * e) Reads the list from a text file written by writeListToFile.
 * Any existing contents of the list are freed first, so the list will
 * contain exactly what is in the file (in file order).
 * Returns 1 on success, 0 if the file could not be opened.
 */
int readListFromFile(Node** headRef, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open '%s' for reading.\n", filename);
        return 0;
    }

    freeList(headRef);

    Node* tail = NULL;
    char line[200];

    while (fgets(line, sizeof(line), file) != NULL) {
        char firstName[50];
        char lastName[50];
        int birthYear;

        // Skip malformed or empty lines
        if (sscanf(line, "%49[^;];%49[^;];%d", firstName, lastName, &birthYear) != 3) {
            continue;
        }

        Node* newNode = createNode(firstName, lastName, birthYear);
        if (tail == NULL) {
            *headRef = newNode;
        } else {
            tail->next = newNode;
        }
        tail = newNode;
    }

    fclose(file);
    return 1;
}
