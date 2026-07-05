#define CBASE_IMPLEMENTATION
#ifndef CBASE_H_
#define CBASE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define FNV_OFFSET_BASIS 0xcbf29ce484222325ULL

/* ==================== Basic Types ==================== */

typedef int8_t   Int8;
typedef int16_t  Int16;
typedef int32_t  Int32;
typedef int64_t  Int64;
typedef uint8_t  UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;
typedef float    Float32;
typedef double   Float64;
typedef char     Char;
typedef bool     Bool;

/* ==================== Hash Function ==================== */

UInt64 fnv64(const void *data, size_t data_size, UInt64 input);

/* ==================== Trait Function Types ==================== */

typedef void (*DropFn)(void *self);
typedef int (*CmpFn)(const void *a, const void *b);
typedef bool (*EqFn)(const void *a, const void *b);
typedef UInt64 (*HashFn)(const void *self);
typedef void (*CopyFn)(void *dst, const void *src);
typedef void (*MoveFn)(void *dst, void *src);
typedef void (*SwapFn)(void *a, void *b);

/* ==================== Generic Vec ==================== */

typedef struct {
    void  *data;
    size_t len;
    size_t capacity;
    size_t elem_size;
} Vec;

int    vec_init(Vec *self, size_t elem_size);
int    vec_with_capacity(Vec *self, size_t elem_size, size_t capacity);
void   vec_push_cp(Vec *self, const void *elem, CopyFn copy_fn);
void   vec_push_mv(Vec *self, void *elem, MoveFn move_fn);
bool   vec_pop(Vec *self, void *out);
void  *vec_get(Vec *self, size_t index);
void   vec_set_cp(Vec *self, size_t index, const void *elem, DropFn value_drop, CopyFn copy_fn);
void   vec_set_mv(Vec *self, size_t index, void *elem, DropFn value_drop, MoveFn move_fn);
void  *vec_first(Vec *self);
void  *vec_last(Vec *self);
size_t vec_len(const Vec *self);
bool   vec_is_empty(const Vec *self);
void   vec_clear(Vec *self, DropFn value_drop);
void   vec_truncate(Vec *self, size_t len, DropFn value_drop);
void   vec_drop(Vec *self, DropFn value_drop);
void   vec_reserve(Vec *self, size_t additional);
void   vec_swap_element(Vec *self, size_t a, size_t b);
void   vec_swap(Vec *a, Vec *b);
bool   vec_eq(Vec *a, Vec *b, EqFn eq_fn);
void   vec_insert_cp(Vec *self, size_t index, const void *elem, CopyFn copy_fn);
void   vec_insert_mv(Vec *self, size_t index, void *elem, MoveFn move_fn);
void   vec_remove(Vec *self, size_t index, void *out);
void  *vec_as_ptr(Vec *self);
void   vec_copy(Vec *dst, const Vec *src, CopyFn copy_fn);
void   vec_move(Vec *dst, Vec *src);
void   vec_sort(Vec *self, CmpFn cmp);
int    vec_cmp(Vec *a, Vec *b, CmpFn cmp);
UInt64 vec_hash(Vec *self, HashFn hash_fn);

/* ==================== Generic Box ==================== */

typedef struct {
    void *data;
} Box;

int    box_init(Box *self, void *ptr);
void   box_set(Box *self, void *ptr, DropFn drop_fn);
void   box_drop(Box *self, DropFn drop_fn);
void   box_move(Box *dst, Box *src);
void  *box_release(Box *self);
void  *box_ptr(Box *self);
bool   box_is_null(const Box *self);
int    box_cmp(Box *a, Box *b);
bool   box_eq(Box *a, Box *b);
UInt64 box_hash(Box *self);

/* ==================== Generic PQueue ==================== */
/*
 * Priority Queue (binary heap).
 * Default behavior is a MAX-HEAP: when using T##_cmp (which returns positive
 * if a > b), the largest element is at the top. For a MIN-HEAP, pass a
 * comparator that returns the negated result of T##_cmp.
 */

typedef struct {
    Vec   data;
    CmpFn cmp_fn;
} PQueue;

int    pqueue_init(PQueue *self, size_t elem_size, CmpFn cmp_fn);
int    pqueue_with_capacity(PQueue *self, size_t elem_size, CmpFn cmp_fn, size_t capacity);
void   pqueue_push_cp(PQueue *self, const void *elem, CopyFn copy_fn);
void   pqueue_push_mv(PQueue *self, void *elem, MoveFn move_fn);
void   pqueue_pop(PQueue *self, DropFn drop_fn);
void  *pqueue_top(PQueue *self);
size_t pqueue_len(const PQueue *self);
bool   pqueue_is_empty(const PQueue *self);
void   pqueue_clear(PQueue *self, DropFn drop_fn);
void   pqueue_drop(PQueue *self, DropFn drop_fn);
void   pqueue_copy(PQueue *dst, const PQueue *src, CopyFn copy_fn);
void   pqueue_move(PQueue *dst, PQueue *src);

/* ==================== Generic CBList ==================== */
typedef struct CBListNode {
    struct CBListNode *prev;
    struct CBListNode *next;
    _Alignas(max_align_t) char data[];
} CBListNode;

typedef struct {
    CBListNode *vhead;
    CBListNode *vtail;
    size_t      len;
    size_t      elem_size;
} CBList;

int         list_init(CBList *self, size_t elem_size);
void        list_drop(CBList *self, DropFn value_drop);
void        list_clear(CBList *self, DropFn value_drop);
void        list_copy(CBList *dst, const CBList *src, CopyFn copy_fn);
void        list_move(CBList *dst, CBList *src);
void        list_swap(CBList *a, CBList *b);
CBListNode *list_insert_before_cp(CBList *self, CBListNode *iter, const void *elem, CopyFn copy_fn);
CBListNode *list_insert_before_mv(CBList *self, CBListNode *iter, void *elem, MoveFn move_fn);
CBListNode *list_insert_after_cp(CBList *self, CBListNode *iter, const void *elem, CopyFn copy_fn);
CBListNode *list_insert_after_mv(CBList *self, CBListNode *iter, void *elem, MoveFn move_fn);
void        list_remove(CBList *self, CBListNode *iter, DropFn value_drop);
void        list_set_cp(CBList *self, CBListNode *iter, const void *elem, DropFn value_drop, CopyFn copy_fn);
void        list_set_mv(CBList *self, CBListNode *iter, void *elem, DropFn value_drop, MoveFn move_fn);
CBListNode *list_begin(CBList *self);
CBListNode *list_last(CBList *self);
CBListNode *list_end(CBList *self);
CBListNode *list_next(CBListNode *iter);
CBListNode *list_prev(CBListNode *iter);
void       *list_get(CBListNode *iter);
size_t      list_len(const CBList *self);
bool        list_is_empty(const CBList *self);
CBListNode *list_push_back_cp(CBList *self, const void *elem, CopyFn copy_fn);
CBListNode *list_push_back_mv(CBList *self, void *elem, MoveFn move_fn);
CBListNode *list_push_front_cp(CBList *self, const void *elem, CopyFn copy_fn);
CBListNode *list_push_front_mv(CBList *self, void *elem, MoveFn move_fn);
void        list_pop_back(CBList *self, DropFn value_drop);
void        list_pop_front(CBList *self, DropFn value_drop);
bool        list_eq(CBList *a, CBList *b, EqFn eq_fn);
int         list_cmp(CBList *a, CBList *b, CmpFn cmp_fn);
UInt64      list_hash(CBList *self, HashFn hash_fn);

/* ==================== Generic HashTable ==================== */

typedef struct {
    Vec    buckets; /* Vec of CBList (chaining) */
    size_t key_size;
    size_t value_size;
    size_t value_offset; /* padded offset for value alignment */
    size_t len;
    HashFn key_hash_fn;
    EqFn   key_eq_fn;
} HashTable;

int hashtable_init(
        HashTable *self, size_t key_size, size_t value_size, size_t value_offset, HashFn key_hash_fn, EqFn key_eq_fn);
void   hashtable_drop(HashTable *self, DropFn key_drop, DropFn value_drop);
void   hashtable_clear(HashTable *self, DropFn key_drop, DropFn value_drop);
void   hashtable_insert_cp(HashTable *self,
        const void                 *key,
        const void                 *value,
        CopyFn                      key_copy,
        CopyFn                      value_copy,
        DropFn                      key_drop,
        DropFn                      value_drop);
void   hashtable_insert_mv(HashTable *self,
        void                       *key,
        void                       *value,
        MoveFn                      key_move,
        MoveFn                      value_move,
        DropFn                      key_drop,
        DropFn                      value_drop);
void  *hashtable_get(HashTable *self, const void *key);
bool   hashtable_remove(HashTable *self, const void *key, DropFn key_drop, DropFn value_drop);
size_t hashtable_len(const HashTable *self);
bool   hashtable_is_empty(const HashTable *self);
void   hashtable_copy(
        HashTable *dst, const HashTable *src, CopyFn key_copy, CopyFn value_copy, DropFn key_drop, DropFn value_drop);
void hashtable_move(HashTable *dst, HashTable *src);

/* ==================== HashTable Iterator ==================== */

typedef struct {
    HashTable  *ht;
    size_t      bucket_idx;
    CBListNode *node;
} HashTableIter;

HashTableIter hashtable_begin(HashTable *self);
bool          hashtable_iter_is_end(const HashTableIter *iter);
void          hashtable_iter_next(HashTableIter *iter);
void         *hashtable_iter_key(HashTableIter *iter);
void         *hashtable_iter_value(HashTableIter *iter);

/* ==================== Generic RBTMap ==================== */

typedef struct RBTNode {
    struct RBTNode *left;
    struct RBTNode *right;
    struct RBTNode *parent;
    int             color; /* 0 = BLACK, 1 = RED */
    _Alignas(max_align_t) char data[];
} RBTNode;

typedef struct {
    RBTNode *root;
    size_t   len;
    size_t   key_size;
    size_t   value_size;
    size_t   value_offset;
    CmpFn    key_cmp_fn;
} RBTMap;

int    rbtmap_init(RBTMap *self, size_t key_size, size_t value_size, size_t value_offset, CmpFn key_cmp_fn);
void   rbtmap_drop(RBTMap *self, DropFn key_drop, DropFn value_drop);
void   rbtmap_clear(RBTMap *self, DropFn key_drop, DropFn value_drop);
void   rbtmap_insert_cp(RBTMap *self,
        const void                 *key,
        const void                 *value,
        CopyFn                      key_copy,
        CopyFn                      value_copy,
        DropFn                      key_drop,
        DropFn                      value_drop);
void   rbtmap_insert_mv(RBTMap *self,
        void                       *key,
        void                       *value,
        MoveFn                      key_move,
        MoveFn                      value_move,
        DropFn                      key_drop,
        DropFn                      value_drop);
void  *rbtmap_get(RBTMap *self, const void *key);
bool   rbtmap_remove(RBTMap *self, const void *key, DropFn key_drop, DropFn value_drop);
size_t rbtmap_len(const RBTMap *self);
bool   rbtmap_is_empty(const RBTMap *self);
void   rbtmap_copy(
        RBTMap *dst, const RBTMap *src, CopyFn key_copy, CopyFn value_copy, DropFn key_drop, DropFn value_drop);
void   rbtmap_move(RBTMap *dst, RBTMap *src);

/* ==================== RBTMap Iterator ==================== */

typedef struct {
    RBTMap  *map;
    RBTNode *node;
} RBTMapIter;

RBTMapIter rbtmap_begin(RBTMap *self);
RBTMapIter rbtmap_last(RBTMap *self);
RBTMapIter rbtmap_get_iter(RBTMap *self, const void *key);
RBTMapIter rbtmap_lower_bound(RBTMap *self, const void *key);
RBTMapIter rbtmap_upper_bound(RBTMap *self, const void *key);
bool       rbtmap_iter_is_end(const RBTMapIter *iter);
void       rbtmap_iter_next(RBTMapIter *iter);
void       rbtmap_iter_prev(RBTMapIter *iter);
void      *rbtmap_iter_key(RBTMapIter *iter);
void      *rbtmap_iter_value(RBTMapIter *iter);

/* ==================== Generic Deque ==================== */

typedef struct {
    Vec    blocks;        /* Vec of void* block pointers, used as ring buffer */
    size_t elems_per_blk; /* elements per block, min 16 */
    size_t elem_size;     /* size of single element in bytes */
    size_t len;           /* total number of elements */
    size_t front_offset;  /* offset of first element in first block (0..elems_per_blk-1) */
    size_t block_front;   /* ring buffer index of first active block */
    size_t block_count;   /* number of active blocks in ring buffer */
} Deque;

typedef struct {
    Deque *deque;
    size_t pos;           /* global position */
} DequeIter;

int    deque_init(Deque *self, size_t elem_size);
void   deque_clear(Deque *self, DropFn value_drop);
void   deque_drop(Deque *self, DropFn value_drop);
void   deque_push_back_cp(Deque *self, const void *elem, CopyFn copy_fn);
void   deque_push_back_mv(Deque *self, void *elem, MoveFn move_fn);
void   deque_push_front_cp(Deque *self, const void *elem, CopyFn copy_fn);
void   deque_push_front_mv(Deque *self, void *elem, MoveFn move_fn);
bool   deque_pop_back(Deque *self, void *out);
bool   deque_pop_front(Deque *self, void *out);
void  *deque_get(Deque *self, size_t index);
void   deque_set_cp(Deque *self, size_t index, const void *elem, DropFn value_drop, CopyFn copy_fn);
void   deque_set_mv(Deque *self, size_t index, void *elem, DropFn value_drop, MoveFn move_fn);
void  *deque_front(Deque *self);
void  *deque_back(Deque *self);
void   deque_push_back_raw(Deque *self);
void   deque_push_front_raw(Deque *self);
void   deque_pop_back_raw(Deque *self);
void   deque_pop_front_raw(Deque *self);
void   deque_insert_cp(Deque *self, size_t index, const void *elem, CopyFn copy_fn);
void   deque_insert_mv(Deque *self, size_t index, void *elem, MoveFn move_fn);
void   deque_remove(Deque *self, size_t index, void *out);
void   deque_swap_element(Deque *self, size_t a, size_t b);
void   deque_swap(Deque *a, Deque *b);
void   deque_reserve(Deque *self, size_t additional);
void   deque_copy(Deque *dst, const Deque *src, CopyFn copy_fn);
void   deque_move(Deque *dst, Deque *src);
bool   deque_eq(Deque *a, Deque *b, EqFn eq_fn);
int    deque_cmp(Deque *a, Deque *b, CmpFn cmp_fn);
UInt64 deque_hash(Deque *self, HashFn hash_fn);
size_t deque_len(const Deque *self);
bool   deque_is_empty(const Deque *self);

