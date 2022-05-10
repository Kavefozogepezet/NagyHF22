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
		listNode<T>* prev() const { return this->p; }
		/** @returns Lista következő eleme*/
		listNode<T>* next() const { return this->n; }
	private:
		listNode* p, * n;

		static inline void connect(listNode* n1, listNode* n2) {
			n1->n = n2;
			n2->p = n1;
		}
	};

	/*
	* @brief Lista iterator alap osztálya.
	* @param T - Egy list<Type> típus
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
			return *this;
		}

		/** @returns Aktuális elem által tárolt adatra mutató ptr.*/
		Type* operator -> () const { return &this->item_ptr->item; }

		/** @returns Aktuális elem által tárolt adat referenciája */
		Type& operator * () const { return this->item_ptr->item; }

		bool operator == (const listIterator& other) const { return this->item_ptr == other.item_ptr; }

		bool operator != (const listIterator& other) const { return this->item_ptr != other.item_ptr; }

		inline NodeType* getPtr() const { return this->item_ptr; }
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
		list() :
			head((NodeType*)malloc(2 * sizeof(NodeType))),
			tail(this->head + 1),
			m_size(0)
		{
			/*
			this->head = (NodeType*) malloc(2 * sizeof(NodeType));
			this->tail = head + 1;
			*/
			NodeType::connect(this->head, this->tail);
			this->head->p = this->tail->n =  nullptr;
		}

		~list() {
			this->clear();
			free(this->head);
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
		bool empty() const { return this->head->next() == this->tail; }

		/** @brief Törli a lista összes elemét. */
		void clear() {
			this->erase(forwardIt(this->head->next()), forwardIt(this->tail));
		}

		/** @return lista mérete */
		size_t size() const { return this->m_size; }

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

		/** @brief Törli a lista utolsó elemét. */
		void pop_back() { this->erase(this->rbegin()); }

		/** @brief Törli a lista első elemét. */
		void pop_front() { this->erase(this->begin()); }

		/*
		* @brief Beilleszt egy elemet a listába a megadott helyre.
		* @param pos - Az iterator ami elé az új elem kerül
		* @param data - A beilleszteni kívánt adat
		* @return Iterator mely a beillesztett elemre mutat
		*/
		iterator insert(iterator pos, const T& data) {
			NodeType
				* right = pos.getPtr(),
				* left = right->prev(),
				* new_node = new NodeType(data, left, right);
			left->n = new_node;
			right->p = new_node;
			this->m_size++;
			return iterator(new_node);
		}

		/*
		* @brief Eltávolít egy elemet a listából.
		* @param pos - Az eltávolítandó elemre mutató iterator
		* @return Az eltávolított elem utáni elemre mutató iterator
		*/
		iterator erase(iterator pos) {
			NodeType
				* right = pos.getPtr()->next(),
				* left = pos.getPtr()->prev();
			delete pos.getPtr();
			this->m_size--;
			NodeType::connect(left, right);
			return iterator(right);
		}

		/*
		* @brief Eltávolít intervallumon belüli elemeket a listából. [first, last)
		* @param first - Az intervallum eleje - zárt
		* @param last - Az intervallum vége - nyitott
		* @return Az eltávolított elemek utáni első elemre mutató iterator
		*/
		iterator erase(iterator first, iterator last) {
			NodeType
				* left = first.getPtr()->prev(),
				* right = last.getPtr();

			forwardIt ff(first.getPtr()), fl(last.getPtr());

			while (ff != fl) {
				NodeType* temp = ff.getPtr();
				++ff;
				delete temp;
				this->m_size--;
			}
			NodeType::connect(left, right);
			return iterator(last);
		}
	private:
		NodeType* head,* tail;
		size_t m_size;
	};
}
