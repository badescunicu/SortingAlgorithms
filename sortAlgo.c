#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define GAPS_SIZE 8

/* List to hold the dimensions of the Leonardo heaps used in Smoothsort*/
typedef struct {
    int *v;
    int size;
} leonardo_dimensions;

int L[] = {1, 1, 3, 5, 9, 15, 25, 41, 67, 109,
          177, 287, 465, 753, 1219, 1973, 3193, 5167, 8361};

/* Node structure needed for Strand Sort */
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

    /* Update the initial vector with the sorted list */
    for (i = 0, list_iterator = ordered_list; i < size; i++) {
        to_sort[i] = list_iterator->value;
        list_iterator = list_iterator->next;
    }
    
    /* Free the resources */
    delete_list(ordered_list);
    delete_list(sublist);
    delete_list(list_to_sort);
}

void heapify(int *to_sort, int k, int pos) {
    if (k <= 1) {
        return;
    } else {
        int left, right, largest, tmp_k;
        left = pos - L[k - 2] - 1;
        right = pos - 1;
        largest = pos;
        if (to_sort[left]  > to_sort[largest]) {
            largest = left;
            tmp_k = k - 1;
        }
        if (to_sort[right] > to_sort[largest]) {
            largest = right;
            tmp_k = k - 2;
        }

        if (largest == pos) {
            return;
        } else {
            swap(&to_sort[pos], &to_sort[largest]);
            heapify(to_sort, tmp_k, largest);
        }
    }
}

void smooth_sort(int *to_sort, int size) {
    int i, to_insert, cur_pos, j, tmp_root;
    leonardo_dimensions dim;
    /* Allocate resources for a maximum list size of log(size) */
    dim.v = malloc(((int)log2(size) + 1) * sizeof(int));
    dim.size = 2;
    /* The first two leonardo heaps are L(1) and L(0) */
    dim.v[0] = 1;
    dim.v[1] = 0;

    /* Left Child: cur_pos - L[k - 2] - 1
     * Right Child: cur_pos - 1
     */

    /* Create the Leonardo heaps on top of the existing array */
    for (i = 2; i < size; i++) {
        if ((dim.size >= 2) && (dim.v[dim.size - 2] - dim.v[dim.size - 1]) == 1) {
            dim.v[dim.size - 2]++;
            dim.size--;
        } else if (dim.v[dim.size - 1] == 1) {
            dim.v[dim.size] = 0;
            dim.size++;
        } else {
            dim.v[dim.size] = 1;
            dim.size++;
        }
        
        /* Perform a sligthly modified insertion sort:
         * not only check if the previous leonardo heap root is bigger
         * than the current one, but also if it is bigger than the
         * current root's children */
        to_insert = to_sort[i];
        j = dim.size - 1;
        cur_pos = i;
        while (j > 0 && to_sort[cur_pos - L[dim.v[j]]] > to_insert) {
            if (dim.v[j] >= 2) {
                if (to_sort[cur_pos - L[j]] > to_sort[cur_pos - 1] &&
                        to_sort[cur_pos - L[j]] > to_sort[L[dim.v[j] - 2] - 1]) {
                    to_sort[cur_pos] = to_sort[cur_pos - L[dim.v[j]]];
                    cur_pos -= L[dim.v[j]];
                } else {
                    break;
                }
            } else {
                to_sort[cur_pos] = to_sort[cur_pos - L[dim.v[j]]];
                cur_pos -= L[dim.v[j]];
            }
            j--;
        }
        to_sort[cur_pos] = to_insert;
        heapify(to_sort, dim.v[j], cur_pos);
    }
    printf("Before dequeuing:\n");
    printf("Test leo trees, size %d: ", dim.size);
    print(dim.v, dim.size);

    /* Dequeue from the Leonardo heaps until there are no more heaps */
    i = size - 1;
    while (i >= 0) {
        if (dim.v[dim.size - 1] <= 1) {
            i--;
            dim.size--;
        } else {
            /* Split the root in two */
            i--;
            tmp_root = dim.v[dim.size - 1];
            dim.size++;
            dim.v[dim.size - 2] = tmp_root - 1;
            dim.v[dim.size - 1] = tmp_root - 2;

            /* Rebalance the heaps using the modified insertion sort */
            cur_pos = i - 1;
            to_insert = to_sort[cur_pos];
            j = dim.size - 2;
            while (j > 0 && to_sort[cur_pos - L[dim.v[j]]] > to_insert) {
                if (dim.v[j] >= 2) {
                    if (to_sort[cur_pos - L[j]] > to_sort[cur_pos - 1] &&
                            to_sort[cur_pos - L[j]] > to_sort[L[dim.v[j] - 2] - 1]) {
                        to_sort[cur_pos] = to_sort[cur_pos - L[dim.v[j]]];
                        cur_pos -= L[dim.v[j]];
                    } else {
                        break;
                    }
                } else {
                    to_sort[cur_pos] = to_sort[cur_pos - L[dim.v[j]]];
                    cur_pos -= L[dim.v[j]];
                }
                j--;
            }
            to_sort[cur_pos] = to_insert;
            heapify(to_sort, dim.v[j], cur_pos);

            
            cur_pos = i;
            to_insert = to_sort[cur_pos];
            j = dim.size - 1;
            while (j > 0 && to_sort[cur_pos - L[dim.v[j]]] > to_insert) {
                if (dim.v[j] >= 2) {
                    if (to_sort[cur_pos - L[j]] > to_sort[cur_pos - 1] &&
                            to_sort[cur_pos - L[j]] > to_sort[L[dim.v[j] - 2] - 1]) {
                        to_sort[cur_pos] = to_sort[cur_pos - L[dim.v[j]]];
                        cur_pos -= L[dim.v[j]];
                    } else {
                        break;
                    }
                } else {
                    to_sort[cur_pos] = to_sort[cur_pos - L[dim.v[j]]];
                    cur_pos -= L[dim.v[j]];
                }
                j--;
            }
            to_sort[cur_pos] = to_insert;
            heapify(to_sort, dim.v[j], cur_pos);
        }

    }
    printf("Test leo trees, size %d: ", dim.size);
    print(dim.v, dim.size);
}



int main() {
    int *v = malloc(10 * sizeof(int));
    int u[] = {27, 18, 28, 31, 41, 45, 26, 53, 58, 59, 54, 90, 93, 13};
    int t[] = {3, 1, 4, 2, 11, 9, 6, 7, 0, 8, 5, 10};
    int i, j;
    for (i = 0, j = 10; i < 10; i++)
        v[i] = j--;
    v[0] = -7;
    v[6] = -4;
    v[8] = -1;
    print(v, 9);
    smooth_sort(v, 9);
    print(v, 9);

    print(u, 13);
    smooth_sort(u, 13);
    print(u, 13);
    free(v);

    print(t, 12);
    smooth_sort(t, 12);
    print(t, 12);

    return 0;
}