DequeIter deque_begin(Deque *self);
DequeIter deque_end(Deque *self);
DequeIter deque_last(Deque *self);
bool      deque_iter_is_end(const DequeIter *iter);
void      deque_iter_next(DequeIter *iter);
void      deque_iter_prev(DequeIter *iter);
void     *deque_iter_get(DequeIter *iter);

/* ==================== Basic Type Trait Declarations ==================== */

#define DECLARE_NUMERIC_TRAITS(T) \
    void   T##_drop(T *self); \
    int    T##_cmp(T *a, T *b); \
    bool   T##_eq(T *a, T *b); \
    UInt64 T##_hash(T *self); \
    void   T##_copy(T *dst, const T *src); \
    void   T##_move(T *dst, T *src); \
    void   T##_swap(T *dst, T *src);

DECLARE_NUMERIC_TRAITS(Int8)
DECLARE_NUMERIC_TRAITS(Int16)
DECLARE_NUMERIC_TRAITS(Int32)
DECLARE_NUMERIC_TRAITS(Int64)
DECLARE_NUMERIC_TRAITS(UInt8)
DECLARE_NUMERIC_TRAITS(UInt16)
DECLARE_NUMERIC_TRAITS(UInt32)
DECLARE_NUMERIC_TRAITS(UInt64)
DECLARE_NUMERIC_TRAITS(Float32)
DECLARE_NUMERIC_TRAITS(Float64)
DECLARE_NUMERIC_TRAITS(Char)
DECLARE_NUMERIC_TRAITS(Bool)

/* ==================== CBASE_VECTOR Macro ==================== */

#define CBASE_VECTOR(SELF, T) \
    typedef struct { \
        Vec impl; \
    } SELF; \
\
    static inline int SELF##_init(SELF *self) { \
        return vec_init(&self->impl, sizeof(T)); \
    } \
    static inline int SELF##_with_capacity(SELF *self, size_t capacity) { \
        return vec_with_capacity(&self->impl, sizeof(T), capacity); \
    } \
    static inline void SELF##_pushcp(SELF *self, const T *elem) { \
        vec_push_cp(&self->impl, elem, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_pushmv(SELF *self, T *elem) { \
        vec_push_mv(&self->impl, elem, (MoveFn)T##_move); \
    } \
    static inline bool SELF##_pop(SELF *self, T *out) { \
        return vec_pop(&self->impl, out); \
    } \
    static inline T *SELF##_get(SELF *self, size_t index) { \
        return (T *)vec_get(&self->impl, index); \
    } \
    static inline void SELF##_setcp(SELF *self, size_t index, const T *elem) { \
        vec_set_cp(&self->impl, index, elem, (DropFn)T##_drop, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_setmv(SELF *self, size_t index, T *elem) { \
        vec_set_mv(&self->impl, index, elem, (DropFn)T##_drop, (MoveFn)T##_move); \
    } \
    static inline T *SELF##_first(SELF *self) { \
        return (T *)vec_first(&self->impl); \
    } \
    static inline T *SELF##_last(SELF *self) { \
        return (T *)vec_last(&self->impl); \
    } \
    static inline size_t SELF##_len(const SELF *self) { \
        return vec_len(&self->impl); \
    } \
    static inline bool SELF##_is_empty(const SELF *self) { \
        return vec_is_empty(&self->impl); \
    } \
    static inline void SELF##_clear(SELF *self) { \
        vec_clear(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_truncate(SELF *self, size_t len) { \
        vec_truncate(&self->impl, len, (DropFn)T##_drop); \
    } \
    static inline void SELF##_drop(SELF *self) { \
        vec_drop(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_reserve(SELF *self, size_t additional) { \
        vec_reserve(&self->impl, additional); \
    } \
    static inline void SELF##_swap_element(SELF *self, size_t a, size_t b) { \
        vec_swap_element(&self->impl, a, b); \
    } \
    static inline void SELF##_swap(SELF *a, SELF *b) { \
        vec_swap(&a->impl, &b->impl); \
    } \
    static inline bool SELF##_eq(SELF *a, SELF *b) { \
        return vec_eq(&a->impl, &b->impl, (EqFn)T##_eq); \
    } \
    static inline void SELF##_insertcp(SELF *self, size_t index, const T *elem) { \
        vec_insert_cp(&self->impl, index, elem, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_insertmv(SELF *self, size_t index, T *elem) { \
        vec_insert_mv(&self->impl, index, elem, (MoveFn)T##_move); \
    } \
    static inline void SELF##_remove(SELF *self, size_t index, T *out) { \
        vec_remove(&self->impl, index, out); \
    } \
    static inline T *SELF##_as_ptr(SELF *self) { \
        return (T *)vec_as_ptr(&self->impl); \
    } \
    static inline void SELF##_copy(SELF *dst, const SELF *src) { \
        vec_copy(&dst->impl, &src->impl, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_move(SELF *dst, SELF *src) { \
        vec_move(&dst->impl, &src->impl); \
    } \
    static inline int SELF##_cmp(SELF *a, SELF *b) { \
        return vec_cmp(&a->impl, &b->impl, (CmpFn)T##_cmp); \
    } \
    static inline UInt64 SELF##_hash(SELF *self) { \
        return vec_hash(&self->impl, (HashFn)T##_hash); \
    } \
    static inline void SELF##_sort(SELF *self) { \
        vec_sort(&self->impl, (CmpFn)T##_cmp); \
    }

/* ==================== CBASE_BOX Macro ==================== */

#define CBASE_BOX(SELF, T) \
    typedef struct { \
        Box impl; \
    } SELF; \
\
    static inline int SELF##_init(SELF *self, T *ptr) { \
        return box_init(&self->impl, ptr); \
    } \
    static inline void SELF##_set(SELF *self, T *ptr) { \
        box_set(&self->impl, ptr, (DropFn)T##_drop); \
    } \
    static inline void SELF##_drop(SELF *self) { \
        box_drop(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_move(SELF *dst, SELF *src) { \
        box_move(&dst->impl, &src->impl); \
    } \
    static inline T *SELF##_release(SELF *self) { \
        return (T *)box_release(&self->impl); \
    } \
    static inline T *SELF##_ptr(SELF *self) { \
        return (T *)box_ptr(&self->impl); \
    } \
    static inline bool SELF##_is_null(const SELF *self) { \
        return box_is_null(&self->impl); \
    } \
    static inline int SELF##_cmp(SELF *a, SELF *b) { \
        return box_cmp(&a->impl, &b->impl); \
    } \
    static inline bool SELF##_eq(SELF *a, SELF *b) { \
        return box_eq(&a->impl, &b->impl); \
    } \
    static inline UInt64 SELF##_hash(SELF *self) { \
        return box_hash(&self->impl); \
    }

/* ==================== CBASE_PRIORITY_QUEUE Macro ==================== */
/*
 * Generates a type-safe priority queue wrapper.
 * Uses T##_cmp by default (MAX-HEAP). Use SELF##_init_with_cmp for a custom
 * comparator (e.g. pass a negated cmp for MIN-HEAP).
 */

#define CBASE_PRIORITY_QUEUE(SELF, T) \
    typedef struct { \
        PQueue impl; \
    } SELF; \
\
    static inline int SELF##_init(SELF *self) { \
        return pqueue_init(&self->impl, sizeof(T), (CmpFn)T##_cmp); \
    } \
    static inline int SELF##_init_with_cmp(SELF *self, CmpFn cmp_fn) { \
        return pqueue_init(&self->impl, sizeof(T), cmp_fn); \
    } \
    static inline int SELF##_with_capacity(SELF *self, size_t capacity) { \
        return pqueue_with_capacity(&self->impl, sizeof(T), (CmpFn)T##_cmp, capacity); \
    } \
    static inline void SELF##_pushcp(SELF *self, const T *elem) { \
        pqueue_push_cp(&self->impl, elem, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_pushmv(SELF *self, T *elem) { \
        pqueue_push_mv(&self->impl, elem, (MoveFn)T##_move); \
    } \
    static inline void SELF##_pop(SELF *self) { \
        pqueue_pop(&self->impl, (DropFn)T##_drop); \
    } \
    static inline T *SELF##_top(SELF *self) { \
        return (T *)pqueue_top(&self->impl); \
    } \
    static inline size_t SELF##_len(const SELF *self) { \
        return pqueue_len(&self->impl); \
    } \
    static inline bool SELF##_is_empty(const SELF *self) { \
        return pqueue_is_empty(&self->impl); \
    } \
    static inline void SELF##_clear(SELF *self) { \
        pqueue_clear(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_drop(SELF *self) { \
        pqueue_drop(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_copy(SELF *dst, const SELF *src) { \
        pqueue_copy(&dst->impl, &src->impl, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_move(SELF *dst, SELF *src) { \
        pqueue_move(&dst->impl, &src->impl); \
    }

/* ==================== CBASE_LIST Macro ==================== */

#define CBASE_LIST(SELF, T) \
    typedef struct { \
        CBList impl; \
    } SELF; \
\
    static inline int SELF##_init(SELF *self) { \
        return list_init(&self->impl, sizeof(T)); \
    } \
    static inline void SELF##_drop(SELF *self) { \
        list_drop(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_clear(SELF *self) { \
        list_clear(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_copy(SELF *dst, const SELF *src) { \
        list_copy(&dst->impl, &src->impl, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_move(SELF *dst, SELF *src) { \
        list_move(&dst->impl, &src->impl); \
    } \
    static inline void SELF##_swap(SELF *a, SELF *b) { \
        list_swap(&a->impl, &b->impl); \
    } \
    static inline CBListNode *SELF##_insert_beforecp(SELF *self, CBListNode *iter, const T *elem) { \
        return list_insert_before_cp(&self->impl, iter, elem, (CopyFn)T##_copy); \
    } \
    static inline CBListNode *SELF##_insert_beforemv(SELF *self, CBListNode *iter, T *elem) { \
        return list_insert_before_mv(&self->impl, iter, elem, (MoveFn)T##_move); \
    } \
    static inline CBListNode *SELF##_insert_aftercp(SELF *self, CBListNode *iter, const T *elem) { \
        return list_insert_after_cp(&self->impl, iter, elem, (CopyFn)T##_copy); \
    } \
    static inline CBListNode *SELF##_insert_aftermv(SELF *self, CBListNode *iter, T *elem) { \
        return list_insert_after_mv(&self->impl, iter, elem, (MoveFn)T##_move); \
    } \
    static inline void SELF##_remove(SELF *self, CBListNode *iter) { \
        list_remove(&self->impl, iter, (DropFn)T##_drop); \
    } \
    static inline void SELF##_setcp(SELF *self, CBListNode *iter, const T *elem) { \
        list_set_cp(&self->impl, iter, elem, (DropFn)T##_drop, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_setmv(SELF *self, CBListNode *iter, T *elem) { \
        list_set_mv(&self->impl, iter, elem, (DropFn)T##_drop, (MoveFn)T##_move); \
    } \
    static inline CBListNode *SELF##_begin(SELF *self) { \
        return list_begin(&self->impl); \
    } \
    static inline CBListNode *SELF##_last(SELF *self) { \
        return list_last(&self->impl); \
    } \
    static inline CBListNode *SELF##_end(SELF *self) { \
        return list_end(&self->impl); \
    } \
    static inline CBListNode *SELF##_next(CBListNode *iter) { \
        return list_next(iter); \
    } \
    static inline CBListNode *SELF##_prev(CBListNode *iter) { \
        return list_prev(iter); \
    } \
    static inline T *SELF##_get(CBListNode *iter) { \
        return (T *)list_get(iter); \
    } \
    static inline size_t SELF##_len(const SELF *self) { \
        return list_len(&self->impl); \
    } \
    static inline bool SELF##_is_empty(const SELF *self) { \
        return list_is_empty(&self->impl); \
    } \
    static inline CBListNode *SELF##_push_backcp(SELF *self, const T *elem) { \
        return list_push_back_cp(&self->impl, elem, (CopyFn)T##_copy); \
    } \
    static inline CBListNode *SELF##_push_backmv(SELF *self, T *elem) { \
        return list_push_back_mv(&self->impl, elem, (MoveFn)T##_move); \
    } \
    static inline CBListNode *SELF##_push_frontcp(SELF *self, const T *elem) { \
        return list_push_front_cp(&self->impl, elem, (CopyFn)T##_copy); \
    } \
    static inline CBListNode *SELF##_push_frontmv(SELF *self, T *elem) { \
        return list_push_front_mv(&self->impl, elem, (MoveFn)T##_move); \
    } \
    static inline void SELF##_pop_back(SELF *self) { \
        list_pop_back(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_pop_front(SELF *self) { \
        list_pop_front(&self->impl, (DropFn)T##_drop); \
    } \
    static inline bool SELF##_eq(SELF *a, SELF *b) { \
        return list_eq(&a->impl, &b->impl, (EqFn)T##_eq); \
    } \
    static inline int SELF##_cmp(SELF *a, SELF *b) { \
        return list_cmp(&a->impl, &b->impl, (CmpFn)T##_cmp); \
    } \
    static inline UInt64 SELF##_hash(SELF *self) { \
        return list_hash(&self->impl, (HashFn)T##_hash); \
    }

/* ==================== CBASE_HASHTABLE Macro ==================== */

#define CBASE_HASHTABLE(SELF, K, V) \
    typedef struct { \
        HashTable impl; \
    } SELF; \
    /*弄一个K、V组成的临时struct，然后用sizeof，offset of得到尺寸 */ \
    static inline int SELF##_init(SELF *self) { \
        typedef struct { \
            K _key; \
            V _value; \
        } SELF##_Entry; \
        return hashtable_init( \
                &self->impl, sizeof(K), sizeof(V), offsetof(SELF##_Entry, _value), (HashFn)K##_hash, (EqFn)K##_eq); \
    } \
    static inline void SELF##_drop(SELF *self) { \
        hashtable_drop(&self->impl, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline void SELF##_clear(SELF *self) { \
        hashtable_clear(&self->impl, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline void SELF##_insertcp(SELF *self, const K *key, const V *value) { \
        hashtable_insert_cp( \
                &self->impl, key, value, (CopyFn)K##_copy, (CopyFn)V##_copy, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline void SELF##_insertmv(SELF *self, K *key, V *value) { \
        hashtable_insert_mv( \
                &self->impl, key, value, (MoveFn)K##_move, (MoveFn)V##_move, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline V *SELF##_get(SELF *self, const K *key) { \
        return (V *)hashtable_get(&self->impl, key); \
    } \
    static inline bool SELF##_remove(SELF *self, const K *key) { \
        return hashtable_remove(&self->impl, key, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline size_t SELF##_len(const SELF *self) { \
        return hashtable_len(&self->impl); \
    } \
    static inline bool SELF##_is_empty(const SELF *self) { \
        return hashtable_is_empty(&self->impl); \
    } \
    static inline void SELF##_copy(SELF *dst, const SELF *src) { \
        hashtable_copy( \
                &dst->impl, &src->impl, (CopyFn)K##_copy, (CopyFn)V##_copy, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline void SELF##_move(SELF *dst, SELF *src) { \
        hashtable_move(&dst->impl, &src->impl); \
    } \
    static inline HashTableIter SELF##_begin(SELF *self) { \
        return hashtable_begin(&self->impl); \
    } \
    static inline bool SELF##_iter_is_end(const HashTableIter *iter) { \
        return hashtable_iter_is_end(iter); \
    } \
    static inline void SELF##_iter_next(HashTableIter *iter) { \
        hashtable_iter_next(iter); \
    } \
    static inline K *SELF##_iter_key(HashTableIter *iter) { \
        return (K *)hashtable_iter_key(iter); \
    } \
    static inline V *SELF##_iter_value(HashTableIter *iter) { \
        return (V *)hashtable_iter_value(iter); \
    }

/* ==================== CBASE_RBTMAP Macro ==================== */

#define CBASE_RBTMAP(SELF, K, V) \
    typedef struct { \
        RBTMap impl; \
    } SELF; \
    static inline int SELF##_init(SELF *self) { \
        typedef struct { \
            K _key; \
            V _value; \
        } SELF##_Entry; \
        return rbtmap_init( \
                &self->impl, sizeof(K), sizeof(V), offsetof(SELF##_Entry, _value), (CmpFn)K##_cmp); \
    } \
    static inline int SELF##_init_with_cmp(SELF *self, CmpFn cmp_fn) { \
        typedef struct { \
            K _key; \
            V _value; \
        } SELF##_Entry; \
        return rbtmap_init( \
                &self->impl, sizeof(K), sizeof(V), offsetof(SELF##_Entry, _value), cmp_fn); \
    } \
    static inline void SELF##_drop(SELF *self) { \
        rbtmap_drop(&self->impl, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline void SELF##_clear(SELF *self) { \
        rbtmap_clear(&self->impl, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline void SELF##_insertcp(SELF *self, const K *key, const V *value) { \
        rbtmap_insert_cp( \
                &self->impl, key, value, (CopyFn)K##_copy, (CopyFn)V##_copy, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline void SELF##_insertmv(SELF *self, K *key, V *value) { \
        rbtmap_insert_mv( \
                &self->impl, key, value, (MoveFn)K##_move, (MoveFn)V##_move, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline V *SELF##_get(SELF *self, const K *key) { \
        return (V *)rbtmap_get(&self->impl, key); \
    } \
    static inline RBTMapIter SELF##_get_iter(SELF *self, const K *key) { \
        return rbtmap_get_iter(&self->impl, key); \
    } \
    static inline RBTMapIter SELF##_lower_bound(SELF *self, const K *key) { \
        return rbtmap_lower_bound(&self->impl, key); \
    } \
    static inline RBTMapIter SELF##_upper_bound(SELF *self, const K *key) { \
        return rbtmap_upper_bound(&self->impl, key); \
    } \
    static inline bool SELF##_remove(SELF *self, const K *key) { \
        return rbtmap_remove(&self->impl, key, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline size_t SELF##_len(const SELF *self) { \
        return rbtmap_len(&self->impl); \
    } \
    static inline bool SELF##_is_empty(const SELF *self) { \
        return rbtmap_is_empty(&self->impl); \
    } \
    static inline void SELF##_copy(SELF *dst, const SELF *src) { \
        rbtmap_copy( \
                &dst->impl, &src->impl, (CopyFn)K##_copy, (CopyFn)V##_copy, (DropFn)K##_drop, (DropFn)V##_drop); \
    } \
    static inline void SELF##_move(SELF *dst, SELF *src) { \
        rbtmap_move(&dst->impl, &src->impl); \
    } \
    static inline RBTMapIter SELF##_begin(SELF *self) { \
        return rbtmap_begin(&self->impl); \
    } \
    static inline RBTMapIter SELF##_last(SELF *self) { \
        return rbtmap_last(&self->impl); \
    } \
    static inline bool SELF##_iter_is_end(const RBTMapIter *iter) { \
        return rbtmap_iter_is_end(iter); \
    } \
    static inline void SELF##_iter_next(RBTMapIter *iter) { \
        rbtmap_iter_next(iter); \
    } \
    static inline void SELF##_iter_prev(RBTMapIter *iter) { \
        rbtmap_iter_prev(iter); \
    } \
    static inline K *SELF##_iter_key(RBTMapIter *iter) { \
        return (K *)rbtmap_iter_key(iter); \
    } \
    static inline V *SELF##_iter_value(RBTMapIter *iter) { \
        return (V *)rbtmap_iter_value(iter); \
    }

