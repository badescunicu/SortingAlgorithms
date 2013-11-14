#include <stdio.h>
#include <stdlib.h>

#define GAPS_SIZE 8

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

int main() {
    int *v = malloc(10 * sizeof(int));
    int i, j;
    for (i = 0, j = 10; i < 10; i++)
        v[i] = j--;
    v[6] = -4;
    v[8] = -1;
    print(v, 10);
    shellsort(v, 10);
    print(v, 10);

    return 0;
}

