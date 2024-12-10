/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template <class T>
class DLinkedListSE : public DLinkedList<T>
{
public:
    DLinkedListSE(
        void (*removeData)(DLinkedList<T> *) = 0,
        bool (*itemEQ)(T &, T &) = 0) : DLinkedList<T>(removeData, itemEQ) {};

    DLinkedListSE(const DLinkedList<T> &list)
    {
        this->copyFrom(list);
    }

    void sort(int (*comparator)(T &, T &) = 0)
    {
        // TODO: implement this function
        //      - You should implement the merge sort algorithm
        if (this->size() <= 1)
            return;

        Node *sortedList = mergeSort(this->head->next, this->tail->prev, comparator);

        rebuildList(sortedList);
    };

    DLinkedList<T> toDLinkedList()
    {
        DLinkedList<T> result;
        typename DLinkedList<T>::Node *current = this->head->next;

        while (current != this->tail)
        {
            result.add(current->data);
            current = current->next;
        }

        return result;
    }

protected:
    using typename DLinkedList<T>::Node;
    static int compare(T &lhs, T &rhs, int (*comparator)(T &, T &) = 0)
    {
        if (comparator != 0)
            return comparator(lhs, rhs);
        else
        {
            if (lhs < rhs)
                return -1;
            else if (lhs > rhs)
                return +1;
            else
                return 0;
        }
    }

    Node *mergeSort(Node *start, Node *end, int (*comparator)(T &, T &))
    {
        if (start == nullptr || start == end || start->next == nullptr)
        {
            if (start != nullptr)
            {
                start->prev = start->next = nullptr;
            }
            return start;
        }

        Node *mid = getMiddle(start, end);
        Node *right = mergeSort(mid->next, end, comparator);
        Node *left = mergeSort(start, mid, comparator);

        return merge(left, right, comparator);
    }

    Node *getMiddle(Node *start, Node *end)
    {
        if (start == nullptr || start == end)
        {
            return start;
        }

        Node *slow = start;
        Node *fast = start;

        while (fast != nullptr && fast->next != nullptr && fast != end && fast->next != end)
        {
            fast = fast->next->next;
            slow = slow->next;
        }

        return slow;
    }

    Node *merge(Node *left, Node *right, int (*comparator)(T &, T &))
    {
        Node dummy;
        Node *current = &dummy;

        while (left && right)
        {
            if (compare(left->data, right->data, comparator) <= 0)
            {
                current->next = left;
                left->prev = current;
                left = left->next;
            }
            else
            {
                current->next = right;
                right->prev = current;
                right = right->next; // Fix: move right pointer, not left
            }
            current = current->next;
        }

        if (left)
            current->next = left;
        if (right)
            current->next = right;

        if (current->next)
            current->next->prev = current;

        return dummy.next;
    }

    void rebuildList(Node *sortedList)
    {
        if (!sortedList)
        {
            this->head->next = this->tail;
            this->tail->prev = this->head;
            this->count = 0;
            return;
        }

        this->head->next = sortedList;

        Node *current = this->head;
        while (current->next)
        {
            current->next->prev = current;
            current = current->next;
        }

        this->tail->prev = current;
        current->next = this->tail;

        this->count = 0;
        Node *temp = this->head->next;
        while (temp != this->tail)
        {
            this->count++;
            temp = temp->next;
        }
    }
};

#endif /* DLINKEDLISTSE_H */