/* ==================== CBASE_DEQUE Macro ==================== */

#define CBASE_DEQUE(SELF, T) \
    typedef struct { \
        Deque impl; \
    } SELF; \
\
    static inline int SELF##_init(SELF *self) { \
        return deque_init(&self->impl, sizeof(T)); \
    } \
    static inline void SELF##_drop(SELF *self) { \
        deque_drop(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_clear(SELF *self) { \
        deque_clear(&self->impl, (DropFn)T##_drop); \
    } \
    static inline void SELF##_pushcp(SELF *self, const T *elem) { \
        deque_push_back_cp(&self->impl, elem, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_pushmv(SELF *self, T *elem) { \
        deque_push_back_mv(&self->impl, elem, (MoveFn)T##_move); \
    } \
    static inline void SELF##_push_frontcp(SELF *self, const T *elem) { \
        deque_push_front_cp(&self->impl, elem, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_push_frontmv(SELF *self, T *elem) { \
        deque_push_front_mv(&self->impl, elem, (MoveFn)T##_move); \
    } \
    static inline bool SELF##_pop(SELF *self, T *out) { \
        return deque_pop_back(&self->impl, out); \
    } \
    static inline bool SELF##_pop_front(SELF *self, T *out) { \
        return deque_pop_front(&self->impl, out); \
    } \
    static inline T *SELF##_get(SELF *self, size_t index) { \
        return (T *)deque_get(&self->impl, index); \
    } \
    static inline void SELF##_setcp(SELF *self, size_t index, const T *elem) { \
        deque_set_cp(&self->impl, index, elem, (DropFn)T##_drop, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_setmv(SELF *self, size_t index, T *elem) { \
        deque_set_mv(&self->impl, index, elem, (DropFn)T##_drop, (MoveFn)T##_move); \
    } \
    static inline T *SELF##_front(SELF *self) { \
        return (T *)deque_front(&self->impl); \
    } \
    static inline T *SELF##_back(SELF *self) { \
        return (T *)deque_back(&self->impl); \
    } \
    static inline void SELF##_insertcp(SELF *self, size_t index, const T *elem) { \
        deque_insert_cp(&self->impl, index, elem, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_insertmv(SELF *self, size_t index, T *elem) { \
        deque_insert_mv(&self->impl, index, elem, (MoveFn)T##_move); \
    } \
    static inline void SELF##_remove(SELF *self, size_t index, T *out) { \
        deque_remove(&self->impl, index, out); \
    } \
    static inline void SELF##_swap_element(SELF *self, size_t a, size_t b) { \
        deque_swap_element(&self->impl, a, b); \
    } \
    static inline void SELF##_swap(SELF *a, SELF *b) { \
        deque_swap(&a->impl, &b->impl); \
    } \
    static inline void SELF##_reserve(SELF *self, size_t additional) { \
        deque_reserve(&self->impl, additional); \
    } \
    static inline void SELF##_copy(SELF *dst, const SELF *src) { \
        deque_copy(&dst->impl, &src->impl, (CopyFn)T##_copy); \
    } \
    static inline void SELF##_move(SELF *dst, SELF *src) { \
        deque_move(&dst->impl, &src->impl); \
    } \
    static inline bool SELF##_eq(SELF *a, SELF *b) { \
        return deque_eq(&a->impl, &b->impl, (EqFn)T##_eq); \
    } \
    static inline int SELF##_cmp(SELF *a, SELF *b) { \
        return deque_cmp(&a->impl, &b->impl, (CmpFn)T##_cmp); \
    } \
    static inline UInt64 SELF##_hash(SELF *self) { \
        return deque_hash(&self->impl, (HashFn)T##_hash); \
    } \
    static inline size_t SELF##_len(const SELF *self) { \
        return deque_len(&self->impl); \
    } \
    static inline bool SELF##_is_empty(const SELF *self) { \
        return deque_is_empty(&self->impl); \
    } \
    static inline DequeIter SELF##_begin(SELF *self) { \
        return deque_begin(&self->impl); \
    } \
    static inline DequeIter SELF##_end(SELF *self) { \
        return deque_end(&self->impl); \
    } \
    static inline DequeIter SELF##_last(SELF *self) { \
        return deque_last(&self->impl); \
    } \
    static inline bool SELF##_iter_is_end(const DequeIter *iter) { \
        return deque_iter_is_end(iter); \
    } \
    static inline void SELF##_iter_next(DequeIter *iter) { \
        deque_iter_next(iter); \
    } \
    static inline void SELF##_iter_prev(DequeIter *iter) { \
        deque_iter_prev(iter); \
    } \
    static inline T *SELF##_iter_get(DequeIter *iter) { \
        return (T *)deque_iter_get(iter); \
    }

/* ==================== String ==================== */

typedef struct {
    struct {
        size_t len;
        char   buf[0];
    } *ptr;
} String;

int    String_init(String *self);
int    String_from_c_str(String *self, const char *s);
int    String_from_n(String *self, const char *s, size_t n);
void   String_copy(String *dst, const String *src);
void   String_move(String *dst, String *src);
void   String_drop(String *self);
int    String_cmp(String *a, String *b);
bool   String_eq(String *a, String *b);
UInt64 String_hash(String *self);

const char *String_c_str(const String *self);
size_t      String_len(const String *self);

CBASE_VECTOR(StringVec, String)

StringVec String_split(const String *self, const char *sep);
String    String_join(const StringVec *vec, const char *sep);
StringVec String_splitlines(const String *self);
String    String_strip(const String *self);
String    String_lstrip(const String *self);
String    String_rstrip(const String *self);
String    String_removeprefix(const String *self, const char *prefix);
String    String_removesuffix(const String *self, const char *suffix);
Int64     String_find(const String *self, const char *sub);
size_t    String_count(const String *self, const char *sub);
bool      String_startswith(const String *self, const char *prefix);
bool      String_endswith(const String *self, const char *suffix);
String    String_lower(const String *self);
String    String_upper(const String *self);
String    String_capitalize(const String *self);

#endif /* CBASE_H */

/* ==================== Implementation ==================== */
#ifdef CBASE_IMPLEMENTATION

#include <alloca.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================== FNV-1 64-bit Hash ==================== */

UInt64 fnv64(const void *data, size_t data_size, UInt64 input) {
    UInt64               hash = input;
    const unsigned char *p    = (const unsigned char *)data;
    for (size_t i = 0; i < data_size; i++) {
        hash *= 0x100000001b3ULL;
        hash ^= p[i];
    }
    return hash;
}

/* ==================== Memory Helper ==================== */

static void *xrealloc(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    void *p = realloc(ptr, size);
    if (!p) {
        fprintf(stderr, "cbase: out of memory\n");
        abort();
    }
    return p;
}

/* ==================== Vec Implementation ==================== */

int vec_init(Vec *self, size_t elem_size) {
    self->data      = NULL;
    self->len       = 0;
    self->capacity  = 0;
    self->elem_size = elem_size;
    return 0;
}

int vec_with_capacity(Vec *self, size_t elem_size, size_t capacity) {
    self->elem_size = elem_size;
    self->len       = 0;
    self->capacity  = capacity;
    if (capacity > 0) {
        self->data = xrealloc(NULL, capacity * elem_size);
    } else {
        self->data = NULL;
    }
    return 0;
}

void vec_reserve(Vec *self, size_t additional) {
    size_t needed = self->len + additional;
    if (needed <= self->capacity)
        return;
    size_t new_cap = self->capacity;
    if (new_cap == 0)
        new_cap = 4;
    while (new_cap < needed)
        new_cap *= 2;
    self->data     = xrealloc(self->data, new_cap * self->elem_size);
    self->capacity = new_cap;
}

void vec_push_cp(Vec *self, const void *elem, CopyFn copy_fn) {
    vec_reserve(self, 1);
    void *dst = (char *)self->data + self->len * self->elem_size;
    copy_fn(dst, (void *)elem);
    self->len++;
}

void vec_push_mv(Vec *self, void *elem, MoveFn move_fn) {
    vec_reserve(self, 1);
    void *dst = (char *)self->data + self->len * self->elem_size;
    move_fn(dst, elem);
    self->len++;
}

bool vec_pop(Vec *self, void *out) {
    if (self->len == 0)
        return false;
    self->len--;
    if (out) {
        memcpy(out, (char *)self->data + self->len * self->elem_size, self->elem_size);
    }
    return true;
}

void *vec_get(Vec *self, size_t index) {
    if (index >= self->len)
        return NULL;
    return (char *)self->data + index * self->elem_size;
}

void vec_set_cp(Vec *self, size_t index, const void *elem, DropFn value_drop, CopyFn copy_fn) {
    if (index >= self->len)
        return;
    void *dst = (char *)self->data + index * self->elem_size;
    value_drop(dst);
    copy_fn(dst, (void *)elem);
}

void vec_set_mv(Vec *self, size_t index, void *elem, DropFn value_drop, MoveFn move_fn) {
    if (index >= self->len)
        return;
    void *dst = (char *)self->data + index * self->elem_size;
    value_drop(dst);
    move_fn(dst, elem);
}

void *vec_first(Vec *self) {
    if (self->len == 0)
        return NULL;
    return self->data;
}

void *vec_last(Vec *self) {
    if (self->len == 0)
        return NULL;
    return (char *)self->data + (self->len - 1) * self->elem_size;
}

size_t vec_len(const Vec *self) {
    return self->len;
}

bool vec_is_empty(const Vec *self) {
    return self->len == 0;
}

void vec_clear(Vec *self, DropFn value_drop) {
    for (size_t i = 0; i < self->len; i++) {
        value_drop((char *)self->data + i * self->elem_size);
    }
    self->len = 0;
}

void vec_truncate(Vec *self, size_t len, DropFn value_drop) {
    if (len >= self->len)
        return;
    for (size_t i = len; i < self->len; i++) {
        value_drop((char *)self->data + i * self->elem_size);
    }
    self->len = len;
}

void vec_drop(Vec *self, DropFn value_drop) {
    for (size_t i = 0; i < self->len; i++) {
        value_drop((char *)self->data + i * self->elem_size);
    }
    free(self->data);
    self->data      = NULL;
    self->len       = 0;
    self->capacity  = 0;
    self->elem_size = 0;
}

