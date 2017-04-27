/* ***********************************************************************

  > File Name: list.h
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年04月25日 星期二 11时37分54秒

 ********************************************************************** */

#ifndef AUT_LIST_H
#define AUT_LIST_H

/*
 * @brief   参考linux内核中的list.h实现的一个精简版的双向循环链表
 * @notice  使用其中的某些接口还可以当做队列或者堆栈来使用
 */

#ifdef __cplusplus
extern "C" {

#define typeof decltype     /* 如果在C++代码中调用，需要支持decltype关键字 */
#endif /* end of __cplusplus */

#include <string.h>

struct list_head {
    struct list_head *prev, *next;
};

typedef unsigned char           __u8;
typedef unsigned short int      __u16;
typedef unsigned int            __u32;
#if __WORDSIZE == 64
typedef unsigned long int       __u64;
#else
typedef unsigned long long int  __u64;
#endif

#define barrier()   __asm__ __volatile__("": : :"memory")
#define offsetof(TYPE, MEMBER)  ((size_t)&((TYPE *)0)->MEMBER)

static inline
void __read_once_size(const volatile void *p, void *res, int size)
{
    switch (size) {
        case 1: *(__u8 *)res  = *(volatile __u8 *)p;  break;
        case 2: *(__u16 *)res = *(volatile __u16 *)p; break;
        case 4: *(__u32 *)res = *(volatile __u32 *)p; break;
        case 8: *(__u64 *)res = *(volatile __u64 *)p; break;
        default:
            barrier();
            memcpy((void *)res, (const void *)p, size);
            barrier();
    }
}

#define READ_ONCE(x)          \
({                      \
     union { typeof(x) __val; char __c[1]; } __u;   \
     __read_once_size(&(x), __u.__c, sizeof(x));    \
     __u.__val;     \
})

static inline
void __write_once_size(volatile void *p, void *res, int size)
{
    switch (size) {
        case 1: *(volatile __u8 *)p  = *(__u8 *)res;  break;
        case 2: *(volatile __u16 *)p = *(__u16 *)res; break;
        case 4: *(volatile __u32 *)p = *(__u32 *)res; break;
        case 8: *(volatile __u64 *)p = *(__u64 *)res; break;
        default:
            barrier();
            memcpy((void *)p, (const void *)res, size);
            barrier();
    }
}

#define WRITE_ONCE(x, val)  \
({                      \
     union { typeof(x) __val; char __c[1]; } __u =      \
        { .__val = ( typeof(x)) (val) };    \
     __write_once_size(&(x), __u.__c, sizeof(x));   \
     __u.__val;     \
})

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    WRITE_ONCE(list->next, list);
    list->prev = list;
}

static inline void __list_add(struct list_head *new_entry,
                struct list_head *prev,
                struct list_head *next)
{
    next->prev = new_entry;
    new_entry->next = next;
    new_entry->prev = prev;
    WRITE_ONCE(prev->next, new_entry);
}

/* 头插法 */
static inline void list_add(struct list_head *new_entry, struct list_head *head)
{
    __list_add(new_entry, head, head->next);
}

/* 尾插法 */
static inline void list_add_tail(struct list_head *new_entry, struct list_head *head)
{
    __list_add(new_entry, head->prev, head);
}

#define LIST_POISON1 ((void *) 0x100)
#define LIST_POISON2 ((void *) 0x200)

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    WRITE_ONCE(prev->next, next);
}

static inline void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

/* 删除结点 */
static inline void list_del(struct list_head *entry)
{
    __list_del_entry(entry);
    entry->next = (typeof(entry->next))LIST_POISON1;
    entry->prev = (typeof(entry->prev))LIST_POISON2;
}

/* 替换节点 */
static inline void list_replace(struct list_head *old, struct list_head *new_entry)
{
    new_entry->next = old->next;
    new_entry->next->prev = new_entry;
    new_entry->prev = old->prev;
    new_entry->prev->next = new_entry;
}

/* 替换并初始化替换后的节点 */
static inline void list_replace_init(struct list_head *old, struct list_head *new_entry)
{
    list_replace(old, new_entry);
    INIT_LIST_HEAD(old);
}

/* 删除并重新初始化节点 */
static inline void list_del_init(struct list_head *entry)
{
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

/* 将某个节点移动到表头 */
static inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del_entry(list);
    list_add(list, head);
}

/* 将某个节点移动到表尾 */
static inline void list_move_tail(struct list_head *list, struct list_head *head)
{
    __list_del_entry(list);
    list_add_tail(list, head);
}

/* 检测节点是否是最后一个节点 */
static inline int list_is_last(const struct list_head *list, const struct list_head *head)
{
    return list->next == head;
}

/* 检测是否是空链表 */
static inline int list_empty(const struct list_head *head)
{
    return READ_ONCE(head->next) == head;
}

/* 检测是否是空链表 */
static inline int list_empty_careful(const struct list_head *head)
{
    struct list_head *next = head->next;
    return (next == head) && (next == head->prev);
}

/* 把第一个节点移动到表尾 */
static inline void list_rotate_left(struct list_head *head)
{
    struct list_head *first;

    if (!list_empty(head)) {
        first = head->next;
        list_move_tail(first, head);
    }
}

/* 检测是否只有一个节点 */
static inline int list_is_singular(const struct list_head *head)
{
    return !list_empty(head) && (head->next == head->prev);
}

