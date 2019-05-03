#include "CBofCB.h"
#include <iostream>
#include <stdexcept>

using namespace std;


/*
This is the default constructor for the CBofCB class. It should
initialize all of the data members and set up a single inner circular
buffer that holds 10 int values. Note that m_buffers is an array of
pointers to InnerCB. It is NOT a dynamically allocated array. You do
need to initialize the array of pointers to NULL.
*/
CBofCB::CBofCB() {
	m_obSize = 1;
	m_oldest = 0;
	m_newest = 0;
	for (int i = 0; i < m_obCapacity; i++) {
		m_buffers[i] = nullptr;
	}
	m_buffers[0] = new InnerCB();
}

/*
This is the copy constructor for the CBofCB class. You have to make 
a complete copy of the other object. Make good use of the InnerCB copy
constructor. The admonition to not use the CBofCB assignment operator to
implement this copy constructor also holds here.
*/
CBofCB::CBofCB(const CBofCB& other) {
	m_obSize = other.m_obSize;
	m_oldest = other.m_oldest;
	m_newest = other.m_newest;
	for (int i = 0; i < m_obCapacity; i++) {
		if (other.m_buffers[i] == nullptr) {
			m_buffers[i] = nullptr;
		}
		else {
			m_buffers[i] = new InnerCB(*other.m_buffers[i]);
		}
	}
}

/*
This is the destructor. You must deallocate space used by the InnerCB
objects that the array of pointers in m_buffers points to. Remember that
you don't do that by calling the InnerCB destructor explicitly, because you
never call destructors explicitly.
*/
CBofCB::~CBofCB() {
	for (int i = 0; i < m_obSize; i++) {
		int index = (m_oldest + i) % m_obCapacity;
		if (m_buffers[index] == nullptr) {
			cout << "Trying to delete a second time!" << endl;
		}
		else {
			delete this->m_buffers[index];
		}	
	}
}

/*
This member function adds the value in the data parameter to the data structure.
Recall from the discussion above that you always add to the newest InnerCB.
You may have to create a new InnerCB if the current newest InnerCB is full.
This new newest array should have twice the capacity of the previous newest array.
If this cannot be done because the outer circular buffer is also full, then just
throw an overflow_error exception.
*/
void CBofCB::enqueue(int data) {
	if (isFull()) {
		throw overflow_error("**Overflow ERROR**");
	}
	else if (m_buffers[m_newest]->isFull()) {
		int new_Capacity = m_buffers[m_newest]->capacity() * 2;
		m_newest = (m_newest + 1) % m_obCapacity;
		m_buffers[m_newest] = new InnerCB(new_Capacity);
		m_buffers[m_newest]->enqueue(data);
		m_obSize++;
	}
	else {
		m_buffers[m_newest]->enqueue(data);
	}
}

/*
This function removes and returns the oldest value stored in the data structure.
If the entire data structure is empty, then just throw an underflow_error exception.
Recall from above that the oldest value is stored in the oldest InnerCB. If by removing
this oldest item causes the oldest InnerCB to become empty, then you must deallocate
this InnerCB and remove it from the outer circular buffer. However, if the empty buffer
is the only InnerCB in the whole data structure, then do not deallocate it. We want to
always have at least one InnerCB in place.
*/
int CBofCB::dequeue() {
	int temp;
	if (isEmpty()) {
		throw underflow_error("**Underflow ERROR**");
	}
	else {
		temp = m_buffers[m_oldest]->dequeue();
		if (m_buffers[m_oldest]->isEmpty() && m_obSize != 1) {
			delete this->m_buffers[m_oldest];
			m_buffers[m_oldest] = nullptr;
			m_oldest = (m_oldest + 1) % m_obCapacity;
			m_obSize--;
		}
	}
	return temp;
}

/*
Returns true if it is not possible to add any more items to the data structure.
This would occur if every entry of m_buffers is already pointing to an InnerCB
and the newest InnerCB is full. (This also implies that all except the oldest
InnerCB are full, too.)
*/
bool CBofCB::isFull() {
	return (m_obSize == 7 && m_buffers[m_newest]->isFull());
}

/*
Returns true if there are no items stored in the data structure anywhere.
*/
bool CBofCB::isEmpty() {
	return (m_buffers[m_newest]->isEmpty());
}

/*
Returns the number of items stored in the data structure. Note that this is
not the number of InnerCB objects that you have pointed to by pointers in the
m_buffers array. It is the total number of int values you have stored in all of
the InnerCB objects.
*/
int CBofCB::size() {
	int sum = 0;
	for (int i = 0; i < m_obCapacity; i++) {
		if (m_buffers[i] != nullptr) {
			sum += m_buffers[i]->size();
		}
	}
	return sum;
}

/*
This is the overloaded assignment operator for the CBofCB class. Read the reminders
about assignment operators written in the specifications of the assignment operator
for the InnerCB class. They also apply here.
Before the assignment, the left hand side(LHS) and the right hand side (RHS) might not
have pointers to InnerCBs of the same capacity. However, after the assignment, the LHS
should be an exact duplicate of the RHS. They should have the same value for m_obSize,
each allocated InnerCB should have the same capacity and be referenced by the same
location in m_buffers, and each InnerCB on the RHS should have an exact duplicate on the LHS.
*/
const CBofCB& CBofCB::operator=(const CBofCB& rhs) {
	
	InnerCB *temp[m_obCapacity];
	for (int i = 0; i < m_obCapacity; i++) {
		if (rhs.m_buffers[i] == nullptr) {
			temp[i] = nullptr;
		}
		else {
			temp[i] = new InnerCB(*rhs.m_buffers[i]);
		}
	}

	for (int i = 0; i < m_obCapacity; i++) {
		if (m_buffers[i] != nullptr) {
			delete this->m_buffers[i];
		}
	}

	m_obSize = rhs.m_obSize;
	m_oldest = rhs.m_oldest;
	m_newest = rhs.m_newest;


	for (int i = 0; i < m_obCapacity; i++) {
		m_buffers[i] = temp[i];
	}

	return *this;
}

/*
As before, this is a debugging function that prints out the contents of the entire data
structure. Make good use of InnerCB::dump(). See sample output below for suggested format.
*/
void CBofCB::dump() {
	cout << "Outer Circular buffer dump(), m_obSize = " << m_obSize << ":" << endl;
	for (int i = 0; i < m_obSize; i++) {
		int index = (m_oldest + i) % m_obCapacity;
		cout << "[" << index << "] ";
		m_buffers[index]->dump();
	}
}