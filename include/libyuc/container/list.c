#include <libyuc/container/list.def>

void ListInit(List* list) {
    list->last = LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId;
    list->first = LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId;
}


void ListPutNext(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, entry_id);
    ListEntry* prev = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, prev_id);
    entry->prev = prev_id;
    entry->next = prev->next;
    ListEntry* prev_next = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, prev->next);
    prev->next = entry_id;
    prev_next->prev = entry_id;
    if (list->last == prev_id) {
        list->last = entry_id;
    }
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, prev_next);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, prev);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, entry);
}

void ListPutFirst(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, entry_id);
    if (list->first == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId) {
        assert(list->last == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId);
        entry->prev = entry_id;
        entry->next = entry_id;
        list->last = entry_id;
    }
    else {
        ListEntry* old_first = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, list->first);
        entry->prev = old_first->prev;
        entry->next = list->first;
        ListEntry* last = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, old_first->prev);
        old_first->prev = entry_id;
        last->next = entry_id;
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, last);
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, old_first);
    }
    list->first = entry_id;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, entry);
}

void ListPutLast(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, entry_id);
    if (list->last == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId) {
        assert(list->first == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId);
        entry->prev = entry_id;
        entry->next = entry_id;
        list->first = entry_id;
    }
    else {
        ListEntry* old_last = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, list->last);
        entry->prev = list->last;
        entry->next = old_last->next;
        ListEntry* first = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, old_last->next);
        old_last->next = entry_id;
        first->prev = entry_id;
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, first);
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, old_last);
    }
    list->last = entry_id;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, entry);
}


void ListDelete(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
      assert(iter->cur_id != LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId);
    if (list->first == list->last) {
        if (list->first == LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId) {
            return;
        }
        ListInit(list);
        return;
    }
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, entry_id);
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id = entry->prev;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id next_id = entry->next;
    ListEntry* prev = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, prev_id);
    ListEntry* next = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, next_id);
    prev->next = next_id;
    next->prev = prev_id;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, prev);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, next);

    if (entry_id == list->first) {
        list->first = next_id;
    }
    else if (entry_id == list->last) {
        list->last = prev_id;
    }
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, entry);
}

LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListDeleteFirst(List* list) {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id first = ListFirst(list);
    ListDelete(list, first);
    return first;
}

LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListDeleteLast(List* list) {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id last = ListLast(list);
    ListDelete(list, last);
    return last;
}


void ListReplace(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id new_entry_id) {
    ListEntry* entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, entry_id);
    ListEntry* new_entry = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, new_entry_id);
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id = entry->prev;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id next_id = entry->next;
    if (prev_id == entry_id) {
        prev_id = new_entry_id; 
    }
    if (next_id == entry_id) {
        next_id = new_entry_id; 
    }
    new_entry->prev = prev_id;
    new_entry->next = next_id;
    ListEntry* prev = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, prev_id);
    ListEntry* next = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, next_id);
    prev->next = new_entry_id;
    next->prev = new_entry_id;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, prev);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, next);
    if (entry_id == list->first) {
        list->first = new_entry_id;
    }
    if (entry_id == list->last) {
        list->last = new_entry_id;
    }
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, entry);
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, new_entry);
}


LIBYUC_CONTAINER_LIST_REFERENCER_Type_Offset ListGetCount(List* list) {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Offset count = 0;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id cur_id = ListFirst(list);
    while (cur_id != LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId) {
        count++;
        ListEntry* cur = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, cur_id);
        cur_id = ListNext(list, cur_id);
        LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, cur);
    }
    return count;
}


LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListFirst(List* list) {
    return list->first;
}

LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListLast(List* list) {
    return list->last;
}

LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListPrev(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
    ListEntry* cur = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, entry_id);
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id = cur->prev;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, cur);
    if (prev_id == list->last) {
        return LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId;
    }
    return prev_id;
}

LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListNext(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id) {
    ListEntry* cur = LIBYUC_CONTAINER_LIST_REFERENCER_Reference(list, entry_id);
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id next_id = cur->next;
    LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(list, cur);
    if (next_id == list->first) {
        return LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId;
    }
    return next_id;
}

#include <libyuc/container/list.undef>