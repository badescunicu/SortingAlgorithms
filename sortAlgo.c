#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define GAPS_SIZE 8

typedef struct Node {
    int value;
    struct Node *next;
}Node;

Node* init_list() {
    Node *new = malloc(sizeof(Node));
    new->value = INT_MIN;
    new->next = NULL;
}

/* Insert a node at the beginning of a list */
void push_front(Node **list, int val) {
    Node *new = malloc(sizeof(Node));
    new->value = val;
    new->next = (*list);
    (*list) = new;
}

/* Insert a node after a given node */
void push_after(Node **last, int val) {
    Node *new = malloc(sizeof(Node));
    new->value = val;
    new->next = (*last)->next;
    (*last)->next = new;
    (*last) = new;
}

/* Insert a node at a given location */
void insert_at(Node *node, int val) {
    Node *new = malloc(sizeof(Node));
    new->value = node->value;
    new->next = node->next;
    node->value = val;
    node->next = new;
}

void delete_node(Node *to_delete) {
    Node *tmp = to_delete->next;
    to_delete->value = to_delete->next->value;
    to_delete->next = to_delete->next->next;
    free(tmp);
}

void delete_list(Node *list) {
    while(list) {
        Node *tmp = list;
        list = list->next;
        free(tmp);
    }
}

void print_list(Node *list) {
    while (list->next) {
        printf("%d ", list->value);
        list = list->next;
    }
}

/* Swap 2 values */
void swap(int *a, int *b) {
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

/* Print an array */
void print(int *v, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void bubble_sort(int *to_sort, int size) {
    int i, swapped = 1;
    while (swapped) {
        swapped = 0;
        for (i = 0; i < size - 1; i++) {
            if (to_sort[i] > to_sort[i + 1]) {
                swapped = 1;
                swap(&to_sort[i], &to_sort[i + 1]);
            }
        }
    }
}

void cocktail_sort(int *to_sort, int size) {
    int i, swapped = 1;
    while (swapped) {
        /* Go from left to right */
        swapped = 0;
        for (i = 0; i < size - 1; i++) {
            if (to_sort[i] > to_sort[i + 1]) {
                swapped = 1;
                swap(&to_sort[i], &to_sort[i + 1]);
            }
        }

        /* Break if the array is already sorted */
        if (!swapped) {
            break;
        }

        /* Go from right to left */
        swapped = 0;
        for (i = size - 1; i > 0; i--) {
            if (to_sort[i - 1] > to_sort[i]) {
                swapped = 1;
                swap(&to_sort[i - 1], &to_sort[i]);
            }
        }
    }
}


/* Insertion sort, as shown in "Introduction to Algorithms" */
void insertion_sort(int *to_sort, int size) {
    int i, j;
    for (j = 1; j < size; j++) {
        int to_insert = to_sort[j];
        i = j - 1;
        
        while (i >= 0 && to_sort[i] > to_insert) {
            to_sort[i + 1] = to_sort[i];
            i--;
        }
        to_sort[i + 1] = to_insert;
    }
}

/* Shellsort using Ciura's gaps */
void shellsort(int *to_sort, int size) {
    int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
    int gap_index, i, j, to_insert;
    /* Cycle through gaps */
    for (gap_index = 0; gap_index < GAPS_SIZE; gap_index++) {
        /* Perform an Insertion Sort for each gap */
        for (i = gaps[gap_index]; i < size; i++) {
            to_insert = to_sort[i];
            j = i;
            while (j >= gaps[gap_index] &&\
                    to_sort[j - gaps[gap_index]] > to_insert) {
                to_sort[j] = to_sort[j - gaps[gap_index]];
                j -= gaps[gap_index];
            }
            to_sort[j] = to_insert;
        }
    }
}

/* Quicksort, as shown in Introduction to Algorithms */

/* Returns the final index of the pivot */
int partition(int *to_sort, int left, int right) {
    int i, j;
    /* Choose the last element as a pivot, to allow in place sorting */
    int pivot = to_sort[right];
    i = left - 1;
    for (j = left; j <= right - 1; j++) {
        if (to_sort[j] <= pivot) {
            i++;
            swap(&to_sort[i], &to_sort[j]);
        }
    }
    swap(&to_sort[i + 1], &to_sort[right]);
    return i + 1;
}

void quicksort(int *to_sort, int left, int right) {
    if (left < right) {
        int pivot_index = partition(to_sort, left, right);
        quicksort(to_sort, left, pivot_index - 1);
        quicksort(to_sort, pivot_index + 1, right);
    }
}

/* Allow the quicksort function to be called in the same way
 * as the others */
void quicksort_wrapper(int *to_sort, int size) {
    quicksort(to_sort, 0, size - 1);
}

void strand_sort(int *to_sort, int size) {
    int i;

    Node *list_to_sort = init_list();
    Node *sublist = NULL;
    Node *list_iterator = NULL;
    Node *last_sublist = NULL;
    Node *ordered_list = init_list();

    /* Copy the initial array into a linked list */
    for (i = size - 1; i >= 0; i--) {
        push_front(&list_to_sort, to_sort[i]);
    }
    
    while (list_to_sort->next) {
        /* Delete the old sublist */
        delete_list(sublist);
        sublist = init_list();
        
        /* Add first sequence of ordered numbers in sublist */
        push_front(&sublist, list_to_sort->value);
        delete_node(list_to_sort);
        Node *list_iterator = list_to_sort;
        Node *last_sublist = sublist;
        while (list_iterator->next &&\
               list_iterator->value > last_sublist->value) {
            push_after(&last_sublist, list_iterator->value);
            delete_node(list_iterator);
        }

        /* Merge sublist into ordered_list */
        last_sublist = sublist;
        list_iterator = ordered_list;
        while (last_sublist->next && list_iterator->next) {
            if (last_sublist->value < list_iterator->value) {
                insert_at(list_iterator, last_sublist->value);
                last_sublist = last_sublist->next;
            } else {
                list_iterator = list_iterator->next;
            }
        }
        while (last_sublist->next) {
            insert_at(list_iterator, last_sublist->value);
            list_iterator = list_iterator->next;
            last_sublist = last_sublist->next;
        }
    }
    print_list(ordered_list);
}

int main() {
    int *v = malloc(10 * sizeof(int));
    int i, j;
    for (i = 0, j = 10; i < 10; i++)
        v[i] = j--;
    v[0] = -7;
    v[6] = -4;
    v[8] = -1;
    print(v, 10);
    strand_sort(v, 10);
//    print(v, 10);
    free(v);

    return 0;
}
