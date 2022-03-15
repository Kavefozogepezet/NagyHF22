#pragma once

#include <stdexcept>

namespace glib {
	template <typename T>
	class node {
		template <typename T> friend class list;
	public:
		T item;

		node(node* prev = nullptr, node* next = nullptr) :
			p(prev),
			n(next)
		{}

		inline node<T>* prev() { return this->p; }

		inline node<T>* next() { return this->n; }
	private:
		node* p, * n;
	};

	template <typename T>
	class listIterator {
	public:
		using Type = typename T::Type;
		using NodeType = node<Type>;

		listIterator(NodeType* ptr) : item_ptr(ptr) {}

		listIterator& operator = (const listIterator& other) {
			this->item_ptr = other.item_ptr;
		}

		Type* operator -> () { return &this->item_ptr->item; }

		Type& operator * () { return this->item_ptr->item; }

		bool operator == (const listIterator<T>& other) { return this->item_ptr == other.item_ptr; }

		bool operator != (const listIterator<T>& other) { return this->item_ptr != other.item_ptr; }
	private:
		NodeType* item_ptr;
	};

	template <typename T>
	class forwardListIt : public listIterator<T> {
	public:
		using listIterator<T>::listIterator;

		forwardListIt& operator ++ () {
			this->item_ptr = this->item_ptr->next();
			return *this;
		}

		forwardListIt& operator -- () {
			this->item_ptr = this->item_ptr->prev();
			return *this;
		}

		forwardListIt operator ++ (int) {
			listIterator it = *this;
			++(*this);
			return it;
		}

		forwardListIt operator -- (int) {
			listIterator it = *this;
			--(*this);
			return it;
		}
	};

	template <typename T>
	class reverseListIt : public listIterator<T> {
	public:
		using listIterator<T>::listIterator;

		reverseListIt& operator ++ () {
			this->item_ptr = this->item_ptr->prev();
			return *this;
		}

		reverseListIt& operator -- () {
			this->item_ptr = this->item_ptr->next();
			return *this;
		}

		reverseListIt operator ++ (int) {
			listIterator it = *this;
			++(*this);
			return it;
		}

		reverseListIt operator -- (int) {
			listIterator it = *this;
			--(*this);
			return it;
		}
	};

	template <typename T>
	class list {
	public:
		using Type = T;
		using forwardIt = forwardListIt<list<T>>;
		using reverseIt = reverseListIt<list<T>>;
	public:
		list() :
			head(nullptr),
			tail(nullptr)
		{}

		inline forwardIt begin() { return forwardIt(this->head); }

		inline forwardIt end() { return forwardIt(nullptr); }

		inline reverseIt rbegin() { return reverseIt(this->tail); }

		inline reverseIt rend() { return reverseIt(nullptr); }

	private:
		node<T>* head, tail;
	};
}
