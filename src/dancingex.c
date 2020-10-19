#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>

/**
 * A simple Node structure
 */
struct Node 
{
    int data;
    struct Node* prev;
    struct Node* next;
};

/**
 * Insertion function which inserts each new node into the circular doubly linked list from the end of the list. 
 * 
 * @param struct Node**
 * @param int
 * @return void  
 */
void insertEnd(struct Node** head, int data)
{   
    // If list is empty create a single node
    if (!(*head))
    {
        struct Node* n = malloc(sizeof(struct Node));
        
        // configures pointers 
        n->data = data;
        n->next = n->prev = n;
        *head = n;
        return;
    }
    
    // If list is not empty
    struct Node* last = (*head)->prev;

    struct Node* n = malloc(sizeof(struct Node));

    // set new node to link behind the head.
    n->data = data;
    n->next = *head;
    (*head)->prev = n;
    n->prev = last;
    last->next = n;
}

/**
 * The main delete logic which handles all cases of the circular doubly linked list. 
 * 
 * @param struct Node**
 * @param int
 * @return void
 */
void delete(struct Node** head, int data)
{
    // If head is NULL
    if (!(*head))
        return;
    
    struct Node* curr = *head;
    struct Node* previous = NULL;

    // search for matching node if it exists
    while (curr->data != data)
    {
        // If every node has been searched
        if (curr->next == *head)
            return;
        
        previous = curr;
        curr = curr->next;
    }

    // If node is only node in list
    if (curr->next == *head && !previous)
    {
        *head = NULL;
        free(curr);
        return;
    }

    // if list has more than one node, check if it is first
    if (curr == *head)
    {
        // Move previous to last node
        previous = (*head)->prev;

        // Move head
        *head = (*head)->next;

        // Adjust pointers
        previous->next = *head;
        (*head)->prev = previous;
        free(curr);
    }
    else if (curr->next == *head) // if list has more than one node, check if its last
    {
        previous->next = *head;
        (*head)->prev = previous;
        free(curr);
    }
    else
    {
        struct Node* tmp = curr->next;
        previous->next = tmp;
        tmp->prev = previous;
        free(curr);
    }
}

/**
 * Logic for printing the current state of the circular doubly linked list. The wchar_t represented with
 * the hexcode 0x2b82 is a unicode character with double arrows pointing left and right (simplified the printing).
 * 
 * @param struct Node*
 * @return void
 */
void printState(struct Node* head)
{
    struct Node* curr = head;
    wchar_t double_arrows = 0x2b82;

    // Printed in order to symbolize the circular connection
    printf("%lc  ", double_arrows);

    do
    {
        printf("%d ", curr->data);
        curr = curr->next;

        if (curr != head)
            printf("%lc  ", double_arrows);
        else
            printf("%lc\n", double_arrows); // printed on end to symbolize the circular connection
    } while (curr != head);
}

/**
 * Main logic function which performs the dancing links simulation. By using property 1 & 2, we can "delete" a
 * node from the circular doubly linked list:
 * 
 *      (1) RLINK(LLINK(X)) <- RLINK(X)
 *      (2) LLINK(RLINK(X)) <- LLINK(X)
 * 
 * Typically by convention we are inclined to clean up memory and delete X, however in DLX we leave this in 
 * memory. Through recursion we can navigate through search patterns and use backtracking to restore the state
 * if necessary with property 3 & 4:
 * 
 *      (3) RLINK(LLINK(X)) <- X
 *      (4) LLINK(RLINK(X)) <- X
 * 
 * In this way this method is able to completely "search" through the entire list until there is only a single 
 * node left, where then the backtracking begins to restore the state of the object.
 *  
 * @param struct Node**
 * @return void
 */
void performDancingLinks(struct Node** head)
{
    // nominal case
    // If last node is left return to start backtracking
    if (*head == (*head)->next)
        return;

    // Prints current state of circular doubly linked list
    printState((*head));

    // RLINK(LLINK(X)) <- RLINK(X)
    (*head)->next->prev = (*head)->prev;
    
    // LLINK(RLINK(X)) -> LLINK(X)
    (*head)->prev->next = (*head)->next;
    
    // Perform "search-like algorithm"
    performDancingLinks(&(*head)->next);

    // Restore linked structure through backtracking:
    
    // RLINK(LLINK(X)) <- X
    (*head)->next->prev = *head;
    
    // LINK(RLINK(X)) <- X
    (*head)->prev->next = *head;

    printState((*head));
}

/**
 * Wrapper to delete each node in the circular doubly linked list.
 * 
 * @param struct Node**
 * @return void
 */
void deleteAll(struct Node** head)
{
    while (*head)
        delete(head, (*head)->data);
}

/**
 * Driver code to set up the nodes into a circular doubly linked list to perform the 'dancing links' experiment.
 * 
 * @param int
 * @param char**
 * @return int
 */
int main(int argc, char** argv)
{
    int nodes;
    setlocale(LC_CTYPE, "");
    struct Node* head = NULL;

    if (argc != 2)
    {
        printf("Requires number of nodes to create: ./DANCE 20\n");
        return 1;
    }
    
    nodes = atoi(argv[1]);
    
    for (int i = 0; i < nodes; i++)
        insertEnd(&head, i);

    // Simple visual example of what dancing links provide
    performDancingLinks(&head);

    deleteAll(&head);

    return 0;
}