void vec_swap_element(Vec *self, size_t a, size_t b) {
    if (a == b || a >= self->len || b >= self->len)
        return;
    void *tmp = xrealloc(NULL, self->elem_size);
    char *pa  = (char *)self->data + a * self->elem_size;
    char *pb  = (char *)self->data + b * self->elem_size;
    memcpy(tmp, pa, self->elem_size);
    memcpy(pa, pb, self->elem_size);
    memcpy(pb, tmp, self->elem_size);
    free(tmp);
}

void vec_swap(Vec *a, Vec *b) {
    Vec tmp = *a;
    *a      = *b;
    *b      = tmp;
}

bool vec_eq(Vec *a, Vec *b, EqFn eq_fn) {
    if (a->len != b->len)
        return false;
    for (size_t i = 0; i < a->len; i++) {
        void *ea = (char *)a->data + i * a->elem_size;
        void *eb = (char *)b->data + i * b->elem_size;
        if (!eq_fn(ea, eb))
            return false;
    }
    return true;
}

void vec_insert_cp(Vec *self, size_t index, const void *elem, CopyFn copy_fn) {
    if (index > self->len)
        return;
    vec_reserve(self, 1);
    if (index < self->len) {
        memmove((char *)self->data + (index + 1) * self->elem_size,
                (char *)self->data + index * self->elem_size,
                (self->len - index) * self->elem_size);
    }
    void *dst = (char *)self->data + index * self->elem_size;
    copy_fn(dst, (void *)elem);
    self->len++;
}

void vec_insert_mv(Vec *self, size_t index, void *elem, MoveFn move_fn) {
    if (index > self->len)
        return;
    vec_reserve(self, 1);
    if (index < self->len) {
        memmove((char *)self->data + (index + 1) * self->elem_size,
                (char *)self->data + index * self->elem_size,
                (self->len - index) * self->elem_size);
    }
    void *dst = (char *)self->data + index * self->elem_size;
    move_fn(dst, elem);
    self->len++;
}

void vec_remove(Vec *self, size_t index, void *out) {
    if (index >= self->len)
        return;
    if (out) {
        memcpy(out, (char *)self->data + index * self->elem_size, self->elem_size);
    }
    if (index < self->len - 1) {
        memmove((char *)self->data + index * self->elem_size,
                (char *)self->data + (index + 1) * self->elem_size,
                (self->len - index - 1) * self->elem_size);
    }
    self->len--;
}

void *vec_as_ptr(Vec *self) {
    return self->data;
}

void vec_copy(Vec *dst, const Vec *src, CopyFn copy_fn) {
    if (dst == src)
        return;
    vec_with_capacity(dst, src->elem_size, src->len);
    for (size_t i = 0; i < src->len; i++) {
        void *src_elem = (char *)src->data + i * src->elem_size;
        void *dst_elem = (char *)dst->data + i * dst->elem_size;
        copy_fn(dst_elem, src_elem);
    }
    dst->len = src->len;
}

void vec_move(Vec *dst, Vec *src) {
    if (dst == src)
        return;
    dst->data      = src->data;
    dst->len       = src->len;
    dst->capacity  = src->capacity;
    dst->elem_size = src->elem_size;
    src->data      = NULL;
    src->len       = 0;
    src->capacity  = 0;
    src->elem_size = 0;
}

void vec_sort(Vec *self, CmpFn cmp) {
    if (self->len <= 1)
        return;
    qsort(self->data, self->len, self->elem_size, cmp);
}

int vec_cmp(Vec *a, Vec *b, CmpFn cmp_fn) {
    size_t min_len = a->len < b->len ? a->len : b->len;
    for (size_t i = 0; i < min_len; i++) {
        void *ea = (char *)a->data + i * a->elem_size;
        void *eb = (char *)b->data + i * b->elem_size;
        int   c  = cmp_fn(ea, eb);
        if (c != 0)
            return c;
    }
    if (a->len < b->len)
        return -1;
    if (a->len > b->len)
        return 1;
    return 0;
}

UInt64 vec_hash(Vec *self, HashFn hash_fn) {
    UInt64 hash = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < self->len; i++) {
        void  *elem      = (char *)self->data + i * self->elem_size;
        UInt64 elem_hash = hash_fn(elem);
        hash             = fnv64(&elem_hash, sizeof(UInt64), hash);
    }
    return hash;
}

/* ==================== Box Implementation ==================== */

int box_init(Box *self, void *ptr) {
    self->data = ptr;
    return 0;
}

void box_set(Box *self, void *ptr, DropFn drop_fn) {
    if (self->data) {
        drop_fn(self->data);
        free(self->data);
    }
    self->data = ptr;
}

void box_drop(Box *self, DropFn drop_fn) {
    if (self->data) {
        drop_fn(self->data);
        free(self->data);
        self->data = NULL;
    }
}

void box_move(Box *dst, Box *src) {
    if (dst == src)
        return;
    dst->data = src->data;
    src->data = NULL;
}

void *box_release(Box *self) {
    void *ptr  = self->data;
    self->data = NULL;
    return ptr;
}

void *box_ptr(Box *self) {
    return self->data;
}

bool box_is_null(const Box *self) {
    return self->data == NULL;
}

int box_cmp(Box *a, Box *b) {
    if (a->data < b->data)
        return -1;
    if (a->data > b->data)
        return 1;
    return 0;
}

bool box_eq(Box *a, Box *b) {
    return a->data == b->data;
}

UInt64 box_hash(Box *self) {
    UInt64 val = (UInt64)(uintptr_t)self->data;
    return fnv64(&val, sizeof(val), FNV_OFFSET_BASIS);
}

/* ==================== PQueue Implementation ==================== */

/* Internal: compare elements at indices a and b */
static int pqueue_cmp_idx(PQueue *self, size_t a, size_t b) {
    void *ea = self->data.data + a * self->data.elem_size;
    void *eb = self->data.data + b * self->data.elem_size;
    return self->cmp_fn(ea, eb);
}

/* Internal: swap elements at indices a and b */
static void pqueue_swap_idx(PQueue *self, size_t a, size_t b) {
    size_t es  = self->data.elem_size;
    char  *pa  = self->data.data + a * es;
    char  *pb  = self->data.data + b * es;
    char  *tmp = alloca(es);
    memcpy(tmp, pa, es);
    memcpy(pa, pb, es);
    memcpy(pb, tmp, es);
}

/* Internal: sift up from index idx (restore heap property upward) */
static void pqueue_sift_up(PQueue *self, size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (pqueue_cmp_idx(self, idx, parent) > 0) {
            pqueue_swap_idx(self, idx, parent);
            idx = parent;
        } else {
            break;
        }
    }
}

/* Internal: sift down from index idx (restore heap property downward) */
static void pqueue_sift_down(PQueue *self, size_t idx) {
    size_t len = self->data.len;
    for (;;) {
        size_t left    = 2 * idx + 1;
        size_t right   = 2 * idx + 2;
        size_t largest = idx;
        if (left < len && pqueue_cmp_idx(self, left, largest) > 0) {
            largest = left;
        }
        if (right < len && pqueue_cmp_idx(self, right, largest) > 0) {
            largest = right;
        }
        if (largest == idx) {
            break;
        }
        pqueue_swap_idx(self, idx, largest);
        idx = largest;
    }
}

int pqueue_init(PQueue *self, size_t elem_size, CmpFn cmp_fn) {
    vec_init(&self->data, elem_size);
    self->cmp_fn = cmp_fn;
    return 0;
}

int pqueue_with_capacity(PQueue *self, size_t elem_size, CmpFn cmp_fn, size_t capacity) {
    vec_with_capacity(&self->data, elem_size, capacity);
    self->cmp_fn = cmp_fn;
    return 0;
}

void pqueue_push_cp(PQueue *self, const void *elem, CopyFn copy_fn) {
    vec_push_cp(&self->data, elem, copy_fn);
    pqueue_sift_up(self, self->data.len - 1);
}

void pqueue_push_mv(PQueue *self, void *elem, MoveFn move_fn) {
    vec_push_mv(&self->data, elem, move_fn);
    pqueue_sift_up(self, self->data.len - 1);
}

void pqueue_pop(PQueue *self, DropFn drop_fn) {
    if (self->data.len == 0)
        return;
    size_t es   = self->data.elem_size;
    char  *base = (char *)self->data.data;

    /* drop top element */
    if (drop_fn) {
        drop_fn(base);
    }

    /* move last element to top */
    if (self->data.len > 1) {
        memcpy(base, base + (self->data.len - 1) * es, es);
    }

    self->data.len--;

    /* restore heap property */
    if (self->data.len > 1) {
        pqueue_sift_down(self, 0);
    }
}

void *pqueue_top(PQueue *self) {
    if (self->data.len == 0)
        return NULL;
    return self->data.data;
}

size_t pqueue_len(const PQueue *self) {
    return self->data.len;
}

bool pqueue_is_empty(const PQueue *self) {
    return self->data.len == 0;
}

void pqueue_clear(PQueue *self, DropFn drop_fn) {
    vec_clear(&self->data, drop_fn);
}

void pqueue_drop(PQueue *self, DropFn drop_fn) {
    vec_drop(&self->data, drop_fn);
    self->cmp_fn = NULL;
}

void pqueue_copy(PQueue *dst, const PQueue *src, CopyFn copy_fn) {
    if (dst == src)
        return;
    vec_copy(&dst->data, &src->data, copy_fn);
    dst->cmp_fn = src->cmp_fn;
}

void pqueue_move(PQueue *dst, PQueue *src) {
    if (dst == src)
        return;
    vec_move(&dst->data, &src->data);
    dst->cmp_fn = src->cmp_fn;
    src->cmp_fn = NULL;
}

/* ==================== CBList Implementation ==================== */

/* Internal: allocate a node with space for elem_size data */
static CBListNode *list_new_node(size_t elem_size) {
    return xrealloc(NULL, sizeof(CBListNode) + elem_size);
}

int list_init(CBList *self, size_t elem_size) {
    self->vhead       = xrealloc(NULL, sizeof(CBListNode));
    self->vtail       = xrealloc(NULL, sizeof(CBListNode));
    self->vhead->prev = NULL;
    self->vhead->next = self->vtail;
    self->vtail->prev = self->vhead;
    self->vtail->next = NULL;
    self->len         = 0;
    self->elem_size   = elem_size;
    return 0;
}

void list_drop(CBList *self, DropFn value_drop) {
    CBListNode *cur = self->vhead;
    while (cur != NULL) {
        CBListNode *next = cur->next;
        if (cur != self->vhead && cur != self->vtail && value_drop) {
            value_drop(cur->data);
        }
        free(cur);
        cur = next;
    }
    self->vhead     = NULL;
    self->vtail     = NULL;
    self->len       = 0;
    self->elem_size = 0;
}

void list_clear(CBList *self, DropFn value_drop) {
    CBListNode *cur = self->vhead->next;
    while (cur != self->vtail) {
        CBListNode *next = cur->next;
        if (value_drop) {
            value_drop(cur->data);
        }
        free(cur);
        cur = next;
    }
    self->vhead->next = self->vtail;
    self->vtail->prev = self->vhead;
    self->len         = 0;
}

void list_copy(CBList *dst, const CBList *src, CopyFn copy_fn) {
    if (dst == src)
        return;
    list_init(dst, src->elem_size);
    CBListNode *it = src->vhead->next;
    while (it != src->vtail) {
        list_push_back_cp(dst, it->data, copy_fn);
        it = it->next;
    }
}

void list_move(CBList *dst, CBList *src) {
    if (dst == src)
        return;
    dst->vhead     = src->vhead;
    dst->vtail     = src->vtail;
    dst->len       = src->len;
    dst->elem_size = src->elem_size;
    src->vhead     = NULL;
    src->vtail     = NULL;
    src->len       = 0;
    src->elem_size = 0;
}

void list_swap(CBList *a, CBList *b) {
    CBList tmp = *a;
    *a         = *b;
    *b         = tmp;
}

CBListNode *list_insert_before_cp(CBList *self, CBListNode *iter, const void *elem, CopyFn copy_fn) {
    if (iter == NULL || iter->prev == NULL)
        return NULL;
    CBListNode *node = list_new_node(self->elem_size);
    node->prev       = iter->prev;
    node->next       = iter;
    copy_fn(node->data, (void *)elem);
    iter->prev->next = node;
    iter->prev       = node;
    self->len++;
    return node;
}

CBListNode *list_insert_before_mv(CBList *self, CBListNode *iter, void *elem, MoveFn move_fn) {
    if (iter == NULL || iter->prev == NULL)
        return NULL;
    CBListNode *node = list_new_node(self->elem_size);
    node->prev       = iter->prev;
    node->next       = iter;
    move_fn(node->data, elem);
    iter->prev->next = node;
    iter->prev       = node;
    self->len++;
    return node;
}

CBListNode *list_insert_after_cp(CBList *self, CBListNode *iter, const void *elem, CopyFn copy_fn) {
    if (iter == NULL || iter->next == NULL)
        return NULL;
    CBListNode *node = list_new_node(self->elem_size);
    node->next       = iter->next;
    node->prev       = iter;
    copy_fn(node->data, (void *)elem);
    iter->next->prev = node;
    iter->next       = node;
    self->len++;
    return node;
}

CBListNode *list_insert_after_mv(CBList *self, CBListNode *iter, void *elem, MoveFn move_fn) {
    if (iter == NULL || iter->next == NULL)
        return NULL;
    CBListNode *node = list_new_node(self->elem_size);
    node->next       = iter->next;
    node->prev       = iter;
    if (move_fn) {
        move_fn(node->data, elem);
    }
    iter->next->prev = node;
    iter->next       = node;
    self->len++;
    return node;
}

void list_remove(CBList *self, CBListNode *iter, DropFn value_drop) {
    if (iter == NULL || iter->prev == NULL || iter->next == NULL)
        return;
    if (value_drop) {
        value_drop(iter->data);
    }
    iter->prev->next = iter->next;
    iter->next->prev = iter->prev;
    free(iter);
    self->len--;
}

void list_set_cp(CBList *self, CBListNode *iter, const void *elem, DropFn value_drop, CopyFn copy_fn) {
    (void)self;
    if (iter == NULL || iter->prev == NULL || iter->next == NULL)
        return;
    value_drop(iter->data);
    copy_fn(iter->data, (void *)elem);
}

void list_set_mv(CBList *self, CBListNode *iter, void *elem, DropFn value_drop, MoveFn move_fn) {
    (void)self;
    if (iter == NULL || iter->prev == NULL || iter->next == NULL)
        return;
    value_drop(iter->data);
    move_fn(iter->data, elem);
}

CBListNode *list_begin(CBList *self) {
    return self->vhead->next;
}

CBListNode *list_last(CBList *self) {
    if (self->vtail->prev == self->vhead)
        return NULL;
    return self->vtail->prev;
}

CBListNode *list_end(CBList *self) {
    return self->vtail;
}

