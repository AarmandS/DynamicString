#include <cstring>
#include "linked_list.h"



// The default constructor of a node sets its next pointer to nullptr,
// sets its stored value to 0, and allocates a buffer of chars on the heap
Node::Node() : m_next(nullptr), m_stored(0)
{
	m_buffer = new char[s_buffer_size];
}

// works recursively until it reaches the end of the string
Node::Node(const char* cstring, size_t written)
{
	m_buffer = new char[s_buffer_size];
	size_t len = strlen(cstring);
	m_stored = len - written < s_buffer_size ? len - written : s_buffer_size;
	// copy into the buffer
	for (size_t i = 0; i < m_stored; i++)
		m_buffer[i] = cstring[written + i];
	written += m_stored;
	// if written is not equal to the length of the string,
	// append a new node to this one and continue the process
	m_next = written == len ? nullptr : new Node(cstring, written);
}

// Frees the memory block that was allocated in the constructor
Node::~Node()
{
	delete[] m_buffer;
}

// Takes in an index, and returns a reference to the character
// at the given index of the internal buffer
char& Node::operator[](size_t idx)
{
	return m_buffer[idx];
}

const char& Node::operator[](size_t idx) const
{
	return m_buffer[idx];
}

void Node::SetNext(Node* const & node)
{
	m_next = node;
}

void Node::SetStored(size_t size)
{
	m_stored = size;
}

Node* Node::GetNext() const
{
	return m_next;
}

size_t Node::GetStored() const
{
	return m_stored;
}

const size_t Node::GetBufferSize()
{
	return s_buffer_size;
}


// Overload for the ostream operator, takes in a const reference
// to a node, and inserts the characters stored inside into the stream
std::ostream& operator<<(std::ostream& stream, const Node& node)
{
	for (size_t i = 0; i < node.m_stored; i++)
		stream << node[i];
	return stream;
}



LinkedList::LinkedList() : m_head(nullptr)
{
}

LinkedList::~LinkedList()
{
	Clear();
}

// Takes in a node pointer.
// If the list is empty, then the head pointer is set to the given node,
// otherwise it adds it to the end of the list.
void LinkedList::Append(Node* node)
{
	Node* moving = m_head;
	if (moving)
	{
		for (; moving->GetNext(); moving = moving->GetNext());
		moving->SetNext(node);
	}
	else
		m_head = node;
}


// Iterates through the linked list, deleting each node along the way
void LinkedList::Clear()
{
	Node* moving = m_head;
	while (moving)
	{
		Node* next = moving->GetNext();
		delete moving;
		moving = next;
	}
	m_head = nullptr;
}

Node* LinkedList::GetHead() const
{
	return m_head;
}
