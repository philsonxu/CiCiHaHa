#include <stdlib.h>
#include <stdio.h>
#include "classic.h"

/*
struct list
{
    int data;
    struct list* next;
};
typedef struct list node;
typedef node* link;

link delete_node(link pointer, link tmp)
{
    if (tmp == NULL) 
        return pointer->next;
    else
    {
        if (tmp->next->next == NULL)
            tmp->next = NULL;
        else 
            tmp->next = tmp->next->next;
        return pointer;
    }
}

void selection_sort(link pointer, int num)
{
    link tmp, btmp;
    int i, min;
    for (i = 0; i < num; i++)
    {
        tmp = pointer;
        min = tmp->data;
        btmp = NULL;
        while (tmp->next)
        {
            if (min > tmp->next->data)
            {
                min = tmp->next->data;
                btmp = tmp;
            }
            tmp = tmp->next;
        }
        printf("\40: %d\n", min);
        pointer = delete_node(pointer, btmp);
    }
}

link list_create(int array[], int num)
{
    link tmp1, tmp2, pointer;
    int i;
    pointer = (link)malloc(sizeof(node));
    pointer->data = array[0];
    tmp1 = pointer;
    for (i = 1; i < num; i++)
    {
        tmp2 = (link)malloc(sizeof(node));
        tmp2->next = NULL;
        tmp2->data = array[i];
        tmp1->next = tmp2;
        tmp1 = tmp1->next;
    }
    return pointer;
}

link list_concatenate(link pointer1, link pointer2)
{
    link tmp;
    tmp = pointer1;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = pointer2;
    return pointer1;
}
*/
/*
* C 语言经典100例-74
* https://www.runoob.com/cprogramming/c-exercise-example74.html
* 题目：连接两个链表
*/
int main(void)
{
    printf("连接两个链表\n");
    int arr1[] = { 3,12,8,9,11 };
    int arr2[] = { 101,102,108 };
    LinkList ptr1;
    ptr1 = list_create_array(arr1, 5);
    printf("链表1(未排序)\n");
    list_print(ptr1);
    printf("链表1(排序)\n");
    list_sort(ptr1, 5);
    list_print(ptr1);

    printf("链表2(未排序)\n");
    LinkList ptr2;
    ptr2 = list_create_array(arr2, 3);
    list_print(ptr2);

    printf("链表1 + 链表2\n");
    list_concatenate(ptr1, ptr2);
    list_print(ptr1);

    return 0;
}