CBListNode *list_next(CBListNode *iter) {
    if (iter == NULL)
        return NULL;
    return iter->next;
}

CBListNode *list_prev(CBListNode *iter) {
    if (iter == NULL)
        return NULL;
    if (iter->prev == NULL)
        return NULL;
    if (iter->prev->prev == NULL)
        return NULL;
    return iter->prev;
}

void *list_get(CBListNode *iter) {
    if (iter == NULL)
        return NULL;
    return iter->data;
}

size_t list_len(const CBList *self) {
    return self->len;
}

bool list_is_empty(const CBList *self) {
    return self->len == 0;
}

CBListNode *list_push_back_cp(CBList *self, const void *elem, CopyFn copy_fn) {
    return list_insert_before_cp(self, self->vtail, elem, copy_fn);
}

CBListNode *list_push_back_mv(CBList *self, void *elem, MoveFn move_fn) {
    return list_insert_before_mv(self, self->vtail, elem, move_fn);
}

CBListNode *list_push_front_cp(CBList *self, const void *elem, CopyFn copy_fn) {
    return list_insert_after_cp(self, self->vhead, elem, copy_fn);
}

CBListNode *list_push_front_mv(CBList *self, void *elem, MoveFn move_fn) {
    return list_insert_after_mv(self, self->vhead, elem, move_fn);
}

void list_pop_back(CBList *self, DropFn value_drop) {
    list_remove(self, self->vtail->prev, value_drop);
}

void list_pop_front(CBList *self, DropFn value_drop) {
    list_remove(self, self->vhead->next, value_drop);
}

bool list_eq(CBList *a, CBList *b, EqFn eq_fn) {
    if (a->len != b->len)
        return false;
    CBListNode *ia = a->vhead->next;
    CBListNode *ib = b->vhead->next;
    while (ia != a->vtail) {
        if (!eq_fn(ia->data, ib->data))
            return false;
        ia = ia->next;
        ib = ib->next;
    }
    return true;
}

int list_cmp(CBList *a, CBList *b, CmpFn cmp_fn) {
    CBListNode *ia = a->vhead->next;
    CBListNode *ib = b->vhead->next;
    while (ia != a->vtail && ib != b->vtail) {
        int c = cmp_fn(ia->data, ib->data);
        if (c != 0)
            return c;
        ia = ia->next;
        ib = ib->next;
    }
    if (ia == a->vtail && ib == b->vtail)
        return 0;
    if (ia == a->vtail)
        return -1;
    return 1;
}

UInt64 list_hash(CBList *self, HashFn hash_fn) {
    UInt64      hash = FNV_OFFSET_BASIS;
    CBListNode *it   = self->vhead->next;
    while (it != self->vtail) {
        UInt64 elem_hash = hash_fn(it->data);
        hash             = fnv64(&elem_hash, sizeof(UInt64), hash);
        it               = it->next;
    }
    return hash;
}

/* ==================== HashTable Implementation ==================== */

#define HASHTABLE_INITIAL_BUCKETS       16
#define HASHTABLE_LOAD_FACTOR_THRESHOLD 0.75

/* Internal: push a new CBList bucket into the buckets vec (move semantics) */
static void hashtable_push_bucket(HashTable *self) {
    CBList bucket;
    list_init(&bucket, self->value_offset + self->value_size);
    vec_reserve(&self->buckets, 1);
    void *dst = (char *)self->buckets.data + self->buckets.len * sizeof(CBList);
    memcpy(dst, &bucket, sizeof(CBList));
    self->buckets.len++;
}

/* Internal: get bucket CBList by index */
static CBList *hashtable_bucket(HashTable *self, size_t idx) {
    return (CBList *)((char *)self->buckets.data + idx * sizeof(CBList));
}

/* Internal: find node by key in a bucket */
static CBListNode *hashtable_find_in_bucket(CBList *bucket, const void *key, EqFn eq_fn) {
    CBListNode *it = list_begin(bucket);
    while (it != list_end(bucket)) {
        if (eq_fn(list_get(it), (void *)key))
            return it;
        it = list_next(it);
    }
    return NULL;
}

/* Internal: resize (rehash) the hashtable */
static void hashtable_resize(HashTable *self, size_t new_num_buckets) {
    HashTable new_ht = {0};
    new_ht.key_size     = self->key_size;
    new_ht.value_size   = self->value_size;
    new_ht.value_offset = self->value_offset;
    new_ht.len          = self->len;
    new_ht.key_hash_fn  = self->key_hash_fn;
    new_ht.key_eq_fn    = self->key_eq_fn;
    vec_init(&new_ht.buckets, sizeof(CBList));
    for (size_t i = 0; i < new_num_buckets; i++) {
        hashtable_push_bucket(&new_ht);
    }
    /* Move all nodes from old buckets to new buckets (relink, no copy/drop) */
    for (size_t i = 0; i < self->buckets.len; i++) {
        CBList     *old_bucket = hashtable_bucket(self, i);
        CBListNode *it         = list_begin(old_bucket);
        while (it != list_end(old_bucket)) {
            CBListNode *next       = list_next(it);
            void       *key        = list_get(it);
            UInt64      hash       = self->key_hash_fn(key);
            size_t      idx        = hash % new_num_buckets;
            CBList     *new_bucket = hashtable_bucket(&new_ht, idx);
            /* Detach from old bucket */
            it->prev->next = it->next;
            it->next->prev = it->prev;
            old_bucket->len--;

            /* Insert at front of new bucket */
            it->next                      = new_bucket->vhead->next;
            it->prev                      = new_bucket->vhead;
            new_bucket->vhead->next->prev = it;
            new_bucket->vhead->next       = it;
            new_bucket->len++;

            it = next;
        }
        old_bucket->vhead->next = old_bucket->vtail;
        old_bucket->vtail->prev = old_bucket->vhead;
    }
    hashtable_drop(self, NULL, NULL);
    memcpy(self, &new_ht, sizeof(HashTable));
}

/* Internal: check if resize needed */
static void hashtable_maybe_resize(HashTable *self) {
    if (self->buckets.len == 0)
        return;
    double load = (double)self->len / (double)self->buckets.len;
    if (load >= HASHTABLE_LOAD_FACTOR_THRESHOLD) {
        hashtable_resize(self, self->buckets.len * 2);
    }
}

int hashtable_init(
        HashTable *self, size_t key_size, size_t value_size, size_t value_offset, HashFn key_hash_fn, EqFn key_eq_fn) {
    self->key_size     = key_size;
    self->value_size   = value_size;
    self->value_offset = value_offset;
    self->len          = 0;
    self->key_hash_fn  = key_hash_fn;
    self->key_eq_fn    = key_eq_fn;
    vec_init(&self->buckets, sizeof(CBList));
    for (size_t i = 0; i < HASHTABLE_INITIAL_BUCKETS; i++) {
        hashtable_push_bucket(self);
    }
    return 0;
}

void hashtable_drop(HashTable *self, DropFn key_drop, DropFn value_drop) {
    for (size_t i = 0; i < self->buckets.len; i++) {
        CBList     *bucket = hashtable_bucket(self, i);
        CBListNode *cur    = list_begin(bucket);
        while (cur != list_end(bucket)) {
            CBListNode *next = list_next(cur);
            if (key_drop)
                key_drop(list_get(cur));
            if (value_drop)
                value_drop((char *)list_get(cur) + self->value_offset);
            free(cur);
            cur = next;
        }
        free(bucket->vhead);
        free(bucket->vtail);
        bucket->vhead = NULL;
        bucket->vtail = NULL;
        bucket->len   = 0;
    }
    free(self->buckets.data);
    self->buckets.data     = NULL;
    self->buckets.len      = 0;
    self->buckets.capacity = 0;
    self->len              = 0;
}

void hashtable_clear(HashTable *self, DropFn key_drop, DropFn value_drop) {
    for (size_t i = 0; i < self->buckets.len; i++) {
        CBList     *bucket = hashtable_bucket(self, i);
        CBListNode *cur    = list_begin(bucket);
        while (cur != list_end(bucket)) {
            CBListNode *next = list_next(cur);
            if (key_drop)
                key_drop(list_get(cur));
            if (value_drop)
                value_drop((char *)list_get(cur) + self->value_offset);
            free(cur);
            cur = next;
        }
        bucket->vhead->next = bucket->vtail;
        bucket->vtail->prev = bucket->vhead;
        bucket->len         = 0;
    }
    self->len = 0;
}

void hashtable_insert_cp(HashTable *self,
        const void                 *key,
        const void                 *value,
        CopyFn                      key_copy,
        CopyFn                      value_copy,
        DropFn                      key_drop,
        DropFn                      value_drop) {
    UInt64  hash   = self->key_hash_fn((void *)key);
    size_t  idx    = hash % self->buckets.len;
    CBList *bucket = hashtable_bucket(self, idx);

    CBListNode *existing = hashtable_find_in_bucket(bucket, key, self->key_eq_fn);
    if (existing) {
        void *entry = list_get(existing);
        if (key_drop)
            key_drop(entry);
        if (value_drop)
            value_drop((char *)entry + self->value_offset);
        key_copy(entry, (void *)key);
        value_copy((char *)entry + self->value_offset, (void *)value);
    } else {
        CBListNode *node = list_push_front_mv(bucket, NULL, NULL);
        key_copy(node->data, (void *)key);
        value_copy((char *)node->data + self->value_offset, (void *)value);
        self->len++;
        hashtable_maybe_resize(self);
    }
}

void hashtable_insert_mv(HashTable *self,
        void                       *key,
        void                       *value,
        MoveFn                      key_move,
        MoveFn                      value_move,
        DropFn                      key_drop,
        DropFn                      value_drop) {
    UInt64  hash   = self->key_hash_fn((void *)key);
    size_t  idx    = hash % self->buckets.len;
    CBList *bucket = hashtable_bucket(self, idx);

    CBListNode *existing = hashtable_find_in_bucket(bucket, key, self->key_eq_fn);
    if (existing) {
        void *entry = list_get(existing);
        if (key_drop)
            key_drop(entry);
        if (value_drop)
            value_drop((char *)entry + self->value_offset);
        key_move(entry, key);
        value_move((char *)entry + self->value_offset, value);
    } else {
        CBListNode *node = list_push_front_mv(bucket, NULL, NULL);
        key_move(node->data, key);
        value_move((char *)node->data + self->value_offset, value);
        self->len++;
        hashtable_maybe_resize(self);
    }
}

void *hashtable_get(HashTable *self, const void *key) {
    if (self->buckets.len == 0)
        return NULL;
    UInt64      hash   = self->key_hash_fn((void *)key);
    size_t      idx    = hash % self->buckets.len;
    CBList     *bucket = hashtable_bucket(self, idx);
    CBListNode *node   = hashtable_find_in_bucket(bucket, key, self->key_eq_fn);
    if (node) {
        return (char *)list_get(node) + self->value_offset;
    }
    return NULL;
}

bool hashtable_remove(HashTable *self, const void *key, DropFn key_drop, DropFn value_drop) {
    if (self->buckets.len == 0)
        return false;
    UInt64      hash   = self->key_hash_fn((void *)key);
    size_t      idx    = hash % self->buckets.len;
    CBList     *bucket = hashtable_bucket(self, idx);
    CBListNode *node   = hashtable_find_in_bucket(bucket, key, self->key_eq_fn);
    if (node) {
        void *entry = list_get(node);
        if (key_drop)
            key_drop(entry);
        if (value_drop)
            value_drop((char *)entry + self->value_offset);
        node->prev->next = node->next;
        node->next->prev = node->prev;
        free(node);
        bucket->len--;
        self->len--;
        return true;
    }
    return false;
}

size_t hashtable_len(const HashTable *self) {
    return self->len;
}

bool hashtable_is_empty(const HashTable *self) {
    return self->len == 0;
}

void hashtable_copy(
        HashTable *dst, const HashTable *src, CopyFn key_copy, CopyFn value_copy, DropFn key_drop, DropFn value_drop) {
    if (dst == src)
        return;
    dst->key_size     = src->key_size;
    dst->value_size   = src->value_size;
    dst->value_offset = src->value_offset;
    dst->len          = 0;
    dst->key_hash_fn  = src->key_hash_fn;
    dst->key_eq_fn    = src->key_eq_fn;
    vec_init(&dst->buckets, sizeof(CBList));
    for (size_t i = 0; i < src->buckets.len; i++) {
        hashtable_push_bucket(dst);
    }

    for (size_t i = 0; i < src->buckets.len; i++) {
        CBList     *src_bucket = (CBList *)((char *)src->buckets.data + i * sizeof(CBList));
        CBList     *dst_bucket = hashtable_bucket(dst, i);
        CBListNode *it         = list_begin(src_bucket);
        while (it != list_end(src_bucket)) {
            void       *src_key   = list_get(it);
            void       *src_value = (char *)list_get(it) + src->value_offset;
            CBListNode *node      = list_push_front_mv(dst_bucket, NULL, NULL);
            key_copy(node->data, src_key);
            value_copy((char *)node->data + dst->value_offset, src_value);
            dst->len++;

            it = list_next(it);
        }
    }
    (void)key_drop;
    (void)value_drop;
}

void hashtable_move(HashTable *dst, HashTable *src) {
    if (dst == src)
        return;
    dst->buckets      = src->buckets;
    dst->key_size     = src->key_size;
    dst->value_size   = src->value_size;
    dst->value_offset = src->value_offset;
    dst->len          = src->len;
    dst->key_hash_fn  = src->key_hash_fn;
    dst->key_eq_fn    = src->key_eq_fn;

    src->buckets.data      = NULL;
    src->buckets.len       = 0;
    src->buckets.capacity  = 0;
    src->buckets.elem_size = 0;
    src->key_size          = 0;
    src->value_size        = 0;
    src->value_offset      = 0;
    src->len               = 0;
    src->key_hash_fn       = NULL;
    src->key_eq_fn         = NULL;
}

/* ==================== HashTable Iterator Implementation ==================== */

HashTableIter hashtable_begin(HashTable *self) {
    HashTableIter iter = {0};
    iter.ht         = self;
    iter.bucket_idx = 0;
    iter.node       = NULL;
    if (self->buckets.len > 0) {
        CBList *bucket = hashtable_bucket(self, 0);
        iter.node      = list_begin(bucket);
    }
    /* Skip empty buckets */
    while (iter.node == list_end(hashtable_bucket(self, iter.bucket_idx))) {
        iter.bucket_idx++;
        if (iter.bucket_idx >= self->buckets.len) {
            iter.node = NULL;
            break;
        }
        iter.node = list_begin(hashtable_bucket(self, iter.bucket_idx));
    }
    return iter;
}

bool hashtable_iter_is_end(const HashTableIter *iter) {
    return iter->node == NULL;
}

