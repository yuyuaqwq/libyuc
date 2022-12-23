#include "SinglyList.h"

void SinglyListHeadInit(SinglyListEntry* head) {
	head->next = head;
}

bool SinglyListIsEmpty(SinglyListEntry* head) {
	return head->next == head;
}

void SinglyListInsertHead(SinglyListEntry* head, SinglyListEntry* entry) {
	SinglyListEntry* old = head->next;
	head->next = entry;
	entry->next = old;
}

SinglyListEntry* SinglyListRemoveHead(SinglyListEntry* head) {
	SinglyListEntry* old = head->next;
	head->next = old->next;
	return old;
}

size_t SinglyListEntryCount(SinglyListEntry* head) {
	size_t count = 0;
	SinglyListEntry* cur = head->next;
	while (cur != head) {
		count++;
		cur = cur->next;
	}
	return count;
}

bool SinglyListIteration(SinglyListEntry* head, SinglyListEntry** cur) {
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