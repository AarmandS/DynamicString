#pragma once
#include <iostream>


class Node
{
public:
	Node();
	Node(const char* cstring, size_t written = 0);
	~Node();
	char& operator[](size_t idx);
	const char& operator[](size_t idx) const;
	Node* GetNext() const;
	size_t GetStored() const;
	static const size_t GetBufferSize();
	void SetNext(Node* const & node);
	void SetStored(size_t size);
private:
	char* m_buffer;
	Node* m_next;
	size_t m_stored;
	static const size_t s_buffer_size = 20;
	friend std::ostream& operator<<(std::ostream& stream, const Node& node);
};


class LinkedList
{
public:
	LinkedList();
	~LinkedList();
	void Append(Node* node);
	void Clear();
	Node* GetHead() const;
private:
	Node* m_head;
};