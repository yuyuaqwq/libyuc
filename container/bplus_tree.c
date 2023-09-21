#include <libyuc/basic.h>


#include <libyuc/container/bplus_tree.def>

/*

entry��
    ��ȷ��1��entry�����ܷ���3��element

    Rate���и���ҳ����ָ��
    ����ҳ����4096���ͷ���4096�ֱ�

    entry��������Ҫ�ṩ
        GetMergeThresholdRate(��ȡ�ϲ���ֵn�ֱȣ��Ƽ�Ϊ40%(������40�����ǰٷ�֮40����Ҫ����n�ֱȻ���))
            ���ڸðٷֱȵ�entry������2��element
        GetFreeRate(��ȡ����n�ֱȣ��ɷ����������)
        GetFillRate(��ȡ�����n�ֱ�)
        GetMaxRate(��ȡn)
        GetTempCopyEntry(��ȡ��ʱEntry����������ͷ���)

element:
    ����element����������kv���ܲ�����
    element��������Ҫ�ṩ
        GetNeedRate(��ȡelement��Ҫ��n�ֱ�)


����ʱ��element����Ŀռ䲻�������elementʱ��������
    ����ǰ����ǰentry����������2��element(Ҷ�ӿ���1������������2��������û�������ڵ㣬ͳһ�涨2��)
    ���ȼ��㵱ǰentry������ʣ��ټ�����element����ռ������/2
    ���ѵ�ʱ���������ʽ��з��ѣ�ʹ�ñ������һ����Ѻ������㹻�Ŀ�λ���䲢������Ԫ��
    value���ռ��25%, key���ռ��12.5%
        ��ʵ�֣�������2�Ĵ��ݣ�entry����Ҫ2��element, ��˱������Ƶ���element��ռ���ʲ��ܳ�����ҳ���50%����Ҫȷ��ÿ�η��Ѻ�һ����װ����Ԫ��
        �����value�ͷ������ҳ��
    ����˼·�����ű����ڵ㲢ժ�����ƶ���ʱ����Ҫ����element�Ĳ���
        һֱ�ͷžɣ������£�һֱ����entry�����<=1/2��ֹͣ����
        �����ʱ��Ҫ����element��ֱ�Ӳ��뵽�µ�һ�࣬�ȵ���entry����ʴﵽ��׼������
        �����entry�����<=1/2����û�в���element
            ����entry��λ�Ƿ��㹻����element������ʹ�����Ƭ�����ٲ���


ɾ��ʱ�����������<=40%(���߼�����<=80%)�򴥷��ϲ�
    �������඼��40%��һ���ܹ��ϲ��ɹ�(����һ�����п����пռ���Ƭ����)
һ�������<=40%(���߼�����>80%)�򴥷�element�ƶ�
    elementһ�������ƶ�����Ϊ����element�����25%(����һ�����п����пռ���Ƭ)
    ����ƶ�һ��֮�󻹲���40%(�ͼ����ƶ�)
    ���ܻ�����ƶ�֮���ֵ�entry������ʲ���40%(����ĩβֱ����25%ռ���ʵ�element)
        ���ÿ���ƶ�ǰ�ж��ƶ����ֵܵ�����ʻ��Ϊ���٣��������40%�Ͳ����ƶ���ֱ�ӷ���


��Ƭ����
    ������ֿռ���Ƭ(���ܿ����㹻���޷�����)������Ҫ������Ƭ����
    ��Ƭ��������ָ����
        buddy:
            ��ռ���ʴӴ�С���·�������block

    ��Ƭ�������⣺
        ���뱣֤����ʱb+���������κ�����element����������򽫻ᵼ��element_idʧЧ
        b+����֤����InsertElement���������У������������entry��element��������ʽ�����InsertElement�н�����Ƭ����

    ���ʹ����ȴ�С�Ŀ���з��䣬�߼��������Ŀ�ͨ�����������򲻴����ڴ���Ƭ
        ȱ���Ǵ��ķ����ͷŽ���


kv��������㴦��ģ�b+��������ֻ��element
��Ƭ����Ҳ����߽��У�������λ�㹻������ʧ��ʱ����


*/

