#ifndef PLS_UTIL_H
#define PLS_UTIL_H
#include "order.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


bool is_valid_date(const char *date) {
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) {
        return false;
    }
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        return false;
    }
    int days_in_month[] = {0, 31, 28 + is_leap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (day > days_in_month[month]) {
        return false;
    }
    return true;
}

time_t date_to_timestamp(const char *date) {
    struct tm tm = {0};
    sscanf(date, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
    tm.tm_year -= 1900;
    tm.tm_mon--;       
    return mktime(&tm);
}
// Calculate date difference
int days_diff(const char *date1, const char *date2) {
    time_t timestamp1 = date_to_timestamp(date1);
    time_t timestamp2 = date_to_timestamp(date2);
    return ceil(difftime(timestamp2, timestamp1) / (60 * 60 * 24));
}
// add order to linked list;
void add_to_list(list **head, list **tail, int *counter, order *order) {
    if (*head == NULL) {
        *head = (list *) malloc(sizeof(list));
        *tail = *head;
    } else {
        (*tail)->next = (list *) malloc(sizeof(list));
        *tail = (*tail)->next;
    }
    (*tail)->order = order;
    (*tail)->next = NULL;
    (*counter)++;
}
// release list
void release_list(list *li) {
    if (li == NULL) {
        return;
    }
    list *p = li->next;
    while (p != NULL) {
        free(li);
        li = p;
        p = p->next;
    }
    free(li);
}
#endif//PLS_UTIL_H
