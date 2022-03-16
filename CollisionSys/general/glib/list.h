#pragma once

#include <stdexcept>

namespace glib {
	/*
	* @brief Egy duplán láncolt lista egy eleme.
	* @param T - A tárolt adat típusa
	*/
	template <typename T>
	class listNode {
		template <typename T> friend class list;
	public:
		T item;

		listNode(const T& data, listNode* prev = nullptr, listNode* next = nullptr) :
			item(data),
			p(prev),
			n(next)
		{}

		/** @retruns Lista előző eleme*/
		inline listNode<T>* prev() { return this->p; }
		/** @returns Lista következő eleme*/
		inline listNode<T>* next() { return this->n; }
	private:
		listNode* p, * n;

		static inline void connect(listNode* n1, listNode* n2) {
			n1->n = n2;
			n2->p = n1;
		}
	};

	/*
	* @brief Lista iterator alap osztálya.
	* @param T - Egy lista<Type> típus
	*/
	template <typename T>
	class listIterator {
	public:
		using Type = typename T::Type;
		using NodeType = listNode<Type>;

		listIterator(NodeType* ptr) : item_ptr(ptr) {}

		listIterator(const listIterator& other) : item_ptr(other.item_ptr) {}

		listIterator& operator = (const listIterator& other) {
			this->item_ptr = other.item_ptr;
		}

		/** @returns Aktuális elem által tárolt adatra mutató ptr.*/
		Type* operator -> () { return &this->item_ptr->item; }

		/** @returns Aktuális elem által tárolt adat referenciája */
		Type& operator * () { return this->item_ptr->item; }

		bool operator == (const listIterator<T>& other) { return this->item_ptr == other.item_ptr; }

		bool operator != (const listIterator<T>& other) { return this->item_ptr != other.item_ptr; }

		inline NodeType* getPtr() { return this->item_ptr; }
	protected:
		NodeType* item_ptr;
	};

	/** @brief Lista iterator. Bejárás a lista elejétől a végéig. */
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
			forwardListIt it = *this;
			++(*this);
			return it;
		}

		forwardListIt operator -- (int) {
			forwardListIt it = *this;
			--(*this);
			return it;
		}
	};

	/** @brief Lista iterator. Bejárás a lista végétől az elejéig. */
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
			reverseListIt it = *this;
			++(*this);
			return it;
		}

		reverseListIt operator -- (int) {
			reverseListIt it = *this;
			--(*this);
			return it;
		}
	};

	/* 
	* @brief Duplán láncolt lista.
	* @param T - A lista által tárolt adat típusa
	*/
	template <typename T>
	class list {
	private:
		using iterator = listIterator<list<T>>;
	public:
		using Type = T;
		using NodeType = listNode<Type>;
		using forwardIt = forwardListIt<list<Type>>;
		using reverseIt = reverseListIt<list<Type>>;
	public:
		list() {
			this->head = (NodeType*) operator new(2 * sizeof(NodeType));
			this->tail = head + 1;
			this->head->n = this->tail;
			this->tail->p = this->head;
			this->head->p = this->tail->n =  nullptr;
		}

		~list() {
			delete this->head;
		}

		/** @returns Forward iterator a lista első elemén. */
		inline forwardIt begin() { return forwardIt(this->head->next()); }
		
		/** @returns Forward iterator a lista utolsó eleme után. */
		inline forwardIt end() { return forwardIt(this->tail); }

		/** @returns Reverse iterator a lista első eleme előtt. */
		inline reverseIt rbegin() { return reverseIt(this->tail->prev()); }

		/** @returns Reverse iterator a lista utolsó elemén. */
		inline reverseIt rend() { return reverseIt(this->head); }

		/** @returns true: a lista üres; false: nem üres. */
		bool empty() {
			return this->head->next() == this->tail();
		}

		/** @brief Törli a lista összes elemét. */
		void clear() {
			this->erase(forwardIt(this->head->next()), forwardIt(this->tail));
		}

		/*
		* @brief A lista végére illeszt egy elemet.
		* @param data - A beilleszteni kívánt adat
		*/
		void push_back(const Type& data) {
			forwardIt it = forwardIt(this->tail);
			this->insert(it, data);
		}

		/*
		* @brief A lista elejére illeszt egy elemet.
		* @param data - A beilleszteni kívánt adat
		*/
		void push_front(const Type& data) {
			forwardIt it = forwardIt(this->head->next());
			this->insert(this->head->next());
		}

		void pop_back() {
			forwardIt it = forwardIt(this->tail->prev());
			this->erase(it);
		}

		void pop_front() {
			forwardIt it = forwardIt(this->head->next());
			this->erase(it);
		}

		iterator insert(iterator pos, const T& data) {
			NodeType
				* right = pos.getPtr(),
				* left = right->prev(),
				* new_node = new NodeType(data, left, right);
			left->n = new_node;
			right->p = new_node;
			return iterator(new_node);
		}

		iterator erase(iterator pos) {
			NodeType
				* right = pos.getPtr()->next(),
				* left = pos.getPtr()->prev();
			delete pos.getPtr();
			NodeType::connect(left, right);
			return iterator(right);
		}

		iterator erase(iterator first, iterator last) {
			NodeType
				* left = first.getPtr()->prev(),
				* right = last.getPtr();

			while (first != last) {
				NodeType* temp = first.getPtr();
				++first;
				delete temp;
			}
			listNode::connect(left, right);
			return iterator(last);
		}
	private:
		NodeType* head,* tail;
	};
}
