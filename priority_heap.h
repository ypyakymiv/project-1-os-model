#ifndef PRIORITY_HEAP_INCLUDE
#define PRIORITY_HEAP_INCLUDE

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"

const int DEFAULT_HEAP_SZ = 1024;

struct heap;

struct heap {
  void **arr;
  int (*get_priority)(void *);
  int curr_sz;
  int alloc_sz;
};

struct heap *heap_init(int init_sz, int (*get_priority)(void *)) {
  struct heap *h = ec_malloc(sizeof(struct heap));
  h->arr = ec_malloc(init_sz * sizeof(void *));
  h->get_priority = get_priority;
  h->alloc_sz = init_sz;
  h->curr_sz = 0;
  return h;
}

struct heap *heap_free(struct heap *h, void (*free_item)(void *)) {
  for(int i = 0; i < h->curr_sz; i++)
    free_item(h->arr + i);
  free(h->arr);
  free(h);
}

int max(int a, int b) {
  return a > b ? a : b;
}

int min(int a, int b) {
  return a < b ? a : b;
}

int parent_of(int c) {
  return floor((c - 1) / 2.0);
} 

int left_child_of(int p) {
  return p * 2 + 1;
}

int right_child_of(int p) {
  return p * 2 + 2;
}

void swap(struct heap *h, int p1, int p2) {
  void *temp = h->arr[p1];
  h->arr[p1] = h->arr[p2];
  h->arr[p2] = temp;
}

void sink(struct heap *h, int pos) {
  int (*get_priority)(void *) = h->get_priority; 
  int left, right;
  int h_prior;
  left = left_child_of(pos);
  right = right_child_of(pos);
  if(left >= h->curr_sz) left = pos;
  if(right >= h->curr_sz) right = pos;

  while((h_prior = min(get_priority(h->arr[left]), get_priority(h->arr[right]))) < get_priority(h->arr[pos])) {
    if(h_prior == get_priority(h->arr[left])) {
      swap(h, pos, left);
      pos = left;
    } else {
      swap(h, pos, right);
      pos = right;
    }

    left = left_child_of(pos);
    right = right_child_of(pos);

    if(left >= h->curr_sz) left = pos;
    if(right >= h->curr_sz) right = pos;
  }

}

void heap_insert(struct heap *h, void *item) {
  if(h->curr_sz == h->alloc_sz) {
    int new_alloc_sz = 2 * h->alloc_sz; //magic number
    void **arr = ec_malloc(new_alloc_sz * sizeof(void *));
    memcpy(arr, h->arr, h->curr_sz * sizeof(void *));
    free(h->arr);
    h->arr = arr;
    h->alloc_sz = new_alloc_sz;
  }
  int c = h->curr_sz;
  h->arr[h->curr_sz] = item;
  h->curr_sz++;

  int p;
  while(c) {
    p = parent_of(c);
    if(h->get_priority(h->arr[p]) > h->get_priority(h->arr[c]))
      swap(h, p, c);
    else
      break;

    c = p;
  }
}

void *heap_pop(struct heap *h) {
  if(!h->curr_sz) return NULL;
  
  void *top = h->arr[0];
  h->curr_sz--;
  swap(h, 0, h->curr_sz);
  sink(h, 0);

  return top;
}

#endif