/*
* ��ͨ�������������ķ�������������δ���޸�Ϊartʱʹ��
*/
//static BPlusElement BPlusEntrySplit(BPlusTree* tree, BPlusEntry* left, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id left_id, BPlusEntry* parent, int16_t parent_element_id, BPlusElement* insert_element, int16_t insert_id, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id* out_right_id) {
//    /* assert(insert_id != BPlusEntryRbReferencer_InvalidId); */
//    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id right_id = BPlusEntryCreate(tree, left->type);
//    BPlusEntry* right = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, right_id);
//    BPlusElement up_element;
//    int32_t mid;
//    int32_t right_count;
//    if (left->type == kBPlusEntryLeaf) {
//        //BPlusLeafListPutEntryNext(&tree->leaf_list, left_id, right_id);
//        /* ԭ�ط���˼·��mid��δ�����Ԫ��Ҳ���ϣ��ü���newCount��4�ײ����4�ڵ����2(��2��2)��5�ײ����5�ڵ㻹��2(��2��3)
//            ������ǰ����Ҳ�Ӧ���ж��ٸ�Ԫ�أ�������ȥ���м�������Ԫ�ز���ʹ�����һ�εĿ�����û�����ٲ��뵽��� */
//        mid = tree->leaf_m / 2;
//        right_count = left->element_count + 1 - mid;        /* +1����Ϊ���ʱ��entry->count��û�а�δ����Ԫ��Ҳ���� */
//    }
//    else {
//        /* ԭ�ط���˼·��mid��δ�����Ԫ�غͼ���������Ԫ�ض����ϣ��ü���newCount��4�ײ����4�ڵ����4/2=2(��1��2)��5�ײ����5�ڵ�Ҳ��2(��2��2)������һ������Ϊ������Ҳ������
//            �Ƚ���벿�ֿ������½ڵ㣬����м������������Ĳ��룬�Ǿ�һ�����룬����midkey��entry->indexData[entry->count-1]����Ϊ�Ҳ����������ǰ��õģ��������һ���ŵ���� */
//        mid = (tree->index_m - 1) / 2;
//        right_count = left->element_count - mid;        /* ���ʱ��entry->count��û�а�δ����Ԫ��Ҳ���ϣ����ǻ�����һ��Ԫ�أ������ʲ����� */
//    }
//    int32_t i = right_count - 1;
//    int16_t left_elemeng_id = BPlusEntryRbTreeIteratorLast(&left->rb_tree);
//    bool insert = false;
//    for (; i >= 0; i--) {
//        if (!insert && left_elemeng_id == insert_id) {
//            BPlusEntryInsertElement(tree, right, insert_element);
//            insert = true;
//            continue;
//        }
//         assert(left_elemeng_id != BPlusEntryRbReferencer_InvalidId);
//        int16_t next_elemeng_id = BPlusEntryRbTreeIteratorPrev(&left->rb_tree, left_elemeng_id);
//        BPlusEntryInsertElement(tree, right, BPlusEntryDeleteElement(tree, left, left_elemeng_id));
//        left_elemeng_id = next_elemeng_id;
//    }
//    /* ��Ԫ�ػ�û�в��룬������� */
//    if (!insert) {
//        BPlusEntryInsertElement(tree, left, insert_element);
//    }
//   
//    if (left->type == kBPlusEntryLeaf) {
//        /* ��mid�õ�����Ԫ�أ�Ҷ��Ԫ��ת��Ϊ����Ԫ�أ�����Ԫ�ص��ӽڵ�ָ����ڵ� */
//        up_element = *BPlusElementGet(tree, right, BPlusEntryRbTreeIteratorFirst(&right->rb_tree));
//        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key key = up_element.leaf.key;
//        up_element.index.key = key;
//    }
//    else {
//        /* �������½ڵ���Ҫ����
//                        15            18
//                    /                |
//                2     4     8     12        ...
//                |     |     |     |   
//                1     3     5     10     13     
//            ---------------------------
//                2     4            8     12
//                |     |            |     |    
//                1     3     13     5     10     
//            ---------------------------
//                    4         15                18
//                /            |                 |
//                2             8     12
//                |               |     |    
//                1     3         5     10    13
//            ��ʱ�ҽڵ�ȱ����һ�����ӣ���������ѡ����ڵ��ĩβԪ��(4)��Ϊ����Ԫ�أ�����ڵ��ĩβԪ�ص��Ҳ��ӽڵ�(13)�Ϳ��Թҽӵ��ҽڵ��ĩβԪ�ص��Ҳ��ӽڵ��� */
//        right->index.tail_child_id = left->index.tail_child_id;
//       
//        /* ������ڵ�ĩβ�õ�����Ԫ�أ�����ժ�� */
//        up_element = *BPlusEntryDeleteElement(tree, left, BPlusEntryRbTreeIteratorLast(&left->rb_tree));
//        left->index.tail_child_id = up_element.index.child_id;         /* 3ָ��Ϊ2���Ҳ��ӽڵ� */
//    }
//    /* ������4���ӽڵ�Ϊ�� */
//    up_element.index.child_id = left_id;
//   
//    /* 4������ԭ��ָ��4�ĸ�Ԫ�أ���ָ��8|12��(ԭ��ָ����ڵ�ĸ�Ԫ��ָ���ҽڵ㣬��Ϊ������Ԫ�ػ��ɸ�Ԫ�ص��ֵܣ�ָ����ڵ�) */
//    BPlusElementSetChildId(tree, parent, parent_element_id, right_id);
//   
//    *out_right_id = right_id;
//    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, right);
//    return up_element;
//}


#include <libyuc/container/bplus_tree_BPlusIteratorStackVector.cfg>
#include <libyuc/container/vector.c>

/*
* B+��������
*/\
static const LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id BPlusLeafEntryReferencer_InvalidId = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId;

//forceinline BPlusLeafListEntry* BPlusLeafEntryReferencer_Reference(BPlusLeafListHead* head, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id entry_id) {
//    BPlusTree* tree = ObjectGetFromField(head, BPlusTree, leaf_list);
//    BPlusEntry* entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, entry_id);
//    return &entry->leaf.list_entry;
//}
//forceinline void BPlusLeafEntryReferencer_Dereference(BPlusLeafListHead* head, BPlusLeafListEntry* list_entry) {
//    BPlusTree* tree = ObjectGetFromField(head, BPlusTree, leaf_list);
//    BPlusLeafEntry* entry = ObjectGetFromField(list_entry, BPlusLeafEntry, list_entry);
//    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, (BPlusEntry*)entry);
//}

//LIBYUC_CONTAINER_VECTOR_DEFINE(
//    BPlusIteratorStack,
//    LIBYUC_CONTAINER_VECTOR_MODE_STATIC,
//    entry_count_type,
//    entry_count_type,
//    BPlusElementPos,
//    BPlusElementPos,
//    LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT,
//    iterator_allocator,
//    LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT,
//    LIBYUC_CONTAINER_VECTOR_COMPARER_INVALID,
//    LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT
//    )



/*
* ҳ�ں����
*/\
static const LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntryRbReferencer_InvalidId = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId;
forceinline BPlusEntryRbEntry* BPlusEntryRbReferencer_Reference(BPlusEntryRbTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id) {
    if (element_id == BPlusEntryRbReferencer_InvalidId) {
        return NULL;
    }
    BPlusEntry* entry = ObjectGetFromField(tree, BPlusEntry, rb_tree);
    return &(LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(entry, element_id)->rb_entry);
}
forceinline void BPlusEntryRbReferencer_Dereference(BPlusEntryRbTree* tree, BPlusEntryRbEntry* rb_entry) {
    BPlusEntry* entry = ObjectGetFromField(tree, BPlusEntry, rb_tree);
    BPlusElement* element = ObjectGetFromField(rb_entry, BPlusElement, rb_entry);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(entry, element);
}

typedef struct {
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id left : sizeof(LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id) * 8 - 1;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset color : 1;
} BPlusEntryRbParentColor;