/* 把head链表的第一个节点到entry节点之间的所有节点拿出来放在list链表中 */
static inline void __list_cut_position(struct list_head *list,
                    struct list_head *head, 
                    struct list_head *entry)
{
    struct list_head *new_entry_first = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = new_entry_first;
    new_entry_first->prev = head;
}

/* __list_cut_position的调用接口，增加了安全检查 */
static inline void list_cut_position(struct list_head *list,
                    struct list_head *head,
                    struct list_head *entry)
{
    if (list_empty(head))                       /* 空表 */
    {
        return;
    }
    if (list_is_singular(head) &&
        (head->next != entry && head != entry)) /* head是单节点并且entry不在head链表中 */
        return;
    if (entry == head)                          /* 要切的部分为空表 */
        INIT_LIST_HEAD(list);
    else
        __list_cut_position(list, head, entry);
}

/* 把list合并到prev和next之间 */
static inline void __list_splice(const struct list_head *list,
                    struct list_head *prev,
                    struct list_head *next)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

/* 把list合并到head表的头部 */
static inline void list_splice(const struct list_head *list, struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head, head->next);
}

/* 把list合并到head表的尾部 */
static inline void list_splice_tail(struct list_head *list, struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head->prev, head);
}

/* 把list合并到head表的头部并初始化list */
static inline void list_splice_init(struct list_head *list, struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
        INIT_LIST_HEAD(list);
    }
}

/* 把list合并到head表的尾部并初始化list */
static inline void list_splice_tail_init(struct list_head *list, struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head->prev, head);
        INIT_LIST_HEAD(list);
    }
}

#define container_of(ptr, type, member) ({      \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

/* 通过ptr指针获取所属节点的指针，ptr指向member，type是包含member的数据结构 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

/* 获取第一个元素的指针 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

/* 获取最后一个元素的指针 */
#define list_last_entry(ptr, type, member) \
    list_entry((ptr)->prev, type, member)

/* 获取第一个元素的指针，链表为空时返回NULL */
#define list_first_entry_or_null(ptr, type, member) ({ \
    struct list_head *head__ = (ptr);   \
    struct list_head *pos__ = READ_ONCE(head__->next); \
    pos__ != head__ ? list_entry(pos__, type, member) : NULL;   \
})

/* 获取pos的下一个节点的指针 */
#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, typeof(*(pos)), member)

/* 获取pos的上一个节点的指针 */
#define list_prev_entry(pos, member) \
    list_entry((pos)->member.prev, typeof(*(pos)), member)

/* 正序遍历，通过list_head指针 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/* 逆序遍历，通过list_head指针 */
#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

/* 正序遍历，缓存当前访问节点的下一个节点，防止链表断裂 */
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

/* 逆序遍历，缓存当前访问节点的下一个节点，防止链表断裂 */
#define list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
         pos != (head);     \
         pos = n, n = pos->next)

/* 正序遍历，通过包含list_head的数据结构指针 */
#define list_for_each_entry(pos, head, member) \
    for (pos = list_first_entry(head, typeof(*pos), member);    \
        &pos->member != (head);     \
        pos = list_next_entry(pos, member))

/* 逆序遍历，通过包含list_head的数据结构指针 */
#define list_for_each_entry_reverse(pos, head, member) \
    for (pos = list_last_entry(head, typeof(*pos), member); \
        &pos->member != (head); \
        pos = list_prev_entry(pos, member))

/* 正序从pos节点的下一个节点开始遍历 */
#define list_for_each_entry_continue(pos, head, member) 		\
	for (pos = list_next_entry(pos, member);			\
	     &pos->member != (head);					\
	     pos = list_next_entry(pos, member))

/* 逆序从pos节点的下一个节点开始遍历 */
#define list_for_each_entry_continue_reverse(pos, head, member)		\
	for (pos = list_prev_entry(pos, member);			\
	     &pos->member != (head);					\
	     pos = list_prev_entry(pos, member))

/* 从pos节点开始遍历 */
#define list_for_each_entry_from(pos, head, member) 			\
	for (; &pos->member != (head);					\
	     pos = list_next_entry(pos, member))

/* 带缓存的正序遍历 */
#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_first_entry(head, typeof(*pos), member),	\
		 n = list_next_entry(pos, member);			\
	     &pos->member != (head); 					\
	     pos = n, n = list_next_entry(n, member))

/* 带缓存的正序遍历，从pos节点的下一个节点开始 */
#define list_for_each_entry_safe_continue(pos, n, head, member) 		\
	for (pos = list_next_entry(pos, member), 				\
		 n = list_next_entry(pos, member);				\
	     &pos->member != (head);						\
	     pos = n, n = list_next_entry(n, member))

/* 带缓存的正序遍历，从pos节点开始 */
#define list_for_each_entry_safe_from(pos, n, head, member) 			\
	for (n = list_next_entry(pos, member);					\
	     &pos->member != (head);						\
	     pos = n, n = list_next_entry(n, member))

/* 带缓存的逆序遍历，从pos节点的下一个节点开始 */
#define list_for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = list_last_entry(head, typeof(*pos), member),		\
		 n = list_prev_entry(pos, member);			\
	     &pos->member != (head); 					\
	     pos = n, n = list_prev_entry(n, member))

#define list_safe_reset_next(pos, n, member)				\
    n = list_next_entry(pos, member)

#define list_prepare_entry(pos, head, member) \
    ((pos) ? : list_entry(head, typeof(*pos), member))

#ifdef __cplusplus
#undef typeof

}
#endif  /* end of __cplusplus */

#endif  /* end of AUT_LIST_H */
