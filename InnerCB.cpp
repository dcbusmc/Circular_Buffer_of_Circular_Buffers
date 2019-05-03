#include "InnerCB.h"
#include <iostream>
#include <stdexcept>

using namespace std;

/*
This is the constructor for the InnerCB class. It should
initialize the data members and allocate memory to hold n
int values. The member m_buffer should point to this memory.
The default value of n is 10. (Yes, that's a magic number.)
*/
InnerCB::InnerCB(int n) {
	m_buffer = new int[n];
	m_start = 0;
	m_end = 0;
	m_size = 0;
	m_capacity = n;
}

/*
This is the copy constructor for the InnerCB class.
The copy constructor must create a complete copy of the
InnerCB object other. The new copy must have separately
allocated memory for the circular buffer of int. Do not
use the assignment operator to implement the copy constructor.
Just don't. No really, it's a bad idea.
*/

InnerCB::InnerCB(const InnerCB& other) {
	m_buffer = new int[other.m_capacity];
	m_start = other.m_start;
	m_end = other.m_end;
	m_size = other.m_size;
	m_capacity = other.m_capacity;
	for (int i = 0; i < m_capacity; i++) {
		m_buffer[i] = other.m_buffer[i];
	}
}


/*
This is the destructor for the InnerCB class. You must
deallocate memory. Remember that you should never call
a destructor explicitly. Well, almost never. If you have
never heard of "placement new" in C++, then don't ever
call the destructor explicitly.
*/
InnerCB::~InnerCB() {
	if (m_buffer == nullptr) {
		cout << "Trying to delete a second time!" << endl;
	}
	else {
		delete[] m_buffer;
	}
}

/*
This function should add the value in the data parameter to
the circular buffer. Remember to wrap around. If the buffer
is full, then throw an overflow_error exception. This exception
is defined in stdexcept so #include <stdexcept> at the top.
*/
void InnerCB::enqueue(int data) {
	if (isFull()) {
		throw overflow_error("**Overflow ERROR**");
	}
	else if (isEmpty()){
		m_buffer[m_end] = data;		
		m_size++;
	}
	else {
		m_end = (m_end + 1) % m_capacity;
		m_buffer[m_end] = data;
		m_size++;
	}
}

/*
This function should remove the oldest item in the circular
buffer and return that value. If the buffer is empty, then
throw an underflow_error exception. This is also defined
in stdexcept.
*/
int InnerCB::dequeue() {
	int temp;
	if (isEmpty()) {
		throw underflow_error("**Underflow ERROR**");
	}
	else {
		temp = m_buffer[m_start];
		m_start = (m_start + 1) % m_capacity;
		m_size--;
	}
	return temp;
}

/*
This function should return true if the circular buffer is full.
*/
bool InnerCB::isFull() {
	return (m_size == m_capacity);
}

/*
This function should return true if the circular buffer is empty.
*/
bool InnerCB::isEmpty() {
	return (m_size == 0);
}

/*
This function should return the number of int values that can
be stored in the amount of space allocated in the array of int
that m_buffer points to. I.e., it's the length of the array of
the circular buffer.
*/
int InnerCB::capacity() {
	return m_capacity;
}

/*
Returns the number of items stored in the circular buffer.
*/
int InnerCB::size() {
	return m_size;
}

/*
This is the overloaded assignment operator. If you forgot what that means,
crack open your C++ textbook again. Remember to check for self-assignment.
Remember to deallocate space of the host object. Remember to allocate new
space. The InnerCB objects on the left hand side (LHS) and right hand side
of the assignment (RHS) are not required to have the same capacity before
the assignment. After the assignment the LHS should be an exact duplicate
of the RHS with the same capacity, and have the items stored in the exact
same locations of m_buffer. You can't use the copy constructor to implement
the assignment operator. That would create a third object, which isn't
what you want. Really, that doesn't work. If you don't "remember" why you
have to check for self-assignment, then go read up on assignment operators.
It will actually save you time.
*/
const InnerCB& InnerCB::operator=(const InnerCB& rhs) {

	int *temp = new int[rhs.m_capacity];

	for (int i = 0; i < rhs.m_capacity; i++) {
		temp[i] = rhs.m_buffer[i];
	}

	delete[] m_buffer;

	m_buffer = temp;
	m_capacity = rhs.m_capacity;
	m_end = rhs.m_end;
	m_size = rhs.m_size;
	m_start = rhs.m_start;

	return *this;
}

/*
This is a debugging function that prints out the contents
of the InnerCB object. See sample outputs below for suggested format.
(You do not have to follow the format exactly, so don't worry about
counting the number of dashes or spaces.)
*/
void InnerCB::dump() {
	cout << "InnerCB dump(): m_size = " << size() << endl;
	for (int i = 0; i < m_size; i++) {
		int index = (m_start + i) % m_capacity;
		cout << "[" << index << "] " << m_buffer[index] << ", ";
	}
	cout << endl;
}