forceinline LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntryRbAccessor_GetLeft(BPlusEntryRbTree* tree, BPlusEntryRbBsEntry* bs_entry) {
    return ((BPlusEntryRbParentColor*)bs_entry)->left;
}
forceinline BPlusEntryRbColor BPlusEntryRbAccessor_GetColor(BPlusEntryRbTree* tree, BPlusEntryRbBsEntry* bs_entry) {
    return ((BPlusEntryRbParentColor*)bs_entry)->color;
}
forceinline void BPlusEntryRbAccessor_SetLeft(BPlusEntryRbTree* tree, BPlusEntryRbBsEntry* bs_entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id new_id) {
    ((BPlusEntryRbParentColor*)bs_entry)->left = new_id;
}
forceinline void BPlusEntryRbAccessor_SetColor(BPlusEntryRbTree* tree, BPlusEntryRbBsEntry* bs_entry, BPlusEntryRbColor new_color) {
    ((BPlusEntryRbParentColor*)bs_entry)->color = new_color;
}
forceinline LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntryRbAccessor_GetRight(BPlusEntryRbTree* tree, BPlusEntryRbBsEntry* bs_entry) {
    return (LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id)bs_entry->right;
}
forceinline void BPlusEntryRbAccessor_SetRight(BPlusEntryRbTree* tree, BPlusEntryRbBsEntry* bs_entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id new_id) {
    bs_entry->right = (BPlusEntryRbEntry*)new_id;
}
forceinline LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* BPlusEntryRbAccessor_GetKey(BPlusEntryRbTree* tree, BPlusEntryRbBsEntry* bs_entry) {
    BPlusElement* element = ObjectGetFromField(bs_entry, BPlusElement, rb_entry);
    return &element->key;
}

// LIBYUC_CONTAINER_RB_TREE_DEFINE(BPlusEntry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key, BPlusEntryRbReferencer, BPlusEntryRbAccessor, element_key_comparer)

#include <libyuc/container/bplus_tree_BPlusEntryRbTree.cfg>
#define LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference BPlusEntryRbReferencer_Reference
#define LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference BPlusEntryRbReferencer_Dereference
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft BPlusEntryRbAccessor_GetLeft
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight BPlusEntryRbAccessor_GetRight
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor BPlusEntryRbAccessor_GetColor
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetLeft BPlusEntryRbAccessor_SetLeft
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetRight BPlusEntryRbAccessor_SetRight
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor BPlusEntryRbAccessor_SetColor
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetKey BPlusEntryRbAccessor_GetKey
#include <libyuc/container/rb_tree.c>



/*
* B+��
*/\
static void BPlusElementSet(BPlusEntry* dst_entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id, BPlusEntry* src_entry, BPlusElement* element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id element_child_id) {
    assert(element_id >= 0);
    BPlusElement* dst_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(dst_entry, element_id);
    if (dst_entry->type == kBPlusEntryLeaf) {
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetKey(dst_entry, dst_element, src_entry, &element->leaf.key);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetValue(dst_entry, dst_element, src_entry, &element->leaf.value);
    }
    else {
        /* ����ǲ��뵽index_entry������������leaf��element */
        if (src_entry->type == kBPlusEntryLeaf) {
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetKey(dst_entry, dst_element, src_entry, &element->leaf.key);
            assert(element_child_id != LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
            dst_element->index.child_id = element_child_id;
        }
        else {
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetKey(dst_entry, dst_element, src_entry, &element->index.key);
            /* ����и����ĺ���id����ʹ�ø����ĺ���id */
            if (element_child_id != LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId) {
                dst_element->index.child_id = element_child_id;
            }
            else {
                dst_element->index.child_id = element->index.child_id;
            }
        }
    }
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(dst_entry, dst_element);
}
static LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id BPlusElementGetChildId(const BPlusEntry* index, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id) {
    if (element_id == BPlusEntryRbReferencer_InvalidId) {
        return index->index.tail_child_id;
    }
    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(index, element_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id child_id = element->index.child_id;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(index, element);
    return child_id;
}
static void BPlusElementSetChildId(BPlusEntry* index, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id entry_id) {
    if (element_id == BPlusEntryRbReferencer_InvalidId) {
        index->index.tail_child_id = entry_id;
        return;
    }
    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(index, element_id);
    element->index.child_id = entry_id;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(index, element);
}
static LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusElementCreate(BPlusEntry* entry) {
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ALLOCATOR_CreateBySize(entry, entry->type == kBPlusEntryLeaf ? sizeof(BPlusLeafElement) : sizeof(BPlusIndexElement));
      assert(element_id >= 0);
    return element_id;
}
static BPlusElement* BPlusElementRelease(BPlusEntry* entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id) {
      assert(element_id >= 0);
    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(entry, element_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(entry, element);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ALLOCATOR_Release(entry, element_id);
    return element;
}


/* B+�������� */
BPlusIteratorStatus BPlusIteratorTop(BPlusTree* tree, BPlusIterator* iterator, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key) {
    BPlusIteratorStackVectorInit(&iterator->stack, STACK_SIZE);
    iterator->stack.count = 0;
    iterator->level = -1;
    return BPlusIteratorDown(tree, iterator, key);
}
void BPlusIteratorRelease(BPlusTree* tree, BPlusIterator* iterator) {
    BPlusIteratorStackVectorRelease(&iterator->stack);
}
BPlusElementPos* BPlusIteratorCur(BPlusTree* tree, BPlusIterator* iterator) {
    if (iterator->level < 0) {
        return NULL;
    }
    return &iterator->stack.obj_arr[iterator->level];
}
BPlusElementPos* BPlusIteratorUp(BPlusTree* tree, BPlusIterator* iterator) {
    if (iterator->level <= 0) {
        return NULL;
    }
    return &iterator->stack.obj_arr[--iterator->level];
}
BPlusIteratorStatus BPlusIteratorDown(BPlusTree* tree, BPlusIterator* iterator, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key) {
    BPlusElementPos cur;
    BPlusElementPos* parent = BPlusIteratorCur(tree, iterator);
    if (parent) {
        BPlusEntry* parent_entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, parent->entry_id);
        if (parent_entry->type == kBPlusEntryLeaf) {
            return kBPlusIteratorEnd;
        }
        cur.entry_id = BPlusElementGetChildId(parent_entry, parent->element_iterator.cur_id);
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, parent_entry);
    }
    else {
        cur.entry_id = tree->root_id;
    }
    BPlusEntry* cur_entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, cur.entry_id);
    int8_t cmp_status = -1;
    if (cur_entry->element_count > 0) {
        BPlusEntryRbTreeIteratorLocate(&cur_entry->rb_tree, &cur.element_iterator, key, &cmp_status);
        if (cmp_status == -1) { /* keyС�ڵ�ǰ��λԪ�� */ }
        else { /* key���ڵ��ڵ�ǰ��λԪ�� */
            if (cur_entry->type == kBPlusEntryIndex || cmp_status == 1) {
                BPlusEntryRbTreeIteratorNext(&cur_entry->rb_tree, &cur.element_iterator);
            }
        }
    }
    else {
        cur.element_iterator.cur_id = BPlusEntryRbReferencer_InvalidId;
    }
    BPlusIteratorStackVectorPushTail(&iterator->stack, &cur);
    BPlusIteratorStatus status = kBPlusIteratorDown;
    if (cur_entry->type == kBPlusEntryLeaf) {
        if (cmp_status != 0) {
            status = kBPlusIteratorNe;
        }
        else {
            status = kBPlusIteratorEq;
        }
        iterator->leaf_status = status;
    }
    ++iterator->level;
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, cur_entry);
    return status;
}


