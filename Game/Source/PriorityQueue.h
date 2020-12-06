#pragma once
#include <List>
#include "DynArray.h"

template <class T>
class IComparer {
public:
	//virtual ~IComparer(){}
	virtual int Compare(T a, T b) = 0;
};

template <class T>
class PriorityQueue
{
protected:
	DynArray<T> innerList;
	IComparer<T>*  mComparer;

public:
	/*PriorityQueue()
	{
		//mComparer = Comparer<T>.Default;
	}*/

	PriorityQueue(IComparer<T> * comparer)
	{
		mComparer = comparer;
	}

	PriorityQueue(IComparer<T> * comparer, int capacity)
	{
		mComparer = comparer;
		innerList.memCapacity = capacity;
	}

protected:
	void SwitchElements(int i, int j)
	{
		T h = innerList[i];
		innerList[i] = innerList[j];
		innerList[j] = h;
	}

	int OnCompare(int i, int j)
	{
		return mComparer->Compare(innerList[i], innerList[j]);
	}

	/// <summary>
	/// Push an object onto the PQ
	/// </summary>
	/// <param name="O">The new object</param>
	/// <returns>The index in the list where the object is _now_. This will change when objects are taken from or put onto the PQ.</returns>
public:
	int Push(T item)
	{
		int p = innerList.Count();
		int p2;
		innerList.PushBack(item);
		//innerList.Add(item); // E[p] = O
		do
		{
			if (p == 0)
				break;
			p2 = (p - 1) / 2; //getting the parent index of a binnary heap (p = parent index) (p2 = left child index)
			if (OnCompare(p, p2) < 0)
			{
				SwitchElements(p, p2);
				p = p2;
			}
			else
				break;
		} while (true);
		return p;
	}

	/// <summary>
	/// Get the smallest object and remove it.
	/// </summary>
	/// <returns>The smallest object</returns>
	T Pop()
	{
		T result = innerList[0];
		int p = 0, p1, p2, pn;
		//innerList[0] = innerList[innerList.Count() - 1];
		innerList.Pop(innerList[0]);
		do
		{
			pn = p;
			p1 = 2 * p + 1;
			p2 = 2 * p + 2;
			if (innerList.Count() > p1 && OnCompare(p, p1) > 0) // links kleiner
				p = p1;
			if (innerList.Count() > p2 && OnCompare(p, p2) > 0) // rechts noch kleiner
				p = p2;

			if (p == pn)
				break;
			SwitchElements(p, pn);
		} while (true);

		return result;
	}

	/// <summary>
	/// Notify the PQ that the object at position i has changed
	/// and the PQ needs to restore order.
	/// Since you dont have access to any indexes (except by using the
	/// explicit IList.this) you should not call this function without knowing exactly
	/// what you do.
	/// </summary>
	/// <param name="i">The index of the changed object.</param>
	void Update(int i)
	{
		int p = i, pn;
		int p1, p2;
		do	// aufsteigen
		{
			if (p == 0)
				break;
			p2 = (p - 1) / 2;
			if (OnCompare(p, p2) < 0)
			{
				SwitchElements(p, p2);
				p = p2;
			}
			else
				break;
		} while (true);
		if (p < i)
			return;
		do	   // absteigen
		{
			pn = p;
			p1 = 2 * p + 1;
			p2 = 2 * p + 2;
			if (innerList.Count() > p1 && OnCompare(p, p1) > 0) // links kleiner
				p = p1;
			if (innerList.Count() > p2 && OnCompare(p, p2) > 0) // rechts noch kleiner
				p = p2;

			if (p == pn)
				break;
			SwitchElements(p, pn);
		} while (true);
	}

	/// <summary>
	/// Get the smallest object without removing it.
	/// </summary>
	/// <returns>The smallest object</returns>
	T Peek()
	{
		if (innerList.Count() > 0)
			return innerList[0];
		return default(T);
	}

	void Clear()
	{
		innerList.Clear();
	}

	int Count()
	{
		return innerList.Count();
	}

	void RemoveLocation(T item)
	{
		int index = -1;
		for (int i = 0; i < innerList.Count(); i++)
		{

			if (mComparer->Compare(innerList[i], item) == 0)
				index = i;
		}

		if (index != -1)
			innerList.RemoveAt(index);
	}

	/*//TODO: S'ha de cridar Update si canvia el valor en el index!!!
	T& operator[] (int index)
	{
		return innerList[index];
	}*/

	/*void operator[] = (T& value)
	{
		innerList[index] = value;
		Update(index);
	}*/

	/*T operator[] (int index)
	{
		set
		{
		}
	}*/
};