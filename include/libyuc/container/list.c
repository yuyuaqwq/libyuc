#include <libyuc/container/list.def>

void ListInit(ListHead* head) {
    head->last = LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId;
    head->first = LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId;
}
void ListPutEntryNext(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, entry_id);
    ListEntry* prev = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, prev_id);
    entry->prev = prev_id;
    entry->next = prev->next;
    ListEntry* prev_next = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, prev->next);
    prev->next = entry_id;
    prev_next->prev = entry_id;
    if (head->last == prev_id) head->last = entry_id; \
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, prev_next);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, prev);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, entry);
}
void ListPutFirst(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, entry_id);
    if (head->first == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId) {
        assert(head->last == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId);
        entry->prev = entry_id;
        entry->next = entry_id;
        head->last = entry_id;
    }
    else {
        ListEntry* old_first = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, head->first);
        entry->prev = old_first->prev;
        entry->next = head->first;
        ListEntry* last = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, old_first->prev);
        old_first->prev = entry_id;
        last->next = entry_id;
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, last);
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, old_first);
    }
    head->first = entry_id;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, entry);
}
void ListPutLast(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, entry_id);
    if (head->last == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId) {
        assert(head->first == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId);
        entry->prev = entry_id;
        entry->next = entry_id;
        head->first = entry_id;
    }
    else {
        ListEntry* old_last = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, head->last);
        entry->prev = head->last;
        entry->next = old_last->next;
        ListEntry* first = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, old_last->next);
        old_last->next = entry_id;
        first->prev = entry_id;
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, first);
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, old_last);
    }
    head->last = entry_id;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, entry);
}
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListDeleteEntry(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
    if (head->first == head->last) {
        if (head->first == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId) return LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId;
        entry_id = head->first;
        ListInit(head);
        return entry_id;
    }
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, entry_id);
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id = entry->prev;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id next_id = entry->next;
    ListEntry* prev = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, prev_id);
    ListEntry* next = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, next_id);
    prev->next = next_id;
    next->prev = prev_id;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, prev);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, next);

    if (entry_id == head->first) {
        head->first = next_id;
    }
    else if (entry_id == head->last) {
        head->last = prev_id;
    }
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, entry);
}
void ListDeleteFirst(ListHead* head) {
    ListDeleteEntry(head, head->first);
}
void ListDeleteLast(ListHead* head) {
    ListDeleteEntry(head, head->last);
}
void ListReplaceEntry(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id new_entry_id) {
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, entry_id);
    ListEntry* new_entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, new_entry_id);
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id = entry->prev;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id next_id = entry->next;
    if (prev_id == entry_id) { prev_id = new_entry_id; }
    if (next_id == entry_id) { next_id = new_entry_id; }
    new_entry->prev = prev_id;
    new_entry->next = next_id;
    ListEntry* prev = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, prev_id);
    ListEntry* next = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, next_id);
    prev->next = new_entry_id;
    next->prev = new_entry_id;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, prev);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, next);
    if (entry_id == head->first) {
        head->first = new_entry_id;
    }
    if (entry_id == head->last) {
        head->last = new_entry_id;
    }
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, entry);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, new_entry);
}
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Offset ListGetCount(ListHead* head) {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Offset count = 0;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id cur_id = ListFirst(head);
    while (cur_id != LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId) {
        count++;
        ListEntry* cur = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, cur_id);
        cur_id = ListNext(head, cur_id);
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, cur); \
    }
    return count;
}
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListFirst(ListHead* head) {
    return head->first;
}
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListLast(ListHead* head) {
    return head->last;
}
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListPrev(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id cur_id) {
    ListEntry* cur = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, cur_id);
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id = cur->prev;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, cur);
    if (prev_id == head->last) {
        return LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId;
    }
    return prev_id;
}
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListNext(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id cur_id) {
    ListEntry* cur = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(head, cur_id);
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id next_id = cur->next;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(head, cur);
    if (next_id == head->first) {
        return LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId;
    }
    return next_id;
}

#include <libyuc/container/list.undef>