/*
* ���ӹ�ϵ��Ӧ����
*         3     |    6    |    8
*        /            /        /
*     1|2         4|5         7        10|11
* �����滭��ʾ����Ĭ����3��Ӧ1|2��6��Ӧ4|5��8��Ӧ7
*/
/*
* ��ڵ����Ԫ��
* ��֧�ִ���ĩβ�ӽڵ�
*/
static LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntryInsertElement(BPlusEntry* dst_entry, BPlusEntry* src_entry, BPlusElement* insert_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id element_child_id) {
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id = BPlusElementCreate(dst_entry);
      assert(element_id != BPlusEntryRbReferencer_InvalidId);
    BPlusElementSet(dst_entry, element_id, src_entry, insert_element, element_child_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id old_element_id = BPlusEntryRbTreePut(&dst_entry->rb_tree, element_id);
    if (old_element_id != BPlusEntryRbReferencer_InvalidId && old_element_id != element_id) BPlusElementRelease(dst_entry, old_element_id);
    dst_entry->element_count++;
    return element_id;
}
/*
* �ӽڵ���ɾ��ָ����Ԫ��
* ��֧�ִ���ĩβ�ӽڵ�
* ���ر�ɾ����Ԫ��
*/
static void BPlusEntryDeleteElement(BPlusEntry* entry, BPlusEntryRbTreeIterator* element_iterator) {
    assert(element_iterator->cur_id != BPlusEntryRbReferencer_InvalidId);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id del_element_id = element_iterator->cur_id;
    BPlusEntryRbTreeDeleteByIterator(&entry->rb_tree, element_iterator);
    entry->element_count--;
    BPlusElementRelease(entry, del_element_id);
}

void BPlusEntryInit(BPlusTree* tree, BPlusEntry* entry, BPlusEntryType type) {
    entry->type = type;
    entry->element_count = 0;
    BPlusEntryRbTreeInit(&entry->rb_tree);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_InitCallback(tree, entry);
}
LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id BPlusEntryCreate(BPlusTree* tree, BPlusEntryType type) {
    size_t size;
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id entry_id;
    if (type == kBPlusEntryIndex) {
        entry_id = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ALLOCATOR_CreateBySize(tree, sizeof(BPlusEntry) - sizeof(BPlusLeafEntry) + sizeof(BPlusIndexEntry));
    }
    else {
        entry_id = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ALLOCATOR_CreateBySize(tree, sizeof(BPlusEntry) - sizeof(BPlusIndexEntry) + sizeof(BPlusLeafEntry));
    }
    BPlusEntry* entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, entry_id);
    BPlusEntryInit(tree, entry, type);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, entry);
    return entry_id;
}
void BPlusEntryRelease(BPlusTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id entry_id) {
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ALLOCATOR_Release(tree, entry_id);
}

