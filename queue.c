#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));

    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    while (q->head) {
        list_ele_t *delEle = q->head;
        q->head = q->head->next;
        if (!q->head)
            q->tail = NULL;

        if (delEle->value)
            free(delEle->value);
        if (delEle->next)
            delEle->next = NULL;
        free(delEle);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    if (s) {
        newh->value = (char *) malloc(strlen(s) + 1);
        if (!newh->value) {
            free(newh);
            return false;
        }

        strncpy(newh->value, s, strlen(s));
        newh->value[strlen(s)] = '\0';
    }

    if (!q->tail)
        q->tail = newh;
    newh->next = q->head;
    q->head = newh;
    ++(q->size);
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    if (s) {
        newh->value = (char *) malloc(strlen(s) + 1);
        if (!newh->value) {
            free(newh);
            return false;
        }

        strncpy(newh->value, s, strlen(s));
        newh->value[strlen(s)] = '\0';
    }

    newh->next = NULL;

    if (!q->head)
        q->head = newh;
    if (q->tail)
        q->tail->next = newh;
    q->tail = newh;
    ++(q->size);
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q)
        return false;
    if (q->size == 0)
        return false;

    list_ele_t *delEle = q->head;
    q->head = q->head->next;
    if (!q->head)
        q->tail = NULL;
    --(q->size);

    if (delEle->value) {
        if (sp) {
            if (strlen(delEle->value) >= bufsize) {
                strncpy(sp, delEle->value, bufsize - 1);
                sp[bufsize - 1] = '\0';
            } else {
                strncpy(sp, delEle->value, strlen(delEle->value));
                sp[strlen(delEle->value)] = '\0';
            }
        }

        free(delEle->value);
    }
    delEle->next = NULL;
    free(delEle);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q)
        return;
    if (q->size <= 1)
        return;

    list_ele_t *prevEle = q->head;
    list_ele_t *currEle = prevEle->next;
    list_ele_t *nextEle = currEle->next;
    q->tail = q->head;
    q->tail->next = NULL;

    while (nextEle) {
        currEle->next = prevEle;

        prevEle = currEle;
        currEle = nextEle;
        nextEle = currEle->next;
    }

    currEle->next = prevEle;
    q->head = currEle;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q)
        return;
    if (q->size <= 1)
        return;

    list_ele_t *head = q->head;
    q->head = q->tail = NULL;
    list_ele_t *currTravEle, *nextTravEle;
    list_ele_t *currEle, *nextEle;

    while (head) {
        currTravEle = head;
        nextTravEle = currTravEle->next;
        list_ele_t *prevEle = NULL;
        currEle = currTravEle;
        nextEle = nextTravEle;

        while (nextTravEle) {
            if (strcmp(currTravEle->value, nextTravEle->value) > 0) {
                if (strcmp(currEle->value, nextTravEle->value) > 0) {
                    prevEle = currTravEle;
                    currEle = nextTravEle;
                    nextEle = nextTravEle->next;
                }
            }

            currTravEle = currTravEle->next;
            nextTravEle = nextTravEle->next;
        }

        if (!q->head) {
            q->head = q->tail = currEle;
            currEle->next = NULL;
        } else {
            q->tail->next = currEle;
            q->tail = currEle;
            currEle->next = NULL;
        }

        if (!prevEle)
            head = nextEle;
        else
            prevEle->next = nextEle;
    }
}
