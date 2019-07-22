/**
 * \file mdblist.h
 * \author
 * \date
 *
 * This file includes the list operations.
 *
 \verbatim

    This file is part of mGNCS, a component for MiniGUI.

    Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Or,

    As this program is a library, any link to this program must follow
    GNU General Public License version 3 (GPLv3). If you cannot accept
    GPLv3, you need to be licensed from FMSoft.

    If you have got a commercial license of this program, please use it
    under the terms and conditions of the commercial license.

    For more information about the commercial license, please refer to
    <http://www.minigui.com/en/about/licensing-policy/>.

 \endverbatim
 */

#ifndef _NCS_DBLIST_H
#define _NCS_DBLIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct list_head
 * \brief The structure of double linked list.
 *
 *  - next\n
 *    The next pointer.
 *
 *  - prev\n
 *    The previous pointer.
 */
struct list_head {
    struct list_head *next, *prev;
};

/**
 * \var list_t
 * \brief The structure of double linked list.
 */
typedef struct list_head list_t;

#define LIST_HEAD_INIT(name) { &(name), &(name) }

/* define and init a list head */
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

/* init a list head */
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
 * \fn void __list_add(struct list_head * _new, \
                struct list_head * prev, \
                struct list_head * next)
 * \brief Insert a _new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head * _new,
    struct list_head * prev,
    struct list_head * next)
{
    next->prev = _new;
    _new->next = next;
    _new->prev = prev;
    prev->next = _new;
}

/**
 * \fn void list_add(struct list_head *_new, struct list_head *head) 
 * \brief Add a _new entry, insert a _new entry after the specified head.
 *        This is good for implementing stacks.
 * \param _new _new entry to be added
 * \param head list head to add it after
 *
 * Insert a _new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *_new, struct list_head *head)
{
    __list_add(_new, head, head->next);
}

/**
 * \fn void list_add_tail(struct list_head *_new, struct list_head *head)
 * \brief Add a _new entry, insert a _new entry before the specified head.
 *        This is useful for implementing queues.
 * \param _new _new entry to be added
 * \param head list head to add it before
 *
 * Insert a _new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *_new, struct list_head *head)
{
    __list_add(_new, head->prev, head);
}

/*
 * \fn  void __list_del(struct list_head * prev, \
                  struct list_head * next)
 * \brief Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev,
                  struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * \fn void list_del(struct list_head *entry)
 * \brief Deletes entry from list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

/**
 * \fn void list_del_init(struct list_head *entry)
 * \brief Deletes entry from list and reinitialize it.
 *
 * \param entry The element to delete from the list.
 */
static inline void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry); 
}

/**
 * \fn int list_empty(struct list_head *head)
 * \brief Tests whether a list is empty.
 *
 * \param head The list to test.
 */
static inline int list_empty(struct list_head *head)
{
    return head->next == head;
}

/**
 * \def list_entry(ptr, type, member)
 * \brief Get the struct for this entry.
 *
 * \param ptr The &struct list_head pointer.
 * \param type The type of the struct this is embedded in.
 * \param member The name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * \def list_for_each(pos, head)
 * \brief Iterate over a list.
 * \param pos The &struct list_head to use as a loop counter.
 * \param head The head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)
            
/**
 * \def list_for_each_safe(pos, n, head)    
 * \brief Iterate over a list safe against removal of list entry.
 * \param pos The &struct list_head to use as a loop counter.
 * \param n Another &struct list_head to use as temporary storage
 * \param head The head for your list.
 */
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)
        
/**
 * list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop counter.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)				\
            for (pos = list_entry((head)->next, typeof(*pos), member);  \
                 &pos->member != (head);                    \
                 pos = list_entry(pos->member.next, typeof(*pos), member))
        
        /**
         * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
         * @pos:    the type * to use as a loop counter.
         * @n:      another type * to use as temporary storage
         * @head:   the head for your list.
         * @member: the name of the list_struct within the struct.
         */
#define list_for_each_entry_safe(pos, n, head, member)			\
            for (pos = list_entry((head)->next, typeof(*pos), member),  \
                n = list_entry(pos->member.next, typeof(*pos), member); \
                 &pos->member != (head);                    \
                 pos = n, n = list_entry(n->member.next, typeof(*n), member))

/**
 * \def list_for_each_ex(pos, head, begin)
 * \brief Iterate over a list.
 * \param pos The &struct list_head to use as a loop counter.
 * \param head The head for your list.
 * \param begin The previous item of the begining item
 */
#define list_for_each_ex(pos, head, begin) \
    for (pos = (begin)->next; pos != (head); pos = (pos)->next)

/**
 * \def list_for_index(pos, i, head, index)    
 * \brief Iterate over a list for index.
 *
 * \param pos The &struct list_head to use as a loop counter.
 * \param i The index used to record current position.
 * \param head The head for your list.
 * \param index The maximum index for your list.
 */
#define list_for_index(pos, i, head, index) \
    for (pos = (head)->next, i=0; (pos != (head) && i < index); pos = pos->next,i++)


static inline int list_add_by_index(struct list_head *_new, struct list_head *head, int index)
{
    list_t *pos = head;
    int i = -1;

    if (index >= 0) {
        for (pos = (head)->next, i=0; (pos != (head) && i < index); pos = pos->next,i++);
    }
    list_add_tail (_new, pos);
    return i;
}


/* added for normal list operations */
#define GETBY_EX(func, head, entrytype, member, datatype, ret, equal_cond) \
        void* func (datatype data) \
        { \
            list_t *me; \
            entrytype *pdata; \
            int i = 0; \
            \
            list_for_each(me, head) { \
                pdata = list_entry(me, entrytype, member); \
                if (equal_cond) \
                    return ret; \
            } \
            return NULL; \
        }

#define GET_LIST_BY_DATA(func, head, entrytype, member, datamember, datatype) \
        GETBY_EX(func, head, entrytype, member, datatype, me, (pdata->datamember == data))

#define GET_ENTRY_BY_DATA(func, head, entrytype, member, datamember, datatype) \
        GETBY_EX(func, head, entrytype, member, datatype, pdata, (pdata->datamember == data))

/* for string */
#define GET_LIST_BY_STRING(func, head, entrytype, member, datamember, datatype) \
        GETBY_EX(func, head, entrytype, member, datatype, me, (strcmp(pdata->datamember,data)==0))

#define GET_ENTRY_BY_STRING(func, head, entrytype, member, datamember, datatype) \
        GETBY_EX(func, head, entrytype, member, datatype, pdata, (strcmp(pdata->datamember,data)==0))

/* for index */
#define GET_ENTRY_BY_INDEX(func, entrytype, member) \
        static inline entrytype* func (list_t *head, int index) \
        { \
            list_t *me; \
            int i = 0; \
            \
            list_for_each(me, head) { \
                if (i++ == index) \
                    return list_entry(me, entrytype, member); \
            } \
            return NULL; \
        }

#define GET_ENTRY_INDEX(func, entrytype, member) \
        static inline int func (list_t *head, entrytype* entry) \
        { \
            list_t *me; \
            int i = 0; \
            \
            list_for_each(me, head) { \
                if (entry == list_entry(me, entrytype, member)) \
                    return i; \
                i++; \
            } \
            return -1; \
        }

#ifdef __cplusplus
}
#endif

#endif /* _NCS_DBLIST_H */
