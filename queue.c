#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *newQueue = malloc(sizeof(struct list_head));
    if (newQueue)
        INIT_LIST_HEAD(newQueue);
    return newQueue;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    element_t *cur, *safe;
    list_for_each_entry_safe (cur, safe, l, list) {
        list_del(&cur->list);
        q_release_element(cur);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *node = malloc(sizeof(element_t));
    if (!node)
        return false;
    node->value = strdup(s);
    if (!node->value) {
        q_release_element(node);
        return false;
    }
    list_add(&node->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *node = malloc(sizeof(element_t));
    if (!node)
        return false;
    node->value = strdup(s);
    if (!node->value) {
        q_release_element(node);
        return false;
    }
    list_add_tail(&node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *node = list_first_entry(head, element_t, list);
    if (sp) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(&node->list);
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *node = list_last_entry(head, element_t, list);
    if (sp) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(&node->list);
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int count = 0;
    struct list_head *li;

    list_for_each (li, head)
        count++;

    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *forward = head->next, *backward = head->prev;
    while (forward != backward && forward->next != backward) {
        forward = forward->next;
        backward = backward->prev;
    }
    element_t *middle = list_entry(forward, element_t, list);
    list_del(&middle->list);
    q_release_element(middle);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/

    if (!head)
        return false;

    element_t *curr, *next, *temp;
    list_for_each_entry_safe (curr, next, head, list) {
        if (&next->list != head && !strcmp(curr->value, next->value)) {
            do {
                temp = next;
                next = list_entry(next->list.next, element_t, list);
                q_release_element(temp);
            } while (&next->list != head && !strcmp(curr->value, next->value));

            curr->list.prev->next = &next->list;
            next->list.prev = curr->list.prev;

            q_release_element(curr);
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;
    struct list_head *curr;
    for (curr = head->next; curr != head && curr->next != head;
         curr = curr->next) {
        struct list_head *next = curr->next;
        list_del_init(curr);
        list_add(curr, next);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *prev = head->prev, *curr = head, *next = NULL;
    while (next != head) {
        next = curr->next;
        curr->next = prev;
        curr->prev = next;
        prev = curr;
        curr = next;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || k <= 1)
        return;
    int count = 0;
    struct list_head *cur, *next;
    list_for_each_safe (cur, next, head) {
        count++;
        if (!count % k) {
            count -= 1;
            struct list_head *backward = cur->prev;
            while (count--) {
                struct list_head *tmp = backward;
                list_del(tmp);
                list_add(tmp, cur);
                cur = cur->next;
                backward = backward->prev;
            }
        }
    }
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
