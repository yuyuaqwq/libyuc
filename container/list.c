#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLASS_NAME
#define CLASS_NAME
#endif

#ifndef REFERENCER_Type_Id
#define REFERENCER_Type_Id struct ListEntry*
#endif

#ifndef REFERENCER_Type_Offset
#define REFERENCER_Type_Offset size_t
#endif

#ifndef REFERENCER_Const_InvalidId
#define REFERENCER_Const_InvalidId NULL
#endif

#ifndef REFERENCER_Reference
#define REFERENCER_Reference(main_obj, reference) (reference) 
#endif

#ifndef REFERENCER_Dereference
#define REFERENCER_Dereference(main_obj, reference)
#endif

#define ListEntry MAKE_NAME(CLASS_NAME, ListEntry)
#define ListHead MAKE_NAME(CLASS_NAME, ListHead)

void MAKE_NAME(CLASS_NAME, ListInit)(ListHead* head) {
    head->last = REFERENCER_Const_InvalidId;
    head->first = REFERENCER_Const_InvalidId;
}
void MAKE_NAME(CLASS_NAME, ListPutEntryNext)(ListHead* head, REFERENCER_Type_Id prev_id, REFERENCER_Type_Id entry_id) {
    ListEntry* entry = REFERENCER_Reference(head, entry_id);
    ListEntry* prev = REFERENCER_Reference(head, prev_id);
    entry->prev = prev_id;
    entry->next = prev->next;
    ListEntry* prev_next = REFERENCER_Reference(head, prev->next);
    prev->next = entry_id;
    prev_next->prev = entry_id;
    if (head->last == prev_id) head->last = entry_id; \
        REFERENCER_Dereference(head, prev_next);
    REFERENCER_Dereference(head, prev);
    REFERENCER_Dereference(head, entry);
}
void MAKE_NAME(CLASS_NAME, ListPutFirst)(ListHead* head, REFERENCER_Type_Id entry_id) {
    ListEntry* entry = REFERENCER_Reference(head, entry_id);
    if (head->first == REFERENCER_Const_InvalidId) {
        assert(head->last == REFERENCER_Const_InvalidId);
        entry->prev = entry_id;
        entry->next = entry_id;
        head->last = entry_id;
    }
    else {
        ListEntry* old_first = REFERENCER_Reference(head, head->first);
        entry->prev = old_first->prev;
        entry->next = head->first;
        ListEntry* last = REFERENCER_Reference(head, old_first->prev);
        old_first->prev = entry_id;
        last->next = entry_id;
        REFERENCER_Dereference(head, last);
        REFERENCER_Dereference(head, old_first);
    }
    head->first = entry_id;
    REFERENCER_Dereference(head, entry);
}
void MAKE_NAME(CLASS_NAME, ListPutLast)(ListHead* head, REFERENCER_Type_Id entry_id) {
    ListEntry* entry = REFERENCER_Reference(head, entry_id);
    if (head->last == REFERENCER_Const_InvalidId) {
        assert(head->first == REFERENCER_Const_InvalidId);
        entry->prev = entry_id;
        entry->next = entry_id;
        head->first = entry_id;
    }
    else {
        ListEntry* old_last = REFERENCER_Reference(head, head->last);
        entry->prev = head->last;
        entry->next = old_last->next;
        ListEntry* first = REFERENCER_Reference(head, old_last->next);
        old_last->next = entry_id;
        first->prev = entry_id;
        REFERENCER_Dereference(head, first);
        REFERENCER_Dereference(head, old_last);
    }
    head->last = entry_id;
    REFERENCER_Dereference(head, entry);
}
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListDeleteEntry)(ListHead* head, REFERENCER_Type_Id entry_id) {
    if (head->first == head->last) {
        if (head->first == REFERENCER_Const_InvalidId) return REFERENCER_Const_InvalidId;
        entry_id = head->first;
        MAKE_NAME(CLASS_NAME, ListInit(head));
        return entry_id;
    }
    ListEntry* entry = REFERENCER_Reference(head, entry_id);
    REFERENCER_Type_Id prev_id = entry->prev;
    REFERENCER_Type_Id next_id = entry->next;
    ListEntry* prev = REFERENCER_Reference(head, prev_id);
    ListEntry* next = REFERENCER_Reference(head, next_id);
    prev->next = next_id;
    next->prev = prev_id;
    REFERENCER_Dereference(head, prev);
    REFERENCER_Dereference(head, next);

    if (entry_id == head->first) {
        head->first = next_id;
    }
    else if (entry_id == head->last) {
        head->last = prev_id;
    }
    REFERENCER_Dereference(head, entry);
}
void MAKE_NAME(CLASS_NAME, ListDeleteFirst)(ListHead* head) {
    MAKE_NAME(CLASS_NAME, ListDeleteEntry)(head, head->first);
}
void MAKE_NAME(CLASS_NAME, ListDeleteLast)(ListHead* head) {
    MAKE_NAME(CLASS_NAME, ListDeleteEntry)(head, head->last);
}
void MAKE_NAME(CLASS_NAME, ListReplaceEntry)(ListHead* head, REFERENCER_Type_Id entry_id, REFERENCER_Type_Id new_entry_id) {
    ListEntry* entry = REFERENCER_Reference(head, entry_id);
    ListEntry* new_entry = REFERENCER_Reference(head, new_entry_id);
    REFERENCER_Type_Id prev_id = entry->prev;
    REFERENCER_Type_Id next_id = entry->next;
    if (prev_id == entry_id) { prev_id = new_entry_id; }
    if (next_id == entry_id) { next_id = new_entry_id; }
    new_entry->prev = prev_id;
    new_entry->next = next_id;
    ListEntry* prev = REFERENCER_Reference(head, prev_id);
    ListEntry* next = REFERENCER_Reference(head, next_id);
    prev->next = new_entry_id;
    next->prev = new_entry_id;
    REFERENCER_Dereference(head, prev);
    REFERENCER_Dereference(head, next);
    if (entry_id == head->first) {
        head->first = new_entry_id;
    }
    if (entry_id == head->last) {
        head->last = new_entry_id;
    }
    REFERENCER_Dereference(head, entry);
    REFERENCER_Dereference(head, new_entry);
}
REFERENCER_Type_Offset MAKE_NAME(CLASS_NAME, ListGetCount)(ListHead* head) {
    REFERENCER_Type_Offset count = 0;
    REFERENCER_Type_Id cur_id = MAKE_NAME(CLASS_NAME, ListFirst)(head);
    while (cur_id != REFERENCER_Const_InvalidId) {
        count++;
        ListEntry* cur = REFERENCER_Reference(head, cur_id);
        cur_id = MAKE_NAME(CLASS_NAME, ListNext)(head, cur_id);
        REFERENCER_Dereference(head, cur); \
    }
    return count;
}
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListFirst)(ListHead* head) {
    return head->first;
}
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListLast)(ListHead* head) {
    return head->last;
}
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListPrev)(ListHead* head, REFERENCER_Type_Id cur_id) {
    ListEntry* cur = REFERENCER_Reference(head, cur_id);
    REFERENCER_Type_Id prev_id = cur->prev;
    REFERENCER_Dereference(head, cur);
    if (prev_id == head->last) {
        return REFERENCER_Const_InvalidId;
    }
    return prev_id;
}
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListNext)(ListHead* head, REFERENCER_Type_Id cur_id) {
    ListEntry* cur = REFERENCER_Reference(head, cur_id);
    REFERENCER_Type_Id next_id = cur->next;
    REFERENCER_Dereference(head, cur);
    if (next_id == head->first) {
        return REFERENCER_Const_InvalidId;
    }
    return next_id;
}

#undef ListEntry
#undef ListHead

#undef CLASS_NAME
#undef REFERENCER_Type_Id
#undef REFERENCER_Type_Offset
#undef REFERENCER_Reference
#undef REFERENCER_Dereference

#ifdef __cplusplus
}
#endif