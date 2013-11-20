#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>

#define GAPS_SIZE 8

/* Variables to keep track of elementary operations */
long long total_smooth, total_quick, total_cocktail, total_bubble,
    total_insertion, total_shell, total_strand;

/* List to hold the dimensions of the Leonardo heaps used in Smoothsort*/
typedef struct {
    int *v;
    int size;
} leonardo_dimensions;

/* List of Leonardo numbers precomputed for efficiency */
int L[] = {1, 1, 3, 5, 9, 15, 25, 41, 67, 109,
          177, 287, 465, 753, 1219, 1973, 3193, 5167, 8361};

/* Node structure needed for Strand Sort */
typedef struct Node {
    int value;
    struct Node *next;
}Node;

/* Copy an array */
int* copy_array(int *v, int size) {
    int i;
    int *new = malloc(size * sizeof(int));
    for (i = 0; i < size; i++) {
        new[i] = v[i];
    }
    return new;
}

/* Generate an ordered array of given size */
int* generate_ordered_array(int size) {
    int i;
    int *new = malloc(size * sizeof(int));
    for (i = 0; i < size; i++) {
        new[i] = i + 1;
    }
    return new;
}

/* Generate a reverse ordered array */
int* generate_reverse_ordered_array(int size) {
    int i, j;
    int *new = malloc(size * sizeof(int));
    for (i = 0, j = size; i < size; i++, j--) {
        new[i] = j;
    }
    return new;
}

/* Generate a random array of given size with values in [1, bound] */
int* generate_random_array(int size, int bound) {
    int i;
    int *v = malloc(size * sizeof(int));
    srand(time(NULL));
    for (i = 0; i < size; i++) {
        v[i] = rand() % bound + 1;
    }
    return v;
}

Node* init_list() {
    Node *new = malloc(sizeof(Node));
    new->value = INT_MIN;
    new->next = NULL;
    total_strand += 8;
    return new;
}

/* Insert a node at the beginning of a list */
void push_front(Node **list, int val) {
    Node *new = malloc(sizeof(Node));
    new->value = val;
    new->next = (*list);
    (*list) = new;
    total_strand += 11;
}

/* Insert a node after a given node */
void push_after(Node **last, int val) {
    Node *new = malloc(sizeof(Node));
    new->value = val;
    new->next = (*last)->next;
    (*last)->next = new;
    (*last) = new;
    total_strand += 15;
}

/* Insert a node at a given location */
void insert_at(Node *node, int val) {
    Node *new = malloc(sizeof(Node));
    new->value = node->value;
    new->next = node->next;
    node->value = val;
    node->next = new;
    total_strand += 18;
}

void delete_node(Node *to_delete) {
    Node *tmp = to_delete->next;
    to_delete->value = to_delete->next->value;
    to_delete->next = to_delete->next->next;
    free(tmp);
    total_strand += 18;
}

void delete_list(Node *list) {
    while(list) {
        Node *tmp = list;
        list = list->next;
        free(tmp);
        total_strand += 6;
    }
    total_strand++;
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
    total_bubble++;
    while (swapped) {
        swapped = 0;
        total_bubble += 5;
        for (i = 0; i < size - 1; i++) {
            total_bubble += 8;
            if (to_sort[i] > to_sort[i + 1]) {
                swapped = 1;
                swap(&to_sort[i], &to_sort[i + 1]);
                total_bubble += 11;
            }
        }
    }
    total_bubble++;
}

void cocktail_sort(int *to_sort, int size) {
    int i, swapped = 1;
    total_cocktail++;
    while (swapped) {
        /* Go from left to right */
        swapped = 0;
        total_cocktail += 7;
        for (i = 0; i < size - 1; i++) {
            total_cocktail += 8;
            if (to_sort[i] > to_sort[i + 1]) {
                swapped = 1;
                swap(&to_sort[i], &to_sort[i + 1]);
                total_cocktail += 11;
            }
        }

        /* Break if the array is already sorted */
        if (!swapped) {
            total_cocktail += 2;
            break;
        }

        /* Go from right to left */
        swapped = 0;
        total_cocktail += 3;
        for (i = size - 1; i > 0; i--) {
            total_cocktail += 7;
            if (to_sort[i - 1] > to_sort[i]) {
                swapped = 1;
                swap(&to_sort[i - 1], &to_sort[i]);
                total_cocktail += 11;
            }
        }
    }
}

/* Insertion sort, as shown in "Introduction to Algorithms" */
void insertion_sort(int *to_sort, int size) {
    int i, j;
    total_insertion += 2;
    for (j = 1; j < size; j++) {
        int to_insert = to_sort[j];
        i = j - 1;
        total_insertion += 7;
        
        while (i >= 0 && to_sort[i] > to_insert) {
            to_sort[i + 1] = to_sort[i];
            i--;
            total_insertion += 10;
        }
        to_sort[i + 1] = to_insert;
        total_insertion += 3;
    }
}