void hashtable_iter_next(HashTableIter *iter) {
    if (iter->node == NULL)
        return;
    iter->node = list_next(iter->node);
    while (iter->ht->buckets.len > 0 && iter->node == list_end(hashtable_bucket(iter->ht, iter->bucket_idx))) {
        iter->bucket_idx++;
        if (iter->bucket_idx >= iter->ht->buckets.len) {
            iter->node = NULL;
            break;
        }
        iter->node = list_begin(hashtable_bucket(iter->ht, iter->bucket_idx));
    }
}

void *hashtable_iter_key(HashTableIter *iter) {
    if (iter->node == NULL)
        return NULL;
    return list_get(iter->node);
}

void *hashtable_iter_value(HashTableIter *iter) {
    if (iter->node == NULL)
        return NULL;
    return (char *)list_get(iter->node) + iter->ht->value_offset;
}

/* ==================== RBTMap Implementation ==================== */

#define RB_RED   1
#define RB_BLACK 0

/* Internal: allocate a node with space for key+value data */
static RBTNode *rbtmap_new_node(size_t data_size) {
    return xrealloc(NULL, sizeof(RBTNode) + data_size);
}

/* Internal: get key pointer from node data */
static void *rbtmap_node_key(RBTNode *node) {
    return node->data;
}

/* Internal: get value pointer from node data */
static void *rbtmap_node_value(RBTMap *self, RBTNode *node) {
    return node->data + self->value_offset;
}

/* Internal: compare a key with a node's key */
static int rbtmap_cmp_key(RBTMap *self, const void *key, RBTNode *node) {
    return self->key_cmp_fn(key, rbtmap_node_key(node));
}

static void rbtmap_rotate_left(RBTMap *self, RBTNode *x) {
    RBTNode *y = x->right;
    x->right   = y->left;
    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        self->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left   = x;
    x->parent = y;
}

static void rbtmap_rotate_right(RBTMap *self, RBTNode *x) {
    RBTNode *y = x->left;
    x->left    = y->right;
    if (y->right)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        self->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right  = x;
    x->parent = y;
}

static void rbtmap_insert_fixup(RBTMap *self, RBTNode *z) {
    while (z->parent && z->parent->color == RB_RED) {
        if (z->parent == z->parent->parent->left) {
            RBTNode *y = z->parent->parent->right;
            if (y && y->color == RB_RED) {
                z->parent->color         = RB_BLACK;
                y->color                 = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z                        = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rbtmap_rotate_left(self, z);
                }
                z->parent->color         = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rbtmap_rotate_right(self, z->parent->parent);
            }
        } else {
            RBTNode *y = z->parent->parent->left;
            if (y && y->color == RB_RED) {
                z->parent->color         = RB_BLACK;
                y->color                 = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z                        = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rbtmap_rotate_right(self, z);
                }
                z->parent->color         = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rbtmap_rotate_left(self, z->parent->parent);
            }
        }
    }
    self->root->color = RB_BLACK;
}

static void rbtmap_transplant(RBTMap *self, RBTNode *u, RBTNode *v) {
    if (u->parent == NULL)
        self->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

static RBTNode *rbtmap_min(RBTNode *x) {
    while (x->left)
        x = x->left;
    return x;
}

static RBTNode *rbtmap_max(RBTNode *x) {
    while (x->right)
        x = x->right;
    return x;
}

static void rbtmap_remove_fixup(RBTMap *self, RBTNode *x, RBTNode *xparent) {
    while (x != self->root && (x == NULL || x->color == RB_BLACK)) {
        if (x == xparent->left) {
            RBTNode *w = xparent->right;
            if (w->color == RB_RED) {
                w->color        = RB_BLACK;
                xparent->color  = RB_RED;
                rbtmap_rotate_left(self, xparent);
                w = xparent->right;
            }
            if ((w->left == NULL || w->left->color == RB_BLACK) &&
                (w->right == NULL || w->right->color == RB_BLACK)) {
                w->color = RB_RED;
                x        = xparent;
                xparent  = x->parent;
            } else {
                if (w->right == NULL || w->right->color == RB_BLACK) {
                    if (w->left)
                        w->left->color = RB_BLACK;
                    w->color = RB_RED;
                    rbtmap_rotate_right(self, w);
                    w = xparent->right;
                }
                w->color        = xparent->color;
                xparent->color  = RB_BLACK;
                if (w->right)
                    w->right->color = RB_BLACK;
                rbtmap_rotate_left(self, xparent);
                x = self->root;
                break;
            }
        } else {
            RBTNode *w = xparent->left;
            if (w->color == RB_RED) {
                w->color        = RB_BLACK;
                xparent->color  = RB_RED;
                rbtmap_rotate_right(self, xparent);
                w = xparent->left;
            }
            if ((w->right == NULL || w->right->color == RB_BLACK) &&
                (w->left == NULL || w->left->color == RB_BLACK)) {
                w->color = RB_RED;
                x        = xparent;
                xparent  = x->parent;
            } else {
                if (w->left == NULL || w->left->color == RB_BLACK) {
                    if (w->right)
                        w->right->color = RB_BLACK;
                    w->color = RB_RED;
                    rbtmap_rotate_left(self, w);
                    w = xparent->left;
                }
                w->color        = xparent->color;
                xparent->color  = RB_BLACK;
                if (w->left)
                    w->left->color = RB_BLACK;
                rbtmap_rotate_right(self, xparent);
                x = self->root;
                break;
            }
        }
    }
    if (x)
        x->color = RB_BLACK;
}

int rbtmap_init(RBTMap *self, size_t key_size, size_t value_size, size_t value_offset, CmpFn key_cmp_fn) {
    self->root         = NULL;
    self->len          = 0;
    self->key_size     = key_size;
    self->value_size   = value_size;
    self->value_offset = value_offset;
    self->key_cmp_fn   = key_cmp_fn;
    return 0;
}

static void rbtmap_destroy_subtree(RBTNode *node, DropFn key_drop, DropFn value_drop, size_t value_offset) {
    if (node == NULL)
        return;
    rbtmap_destroy_subtree(node->left, key_drop, value_drop, value_offset);
    rbtmap_destroy_subtree(node->right, key_drop, value_drop, value_offset);
    if (key_drop)
        key_drop(node->data);
    if (value_drop)
        value_drop(node->data + value_offset);
    free(node);
}

void rbtmap_drop(RBTMap *self, DropFn key_drop, DropFn value_drop) {
    rbtmap_destroy_subtree(self->root, key_drop, value_drop, self->value_offset);
    self->root        = NULL;
    self->len         = 0;
    self->key_size    = 0;
    self->value_size  = 0;
    self->value_offset = 0;
    self->key_cmp_fn  = NULL;
}

void rbtmap_clear(RBTMap *self, DropFn key_drop, DropFn value_drop) {
    rbtmap_destroy_subtree(self->root, key_drop, value_drop, self->value_offset);
    self->root = NULL;
    self->len  = 0;
}

void rbtmap_insert_cp(RBTMap *self,
        const void                 *key,
        const void                 *value,
        CopyFn                      key_copy,
        CopyFn                      value_copy,
        DropFn                      key_drop,
        DropFn                      value_drop) {
    RBTNode *parent = NULL;
    RBTNode *cur    = self->root;
    int      comp   = 0;
    while (cur) {
        parent = cur;
        comp   = rbtmap_cmp_key(self, key, cur);
        if (comp < 0)
            cur = cur->left;
        else if (comp > 0)
            cur = cur->right;
        else {
            /* existing key: replace */
            void *kptr = rbtmap_node_key(cur);
            void *vptr = rbtmap_node_value(self, cur);
            if (key_drop)
                key_drop(kptr);
            if (value_drop)
                value_drop(vptr);
            key_copy(kptr, (void *)key);
            value_copy(vptr, (void *)value);
            return;
        }
    }
    RBTNode *node = rbtmap_new_node(self->value_offset + self->value_size);
    node->left    = NULL;
    node->right   = NULL;
    node->parent  = parent;
    node->color   = RB_RED;
    key_copy(rbtmap_node_key(node), (void *)key);
    value_copy(rbtmap_node_value(self, node), (void *)value);
    if (parent == NULL)
        self->root = node;
    else if (comp < 0)
        parent->left = node;
    else
        parent->right = node;
    self->len++;
    rbtmap_insert_fixup(self, node);
}

void rbtmap_insert_mv(RBTMap *self,
        void                       *key,
        void                       *value,
        MoveFn                      key_move,
        MoveFn                      value_move,
        DropFn                      key_drop,
        DropFn                      value_drop) {
    RBTNode *parent = NULL;
    RBTNode *cur    = self->root;
    int      comp   = 0;
    while (cur) {
        parent = cur;
        comp   = rbtmap_cmp_key(self, key, cur);
        if (comp < 0)
            cur = cur->left;
        else if (comp > 0)
            cur = cur->right;
        else {
            void *kptr = rbtmap_node_key(cur);
            void *vptr = rbtmap_node_value(self, cur);
            if (key_drop)
                key_drop(kptr);
            if (value_drop)
                value_drop(vptr);
            key_move(kptr, key);
            value_move(vptr, value);
            return;
        }
    }
    RBTNode *node = rbtmap_new_node(self->value_offset + self->value_size);
    node->left    = NULL;
    node->right   = NULL;
    node->parent  = parent;
    node->color   = RB_RED;
    key_move(rbtmap_node_key(node), key);
    value_move(rbtmap_node_value(self, node), value);
    if (parent == NULL)
        self->root = node;
    else if (comp < 0)
        parent->left = node;
    else
        parent->right = node;
    self->len++;
    rbtmap_insert_fixup(self, node);
}

void *rbtmap_get(RBTMap *self, const void *key) {
    RBTNode *cur = self->root;
    while (cur) {
        int comp = rbtmap_cmp_key(self, key, cur);
        if (comp < 0)
            cur = cur->left;
        else if (comp > 0)
            cur = cur->right;
        else
            return rbtmap_node_value(self, cur);
    }
    return NULL;
}

RBTMapIter rbtmap_get_iter(RBTMap *self, const void *key) {
    RBTMapIter iter = {0};
    iter.map  = self;
    iter.node = NULL;
    RBTNode *cur = self->root;
    while (cur) {
        int comp = rbtmap_cmp_key(self, key, cur);
        if (comp < 0)
            cur = cur->left;
        else if (comp > 0)
            cur = cur->right;
        else {
            iter.node = cur;
            return iter;
        }
    }
    return iter;
}

RBTMapIter rbtmap_lower_bound(RBTMap *self, const void *key) {
    RBTMapIter iter = {0};
    iter.map  = self;
    iter.node = NULL;
    RBTNode *cur   = self->root;
    RBTNode *best  = NULL;
    while (cur) {
        int comp = rbtmap_cmp_key(self, key, cur);
        if (comp <= 0) {
            best = cur;
            cur  = cur->left;
        } else {
            cur = cur->right;
        }
    }
    iter.node = best;
    return iter;
}

RBTMapIter rbtmap_upper_bound(RBTMap *self, const void *key) {
    RBTMapIter iter = {0};
    iter.map  = self;
    iter.node = NULL;
    RBTNode *cur  = self->root;
    RBTNode *best = NULL;
    while (cur) {
        int comp = rbtmap_cmp_key(self, key, cur);
        if (comp < 0) {
            best = cur;
            cur  = cur->left;
        } else {
            cur = cur->right;
        }
    }
    iter.node = best;
    return iter;
}

bool rbtmap_remove(RBTMap *self, const void *key, DropFn key_drop, DropFn value_drop) {
    RBTNode *z = self->root;
    while (z) {
        int comp = rbtmap_cmp_key(self, key, z);
        if (comp < 0)
            z = z->left;
        else if (comp > 0)
            z = z->right;
        else
            break;
    }
    if (z == NULL)
        return false;

    RBTNode *y      = z;
    int      ycolor = y->color;
    RBTNode *x;
    RBTNode *xparent;

    if (z->left == NULL) {
        x       = z->right;
        xparent = z->parent;
        rbtmap_transplant(self, z, z->right);
    } else if (z->right == NULL) {
        x       = z->left;
        xparent = z->parent;
        rbtmap_transplant(self, z, z->left);
    } else {
        y      = rbtmap_min(z->right);
        ycolor = y->color;
        x      = y->right;
        if (y->parent == z) {
            xparent = y;
        } else {
            xparent = y->parent;
            rbtmap_transplant(self, y, y->right);
            y->right         = z->right;
            y->right->parent = y;
        }
        rbtmap_transplant(self, z, y);
        y->left         = z->left;
        y->left->parent = y;
        y->color        = z->color;
    }

    if (key_drop)
        key_drop(rbtmap_node_key(z));
    if (value_drop)
        value_drop(rbtmap_node_value(self, z));
    free(z);
    self->len--;

    if (ycolor == RB_BLACK)
        rbtmap_remove_fixup(self, x, xparent);

    return true;
}

size_t rbtmap_len(const RBTMap *self) {
    return self->len;
}

bool rbtmap_is_empty(const RBTMap *self) {
    return self->len == 0;
}

static void rbtmap_copy_subtree(
        RBTMap *dst, RBTNode *src_node, CopyFn key_copy, CopyFn value_copy) {
    if (src_node == NULL)
        return;
    rbtmap_copy_subtree(dst, src_node->left, key_copy, value_copy);
    rbtmap_insert_cp(dst, rbtmap_node_key(src_node), rbtmap_node_value(dst, src_node), key_copy, value_copy, NULL, NULL);
    rbtmap_copy_subtree(dst, src_node->right, key_copy, value_copy);
}

void rbtmap_copy(
        RBTMap *dst, const RBTMap *src, CopyFn key_copy, CopyFn value_copy, DropFn key_drop, DropFn value_drop) {
    if (dst == src)
        return;
    rbtmap_clear(dst, key_drop, value_drop);
    dst->key_size     = src->key_size;
    dst->value_size   = src->value_size;
    dst->value_offset = src->value_offset;
    dst->key_cmp_fn   = src->key_cmp_fn;
    rbtmap_copy_subtree(dst, src->root, key_copy, value_copy);
}

void rbtmap_move(RBTMap *dst, RBTMap *src) {
    if (dst == src)
        return;
    dst->root         = src->root;
    dst->len          = src->len;
    dst->key_size     = src->key_size;
    dst->value_size   = src->value_size;
    dst->value_offset = src->value_offset;
    dst->key_cmp_fn   = src->key_cmp_fn;
    src->root         = NULL;
    src->len          = 0;
    src->key_size     = 0;
    src->value_size   = 0;
    src->value_offset = 0;
    src->key_cmp_fn   = NULL;
}

/* ==================== RBTMap Iterator Implementation ==================== */

static RBTNode *rbtmap_successor(RBTNode *x) {
    if (x->right) {
        x = x->right;
        while (x->left)
            x = x->left;
        return x;
    }
    RBTNode *parent = x->parent;
    while (parent && x == parent->right) {
        x      = parent;
        parent = parent->parent;
    }
    return parent;
}

static RBTNode *rbtmap_predecessor(RBTNode *x) {
    if (x->left) {
        x = x->left;
        while (x->right)
            x = x->right;
        return x;
    }
    RBTNode *parent = x->parent;
    while (parent && x == parent->left) {
        x      = parent;
        parent = parent->parent;
    }
    return parent;
}

RBTMapIter rbtmap_begin(RBTMap *self) {
    RBTMapIter iter = {0};
    iter.map  = self;
    iter.node = self->root ? rbtmap_min(self->root) : NULL;
    return iter;
}

RBTMapIter rbtmap_last(RBTMap *self) {
    RBTMapIter iter = {0};
    iter.map  = self;
    iter.node = self->root ? rbtmap_max(self->root) : NULL;
    return iter;
}

bool rbtmap_iter_is_end(const RBTMapIter *iter) {
    return iter->node == NULL;
}

void rbtmap_iter_next(RBTMapIter *iter) {
    if (iter->node == NULL)
        return;
    iter->node = rbtmap_successor(iter->node);
}

void rbtmap_iter_prev(RBTMapIter *iter) {
    if (iter->node == NULL)
        return;
    iter->node = rbtmap_predecessor(iter->node);
}

void *rbtmap_iter_key(RBTMapIter *iter) {
    if (iter->node == NULL)
        return NULL;
    return rbtmap_node_key(iter->node);
}

void *rbtmap_iter_value(RBTMapIter *iter) {
    if (iter->node == NULL)
        return NULL;
    return rbtmap_node_value(iter->map, iter->node);
}

/* ==================== Basic Type Trait Implementations ==================== */

#define DEFINE_NUMERIC_TRAITS(T) \
    void T##_drop(T *self) { \
        (void)self; \
    } \
    int T##_cmp(T *a, T *b) { \
        if (*a < *b) \
            return -1; \
        if (*a > *b) \
            return 1; \
        return 0; \
    } \
    bool T##_eq(T *a, T *b) { \
        return *a == *b; \
    } \
    UInt64 T##_hash(T *self) { \
        return fnv64(self, sizeof(T), FNV_OFFSET_BASIS); \
    } \
    void T##_copy(T *dst, const T *src) { \
        *dst = *src; \
    } \
    void T##_move(T *dst, T *src) { \
        *dst = *src; \
    } \
    void T##_swap(T *dst, T *src) { \
        T _tmp = *dst; \
        *dst   = *src; \
        *src   = _tmp; \
    }

