#pragma once
#include "linked_list.h"

class String
{
public:
	String();
	String(const char* c_str);
	String(const String& other);
	size_t Length() const;
	char& operator[](size_t idx);
	const char& operator[](size_t idx) const;
	bool operator==(const String& other) const;
	bool operator!=(const String& other) const;
	String operator+(const String& other);
	String& operator+=(const String& other);
	String& operator=(const String& other);

	class iterator
	{
	public:
		iterator(Node* node, size_t idx);
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;
		iterator& operator++();
		iterator operator++(int);
		char& operator*();
	private:
		Node* m_node;
		size_t m_idx;
	};
	iterator begin();
	iterator end();
private:
	LinkedList m_list;

	void Copy(const String& other);
	void Copy(Node* starting_node, const String& other);
	friend std::ostream& operator<<(std::ostream& stream, const String& string);
	friend std::istream& operator>>(std::istream& stream, String& string);
};