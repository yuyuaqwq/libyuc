#include "list.h"

void ListHeadInit(ListHead* head) {
	head->prev = head;
	head->next = head;
}

void ListEntryInit(ListEntry* entry) {
	ListHeadInit(entry);
}

bool ListIsEmpty(ListHead* head) {
	return head->next == head && head->prev == head;
}

void ListInsertHead(ListHead* head, ListEntry* entry) {
	ListEntry* old = head->next;
	head->next = entry;
	entry->prev = head;
	entry->next = old;
	old->prev = entry;
}

void ListInsertTail(ListHead* head, ListEntry* entry) {
	ListEntry* old = head->prev;
	head->prev = entry;
	entry->next = head;
	entry->prev = old;
	old->next = entry;
}

ListEntry* ListRemoveEntry(ListEntry* entry, bool empty) {
	ListEntry* prev = entry->prev;
	ListEntry* next = entry->next;
	prev->next = next;
	next->prev = prev;
	if (empty) {
		ListEntryInit(entry);
	}
	return entry;
}

ListEntry* ListRemoveHead(ListHead* head) {
	return ListRemoveEntry(head->next, true);
}

ListEntry* ListRemoveTail(ListHead* head) {
	return ListRemoveEntry(head->prev, true);
}

size_t ListEntryCount(ListHead* head) {
	size_t count = 0;
	ListEntry* cur = head->next;
	while (cur != head) {
		count++;
		cur = cur->next;
	}
	return count;
}

bool ListIteration(ListHead* head, ListEntry** cur) {
	if (*cur == NULL) {
		*cur = head->next;
		if (head == *cur) {
			return false;
		}
		return true;
	}
	if (head == *cur) {
		return false;
	}
	*cur = (*cur)->next;
	return true;
}