/*
* �������������ο�https://leetcode.cn/problems/convert-sorted-list-to-binary-search-tree/solution/you-xu-lian-biao-zhuan-huan-er-cha-sou-suo-shu-1-3/
*/
static LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BuildRbTree(BPlusEntry* src_entry, BPlusEntryRbTreeIterator* element_iterator, BPlusEntry* dst_entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id left, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id right, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id parent_id, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset max_level, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset level) {
    if (left > right) return LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id mid = (left + right + 1) / 2;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id new_element_id = BPlusElementCreate(dst_entry);
    BPlusElement* new_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(dst_entry, new_element_id);
    BPlusEntryRbEntry* rb_entry = &new_element->rb_entry;
    BPlusEntryRbAccessor_SetColor(&dst_entry->rb_tree, rb_entry, level == max_level && max_level >= 1 ? kRbRed : kRbBlack);
    BPlusEntryRbAccessor_SetLeft(&dst_entry->rb_tree, rb_entry, BuildRbTree(src_entry, element_iterator, dst_entry, left, mid - 1, new_element_id, max_level, level + 1));
    assert(element_iterator->cur_id != LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId);
    BPlusElement* src_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(src_entry, element_iterator->cur_id);
    BPlusElementSet(dst_entry, new_element_id, src_entry, src_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(src_entry, src_element);
    BPlusEntryRbTreeIteratorNext(&src_entry->rb_tree, element_iterator);
    BPlusEntryRbAccessor_SetRight(&dst_entry->rb_tree, rb_entry, BuildRbTree(src_entry, element_iterator, dst_entry, mid + 1, right, new_element_id, max_level, level + 1));
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(dst_entry, new_element);
    return new_element_id;
}
/*
* ���ѽڵ�
* ��������������Ԫ��
*
* ͬһ��Ҷ�ڵ��η��ѻᵼ���ظ���key������
* ���Ҷ�ڵ㲻������ͬ��key����ô�ǲ����
* ��Ϊ���Ѻ�ѡ���ҽڵ������Ԫ�ص�key��Ϊ������key
* ���ʱ��������ô����Ԫ�ض�������뵽�ýڵ�����Ԫ�ص����(����С�Ļᱻ�ֵ����ڵ㣬��Ϊ��Ԫ��key���ڸ�Ԫ��)���ýڵ��ٷ���Ҳ�Ͳ���������Ԫ���ٴ������Ŀ�����
*
* ���޸�src_entryָ���µ�right_entry���������ͷ�ԭ����src_entry
*/
static LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntrySplit(BPlusTree* tree, BPlusEntry* left, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id left_id, BPlusEntry* parent, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id parent_element_id, BPlusEntry** src_entry, BPlusElement* insert_element, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id insert_id, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id insert_element_child_id, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id* out_right_id, BPlusEntryRbTreeIterator* up_element_iterator) {
    /* assert(insert_id != BPlusEntryRbReferencer_InvalidId); */
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id right_id = BPlusEntryCreate(tree, left->type);
    BPlusEntry* right = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, right_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id up_element_id;
    if (left->type == kBPlusEntryLeaf) {
        //BPlusLeafListPutEntryNext(&tree->leaf_list, left_id, right_id);
    }
    BPlusEntryRbTreeIterator left_element_iterator;
    BPlusEntryRbTreeIteratorLast(&left->rb_tree, &left_element_iterator);
    int insert_right = 0;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset fill_rate = (LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFillRate(tree, left) + LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(right, *src_entry, insert_element)) / 2;
    /*
    * ������Ҳ�������ԵĽڵ�����
    */
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset right_count = 1, left_count = 0;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset left_fill_rate = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFillRate(tree, left);
    while (true) {
        if (!insert_right && left_element_iterator.cur_id == insert_id) {
            insert_right = 1;
        }
        if (left_fill_rate <= fill_rate || left->element_count - right_count <= 2 + insert_right) {
            break;
        }
        left_fill_rate -= LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(left, right, insert_element);
        BPlusEntryRbTreeIteratorPrev(&left->rb_tree, &left_element_iterator);
        ++right_count;
    }
    assert(left_element_iterator.cur_id != LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId);
    assert(right_count > 0);
    /* �ȹ����Ҳ�ڵ��rb�� */
    BPlusEntryRbTreeIterator mid_element_iterator;
    BPlusEntryRbTreeIteratorCopy(&mid_element_iterator, &left_element_iterator);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset logn = 0;
    for (LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset i = right_count; i > 0; i /= 2)
        ++logn;
    --logn;
    right->rb_tree.root = BuildRbTree(left, &left_element_iterator, right, 0, right_count - 1, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId, logn, 0);

    /* ����������� */
    /* ��������ʱ�ڵ㣬��ԭ�ڵ����ع�rb�� */
    BPlusEntry* temp_entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetTempCopyEntry(tree, left);
    BPlusEntryInit(tree, left, right->type);
    do {
        BPlusEntryRbTreeIteratorPrev(&temp_entry->rb_tree, &mid_element_iterator);
        ++left_count;
    } while (mid_element_iterator.cur_id != LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId);
    --left_count;
    BPlusEntryRbTreeIteratorFirst(&temp_entry->rb_tree, &left_element_iterator);
    logn = 0;
    for (LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset i = left_count; i > 0; i /= 2)
        ++logn;
    --logn;
    left->rb_tree.root = BuildRbTree(temp_entry, &left_element_iterator, left, 0, left_count - 1, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId, logn, 0);

    /* ���ղ���ڵ� */
    if (insert_right == 1) {
        BPlusEntryInsertElement(right, *src_entry, insert_element, insert_element_child_id);
        ++right_count;
    }
    else {
        BPlusEntryInsertElement(left, *src_entry, insert_element, insert_element_child_id);
        ++left_count;
    }
    left->element_count = left_count;
    right->element_count = right_count;

    BPlusElement* up_element;
    if (left->type == kBPlusEntryLeaf) {
        /* ��mid�õ�����Ԫ�أ�Ҷ��Ԫ��ת��Ϊ����Ԫ�أ�����Ԫ�ص��ӽڵ�ָ����ڵ㣬Ҷ����������Ҫ��ժ�� */
        *src_entry = right;
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, right_id);
        up_element_id = BPlusEntryRbTreeIteratorFirst(&right->rb_tree, up_element_iterator);
        up_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(*src_entry, up_element_id);
    }
    else {
        /* �������½ڵ���Ҫ����
                    15             18
                    /                |
                2     4     8     12        ...
                |     |     |     |
                1     3     5     10     13
            ---------------------------
                2     4              8     12
                |     |            |     |
                1     3     13       5     10
            ---------------------------
                    4             15                18
                /              |                 |
                2               8     12
                |             |     |
                1     3         5     10    13
            ��ʱ�ҽڵ�ȱ����һ�����ӣ���������ѡ����ڵ��ĩβԪ��(4)��Ϊ����Ԫ�أ�����ڵ��ĩβԪ�ص��Ҳ��ӽڵ�(13)�Ϳ��Թҽӵ��ҽڵ��ĩβԪ�ص��Ҳ��ӽڵ��� */
        right->index.tail_child_id = left->index.tail_child_id;

        /* ������ڵ�ĩβ�õ�����Ԫ�أ��ȴ�����ժ�� */
        up_element_id = BPlusEntryRbTreeIteratorLast(&left->rb_tree, up_element_iterator);
        *src_entry = left;
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, left_id); /* �ۼ�һ�μ��� */
        up_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(*src_entry, up_element_id);
        left->index.tail_child_id = up_element->index.child_id;         /* 3ָ��Ϊ2���Ҳ��ӽڵ� */
    }
    /* ������4���ӽڵ�Ϊ�� */
    /*up_element->index.child_id = left_id;     up_element������leaf��Ԫ�أ�����ֱ���ڴ˴���ֵ */

    /* 4������ԭ��ָ��4�ĸ�Ԫ�أ���ָ��8|12��(ԭ��ָ����ڵ�ĸ�Ԫ��ָ���ҽڵ㣬��Ϊ������Ԫ�ػ��ɸ�Ԫ�ص��ֵܣ�ָ����ڵ�) */
    BPlusElementSetChildId(parent, parent_element_id, right_id);

    *out_right_id = right_id;
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, right);
    assert(left->element_count >= 1);
    assert(right->element_count >= 1);
    return up_element_id;
}
/*
* �ϲ��ڵ�
* �ͷ��ҽڵ㣬����ʱ�ҽڵ㲻����Ч
*     2
* 1     3
*/
static void BPlusEntryMerge(BPlusTree* tree, BPlusEntry* left, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id left_id, BPlusEntry* right, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id right_id, BPlusEntry* parent, BPlusEntryRbTreeIterator* parent_iterator) {
    BPlusEntryRbTreeIterator element_iterator;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id right_element_id = BPlusEntryRbTreeIteratorLast(&right->rb_tree, &element_iterator);
    for (LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset i = 0; i < right->element_count; i++) {
        assert(right_element_id != BPlusEntryRbReferencer_InvalidId);
        BPlusElement* right_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(right, right_element_id);
        BPlusEntryInsertElement(left, right, right_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(right, right_element);
        right_element_id = BPlusEntryRbTreeIteratorPrev(&right->rb_tree, &element_iterator);
    }
    if (left->type == kBPlusEntryLeaf) {
        //BPlusLeafListDeleteEntry(&tree->leaf_list, right_id);
        /* ��Ҷ�ӽڵ㣬��right����left�У���ɾ����Ԫ�� */
    }
    else {
        /* �������ڵ㣬����Ԫ��(�ӽڵ�ԭ��ָ������ң��½���Ҫָ�����ĩβ�ӽڵ�)��right�����뵽left�У����ϴ���ɾ����Ԫ�� */
        BPlusElement* parent_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, parent_iterator->cur_id);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id left_element_id = BPlusEntryInsertElement(left, parent, parent_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, parent_element);
        BPlusElementSetChildId(left, left_element_id, left->index.tail_child_id);         /* left��ĩβԪ�ش�ʱΪ�½��ĸ�Ԫ�أ��޸����ӽڵ� */
        BPlusElementSetChildId(left, -1, right->index.tail_child_id);
    }

    /* ��Ԫ�ػᱻɾ���������ֵ�Ԫ�ص�����ӽڵ�(��β�ӽڵ�)ԭ��ָ���ҽڵ�(��ɾ��)����ʱ�̳���ڵ�(�ϲ���ʣ�µĽڵ�) */
    BPlusElementSetChildId(parent, BPlusEntryRbTreeIteratorNext(&parent->rb_tree, parent_iterator), left_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, right);
    BPlusEntryRelease(tree, right_id);
    /* �ϲ�������ɣ�ɾ�����ֽ��������� */
}