DEFINE_NUMERIC_TRAITS(Int8)
DEFINE_NUMERIC_TRAITS(Int16)
DEFINE_NUMERIC_TRAITS(Int32)
DEFINE_NUMERIC_TRAITS(Int64)
DEFINE_NUMERIC_TRAITS(UInt8)
DEFINE_NUMERIC_TRAITS(UInt16)
DEFINE_NUMERIC_TRAITS(UInt32)
DEFINE_NUMERIC_TRAITS(UInt64)
DEFINE_NUMERIC_TRAITS(Float32)
DEFINE_NUMERIC_TRAITS(Float64)
DEFINE_NUMERIC_TRAITS(Char)
DEFINE_NUMERIC_TRAITS(Bool)

/* ==================== String Implementation ==================== */

int String_init(String *self) {
    self->ptr = NULL;
    return 0;
}

int String_from_n(String *self, const char *s, size_t n) {
    self->ptr      = xrealloc(NULL, sizeof(size_t) + n + 1);
    self->ptr->len = n;
    if (s && n > 0) {
        memcpy(self->ptr->buf, s, n);
    }
    self->ptr->buf[n] = '\0';
    return 0;
}

int String_from_c_str(String *self, const char *s) {
    if (s) {
        return String_from_n(self, s, strlen(s));
    }
    return String_from_n(self, NULL, 0);
}

void String_copy(String *dst, const String *src) {
    if (dst == src)
        return;
    if (src->ptr) {
        String_from_n(dst, src->ptr->buf, src->ptr->len);
    } else {
        dst->ptr = NULL;
    }
}

void String_move(String *dst, String *src) {
    if (dst == src)
        return;
    dst->ptr = src->ptr;
    src->ptr = NULL;
}

void String_drop(String *self) {
    if (self->ptr) {
        free(self->ptr);
        self->ptr = NULL;
    }
}

int String_cmp(String *a, String *b) {
    size_t la      = String_len(a);
    size_t lb      = String_len(b);
    size_t min_len = la < lb ? la : lb;
    if (min_len > 0) {
        int c = memcmp(String_c_str(a), String_c_str(b), min_len);
        if (c != 0)
            return c < 0 ? -1 : 1;
    }
    if (la < lb)
        return -1;
    if (la > lb)
        return 1;
    return 0;
}

bool String_eq(String *a, String *b) {
    if (String_len(a) != String_len(b))
        return false;
    return String_cmp(a, b) == 0;
}

UInt64 String_hash(String *self) {
    return fnv64(String_c_str(self), String_len(self), FNV_OFFSET_BASIS);
}

const char *String_c_str(const String *self) {
    return self->ptr ? self->ptr->buf : "";
}

size_t String_len(const String *self) {
    return self->ptr ? self->ptr->len : 0;
}

StringVec String_split(const String *self, const char *sep) {
    StringVec result = {0};
    StringVec_init(&result);

    const char *str     = String_c_str(self);
    size_t      len     = String_len(self);
    size_t      sep_len = strlen(sep);

    if (sep_len == 0) {
        String whole = {0};
        String_from_n(&whole, str, len);
        StringVec_pushmv(&result, &whole);
        return result;
    }

    size_t start = 0;
    for (size_t i = 0; i + sep_len <= len;) {
        if (memcmp(str + i, sep, sep_len) == 0) {
            String part = {0};
            String_from_n(&part, str + start, i - start);
            StringVec_pushmv(&result, &part);
            i += sep_len;
            start = i;
        } else {
            i++;
        }
    }
    String part = {0};
    String_from_n(&part, str + start, len - start);
    StringVec_pushmv(&result, &part);

    return result;
}

String String_join(const StringVec *vec, const char *sep) {
    size_t sep_len   = strlen(sep);
    size_t count     = vec->impl.len;
    size_t total_len = 0;

    for (size_t i = 0; i < count; i++) {
        const String *s = (const String *)((const char *)vec->impl.data + i * vec->impl.elem_size);
        total_len += String_len(s);
        if (i + 1 < count) {
            total_len += sep_len;
        }
    }

    String result = {0};
    String_from_n(&result, NULL, total_len);

    if (result.ptr) {
        char  *buf = result.ptr->buf;
        size_t pos = 0;
        for (size_t i = 0; i < count; i++) {
            const String *s    = (const String *)((const char *)vec->impl.data + i * vec->impl.elem_size);
            size_t        slen = String_len(s);
            if (slen > 0) {
                memcpy(buf + pos, String_c_str(s), slen);
            }
            pos += slen;
            if (i + 1 < count) {
                memcpy(buf + pos, sep, sep_len);
                pos += sep_len;
            }
        }
    }

    return result;
}

StringVec String_splitlines(const String *self) {
    StringVec result = {0};
    StringVec_init(&result);

    const char *str   = String_c_str(self);
    size_t      len   = String_len(self);
    size_t      start = 0;

    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\n') {
            size_t end = i;
            if (end > start && str[end - 1] == '\r') {
                end--;
            }
            String part;
            String_from_n(&part, str + start, end - start);
            StringVec_pushmv(&result, &part);
            start = i + 1;
        }
    }

    if (start < len) {
        String part = {0};
        String_from_n(&part, str + start, len - start);
        StringVec_pushmv(&result, &part);
    }

    return result;
}

String String_lstrip(const String *self) {
    const char *str   = String_c_str(self);
    size_t      len   = String_len(self);
    size_t      start = 0;
    while (start < len && isspace((unsigned char)str[start])) {
        start++;
    }
    String result = {0};
    String_from_n(&result, str + start, len - start);
    return result;
}

String String_rstrip(const String *self) {
    const char *str = String_c_str(self);
    size_t      len = String_len(self);
    size_t      end = len;
    while (end > 0 && isspace((unsigned char)str[end - 1])) {
        end--;
    }
    String result = {0};
    String_from_n(&result, str, end);
    return result;
}

String String_strip(const String *self) {
    const char *str   = String_c_str(self);
    size_t      len   = String_len(self);
    size_t      start = 0;
    while (start < len && isspace((unsigned char)str[start])) {
        start++;
    }
    size_t end = len;
    while (end > start && isspace((unsigned char)str[end - 1])) {
        end--;
    }
    String result = {0};
    String_from_n(&result, str + start, end - start);
    return result;
}

String String_removeprefix(const String *self, const char *prefix) {
    size_t plen = strlen(prefix);
    if (String_startswith(self, prefix)) {
        String result;
        String_from_n(&result, String_c_str(self) + plen, String_len(self) - plen);
        return result;
    }
    String result = {0};
    String_copy(&result, self);
    return result;
}

String String_removesuffix(const String *self, const char *suffix) {
    size_t slen = strlen(suffix);
    if (String_endswith(self, suffix)) {
        String result = {0};
        String_from_n(&result, String_c_str(self), String_len(self) - slen);
        return result;
    }
    String result = {0};
    String_copy(&result, self);
    return result;
}

Int64 String_find(const String *self, const char *sub) {
    size_t sublen = strlen(sub);
    if (sublen == 0)
        return 0;
    size_t len = String_len(self);
    if (sublen > len)
        return -1;
    const char *str = String_c_str(self);
    for (size_t i = 0; i + sublen <= len; i++) {
        if (memcmp(str + i, sub, sublen) == 0) {
            return (Int64)i;
        }
    }
    return -1;
}

size_t String_count(const String *self, const char *sub) {
    size_t sublen = strlen(sub);
    size_t len    = String_len(self);
    if (sublen == 0)
        return len + 1;
    if (sublen > len)
        return 0;
    const char *str = String_c_str(self);
    size_t      cnt = 0;
    for (size_t i = 0; i + sublen <= len;) {
        if (memcmp(str + i, sub, sublen) == 0) {
            cnt++;
            i += sublen;
        } else {
            i++;
        }
    }
    return cnt;
}

bool String_startswith(const String *self, const char *prefix) {
    size_t plen = strlen(prefix);
    size_t len  = String_len(self);
    if (plen > len)
        return false;
    return memcmp(String_c_str(self), prefix, plen) == 0;
}

bool String_endswith(const String *self, const char *suffix) {
    size_t slen = strlen(suffix);
    size_t len  = String_len(self);
    if (slen > len)
        return false;
    return memcmp(String_c_str(self) + len - slen, suffix, slen) == 0;
}

String String_lower(const String *self) {
    const char *str = String_c_str(self);
    size_t      len = String_len(self);
    String      result = {0};
    String_from_n(&result, NULL, len);
    if (result.ptr) {
        for (size_t i = 0; i < len; i++) {
            result.ptr->buf[i] = tolower((unsigned char)str[i]);
        }
    }
    return result;
}

String String_upper(const String *self) {
    const char *str = String_c_str(self);
    size_t      len = String_len(self);
    String      result = {0};
    String_from_n(&result, NULL, len);
    if (result.ptr) {
        for (size_t i = 0; i < len; i++) {
            result.ptr->buf[i] = toupper((unsigned char)str[i]);
        }
    }
    return result;
}

String String_capitalize(const String *self) {
    const char *str = String_c_str(self);
    size_t      len = String_len(self);
    String      result = {0};
    String_from_n(&result, NULL, len);
    if (result.ptr) {
        for (size_t i = 0; i < len; i++) {
            if (i == 0) {
                result.ptr->buf[i] = toupper((unsigned char)str[i]);
            } else {
                result.ptr->buf[i] = tolower((unsigned char)str[i]);
            }
        }
    }
    return result;
}

/* ==================== Deque Implementation ==================== */

static void *deque_block_at(Deque *self, size_t ring_idx) {
    size_t actual_idx = (self->block_front + ring_idx) % self->blocks.capacity;
    return ((void **)self->blocks.data)[actual_idx];
}

static void deque_grow_blocks(Deque *self) {
    size_t old_cap = self->blocks.capacity;
    size_t new_cap;
    if (old_cap == 0)
        new_cap = 4;
    else
        new_cap = old_cap * 2;
    vec_reserve(&self->blocks, new_cap);
    /* self->blocks.capacity may have grown beyond new_cap if vec_reserve doubled, but that's fine */
    if (self->block_front != 0) {
        size_t tail_count = old_cap - self->block_front;
        memmove(((void **)self->blocks.data) + (self->blocks.capacity - tail_count),
                ((void **)self->blocks.data) + self->block_front,
                tail_count * sizeof(void *));
        self->block_front = self->blocks.capacity - tail_count;
    }
}

static void deque_push_block_back(Deque *self) {
    if (self->block_count == self->blocks.capacity)
        deque_grow_blocks(self);
    size_t block_idx = (self->block_front + self->block_count) % self->blocks.capacity;
    void *block = xrealloc(NULL, self->elems_per_blk * self->elem_size);
    ((void **)self->blocks.data)[block_idx] = block;
    self->block_count++;
}

static void deque_push_block_front(Deque *self) {
    if (self->block_count == self->blocks.capacity)
        deque_grow_blocks(self);
    self->block_front = (self->block_front - 1 + self->blocks.capacity) % self->blocks.capacity;
    void *block = xrealloc(NULL, self->elems_per_blk * self->elem_size);
    ((void **)self->blocks.data)[self->block_front] = block;
    self->block_count++;
}

static void deque_pop_block_front(Deque *self) {
    free(((void **)self->blocks.data)[self->block_front]);
    self->block_front = (self->block_front + 1) % self->blocks.capacity;
    self->block_count--;
}

static void deque_pop_block_back(Deque *self) {
    size_t block_idx = (self->block_front + self->block_count - 1) % self->blocks.capacity;
    free(((void **)self->blocks.data)[block_idx]);
    self->block_count--;
}

int deque_init(Deque *self, size_t elem_size) {
    vec_init(&self->blocks, sizeof(void *));
    self->elems_per_blk = 4096 / elem_size;
    if (self->elems_per_blk < 16)
        self->elems_per_blk = 16;
    self->elem_size   = elem_size;
    self->len         = 0;
    self->front_offset = 0;
    self->block_front = 0;
    self->block_count = 0;
    deque_push_block_back(self);
    return 0;
}

void deque_clear(Deque *self, DropFn value_drop) {
    for (size_t i = 0; i < self->len; i++) {
        value_drop(deque_get(self, i));
    }
    for (size_t i = 0; i < self->block_count; i++) {
        size_t actual_idx = (self->block_front + i) % self->blocks.capacity;
        free(((void **)self->blocks.data)[actual_idx]);
    }
    self->block_count  = 0;
    self->block_front  = 0;
    self->len          = 0;
    self->front_offset = 0;
}

