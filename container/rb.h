void BPlusLeafListInit(BPlusLeafListHead* head) {
	head->last = kBPlusEntryInvalidId;
	head->first = kBPlusEntryInvalidId;
}
void BPlusLeafListPutEntryNext(BPlusLeafListHead* head, BPlusEntryId prev_id, BPlusEntryId entry_id) {
	BPlusLeafListEntry* entry = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, entry_id);
	BPlusLeafListEntry* prev = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, prev_id);
	entry->prev = prev_id;
	entry->next = prev->next;
	BPlusLeafListEntry* prev_next = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, prev->next);
	prev->next = entry_id;
	prev_next->prev = entry_id;
	if (head->last == prev_id) head->last = entry_id;
	;
	;
	;
}
void BPlusLeafListPutFirst(BPlusLeafListHead* head, BPlusEntryId entry_id) {
	BPlusLeafListEntry* entry = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, entry_id);
	BPlusEntryId old_first_id = head->first;
	head->first = entry_id;
	if (old_first_id == kBPlusEntryInvalidId) {
		{
			if (!(head->last == kBPlusEntryInvalidId)) {
				*(int*)0 = 0;
			}
		}
		;
		entry->prev = entry_id;
		entry->next = entry_id;
		head->last = entry_id;
	}
	else {
		BPlusLeafListEntry* old_first = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, old_first_id);
		entry->prev = old_first->prev;
		entry->next = old_first_id;
		BPlusLeafListEntry* last = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, old_first->prev);
		old_first->prev = entry_id;
		last->next = entry_id;
		;
		;
	}
	;
}
void BPlusLeafListPutLast(BPlusLeafListHead* head, BPlusEntryId entry_id) {
	BPlusLeafListEntry* entry = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, entry_id);
	BPlusEntryId old_last_id = head->last;
	head->last = entry_id;
	if (old_last_id == kBPlusEntryInvalidId) {
		{
			if (!(head->first == kBPlusEntryInvalidId)) {
				*(int*)0 = 0;
			}
		}
		;
		entry->prev = entry_id;
		entry->next = entry_id;
		head->first = entry_id;
	}
	else {
		BPlusLeafListEntry* old_last = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, old_last_id);
		entry->prev = old_last_id;
		entry->next = old_last->next;
		BPlusLeafListEntry* frist = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, old_last->next);
		old_last->next = entry_id;
		frist->prev = entry_id;
		;
		;
	}
	;
}
BPlusEntryId BPlusLeafListDeleteEntry(BPlusLeafListHead* head, BPlusEntryId entry_id) {
	if (head->first == head->last) {
		entry_id = head->first;
		BPlusLeafListInit(head);
		return entry_id;
	}
	BPlusLeafListEntry* entry = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, entry_id);
	BPlusEntryId prev_id = entry->prev;
	BPlusEntryId next_id = entry->next;
	BPlusLeafListEntry* prev = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, prev_id);
	BPlusLeafListEntry* next = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, next_id);
	prev->next = next_id;
	next->prev = prev_id;
	;
	;
	if (entry_id == head->first) {
		head->first = next_id;
	}
	else if (entry_id == head->last) {
		head->last = prev_id;
	}
	;
	return;
}
BPlusEntryId BPlusLeafListDeleteFirst(BPlusLeafListHead* head) {
	return BPlusLeafListDeleteEntry(head, head->first);
}
BPlusEntryId BPlusLeafListDeleteLast(BPlusLeafListHead* head) {
	return BPlusLeafListDeleteEntry(head, head->last);
}
size_t BPlusLeafListGetCount(BPlusLeafListHead* head) {
	size_t count = 0;
	BPlusEntryId cur_id = BPlusLeafListFirst(head);
	while (cur_id != kBPlusEntryInvalidId) {
		count++;
		BPlusLeafListEntry* cur = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, cur_id);
		cur_id = BPlusLeafListNext(head, cur_id);
		;
	}
	return count;
}
BPlusEntryId BPlusLeafListFirst(BPlusLeafListHead* head) {
	return head->first;
}
BPlusEntryId BPlusLeafListLast(BPlusLeafListHead* head) {
	return head->last;
}
BPlusEntryId BPlusLeafListPrev(BPlusLeafListHead* head, BPlusEntryId cur_id) {
	BPlusLeafListEntry* cur = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, cur_id);
	BPlusEntryId prev_id = cur->prev;
	;
	if (prev_id == head->last) {
		return kBPlusEntryInvalidId;
	}
	return prev_id;
}
BPlusEntryId BPlusLeafListNext(BPlusLeafListHead* head, BPlusEntryId cur_id) {
	BPlusLeafListEntry* cur = CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(head, cur_id);
	BPlusEntryId next_id = cur->next;
	;
	if (next_id == head->first) {
		return kBPlusEntryInvalidId;
	}
	return next_id;
}