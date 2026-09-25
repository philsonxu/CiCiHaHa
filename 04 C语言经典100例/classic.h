#pragma once
#ifndef LESSON_HEAD
#define LESSON_HEAD

int is_leap_year(int year);
void swap(int* a, int* b);
void strswap(char* str1, char* str2);
unsigned long long factorial(int n);
unsigned long long factorial_recursion(int n);
void strrevs(char* s);
void print_binary(int num);
void print_binary_formula(int a, int b, int c);

typedef struct LinkNode {
    int          data;
    struct LinkNode* next;
} LinkNode, * LinkList;

LinkList list_build(int n);
LinkList list_reverse(LinkList L);
void list_print(LinkList h);
LinkList list_delete_node(LinkList pointer, LinkList tmp);
void list_sort(LinkList pointer, int num);
LinkList list_create_array(int array[], int num);
LinkList list_concatenate(LinkList pointer1, LinkList pointer2);

#endif