void deque_drop(Deque *self, DropFn value_drop) {
    deque_clear(self, value_drop);
    vec_drop(&self->blocks, NULL);
}

void deque_push_back_cp(Deque *self, const void *elem, CopyFn copy_fn) {
    size_t global_pos    = self->front_offset + self->len;
    size_t block_in_ring = global_pos / self->elems_per_blk;
    size_t off_in_blk    = global_pos % self->elems_per_blk;
    if (block_in_ring >= self->block_count)
        deque_push_block_back(self);
    void *block = deque_block_at(self, block_in_ring);
    void *dst   = (char *)block + off_in_blk * self->elem_size;
    copy_fn(dst, elem);
    self->len++;
}

void deque_push_back_mv(Deque *self, void *elem, MoveFn move_fn) {
    size_t global_pos    = self->front_offset + self->len;
    size_t block_in_ring = global_pos / self->elems_per_blk;
    size_t off_in_blk    = global_pos % self->elems_per_blk;
    if (block_in_ring >= self->block_count)
        deque_push_block_back(self);
    void *block = deque_block_at(self, block_in_ring);
    void *dst   = (char *)block + off_in_blk * self->elem_size;
    move_fn(dst, elem);
    self->len++;
}

void deque_push_front_cp(Deque *self, const void *elem, CopyFn copy_fn) {
    if (self->front_offset == 0) {
        deque_push_block_front(self);
        self->front_offset = self->elems_per_blk;
    }
    self->front_offset--;
    void *block = deque_block_at(self, 0);
    void *dst   = (char *)block + self->front_offset * self->elem_size;
    copy_fn(dst, elem);
    self->len++;
}

void deque_push_front_mv(Deque *self, void *elem, MoveFn move_fn) {
    if (self->front_offset == 0) {
        deque_push_block_front(self);
        self->front_offset = self->elems_per_blk;
    }
    self->front_offset--;
    void *block = deque_block_at(self, 0);
    void *dst   = (char *)block + self->front_offset * self->elem_size;
    move_fn(dst, elem);
    self->len++;
}

bool deque_pop_back(Deque *self, void *out) {
    if (self->len == 0)
        return false;
    size_t global_pos    = self->front_offset + self->len - 1;
    size_t block_in_ring = global_pos / self->elems_per_blk;
    size_t off_in_blk    = global_pos % self->elems_per_blk;
    void *block = deque_block_at(self, block_in_ring);
    void *src   = (char *)block + off_in_blk * self->elem_size;
    if (out)
        memcpy(out, src, self->elem_size);
    self->len--;
    if (self->len == 0) {
        for (size_t i = 0; i < self->block_count; i++) {
            size_t actual_idx = (self->block_front + i) % self->blocks.capacity;
            free(((void **)self->blocks.data)[actual_idx]);
        }
        self->block_count  = 0;
        self->block_front  = 0;
        self->front_offset = 0;
    } else if (off_in_blk == 0) {
        deque_pop_block_back(self);
    }
    return true;
}

bool deque_pop_front(Deque *self, void *out) {
    if (self->len == 0)
        return false;
    void *block = deque_block_at(self, 0);
    void *src   = (char *)block + self->front_offset * self->elem_size;
    if (out)
        memcpy(out, src, self->elem_size);
    self->front_offset++;
    self->len--;
    if (self->len == 0) {
        for (size_t i = 0; i < self->block_count; i++) {
            size_t actual_idx = (self->block_front + i) % self->blocks.capacity;
            free(((void **)self->blocks.data)[actual_idx]);
        }
        self->block_count  = 0;
        self->block_front  = 0;
        self->front_offset = 0;
    } else if (self->front_offset == self->elems_per_blk) {
        deque_pop_block_front(self);
        self->front_offset = 0;
    }
    return true;
}

void *deque_get(Deque *self, size_t index) {
    if (index >= self->len)
        return NULL;
    size_t global_pos    = self->front_offset + index;
    size_t block_in_ring = global_pos / self->elems_per_blk;
    size_t off_in_blk    = global_pos % self->elems_per_blk;
    void *block = deque_block_at(self, block_in_ring);
    return (char *)block + off_in_blk * self->elem_size;
}

void deque_set_cp(Deque *self, size_t index, const void *elem, DropFn value_drop, CopyFn copy_fn) {
    void *ptr = deque_get(self, index);
    if (ptr == NULL)
        return;
    value_drop(ptr);
    copy_fn(ptr, elem);
}

void deque_set_mv(Deque *self, size_t index, void *elem, DropFn value_drop, MoveFn move_fn) {
    void *ptr = deque_get(self, index);
    if (ptr == NULL)
        return;
    value_drop(ptr);
    move_fn(ptr, elem);
}

void *deque_front(Deque *self) {
    if (self->len == 0)
        return NULL;
    void *block = deque_block_at(self, 0);
    return (char *)block + self->front_offset * self->elem_size;
}

void *deque_back(Deque *self) {
    if (self->len == 0)
        return NULL;
    size_t global_pos    = self->front_offset + self->len - 1;
    size_t block_in_ring = global_pos / self->elems_per_blk;
    size_t off_in_blk    = global_pos % self->elems_per_blk;
    void *block = deque_block_at(self, block_in_ring);
    return (char *)block + off_in_blk * self->elem_size;
}

void deque_push_back_raw(Deque *self) {
    size_t global_pos    = self->front_offset + self->len;
    size_t block_in_ring = global_pos / self->elems_per_blk;
    if (block_in_ring >= self->block_count)
        deque_push_block_back(self);
    self->len++;
}

void deque_push_front_raw(Deque *self) {
    if (self->front_offset == 0) {
        deque_push_block_front(self);
        self->front_offset = self->elems_per_blk;
    }
    self->front_offset--;
    self->len++;
}

void deque_pop_back_raw(Deque *self) {
    size_t global_pos = self->front_offset + self->len - 1;
    size_t off_in_blk = global_pos % self->elems_per_blk;
    self->len--;
    if (self->len == 0) {
        for (size_t i = 0; i < self->block_count; i++) {
            size_t actual_idx = (self->block_front + i) % self->blocks.capacity;
            free(((void **)self->blocks.data)[actual_idx]);
        }
        self->block_count  = 0;
        self->block_front  = 0;
        self->front_offset = 0;
    } else if (off_in_blk == 0) {
        deque_pop_block_back(self);
    }
}

void deque_pop_front_raw(Deque *self) {
    self->front_offset++;
    self->len--;
    if (self->len == 0) {
        for (size_t i = 0; i < self->block_count; i++) {
            size_t actual_idx = (self->block_front + i) % self->blocks.capacity;
            free(((void **)self->blocks.data)[actual_idx]);
        }
        self->block_count  = 0;
        self->block_front  = 0;
        self->front_offset = 0;
    } else if (self->front_offset == self->elems_per_blk) {
        deque_pop_block_front(self);
        self->front_offset = 0;
    }
}

void deque_insert_cp(Deque *self, size_t index, const void *elem, CopyFn copy_fn) {
    if (index > self->len)
        return;
    if (index <= self->len / 2) {
        deque_push_front_raw(self);
        for (size_t i = 0; i < index; i++) {
            void *dst = deque_get(self, i);
            void *src = deque_get(self, i + 1);
            memcpy(dst, src, self->elem_size);
        }
    } else {
        deque_push_back_raw(self);
        for (size_t i = self->len - 1; i > index; i--) {
            void *dst = deque_get(self, i);
            void *src = deque_get(self, i - 1);
            memcpy(dst, src, self->elem_size);
        }
    }
    copy_fn(deque_get(self, index), elem);
}

void deque_insert_mv(Deque *self, size_t index, void *elem, MoveFn move_fn) {
    if (index > self->len)
        return;
    if (index <= self->len / 2) {
        deque_push_front_raw(self);
        for (size_t i = 0; i < index; i++) {
            void *dst = deque_get(self, i);
            void *src = deque_get(self, i + 1);
            memcpy(dst, src, self->elem_size);
        }
    } else {
        deque_push_back_raw(self);
        for (size_t i = self->len - 1; i > index; i--) {
            void *dst = deque_get(self, i);
            void *src = deque_get(self, i - 1);
            memcpy(dst, src, self->elem_size);
        }
    }
    move_fn(deque_get(self, index), elem);
}

void deque_remove(Deque *self, size_t index, void *out) {
    if (index >= self->len)
        return;
    if (out)
        memcpy(out, deque_get(self, index), self->elem_size);
    if (index <= self->len / 2) {
        for (size_t i = index; i > 0; i--) {
            void *dst = deque_get(self, i);
            void *src = deque_get(self, i - 1);
            memcpy(dst, src, self->elem_size);
        }
        deque_pop_front_raw(self);
    } else {
        for (size_t i = index; i < self->len - 1; i++) {
            void *dst = deque_get(self, i);
            void *src = deque_get(self, i + 1);
            memcpy(dst, src, self->elem_size);
        }
        deque_pop_back_raw(self);
    }
}

void deque_swap_element(Deque *self, size_t a, size_t b) {
    if (a == b || a >= self->len || b >= self->len)
        return;
    void *ptr_a = deque_get(self, a);
    void *ptr_b = deque_get(self, b);
    void *tmp   = xrealloc(NULL, self->elem_size);
    memcpy(tmp, ptr_a, self->elem_size);
    memcpy(ptr_a, ptr_b, self->elem_size);
    memcpy(ptr_b, tmp, self->elem_size);
    free(tmp);
}

void deque_swap(Deque *a, Deque *b) {
    Deque tmp = *a;
    *a       = *b;
    *b       = tmp;
}

void deque_reserve(Deque *self, size_t additional) {
    size_t total_needed  = self->front_offset + self->len + additional;
    size_t needed_blocks = (total_needed + self->elems_per_blk - 1) / self->elems_per_blk;
    while (self->block_count < needed_blocks)
        deque_push_block_back(self);
}

void deque_copy(Deque *dst, const Deque *src, CopyFn copy_fn) {
    if (dst == src)
        return;
    deque_init(dst, src->elem_size);
    for (size_t i = 0; i < src->len; i++) {
        deque_push_back_cp(dst, deque_get((Deque *)src, i), copy_fn);
    }
}

void deque_move(Deque *dst, Deque *src) {
    if (dst == src)
        return;
    vec_move(&dst->blocks, &src->blocks);
    dst->elems_per_blk  = src->elems_per_blk;
    dst->elem_size      = src->elem_size;
    dst->len            = src->len;
    dst->front_offset   = src->front_offset;
    dst->block_front    = src->block_front;
    dst->block_count    = src->block_count;
    src->len            = 0;
    src->front_offset   = 0;
    src->block_front    = 0;
    src->block_count    = 0;
}

bool deque_eq(Deque *a, Deque *b, EqFn eq_fn) {
    if (a->len != b->len)
        return false;
    for (size_t i = 0; i < a->len; i++) {
        if (!eq_fn(deque_get(a, i), deque_get(b, i)))
            return false;
    }
    return true;
}

int deque_cmp(Deque *a, Deque *b, CmpFn cmp_fn) {
    size_t min_len = a->len < b->len ? a->len : b->len;
    for (size_t i = 0; i < min_len; i++) {
        int c = cmp_fn(deque_get(a, i), deque_get(b, i));
        if (c != 0)
            return c;
    }
    if (a->len < b->len)
        return -1;
    if (a->len > b->len)
        return 1;
    return 0;
}

UInt64 deque_hash(Deque *self, HashFn hash_fn) {
    UInt64 hash = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < self->len; i++) {
        UInt64 elem_hash = hash_fn(deque_get(self, i));
        hash             = fnv64(&elem_hash, sizeof(UInt64), hash);
    }
    return hash;
}

size_t deque_len(const Deque *self) {
    return self->len;
}

bool deque_is_empty(const Deque *self) {
    return self->len == 0;
}

DequeIter deque_begin(Deque *self) {
    DequeIter iter;
    iter.deque = self;
    iter.pos   = self->front_offset;
    return iter;
}

DequeIter deque_end(Deque *self) {
    DequeIter iter;
    iter.deque = self;
    iter.pos   = self->front_offset + self->len;
    return iter;
}

DequeIter deque_last(Deque *self) {
    DequeIter iter;
    iter.deque = self;
    iter.pos   = self->front_offset + self->len - 1;
    return iter;
}

bool deque_iter_is_end(const DequeIter *iter) {
    return iter->pos == iter->deque->front_offset + iter->deque->len;
}

void deque_iter_next(DequeIter *iter) {
    iter->pos++;
}

void deque_iter_prev(DequeIter *iter) {
    iter->pos--;
}

void *deque_iter_get(DequeIter *iter) {
    size_t block_in_ring = iter->pos / iter->deque->elems_per_blk;
    size_t off_in_blk    = iter->pos % iter->deque->elems_per_blk;
    void *block = deque_block_at(iter->deque, block_in_ring);
    return (char *)block + off_in_blk * iter->deque->elem_size;
}

#endif /* CBASE_IMPLEMENTATION */


#include <stdio.h>
#include <stdlib.h>

/* ==================== Calendar Map Type ==================== */

CBASE_RBTMAP(IntCalendar, Int32, Int32)

/* ==================== MyCalendar Implementation ==================== */

typedef struct {
    IntCalendar map;
} MyCalendar;

MyCalendar *myCalendarCreate() {
    MyCalendar *obj = malloc(sizeof(MyCalendar));
    IntCalendar_init(&obj->map);
    return obj;
}

bool myCalendarBook(MyCalendar *obj, int startTime, int endTime) {
    Int32 start = startTime;
    Int32 end   = endTime;

    /* Find first entry with key >= start */
    RBTMapIter it = IntCalendar_lower_bound(&obj->map, &start);

    /* Check if the found event starts before our end → overlap */
    if (!IntCalendar_iter_is_end(&it)) {
        Int32 *key = IntCalendar_iter_key(&it);
        if (*key < end) {
            return false;
        }
    }

    /* Check the previous event's end > start → overlap */
    if (IntCalendar_iter_is_end(&it)) {
        it = IntCalendar_last(&obj->map);
    } else {
        IntCalendar_iter_prev(&it);
    }

    if (!IntCalendar_iter_is_end(&it)) {
        Int32 *val = IntCalendar_iter_value(&it);
        if (*val > start) {
            return false;
        }
    }

    IntCalendar_insertcp(&obj->map, &start, &end);
    return true;
}

void myCalendarFree(MyCalendar *obj) {
    IntCalendar_drop(&obj->map);
    free(obj);
}

/**
 * Your MyCalendar struct will be instantiated and called as such:
 * MyCalendar* obj = myCalendarCreate();
 * bool param_1 = myCalendarBook(obj, startTime, endTime);
 
 * myCalendarFree(obj);
*/