/*
* ��������Ԫ��
* ������src_entry������Ȩ
*/
static bool BPlusTreeInsertElement(BPlusTree* tree, BPlusIterator* iterator, BPlusEntry* src_entry, BPlusElement* insert_element, BPlusEntryRbTreeIterator* insert_element_iterator, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id insert_element_child_id) {
    BPlusElementPos* cur_pos = BPlusIteratorCur(tree, iterator);
    BPlusElementPos* parent_pos = BPlusIteratorUp(tree, iterator);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id right_id;
    BPlusEntry* old_src_entry = src_entry;
    BPlusEntry* cur = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, cur_pos->entry_id);
    bool success = true, insert_up = false;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id up_element_id = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId;
    BPlusEntryRbTreeIterator up_element_iterator;
    do {
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset free_rate = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFreeRate(tree, cur);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset need_rate = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(cur, old_src_entry, insert_element);
        if (iterator->leaf_status == kBPlusIteratorEq) {
            BPlusElement* raw = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(cur, cur_pos->element_iterator.cur_id);
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset raw_rate = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(cur, cur, raw);
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(cur, raw);
            if (free_rate + need_rate >= raw_rate) {
                /* SetValue�����ͷ�ԭ�пռ䣬��˿��пռ���Լ����ѷ���Ĳ��� */
                LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetValue(cur, raw, cur, &insert_element->leaf.value);
                break;
            }
            else {
                /* �ռ䲻�㽫�ᴥ�����ѣ�����ʲô���������ڷ���ʱ����SetValue */
                assert(0);
            }
        }
        else if (LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFreeRate(tree, cur) >= LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(cur, old_src_entry, insert_element)) {
            /* �п����λ�ò��� */
            BPlusEntryInsertElement(cur, old_src_entry, insert_element, insert_element_child_id);
            break;
        }
        else {
            /* û�ж���λ�ã���Ҫ�������ϲ��룬�����λ����Ҫ�ǵ�һ��С��key��Ԫ�أ�element_idָ���һ������key��Ԫ�� */
            if (cur_pos->element_iterator.cur_id == BPlusEntryRbReferencer_InvalidId) { /* �����ڴ���key��Ԫ�أ�ֱ����ĩβԪ�� */
                BPlusEntryRbTreeIteratorLast(&cur->rb_tree, &cur_pos->element_iterator);
            }
            else {
                BPlusEntryRbTreeIteratorPrev(&cur->rb_tree, &cur_pos->element_iterator);
            }
        }
        if (!parent_pos) {
            /* û�и��ڵ㣬���� */
            LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id parent_id = BPlusEntryCreate(tree, kBPlusEntryIndex);
            BPlusEntry* parent = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, parent_id);
            up_element_id = BPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, -1, &src_entry, insert_element, cur_pos->element_iterator.cur_id, insert_element_child_id, &right_id, &up_element_iterator);
            BPlusElement* up_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(src_entry, up_element_id);
            BPlusEntryInsertElement(parent, src_entry, up_element, cur_pos->entry_id);
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(src_entry, up_element);
            tree->root_id = parent_id;
            LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, parent);
            if (src_entry->type == kBPlusEntryIndex) {
                /* �������ڵ�Ļ�����������Ҫ�����ˣ�ֱ��ժ��Ҫ�����Ľڵ� */
                BPlusEntryDeleteElement(src_entry, &up_element_iterator);
            }
            break;
        }
        BPlusEntry* parent = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, parent_pos->entry_id);
        up_element_id = BPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, parent_pos->element_iterator.cur_id, &src_entry, insert_element, cur_pos->element_iterator.cur_id, insert_element_child_id, &right_id, &up_element_iterator);
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, parent);
        insert_up = true;
    } while (false);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, cur);
    if (old_src_entry) {
        /* �����ڵ�������ԭ�ڵ���Ҫ��ժ�� */
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(old_src_entry, insert_element);
        if (old_src_entry->type == kBPlusEntryIndex) {
            BPlusEntryDeleteElement(old_src_entry, insert_element_iterator);
        }
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, old_src_entry);
    }
    if (insert_up) {
        BPlusElement* up_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(src_entry, up_element_id);
        return BPlusTreeInsertElement(tree, iterator, src_entry, up_element, &up_element_iterator, cur_pos->entry_id);
    }
    return success;
}

