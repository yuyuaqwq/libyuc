#include <libyuc/container/vector.def>

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_STATIC
void VectorInit(Vector* vector) {
    VectorSetCount(vector, 0);
}

void VectorRelease(Vector* vector) {
}


LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset VectorGetCapacity(Vector* vector) {
    return sizeof(vector->obj_arr) / sizeof(vector->obj_arr[0]);
}
#endif

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
void VectorInit(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset capacity) {
    vector->obj_arr = NULL;
    if (capacity != 0) {
        VectorResetCapacity(vector, capacity);
    }
    else {
        vector->capacity = 0;
    }
    vector->count = 0;
}
void VectorRelease(Vector* vector) {
    if (vector->obj_arr) {
        LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release(vector, vector->obj_arr);
        vector->obj_arr = LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidDynamicArray;
    }
    vector->capacity = 0;
    vector->count = 0;
}

LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset VectorGetCapacity(Vector* vector) {
    return vector->capacity;
}

void VectorResetCapacity(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset capacity) {
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* new_buf = LIBYUC_CONTAINER_VECTOR_ALLOCATOR_CreateMultiple(arr, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element, capacity);
    if (vector->obj_arr) {
        MemoryCopy(new_buf, vector->obj_arr, sizeof(LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element) * vector->count);
        LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release(vector, vector->obj_arr);
    }
    vector->obj_arr = new_buf;
    vector->capacity = capacity;
}
void VectorExpand(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset add_count) {
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset old_capacity = VectorGetCapacity(vector);
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset cur_capacity = old_capacity;
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset target_count = cur_capacity + add_count;
    if (cur_capacity == 0) {
        cur_capacity = 1;
    }
    while (cur_capacity < target_count) {
        cur_capacity *= 2;
    }
    VectorResetCapacity(vector, cur_capacity);
    LIBYUC_CONTAINER_VECTOR_CALLBACK_Expand(vector, old_capacity, cur_capacity);
}
#endif

bool VectorIsEmpty(Vector* vector) {
    return vector->count == 0;
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset VectorGetCount(Vector* vector) {
    return vector->count;
}
void VectorSetCount(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset count) {
    vector->count = count;
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorGetArray(Vector* vector) {
    return vector->obj_arr;
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIndex(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id index) {
    return &vector->obj_arr[index];
}

LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPushTail(Vector* vector, const LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* obj) {
    if (VectorGetCapacity(vector) <= VectorGetCount(vector)) {
#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
        VectorExpand(vector, 1);
#else
        release_assert(false);
#endif
    }
    MemoryCopy(VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector)), (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element*)obj, sizeof(LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element));
    VectorSetCount(vector, VectorGetCount(vector) + 1);
    return VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector) - 1);
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPushMultipleTail(Vector* vector, const LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* obj, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset count) {
    if (VectorGetCapacity(vector) <= (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector) + (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)count) {
#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
        VectorExpand(vector, count);
#else
        release_assert(false);
#endif
    }
    MemoryCopy(VectorIndex(vector, VectorGetCount(vector)), obj, sizeof(LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element) * count);
    VectorSetCount(vector, VectorGetCount(vector) + count);
    return VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector) - (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)count);
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorGetTail(Vector* vector) {
      release_assert(VectorGetCount(vector) > 0);
    return VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector) - 1);
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPopTail(Vector* vector) {
      release_assert(VectorGetCount(vector) > 0);
    VectorSetCount(vector, VectorGetCount(vector) - 1);
    return VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector));
}


//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorGetIndex(Vector* vector, VectorIterator* iter) {
//    if (iter->cur_index == LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidId || iter->cur_index >= VectorGetCount(vector)) return NULL;
//        return VectorIndex(vector, iter->cur_index);
//}
//void VectorIteratorDelete(Vector* vector, VectorIterator* iter) {
//      assert(VectorGetCount(vector) > 0);
//      assert(iter->cur_index < VectorGetCount(vector));
//    VectorArrayDelete(VectorGetArray(vector), VectorGetCount(vector), iter->cur_index);
//    VectorSetCount(vector, VectorGetCount(vector) - 1);
//}
//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorFirst(Vector* vector, VectorIterator* iter) {
//    iter->cur_index = 0;
//    return VectorGetCount(vector) > 0 ? VectorIndex(vector, 0) : NULL;
//}
//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorNext(Vector* vector, VectorIterator* iter) {
//    if (iter->cur_index == LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidId || iter->cur_index >= VectorGetCount(vector)) return NULL;
//        ++iter->cur_index;
//        return VectorIndex(vector, iter->cur_index);
//}

#include <libyuc/container/vector.undef>
