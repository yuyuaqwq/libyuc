#include "list.h"

void ListHeadInit(ListEntry* head) {
	head->prev = head;
	head->next = head;
}

bool ListHeadEmpty(ListEntry* head) {
	return head->next == head && head->prev == head;
}

void ListInsertHead(ListEntry* head, ListEntry* entry) {
	ListEntry* old = head->next;
	head->next = entry;
	entry->prev = head;
	entry->next = old;
	old->prev = entry;
}

void ListInsertTail(ListEntry* head, ListEntry* entry) {
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
		ListHeadInit(entry);
	}
	return entry;
}

ListEntry* ListRemoveHead(ListEntry* head) {
	return ListRemoveEntry(head->next, true);
}

ListEntry* ListRemoveTail(ListEntry* head) {
	return ListRemoveEntry(head->prev, true);
}

bool ListTraversal(ListEntry* head, ListEntry** cur) {
	if (head == *cur) {
		return false;
	}
	*cur = head->next;
	return true;
}