/*
* ɾ�����е�ָ��Ԫ��
*/
static bool BPlusTreeDeleteElement(BPlusTree* tree, BPlusIterator* iterator) {
    BPlusElementPos* cur_pos = BPlusIteratorCur(tree, iterator);
    BPlusElementPos* parent_pos = BPlusIteratorUp(tree, iterator);
    BPlusEntry* entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, cur_pos->entry_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id sibling_entry_id = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId;
    BPlusEntry* sibling = NULL;
    BPlusEntry* parent = NULL;
    bool success = true, delete_up = false;
    BPlusEntryDeleteElement(entry, &cur_pos->element_iterator);        /* ֱ��ɾ�����ɣ�Ҷ��Ԫ��û���ӽڵ㣬����Ԫ���ںϲ�ʱ�Ѿ������ӽڵ��� */
    do {
        if (LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFillRate(tree, entry) >= LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetMergeThresholdRate(tree, entry)) {
            break;
        }
        if (!parent_pos) {
            /* û�и��ڵ���Ѿ������ڵ��ˣ���Ҷ�ӽڵ���������������ڵ����ж��Ƿ�û���κ��ӽڵ��ˣ��������������һ���ӽڵ�Ϊ���ڵ㣬����ֱ�ӽ��� */
            if (entry->type == kBPlusEntryIndex && entry->element_count == 0) {
                LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id childId = entry->index.tail_child_id;
                tree->root_id = childId;
                LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, entry);
                BPlusEntryRelease(tree, cur_pos->entry_id);
                return true;
            }
            else {
                break;
            }
        }
        parent = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, parent_pos->entry_id);
        bool left_sibling = true;
        BPlusEntryRbTreeIterator common_parent_element_iterator;
        BPlusEntryRbTreeIteratorCopy(&common_parent_element_iterator, &parent_pos->element_iterator);     /* ʼ����ָ�����Ԫ�� */
        BPlusEntryRbTreeIterator sibling_element_iterator;
        if (common_parent_element_iterator.cur_id == BPlusEntryRbReferencer_InvalidId) { /* �����ڴ���key�ĸ�Ԫ�أ���ǰ�ڵ���ĩβ�ڵ㣬�����ֵ� */
            BPlusEntryRbTreeIteratorLast(&parent->rb_tree, &sibling_element_iterator);
        }
        else {
            BPlusEntryRbTreeIteratorCopy(&sibling_element_iterator, &common_parent_element_iterator);
            BPlusEntryRbTreeIteratorPrev(&parent->rb_tree, &sibling_element_iterator);
            if (sibling_element_iterator.cur_id == BPlusEntryRbReferencer_InvalidId) {     /* ��ǰԪ���Ѿ�����С��Ԫ�� */
                left_sibling = false;
                BPlusEntryRbTreeIteratorCopy(&sibling_element_iterator, &common_parent_element_iterator);
                BPlusEntryRbTreeIteratorNext(&parent->rb_tree, &sibling_element_iterator);
                if (sibling_element_iterator.cur_id == BPlusEntryRbReferencer_InvalidId) { /* ֱ���Ҽ�û�����ֵ�Ҳû�����ֵܣ��Ǿ���ĩβ�ڵ������ֵ� */
                    sibling_entry_id = parent->index.tail_child_id;
                }
            }
        }
        if (sibling_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId) {
            BPlusElement* sibling_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, sibling_element_iterator.cur_id);
            sibling_entry_id = sibling_element->index.child_id;
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, sibling_element);
        }
        if (left_sibling) {
            BPlusEntryRbTreeIteratorCopy(&common_parent_element_iterator, &sibling_element_iterator);
            BPlusEntryRbTreeIteratorCopy(&parent_pos->element_iterator, &sibling_element_iterator);       /* ����һ�£������ڵ�ɾ��ʹ�� */
        }
        assert(common_parent_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
        assert(sibling_entry_id != LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
        sibling = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, sibling_entry_id);
        if (LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFillRate(tree, sibling) > LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetMergeThresholdRate(tree, sibling)) {
            /* ���ֵܽ�ڵ� */
            assert(sibling->element_count >= 2);
            if (entry->type == kBPlusEntryLeaf) {
                /* Ҷ�ӽڵ㴦��ϼ򵥣�����ֱ���ƶ� */
                if (left_sibling) {
                    /* ���ֵܽڵ��ĩβ��Ԫ�ز��뵽��ǰ�ڵ��ͷ�������¸�Ԫ��keyΪ������key */
                    BPlusEntryRbTreeIterator last_element_iterator;
                    BPlusEntryRbTreeIteratorLast(&sibling->rb_tree, &last_element_iterator);
                    assert(last_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, last_element_iterator.cur_id);
                    BPlusEntryInsertElement(entry, sibling, element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, element);
                    BPlusEntryDeleteElement(sibling, &last_element_iterator);

                    BPlusElement* common_parent_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
                    common_parent_element->index.key = element->leaf.key;         /* �������� */
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, common_parent_element);
                }
                else {
                    /* ���ֵܽڵ��ͷ����Ԫ�ز��뵽��ǰ�ڵ��β�������¸�Ԫ��keyΪ���ֵܵ�����Ԫ�� */
                    BPlusEntryRbTreeIterator first_element_iterator;
                    BPlusEntryRbTreeIteratorFirst(&sibling->rb_tree, &first_element_iterator);
                    BPlusEntryRbTreeIterator new_element_iterator;
                    BPlusEntryRbTreeIteratorCopy(&new_element_iterator, &first_element_iterator);
                    BPlusEntryRbTreeIteratorNext(&sibling->rb_tree, &new_element_iterator);
                    assert(first_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    assert(new_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, first_element_iterator.cur_id);
                    BPlusEntryInsertElement(entry, sibling, element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, element);
                    BPlusEntryDeleteElement(sibling, &first_element_iterator);
                    /* �ҽڵ��ͷԪ��key�������ú͹�ͬ���ڵ����(��ʱ��������ȵ�key�ܵ���ߣ��ͻᵼ���Ҳ���)�����key����Ϊ�µ���Ԫ������õ� */
                    BPlusElement* common_parent_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
                    BPlusElement* sibling_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, new_element_iterator.cur_id);
                    common_parent_element->index.key = sibling_element->leaf.key;         /* �������� */
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, common_parent_element);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, sibling_element);
                }
            }
            else {
                /* �����ڵ㴦��ϸ��ӣ���Ҫ�½����ڵ��е�ǰ�ڵ���ֵܽڵ�Ĺ�ͬ��Ԫ�أ������ֵܵ�Ԫ�ص���ͬ��Ԫ��(������) */
                if (left_sibling) {
                    /* ���ֵܽڵ��ĩβԪ�����������ڵ��ͷ�������ڵ�Ķ�ӦԪ���½�����ǰ�ڵ��ͷ��������Ԫ�������ӽڵ�����½��ĸ��ڵ�Ԫ�ص���� */
                    BPlusEntryRbTreeIterator last_element_iterator;
                    BPlusEntryRbTreeIteratorLast(&sibling->rb_tree, &last_element_iterator);
                    assert(last_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    BPlusElement* left_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, last_element_iterator.cur_id);
                    ObjectSwap(LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id, left_element->index.child_id, sibling->index.tail_child_id);        /* Ҫ�õ���ĩβ���ӽڵ㣬����һ�� */
                    BPlusElement* par_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
                    par_element->index.child_id = left_element->index.child_id;
                    BPlusEntryInsertElement(entry, parent, par_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, par_element);
                    BPlusEntryDeleteElement(parent, &common_parent_element_iterator);
                    left_element->index.child_id = sibling_entry_id;
                    BPlusEntryInsertElement(parent, sibling, left_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, left_element);
                    BPlusEntryDeleteElement(sibling, &last_element_iterator);
                }
                else {
                    /* ���ֵܽڵ��ͷԪ�����������ڵ��ͷ�������ڵ�Ķ�ӦԪ���½�����ǰ�ڵ��β��������Ԫ�������ӽڵ�����½��ĸ��ڵ�Ԫ�ص��Ҳ� */
                    BPlusEntryRbTreeIterator first_element_iterator;
                    BPlusEntryRbTreeIteratorFirst(&sibling->rb_tree, &first_element_iterator);
                    assert(first_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    BPlusElement* right_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, first_element_iterator.cur_id);
                    BPlusElement* par_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
                    par_element->index.child_id = right_element->index.child_id;
                    ObjectSwap(LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id, par_element->index.child_id, entry->index.tail_child_id);        /* Ҫ�������ĩβ���ӽڵ㣬����һ�� */
                    BPlusEntryInsertElement(entry, parent, par_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, par_element);
                    BPlusEntryDeleteElement(parent, &common_parent_element_iterator);
                    right_element->index.child_id = cur_pos->entry_id;
                    BPlusEntryInsertElement(parent, sibling, right_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, right_element);
                    BPlusEntryDeleteElement(sibling, &first_element_iterator);
                }
            }
            break;
        }

        /* �ֵܽڵ㲻���裬��Ҫ�ϲ�(�ϲ���Ҳ���ᳬ��m-1����Ϊһ�߲���m-1��һ�룬һ����m-1��һ�룬�������ڵ�ĺϲ�Ҳ�㹻�½�һ����Ԫ��) */
        if (left_sibling) {
            BPlusEntryMerge(tree, sibling, sibling_entry_id, entry, cur_pos->entry_id, parent, &common_parent_element_iterator);
            entry = NULL;
        }
        else {
            BPlusEntryMerge(tree, entry, cur_pos->entry_id, sibling, sibling_entry_id, parent, &common_parent_element_iterator);
            sibling = NULL;
        }
        delete_up = true;
    } while (false);
    if (parent) { LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, parent); }
    if (sibling) { LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, sibling); }
    if (entry) { LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, entry); }

    if (delete_up) {
        return BPlusTreeDeleteElement(tree, iterator);     /* β�ݹ� */
    }
    return success;
}