/* Shellsort using Ciura's gaps */
void shellsort(int *to_sort, int size) {
    int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
    int gap_index, i, j, to_insert;
    total_shell += 2;
    /* Cycle through gaps */
    for (gap_index = 0; gap_index < GAPS_SIZE; gap_index++) {
        /* Perform an Insertion Sort for each gap */
        total_shell += 6;
        for (i = gaps[gap_index]; i < size; i++) {
            to_insert = to_sort[i];
            j = i;
            total_shell += 10;
            while (j >= gaps[gap_index] &&\
                    to_sort[j - gaps[gap_index]] > to_insert) {

                to_sort[j] = to_sort[j - gaps[gap_index]];
                j -= gaps[gap_index];
                total_shell += 14;
            }
            to_sort[j] = to_insert;
            total_shell += 2;
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
    total_quick += 7;
    for (j = left; j <= right - 1; j++) {
        total_quick += 6;
        if (to_sort[j] <= pivot) {
            i++;
            swap(&to_sort[i], &to_sort[j]);
            total_quick += 4;
        }
    }
    swap(&to_sort[i + 1], &to_sort[right]);
    total_quick += 4;
    return i + 1;
}

void quicksort(int *to_sort, int left, int right) {
    if (left < right) {
        int pivot_index = partition(to_sort, left, right);
        quicksort(to_sort, left, pivot_index - 1);
        quicksort(to_sort, pivot_index + 1, right);
        total_quick += 4;
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

    total_strand += 5;

    /* Copy the initial array into a linked list */
    total_strand += 3;
    for (i = size - 1; i >= 0; i--) {
        push_front(&list_to_sort, to_sort[i]);
        total_strand++;
    }
    
    total_strand++;
    while (list_to_sort->next) {
        /* Delete the old sublist */
        delete_list(sublist);
        sublist = init_list();
        
        /* Add first sequence of ordered numbers in sublist */
        push_front(&sublist, list_to_sort->value);
        delete_node(list_to_sort);
        Node *list_iterator = list_to_sort;
        Node *last_sublist = sublist;
        total_strand += 15;
        while (list_iterator->next &&\
               list_iterator->value > last_sublist->value) {
            push_after(&last_sublist, list_iterator->value);
            delete_node(list_iterator);
            total_strand += 11;
        }

        /* Merge sublist into ordered_list */
        last_sublist = sublist;
        list_iterator = ordered_list;
        total_strand += 7;
        while (last_sublist->next && list_iterator->next) {
            total_strand += 5;
            if (last_sublist->value < list_iterator->value) {
                insert_at(list_iterator, last_sublist->value);
                last_sublist = last_sublist->next;
                total_strand += 9;
            } else {
                list_iterator = list_iterator->next;
                total_strand += 2;
            }
        }
        total_strand += 2;
        while (last_sublist->next) {
            insert_at(list_iterator, last_sublist->value);
            list_iterator = list_iterator->next;
            last_sublist = last_sublist->next;
            total_strand += 8;
        }
    }

    /* Update the initial vector with the sorted list */
    total_strand += 3;
    for (i = 0, list_iterator = ordered_list; i < size; i++) {
        to_sort[i] = list_iterator->value;
        list_iterator = list_iterator->next;
        total_strand += 10;
    }
    
    /* Free the resources */
    delete_list(ordered_list);
    delete_list(sublist);
    delete_list(list_to_sort);
}

void heapify(int *to_sort, int k, int pos) {
    total_smooth++;
    if (k <= 1) {
        return;
    } else {
        int left, right, largest, tmp_k;
        left = pos - L[k - 2] - 1;
        right = pos - 1;
        largest = pos;

        total_smooth += 15;
        if (to_sort[left]  > to_sort[largest]) {
            largest = left;
            tmp_k = k - 1;
            total_smooth += 3;
        }
        if (to_sort[right] > to_sort[largest]) {
            largest = right;
            tmp_k = k - 2;
            total_smooth += 3;
        }

        if (largest == pos) {
            return;
        } else {
            swap(&to_sort[pos], &to_sort[largest]);
            heapify(to_sort, tmp_k, largest);
            total_smooth += 2;
        }
    }
}

/* Perform a sligthly modified insertion sort:
 * not only check if the previous leonardo heap root is bigger
 * than the current one, but also if it is bigger than the
 * current root's children.
 * Also, do a heapify operation on the modified heap*/
void rebalance_heaps(int *to_sort, int *v, int size, int position) {
    int j, cur_pos, to_insert;
    j = size - 1;
    cur_pos = position;
    to_insert = to_sort[cur_pos];
    total_smooth += 12;
    while (j > 0 && to_sort[cur_pos - L[v[j]]] > to_insert) {
        total_smooth += 11;
        if (v[j] >= 2) {
            total_smooth += 18;
            if (to_sort[cur_pos - L[v[j]]] > to_sort[cur_pos - 1] &&
                    to_sort[cur_pos - L[v[j]]] > to_sort[cur_pos - L[v[j] - 2] - 1]) {
                to_sort[cur_pos] = to_sort[cur_pos - L[v[j]]];
                cur_pos -= L[v[j]];
                total_smooth += 9;
            } else {
                break;
            }
        } else {
            to_sort[cur_pos] = to_sort[cur_pos - L[v[j]]];
            cur_pos -= L[v[j]];
            total_smooth += 10;
        }
        j--;
        total_smooth += 2;
    }
    to_sort[cur_pos] = to_insert;
    heapify(to_sort, v[j], cur_pos);
    total_smooth += 3;
}

void smooth_sort(int *to_sort, int size) {
    int i, to_insert, cur_pos, j, tmp_root;
    leonardo_dimensions dim;
    /* Allocate resources for a maximum list size of log(size) */
    dim.v = malloc(((int)log2(size) + 1) * sizeof(int));
    dim.size = 1;
    /* The first leonardo heap is L(1) */
    dim.v[0] = 1;

    total_smooth += 11;

    /* Create the Leonardo heaps on top of the existing array */
    total_smooth += 2;
    for (i = 1; i < size; i++) {
        
        total_smooth += 18;
        if ((dim.size >= 2) && (dim.v[dim.size - 2] - dim.v[dim.size - 1]) == 1) {
            dim.v[dim.size - 2]++;
            dim.size--;
            total_smooth += 9;
        } else if (dim.v[dim.size - 1] == 1) {
            dim.v[dim.size] = 0;
            dim.size++;
            total_smooth += 7;
        } else {
            dim.v[dim.size] = 1;
            dim.size++;
            total_smooth += 7;
        }
        
        rebalance_heaps(to_sort, dim.v, dim.size, i);
        total_smooth += 2;
    }

    /* Dequeue from the Leonardo heaps until there are no more elements */
    i = size - 1;
    total_smooth += 3;
    while (i >= 0) {
        total_smooth += 6;
        if (dim.v[dim.size - 1] <= 1) {
            i--;
            dim.size--;
            total_smooth += 5;
        } else {
            /* Split the root in two */
            i--;
            tmp_root = dim.v[dim.size - 1];
            dim.size++;
            dim.v[dim.size - 2] = tmp_root - 1;
            dim.v[dim.size - 1] = tmp_root - 2;

            /* Rebalance the left child */
            rebalance_heaps(to_sort, dim.v, dim.size - 1, i - L[tmp_root - 2]);

            /* Rebalance the right child */
            rebalance_heaps(to_sort, dim.v, dim.size, i);
            total_smooth += 30;
        }
    }
    free(dim.v);
    total_smooth++;
}

void apply_sorts(int *v, int no_of_elements) {
    int *tmp;
    /* Reset the counters to 0 */
    total_smooth = 0;
    total_strand = 0;
    total_shell = 0;
    total_quick = 0;
    total_cocktail = 0;
    total_bubble = 0;
    total_insertion = 0;

    tmp = copy_array(v, no_of_elements);
    printf("Smoothsort -> ");
    smooth_sort(tmp, no_of_elements);
    free(tmp);
    printf("Total operations: %lld\n", total_smooth);

    tmp = copy_array(v, no_of_elements);
    printf("Strand Sort -> ");
    strand_sort(tmp, no_of_elements);
    free(tmp);
    printf("Total operations: %lld\n", total_strand);

    tmp = copy_array(v, no_of_elements);
    printf("Shell Sort -> ");
    shellsort(tmp, no_of_elements);
    free(tmp);
    printf("Total operations: %lld\n", total_shell);

    tmp = copy_array(v, no_of_elements);
    printf("Quicksort -> ");
    quicksort_wrapper(tmp, no_of_elements);
    free(tmp);
    printf("Total operations: %lld\n", total_quick);

    tmp = copy_array(v, no_of_elements);
    printf("Cocktail Sort -> ");
    cocktail_sort(tmp, no_of_elements);
    free(tmp);
    printf("Total operations: %lld\n", total_cocktail);

    tmp = copy_array(v, no_of_elements);
    printf("Bubble Sort -> ");
    bubble_sort(tmp, no_of_elements);
    free(tmp);
    printf("Total operations: %lld\n", total_bubble);

    tmp = copy_array(v, no_of_elements);
    printf("Insertion Sort -> ");
    insertion_sort(tmp, no_of_elements);
    free(tmp);
    printf("Total operations: %lld\n", total_insertion);

    free(v);
}

int main() {
    int *v, *tmp, no_of_elements;
    char choice;
    printf("Analysis of Algorithms - Homework #1\n");
    printf("Enter 1 for Testing Mode\n");
    printf("Enter 2 for Analysis Mode\n");
    scanf("%c", &choice);

    switch(choice) {
        case '1':
            no_of_elements = 10;
            v = generate_random_array(no_of_elements, 100);
            printf("Random array: ");
            print(v, no_of_elements);

            tmp = copy_array(v, no_of_elements);
            printf("\nSmoothsort:\n");
            smooth_sort(tmp, no_of_elements);
            print(tmp, no_of_elements);
            free(tmp);
            printf("Total operations: %lld\n", total_smooth);

            tmp = copy_array(v, no_of_elements);
            printf("\nStrand Sort\n");
            strand_sort(tmp, no_of_elements);
            print(tmp, no_of_elements);
            free(tmp);
            printf("Total operations: %lld\n", total_strand);

            tmp = copy_array(v, no_of_elements);
            printf("\nShell Sort\n");
            shellsort(tmp, no_of_elements);
            print(tmp, no_of_elements);
            free(tmp);
            printf("Total operations: %lld\n", total_shell);

            tmp = copy_array(v, no_of_elements);
            printf("\nQuicksort\n");
            quicksort_wrapper(tmp, no_of_elements);
            print(tmp, no_of_elements);
            free(tmp);
            printf("Total operations: %lld\n", total_quick);

            tmp = copy_array(v, no_of_elements);
            printf("\nCocktail Sort\n");
            cocktail_sort(tmp, no_of_elements);
            print(tmp, no_of_elements);
            free(tmp);
            printf("Total operations: %lld\n", total_cocktail);

            tmp = copy_array(v, no_of_elements);
            printf("\nBubble Sort\n");
            bubble_sort(tmp, no_of_elements);
            print(tmp, no_of_elements);
            free(tmp);
            printf("Total operations: %lld\n", total_bubble);

            tmp = copy_array(v, no_of_elements);
            printf("\nInsertion Sort\n");
            insertion_sort(tmp, no_of_elements);
            print(tmp, no_of_elements);
            free(tmp);
            printf("Total operations: %lld\n", total_insertion);

            free(v);
            break;
        
        case '2':
            /* Sorted arrays */
            /* Size 10 */
            no_of_elements = 10;
            printf("\nSorted array (size %d)\n", no_of_elements);
            v = generate_ordered_array(no_of_elements);
            apply_sorts(v, no_of_elements);

            /* Size 300 */
            no_of_elements = 300;
            printf("\nSorted array (size %d)\n", no_of_elements);
            v = generate_ordered_array(no_of_elements);
            apply_sorts(v, no_of_elements);


            /* Size 10000 */
            no_of_elements = 10000;
            printf("\nSorted array (size %d)\n", no_of_elements);
            v = generate_ordered_array(no_of_elements);
            apply_sorts(v, no_of_elements);

            /* Random arrays */
            /* Size 10 */
            no_of_elements = 10;
            printf("\nRandom array (size %d)\n", no_of_elements);
            v = generate_random_array(no_of_elements, INT_MAX);
            apply_sorts(v, no_of_elements);

            /* Size 300 */
            no_of_elements = 300;
            printf("\nRandom array (size %d)\n", no_of_elements);
            v = generate_random_array(no_of_elements, INT_MAX);
            apply_sorts(v, no_of_elements);


            /* Size 10000 */
            no_of_elements = 10000;
            printf("\nRandom array (size %d)\n", no_of_elements);
            v = generate_random_array(no_of_elements, INT_MAX);
            apply_sorts(v, no_of_elements);

            /* Reverse ordered arrays */
            /* Size 10 */
            no_of_elements = 10;
            printf("\nReverse ordered array (size %d)\n", no_of_elements);
            v = generate_reverse_ordered_array(no_of_elements);
            apply_sorts(v, no_of_elements);

            /* Size 300 */
            no_of_elements = 300;
            printf("\nReverse ordered array (size %d)\n", no_of_elements);
            v = generate_reverse_ordered_array(no_of_elements);
            apply_sorts(v, no_of_elements);


            /* Size 10000 */
            no_of_elements = 10000;
            printf("\nReverse ordered array (size %d)\n", no_of_elements);
            v = generate_reverse_ordered_array(no_of_elements);
            apply_sorts(v, no_of_elements);
        }

    return 0;
}
