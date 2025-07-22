// System compatible bidirectional intrusive list.

#ifndef RPLIST_H
#define RPLIST_H

#include <exec/types.h>

template <class T> class RpNode
{
	RpNode<T> *succ;
	RpNode<T> *pred;

	public:

	T* next() { return (T*)(succ->succ ? succ : NULL); }
	T* prev() { return (T*)(pred->pred ? pred : NULL); }

	void insertAfter(RpNode<T> *node) 
	{
		succ = node->succ;
		pred = node;
		node->succ->pred = this;
		node->succ = this;
	}

	void insertBefore(RpNode<T> *node)
	{
		pred = node->pred;
		succ = node;
		node->pred->succ = this;
		node->pred = this;
	}

	void remove()
	{
		pred->succ = succ;
		succ->pred = pred;
	}
};


template <class T> class RpList
{
	T *head;
	T *tail;
	T *tailpred;

	public:

	RpList() { head = (T*)&tail; tail = NULL; tailpred = (T*)&head; }
	bool isEmpty() { return (first() ? FALSE : TRUE); }
	void addHead(T *node) { node->insertAfter((T*)&head); }
	void addTail(T *node) { node->insertBefore((T*)&tail); }
	T* first() { return (head == (T*)&tail) ? NULL : head; }
	T* last() { return (tailpred == (T*)&head) ? NULL : tailpred; }
	T* remHead() { T *node = first(); if (node) node->remove(); return node; }
	T* remTail() { T *node = last(); if (node) node->remove(); return node; }
};


#endif  // RPLIST_H

