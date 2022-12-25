#include "SinglyList.h"

void SinglyListHeadInit(SinglyListHead* head) {
	head->next = head;
}

bool SinglyListIsEmpty(SinglyListHead* head) {
	return head->next == head;
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

size_t SinglyListEntryCount(SinglyListHead* head) {
	size_t count = 0;
	SinglyListEntry* cur = head->next;
	while (cur != head) {
		count++;
		cur = cur->next;
	}
	return count;
}

bool SinglyListIteration(SinglyListHead* head, SinglyListEntry** cur) {
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