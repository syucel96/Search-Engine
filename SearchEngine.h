#ifndef _SearchEngine_h
#define _SearchEngine_h

#include <iostream>
#include <string>
using namespace std;

template <class Object>//incompletely declared so the classes that the compiler knows that they are classes beforehand.
class List;

template <class Object>
class Iterator;

template <class Object>
class Node
{	//constructor
	Node(const Object & theElement = Object(), Node *n = NULL, Node *b = NULL) :element(theElement), next(n), below(b) {}
	bool larger(const string &s)
	{
		if (s[0] < 58 && s[0] > 47)//integer
		{
			if (stoi(s) > stoi(element))
				return true;
			else
				return false;
		}
		else//string, check for alphabetical order, returns true if the word comes later in the dictionary
		{
			string t1 = s;
			string t2 = element;
			for(int k=0;k<t1.length();k++)//Convert to upper for sorting
				t1[k]=toupper(t1[k]);
			for (int l = 0; l<t2.length(); l++)
				t2[l]=toupper(t2[l]);
			int length;//Length is equal to the length of the shorter word to avoid surpassing the length of the other inside the fo loop
			if (t2.length() < t1.length())
				length = t2.length();
			else
				length = t1.length();
			for (int i = 0; i < length; i++)
			{
				if (t1[i] > t2[i])//The first unidentical letter will give us the result
					return true;
				else if (t2[i] > t1[i])
					return false;
			}
			return s.length()<element.length();//If the elements thus far are the same the shorter word comes before
		}
	}
	Object element;
	Node *next;
	Node *below; //points down. Necessary of attaching a list of document numbers to each word without using a list of lists.
	//The classes below can access all functions
	friend class List<Object>;
	friend class Iterator<Object>;
};
template <class Object>
class Iterator
{
public:
	Iterator() :current(NULL) {}
	bool isPastEnd() const
	{return current == NULL;}
	void advance()
	{
		if (!isPastEnd())
			current = current->next;
	}
	void gounder()//advance the iterator using the Node *below pointer in the Node
	{
		if (!isPastEnd())
			current = current->below;
	}
	const Object &retrieve() const
	{//Retrieves the element of the node
		if (isPastEnd())
			throw "Item not found.";
		return current->element;
	}
private:
	Node<Object> *current;
	Iterator(Node<Object> *thenode):current(thenode){}
	friend class List<Object>;
};

template <class Object>
class List
{
public:
	List() :header(new Node<Object>) {}
	bool isEmpty() const
	{
		return header->next == NULL;
	}
	bool goesBelow() const//checks if a node has other nodes attached to it underneath
	{
		if(!isEmpty())
			return header->next->below != NULL;
		return false;
	}
	void insert(const Object &x, const Iterator<Object> &p)
	{
		if(p.current!=NULL)
		{
			p.current->next = new Node<Object>(x, p.current->next);//Add the object next to the given node
		}
		return;
	}
	void insertBelow(const string &x, const Iterator<Object> &p)
	{
		if (p.current != NULL)
		{
			p.current->below = new Node<Object>(x, NULL, p.current->below);//Add the object below the given node
		}
		return;
	}
	Iterator<Object> find(const Object&x)const
	{
		Node<Object> *itr = header->next;
		while (itr != NULL && x!=itr->element)//Check until the item is found or the list is exhausted.
		{
			itr = itr->next;
		}
		return Iterator<Object>(itr);
	}
	Iterator<Object> findprevious(const Object&x) const
	{//This function is not a search function, it will return an iterator pointing to the last object if the item does not exist
		Node<Object> *itr = header;
		while (itr->next != NULL && itr->next->element != x)
			itr = itr->next;
		return Iterator<Object>(itr);
	}
	Iterator<Object> findplaces(const string &s)const//Find a suitable place for a string so that the list remains sorted
	{
		Node<Object> *itr = header;
		while (itr->next != NULL && itr->next->larger(s))//Returns the element before the larger element, returns the last element if it's larger than every element in the list.
		{
			itr = itr->next;
		}
		return Iterator<Object>(itr);
	}
	Iterator<Object> findvplaces(const string &s, const Iterator<Object> &p)const//Find a suitable place for a string so that the vertical list remains sorted
	{
		Node<Object> *itr = p.current;
		while (itr->below != NULL && itr->below->larger(s))//Returns the element before the larger element, returns the last element if it's larger than every element in the list.
		{
			itr = itr->below;
		}
		return Iterator<Object>(itr);
	}
	void remove(const Object &x)
	{
		Iterator<Object> p = findprevious(x);
		if (p.current->next != NULL)
		{
			Node<Object> *oldNode = p.current->next;
			if (goesBelow())
			{
				while (oldNode->below != NULL)
					removeBottom(oldNode->element);
			}
			p.current->next = p.current->next->next;	//bypass
			delete oldNode;	//delete to avoid memory leaks
		}
		return;
	}
	void removeBottom(const Object &x)
	{//Removes the nodes below the node that contains the given object
		Iterator<Object> p = findprevious(x);
		if (p.current->next != NULL && goesBelow())
		{
			p.current = p.current->next;
			Node<Object> *oldNode = p.current->below;
			p.current->below = p.current->below->below;	//bypass
			delete oldNode;	//delete to avoid memory leaks
		}
		return;
	}
	List<Object> getBottom(const Iterator<Object> &p)const
	{
		List<Object> bot;
		if (goesBelow())
		{
			Iterator<Object> right = p;
			right.gounder();
			Iterator<Object> left = bot.zeroth();
			for (; !right.isPastEnd(); right.gounder(), left.advance())
			{
				bot.insert(right.retrieve(), left);
			}
		}
		return bot;
	}
	const List<Object> &intersect(const List<Object> &other)//function that changes the first list to the intersection of the two
	{
		if (!isEmpty())//makes sure the list is not empty to avoid unnecessary computation
		{
			Iterator<Object> itr = first();
			while (!itr.isPastEnd())//check every item in the first list
			{
				if (other.find(itr.retrieve()).isPastEnd())//if the item does not exist in the second list, remove it.
				{
					Iterator<Object> remover = itr;//created to be able to continue using itr as the iterator
					itr.advance();
					remove(remover.retrieve());
				}
				else
					itr.advance();
			}
		}
		return *this;
	}
	Iterator<Object> zeroth() const
	{
		return Iterator<Object>(header);
	}
	Iterator<Object> first() const
	{
		return Iterator<Object>(header->next);
	}
	void makeEmpty()
	{
		while (!isEmpty())
			remove(first().retrieve());
	}
	const List<Object> &operator =(const List<Object> & rhs)
	{
		makeEmpty();	//Clean up
		bool check = rhs.goesBelow();
		Iterator<Object> right = rhs.first();	// Initialize iterators
		Iterator<Object> left = zeroth();
		for (; !right.isPastEnd(); right.advance(), left.advance())
		{
			insert(right.retrieve(), left);
			if (check)
			{
				Iterator<Object> up = left;
				up.advance();
				up.gounder();
				Iterator<Object> down = right;
				down.gounder();
				while (!down.isPastEnd())//Loop to insert the list connected below each element
				{
					insertBelow(down.retrieve(), up);
					up.gounder();
					down.gounder();
				}
			}
		}	//Insertion loop
		return *this;
	}
	List(const List<Object> &rhs)
	{
		header = new Node<Object>;
		*this = rhs; //Deep Copy
	}
	~List()
	{
		makeEmpty(); //Get rid of all list nodes
		delete header;
	}
private:
	Node<Object> *header;
};
#endif //!SearchEngine