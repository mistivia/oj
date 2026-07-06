struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
    // dummy node simplifies removing the head
    struct ListNode dummy;
    dummy.next = head;
    struct ListNode *a = &dummy;
    struct ListNode *b = &dummy;

    // move a forward n steps
    for (int i = 0; i < n; i++) {
        if (a == NULL) {
            return NULL; // n is larger than list length
        }
        a = a->next;
    }

    // move both until a reaches the last node
    while (a->next != NULL) {
        a = a->next;
        b = b->next;
    }

    // b now points to the node before the target
    struct ListNode *target = b->next;
    b->next = target->next;
    free(target);

    return dummy.next;
}