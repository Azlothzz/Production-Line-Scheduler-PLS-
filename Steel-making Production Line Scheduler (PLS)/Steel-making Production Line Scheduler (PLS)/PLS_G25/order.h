#ifndef PLS_ORDER_H
#define PLS_ORDER_H
#include <stdio.h>

typedef struct order {
    int id;
    char order_num[20];
    char due_date[11];
    int quantity;
    char product_name[20];
} order;

typedef struct schedule_order {
    order *ord;
    int quantity;
} schedule_order;

typedef struct list {
    order *order;
    struct list *next;
} list;
#endif//PLS_ORDER_H
