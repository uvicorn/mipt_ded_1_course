#include "utils.h"
#include "consts.h"
#include <stdio.h>
#include <stdlib.h>




int binary_search(int*buf, int el,int l, int r){
    while (l <= r){
        int m = (l + r)/2;
        if (buf[m] > el)
            r = m-1;
        else if (buf[m] < el)
            l = m+1;
        else return m;
    }
    return l;
}

// Function to swap two elements
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}
void swap_nodes(Node* a, Node* b) {
    Node t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
  
    // Choose the pivot
    int pivot = arr[high];
  
    // Index of smaller element and indicates 
    // the right position of pivot found so far
    int i = low - 1;

    // Traverse arr[low..high] and move all smaller
    // elements on the left side. Elements from low to 
    // i are smaller after every iteration
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    
    // Move pivot after smaller elements and
    // return its position
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

int partition_nodes(Node* arr, int low, int high) {
  
    // Choose the pivot
    Node *pivot = &arr[high];
  
    // Index of smaller element and indicates 
    // the right position of pivot found so far
    int i = low - 1;

    // Traverse arr[low..high] and move all smaller
    // elements on the left side. Elements from low to 
    // i are smaller after every iteration
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].ctr < pivot->ctr) {
            i++;
            swap_nodes(&arr[i], &arr[j]);
        }
    }
    
    // Move pivot after smaller elements and
    // return its position
    swap_nodes(&arr[i + 1], &arr[high]);
    return i + 1;
}
// The QuickSort function implementation
void quickSort(int arr[], int low, int high) {
  
    if (low < high) {
      
        // pi is the partition return index of pivot
        int pi = partition(arr, low, high);

        // Recursion calls for smaller elements
        // and greater or equals elements
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void quickSort_nodes(Node* arr, int low, int high) {
  
    if (low < high) {
      
        // pi is the partition return index of pivot
        int pi = partition_nodes(arr, low, high);

        // Recursion calls for smaller elements
        // and greater or equals elements
        quickSort_nodes(arr, low, pi - 1);
        quickSort_nodes(arr, pi + 1, high);
    }
}
