#define DEBUG_MODE 0

#include "../includes/XORLiLs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static struct XORNode *XOR(struct XORNode *pt1, struct XORNode *pt2) {
    return (void *)((uintptr_t)pt1 ^ (uintptr_t)pt2);
}

void go(struct XORNode *prev, struct XORNode **curr) {
    *curr = XOR((*curr)->nb, prev);
}

struct EndPoints built_list(void) {
    struct EndPoints head_and_tail = {NULL, NULL};
    struct XORNode *prev = NULL;
    int value;
    while (scanf("%d", &value) == 1) {
        struct XORNode *new_node = malloc(sizeof(struct XORNode));
        new_node->data = value;
        new_node->nb = prev;
        if (!head_and_tail.head) {
            head_and_tail.head = new_node;
        } else {
            prev->nb = XOR(prev->nb, new_node); // update_prev
        }
        prev = new_node;
        head_and_tail.tail = new_node;
#if DEBUG_MODE
        if (getchar() == '\n') {
            break;
        }
#endif
    }
    return head_and_tail;
}

void print_list(struct XORNode *head) {
    struct XORNode *cur = head;
    struct XORNode *prev = NULL;
    while (cur) {
        printf("%d ", cur->data);
        struct XORNode *next = XOR(cur->nb, prev);
        prev = cur;
        cur = next;
    }
    printf("\n");
}

struct XORNode *find_elem(struct XORNode *list, int to_find) {
    struct XORNode *prev = NULL, *buffer;
    while (list != NULL && list->data != to_find) {
        buffer = prev;
        prev = list;
        list = XOR(buffer, list->nb);
    }

    return list;
}

void del_el(struct XORNode **head, struct XORNode **tail, int value) {
    struct XORNode *cur = *head;
    struct XORNode *prev = NULL;
    while (cur) {
        struct XORNode *next = XOR(cur->nb, prev);
        if (cur->data == value) {
            if (prev)
                prev->nb = XOR(XOR(prev->nb, cur), next);
            else
                *head = next;
            if (next)
                next->nb = XOR(XOR(next->nb, cur), prev);
            else
                *tail = prev;
            free(cur);
            cur = next;
            continue;
        }
        prev = cur;
        cur = next;
    }
}

int pop(struct XORNode **list) {
    int poped_value;

    if (!(*list)) {
        fprintf(stderr, "Error: poping from empty list.");
        exit(0);
    }

    struct XORNode *tmp = *list;

    poped_value = (*list)->data;

    *list = (*list)->nb;
    if ((*list)) {
        (*list)->nb = XOR(tmp, (*list)->nb);
    }
    free(tmp);
    return poped_value;
}

void insert_first(struct EndPoints *head_and_tail, int value) {
    struct XORNode *new_node = malloc(sizeof(struct XORNode));
    new_node->data = value;
    if (head_and_tail->head == NULL) {
        new_node->nb = XOR(NULL, NULL);
        head_and_tail->head = new_node;
        head_and_tail->tail = new_node;
        return;
    }
    struct XORNode *cur = head_and_tail->head;
    struct XORNode *next = XOR(cur->nb, NULL);
    new_node->nb = XOR(cur, NULL);
    cur->nb = XOR(new_node, next);
    head_and_tail->head = new_node;
}

void clear_list(struct XORNode **head) {
    struct XORNode *cur = *head;
    struct XORNode *prev = NULL;
    struct XORNode *next;
    while (cur) {
        next = XOR(cur->nb, prev);
        free(cur);
        prev = cur;
        cur = next;
    }
    *head = NULL;
}
#if DEBUG_MODE
int main(void) {
    struct EndPoints head_and_tail = built_list();
    print_list(head_and_tail.head);
    struct XORNode *nxt = head_and_tail.head->nb;
    go(NULL, &(head_and_tail.head));
    if (nxt == head_and_tail.head) {
        printf("1\n");
    }
    print_list(head_and_tail.head);
    return 0;
}
#endif
