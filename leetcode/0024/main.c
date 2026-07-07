struct ListNode* next2(struct ListNode* node) {
    if (node == NULL) return NULL;
    if (node->next == NULL) return NULL;
    return node->next->next;
}

struct ListNode* swapPairs(struct ListNode* head) {
    if (head == NULL) {
        return NULL;
    }
    struct ListNode* pa = head;
    struct ListNode* pb = head->next;
    if (pb == NULL) {
        return head;
    }
    
    // Swap the first pair
    struct ListNode* next = pb->next;
    pa->next = next;
    pb->next = pa;
    head = pb;
    
    while (1) {
        struct ListNode* prev = pa;
        pb = next2(pa);
        pa = pa->next;
        if (pa == NULL || pb == NULL) {
            break;
        }
        next = pb->next;
        prev->next = pb;
        pb->next = pa;
        pa->next = next;
    }
    return head;
}
