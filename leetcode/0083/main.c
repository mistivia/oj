#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

// begin

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode* deleteDuplicates(struct ListNode* head) {
    if (head == NULL) {
        return head;
    }
    struct ListNode *prev = head;
    struct ListNode *node = head->next;
    while (node != NULL) {
        if (node->val == prev->val) {
            struct ListNode *next = node->next;
            prev->next = next;
            node = next;
        } else {
            node = node->next;
            prev = prev->next;   
        }
    }
    return head;
}