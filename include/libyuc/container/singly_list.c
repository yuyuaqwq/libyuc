
void SinglyListHeadInit(SinglyListHead* head) {
    head->first = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId;
}
bool SinglyListIsEmpty(SinglyListHead* head) {
    return head->first == LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId;
}
void SinglyListPutFirst(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id entry_id) {
    SinglyListEntry* entry = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, entry_id);
    entry->next = head->first;
    head->first = entry_id;
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, entry);
}
void SinglyListDeleteFirst(SinglyListHead* head) {
    SinglyListEntry* old_first = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, head->first);
    head->first = old_first->next;
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, old_first);
}
void SinglyListDeleteEntry(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id prev_id, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id del_entry_id) {
    SinglyListEntry* prev;
    if (prev_id != LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId) {
        prev = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, prev_id);
    }
    else {
        prev = (SinglyListEntry*)head;
    }
    SinglyListEntry* del_entry = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, del_entry_id);
    prev->next = del_entry->next;
    if (prev_id != LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId) {
        LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, prev);
    }
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, del_entry);
}
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Offset SinglyListGetCount(SinglyListHead* head) {
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Offset count = 0;
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id cur_id = head->first;
    while (cur_id != LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId) {
        count++;
        SinglyListEntry* cur = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, cur_id);
        cur_id = cur->next;
        LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, cur);
    }
    return count;
}
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id SinglyListIteratorFirst(SinglyListHead* head) {
    return head->first;
}
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id SinglyListIteratorNext(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id entry_id) {
    SinglyListEntry* entry = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, entry_id);
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id next_id = entry->next;
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, entry);
    return next_id;
}
