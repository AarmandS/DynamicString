#include <stdexcept>
#include "string.h"

String::String() : m_list(LinkedList()) {}

// Takes in a c string as its argument, creates
// the internal linked list, and adds a new node to it,
// which is constructed using the c string
String::String(const char* c_str) : m_list(LinkedList())
{
	m_list.Append(new Node(c_str));
}

// Copy constructor
// Default constructs the linked list
// and copies the contents of the other string into this one
String::String(const String& other) : m_list(LinkedList())
{
	Copy(other);
}

// Iterates through the internal linked list,
// and sums up the number of characters stored
// in each node, then it returns with the value.
size_t String::Length() const
{
	size_t len = 0;
	for (Node* moving = m_list.GetHead(); moving; moving = moving->GetNext())
		len += moving->GetStored();
	return len;
}

// Const version of the [] operator,
// takes in an index, checks whether the index
// exceeds the length of the string, if so it throws
// an exception. If it is inside the bounds of the string,
// it calculates which node contains the given index,
// iterates to it, and returns the char at the given index.
const char& String::operator[](size_t idx) const
{
	if (idx >= Length())
		throw std::out_of_range("Index is out of range");
	else
	{
		// Each node contains a set number of chars,
		// so if we divide the index by the number of chars
		// in a node, we get the node in which, the char with the given index can be found
		size_t node_idx = idx / Node::GetBufferSize(); 

		// Iterates to the correct node
		Node* node = m_list.GetHead();
		for (size_t cur_idx = 0; cur_idx != node_idx; node = node->GetNext(), cur_idx++); 

		// The char at the index 
		return (*node)[idx % Node::GetBufferSize()];
	}
}

char& String::operator[](size_t idx)
{
	return const_cast<char&>((*const_cast<const String*>(this))[idx]); // removes const qualifier from the const version of function
}


// Takes in a const reference to another String object, first checks whether the
// lengths of the 2 strings are the same, in order to avoid indexing memory outside
// the bounds of the buffers, then iterates over the contents of the strings,
// and compares them at every index, if the characters differ at any point,
// the function returns false, otherwise it returns true
bool String::operator==(const String& other) const
{
	if (this->Length() == other.Length())
	{
		for (size_t i = 0; i < Length(); i++)
			if ((*this)[i] != other[i])
				return false;
	}
	else
		return false;

	return true;
}

// Returns the negation of the == operator's result
bool String::operator!=(const String& other) const
{
	return !(*this == other);
}


// Takes in a const reference to another string,
// creates a new empty string, copies this and
// the other strings' contents into it, and
// then returns a copy of it
String String::operator+(const String& other)
{
	String res;
	res.m_list.Append(new Node());
	Node* moving = res.m_list.GetHead();

	Copy(moving, *this);

	while (moving->GetNext())
		moving = moving->GetNext();

	Copy(moving, other);
	return res;
}


// Takes in a const reference to another string,
// adds its contents to this string, then returns
// a reference to this object
String& String::operator+=(const String& other)
{
	Node* moving = m_list.GetHead();
	if (moving)
	{
		// iterate to the end of the internal linked list
		while (moving->GetNext())
			moving = moving->GetNext();

		// copy the contents of the other string to the end of this string
		Copy(moving, other);
	}
	return *this;
}

// Checks whether this and the other string are the same,
// if they are not, it clears the linked list, and copies the other
// string into this one, then returns a reference to this string
String& String::operator=(const String& other)
{
	if (*this != other)
	{
		m_list.Clear();
		Copy(other);
	}
	return *this;
}

// Takes in a const reference to another string
// and copies it into this string
// this function doesnt clear the linked list before copying,
// it is the caller's responsibility to do so
void String::Copy(const String& other)
{
	m_list.Append(new Node());
	Node* cur_this = m_list.GetHead();
	Node* cur_other = other.m_list.GetHead();
	// iterates over the 2 lists simultaneously
	while (cur_other)
	{
		// copies the contents of each node
		for (size_t i = 0; i < cur_other->GetStored(); i++)
			(*cur_this)[i] = (*cur_other)[i];
		// sets to stored value to match the original node
		cur_this->SetStored(cur_other->GetStored());

		// if there are more nodes to copy, creates a new node in the list of this string
		if (cur_other->GetNext())
		{
			m_list.Append(new Node());
			cur_this = cur_this->GetNext();
		}
		cur_other = cur_other->GetNext();
	}
}

// This is very similar to the other copy function, but this one copies from a given starting node
void String::Copy(Node* starting_node, const String& other)
{
	for (size_t copied = 0; copied < other.Length(); copied++)
	{
		// check whether the node is full, if so create a new node
		if (starting_node->GetStored() == Node::GetBufferSize())
		{
			starting_node->SetNext(new Node());
			starting_node = starting_node->GetNext();
		}
		// copy the node contents
		size_t s = starting_node->GetStored();
		(*starting_node)[s] = other[copied];
		starting_node->SetStored(s + 1);
	}
}

String::iterator String::begin()
{
	if (m_list.GetHead())
		return iterator(m_list.GetHead(), 0);
	else
		return iterator(nullptr, 0);
}

String::iterator String::end()
{
	return iterator(nullptr, 0);
}

String::iterator::iterator(Node* node, size_t idx) : m_node(node), m_idx(idx)
{
}

bool String::iterator::operator==(const iterator& other) const
{
	return this->m_node == other.m_node && this->m_idx == other.m_idx;
}

bool String::iterator::operator!=(const iterator& other) const
{
	return !(*this == other);
}

String::iterator& String::iterator::operator++()
{
	// If the node is not null pointer, checks whether
	// the current index is less then the stored value -1
	// if so, it increments the index, otherwise it jumps
	// to the next node, and sets the index to 0
	if (m_node)
	{
		if (m_idx < m_node->GetStored() - 1)
		{
			m_idx++;
		}
		else
		{
			m_node = m_node->GetNext();
			m_idx = 0;
		}
	}
	return *this;
}


// Essentially the same as the post increment iterator,
// except it creates a copy before incrementing the iterator,
// and returns a copy of it instead of a reference
String::iterator String::iterator::operator++(int)
{
	iterator copy = *this;
	if (m_node)
	{
		if (m_node && m_idx < m_node->GetStored() - 1)
		{
			m_idx++;
		}
		else
		{
			m_node = m_node->GetNext();
			m_idx = 0;
		}
	}
	return copy;
}

char& String::iterator::operator*()
{
	return (*m_node)[m_idx];
}

// Reads a stream into a string
std::istream& operator>>(std::istream& stream, String& string)
{
	// Clears the list to avoid memory leaks
	string.m_list.Clear();
	string.m_list.Append(new Node());
	Node* moving = string.m_list.GetHead();
	// Copies the characters in the stream until it reaches the endline
	for (size_t i = 0; stream.peek() != '\n'; i++)
	{
		if (i == Node::GetBufferSize())
		{
			i = 0;
			string.m_list.Append(new Node());
			moving = moving->GetNext();
		}
		stream >> (*moving)[i];
		moving->SetStored(moving->GetStored() + 1);
	}
	return stream;
}

// Iterates through the linked list and inserts every node into the stream
std::ostream& operator<<(std::ostream& stream, const String& string)
{
	for (Node* moving = string.m_list.GetHead(); moving; moving = moving->GetNext())
		stream << *moving;
	return stream;
}