/*
* ��ʼ��B+��
*/
void BPlusTreeInit(BPlusTree* tree) {
    /* ��������3�ף����������ڵ���ѻ����һ��û�нڵ����� */
    /* Ҷ�ӿ���2�ף�����ɾ���еط���Ҫ��ǿ�ж� */
    tree->root_id = BPlusEntryCreate(tree, kBPlusEntryLeaf);
    //BPlusLeafListInit(&tree->leaf_list); BPlusLeafListPutFirst(&tree->leaf_list, tree->root_id);
}
/*
* ��B+���в���ָ��key
*/
bool BPlusTreeFind(BPlusTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key) {
    BPlusIterator iterator;
    BPlusIteratorStatus status = BPlusIteratorTop(tree, &iterator, key);
    while (status == kBPlusIteratorDown) {
        status = BPlusIteratorDown(tree, &iterator, key);
    }
    return status == kBPlusIteratorEq;
}
/*
* ��B+���в���ָ��key
*/
bool BPlusTreeInsert(BPlusTree* tree, BPlusLeafElement* element) {
    BPlusIterator iterator;
    BPlusIteratorStatus status = BPlusIteratorTop(tree, &iterator, &element->key);
    while (status == kBPlusIteratorDown) {
        status = BPlusIteratorDown(tree, &iterator, &element->key);
    }
    bool success = BPlusTreeInsertElement(tree, &iterator, NULL, (BPlusElement*)element, NULL, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
    BPlusIteratorRelease(tree, &iterator);
    return success;
}
/*
* ��B+����ɾ��ָ��key
*/
bool BPlusTreeDelete(BPlusTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key) {
    BPlusIterator iterator;
    BPlusIteratorStatus status = BPlusIteratorTop(tree, &iterator, key);
    while (status == kBPlusIteratorDown) {
        status = BPlusIteratorDown(tree, &iterator, key);
    }
    if (status == kBPlusIteratorNe) {
        return false;
    }
    bool success = BPlusTreeDeleteElement(tree, &iterator);
    return success;
}

#include <libyuc/container/bplus_tree.undef>