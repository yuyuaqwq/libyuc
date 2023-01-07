#include "SinglyList.h"

void SinglyListHeadInit(SinglyListHead* head) {
	head->next = NULL;
}

void SinglyListEntryInit(SinglyListEntry* entry) {
	entry->next = NULL;
}

bool SinglyListIsEmpty(SinglyListHead* head) {
	return head->next == NULL;
}

void SinglyListInsertHead(SinglyListHead* head, SinglyListEntry* entry) {
	SinglyListEntry* old = head->next;
	head->next = entry;
	entry->next = old;
}

SinglyListEntry* SinglyListRemoveHead(SinglyListHead* head) {
	SinglyListEntry* old = head->next;
	head->next = old->next;
	return old;
}

SinglyListEntry* SinglyListRemoveEntry(SinglyListEntry* prev, SinglyListEntry* removeEntry) {
	prev->next = removeEntry->next;
	return removeEntry;
}


size_t SinglyListGetCount(SinglyListHead* head) {
	size_t count = 0;
	SinglyListEntry* cur = head->next;
	while (cur != NULL) {
		count++;
		cur = cur->next;
	}
	return count;
}

SinglyListEntry* SinglyListFirst(SinglyListHead* head) {
	return head->next;
}

SinglyListEntry* SinglyListNext(SinglyListEntry* entry) {
	return entry->next;
}
