#pragma once

struct XORNode {
    int data;
    struct XORNode *nb;
};
struct EndPoints {
    struct XORNode *head;
    struct XORNode *tail;
};

struct EndPoints build_list(void);
void print_list(struct XORNode *);

void go(struct XORNode *, struct XORNode **);

struct XORNode *find_elem(struct XORNode *, int);
void del_el(struct XORNode **, struct XORNode **, int);

int pop(struct XORNode **);

void insert_first(struct EndPoints *, int);

void clear_list(struct XORNode **);
