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
 * a) Dynamically adds a new element to the beginning of the list.
 */
void addAtBeginning(Node** headRef, const char* firstName, const char* lastName, int birthYear) {
    Node* newNode = createNode(firstName, lastName, birthYear);
    newNode->next = *headRef;
    *headRef = newNode;
}

/**
 * c) Dynamically adds a new element to the end of the list.
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
 * b) Prints all elements in the list.
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
 * d) Finds an element in the list by last name.
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
 * e) Deletes a specific element from the list based on the last name.
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
