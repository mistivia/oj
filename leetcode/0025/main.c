static void reverse(struct ListNode *prev, struct ListNode *end) {
    struct ListNode *first = prev->next;
    struct ListNode *curr = first;
    prev->next = end;

    while (curr != end) {
        struct ListNode *next = curr->next;
        curr->next = prev->next;
        prev->next = curr;
        curr = next;
    }
}

struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    if (head == NULL || k <= 1) {
        return head;
    }
    struct ListNode dummy;
    dummy.next = head;

    struct ListNode *prev = &dummy;

    while (1) {
        struct ListNode *end = prev;
        for (int i = 0; i < k; i++) {
            end = end->next;
            if (end == NULL) {
                return dummy.next;
            }
        }
        struct ListNode *first = prev->next;
        reverse(prev, end->next);
        prev = first;
    }
}