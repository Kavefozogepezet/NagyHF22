#pragma once

namespace glib {
	/*
	* @brief Array iterator alap osztálya.
	* @param T - Egy array<Type> típus
	*/
	template <typename T>
	class arrayIterator {
	public:
		using Type = typename T::Type;

		arrayIterator(Type* ptr) : item_ptr(ptr) {}

		arrayIterator(const arrayIterator& other) : item_ptr(other.item_ptr) {}

		arrayIterator& operator = (const arrayIterator& other) {
			this->item_ptr = other.item_ptr;
		}

		/** @returns Aktuális elem által tárolt adatra mutató ptr.*/
		Type* operator -> () const { return this->item_ptr; }

		/** @returns Aktuális elem által tárolt adat referenciája */
		Type& operator * () const { return *(this->item_ptr); }

		bool operator == (const arrayIterator& other) const { return this->item_ptr == other.item_ptr; }

		bool operator != (const arrayIterator& other) const { return this->item_ptr != other.item_ptr; }

		inline Type* getPtr() const { return this->item_ptr; }
	protected:
		Type* item_ptr;
	};

	/** @brief Array iterator. Bejárás a lista elejétől a végéig. */
	template <typename T>
	class forwardArrayIt : public arrayIterator<T> {
	public:
		using arrayIterator<T>::arrayIterator;

		forwardArrayIt& operator ++ () {
			this->item_ptr++;
			return *this;
		}

		forwardArrayIt& operator -- () {
			this->item_ptr--;
			return *this;
		}

		forwardArrayIt operator ++ (int) {
			forwardArrayIt it = *this;
			++(*this);
			return it;
		}

		forwardArrayIt operator -- (int) {
			forwardArrayIt it = *this;
			--(*this);
			return it;
		}
	};

	/** @brief Array iterator. Bejárás a lista végétől az elejéig. */
	template <typename T>
	class reverseArrayIt : public arrayIterator<T> {
	public:
		using arrayIterator<T>::arrayIterator;

		reverseArrayIt& operator ++ () {
			this->item_ptr--;
			return *this;
		}

		reverseArrayIt& operator -- () {
			this->item_ptr++;
			return *this;
		}

		reverseArrayIt operator ++ (int) {
			reverseArrayIt it = *this;
			++(*this);
			return it;
		}

		reverseArrayIt operator -- (int) {
			reverseArrayIt it = *this;
			--(*this);
			return it;
		}
	};

	template <typename T>
	class array {
	private:
		using iterator = arrayIterator<array<T>>;
	public:
		using Type = T;
		using forwardIt = forwardArrayIt<array<Type>>;
		using reverseIt = reverseArrayIt<array<Type>>;
	public:
		array(size_t size = 0) :
			m_size(size),
			data((T*)malloc(sizeof(T) * this->m_size))
		{}

		array(size_t size, const T& def_value) :
			m_size(size),
			data((T*)malloc(sizeof(T)* this->m_size))
		{
			for (Type& item : *this) {
				item = def_value;
			}
		}

		array(const array& other) :
			m_size(other.size),
			data(malloc(sizeof(T)* this->m_size))
		{
			for (size_t i = 0; i < this->m_size; i++) {
				this->data[i] = other.data[i];
			}
		}

		~array() {
			free(this->data);
		}

		/** @returns Forward iterator a lista első elemén. */
		inline forwardIt begin() { return forwardIt(this->data); }
		/** @returns const Forward iterator a lista első elemén. */
		inline const forwardIt begin() const { return forwardIt(this->data); }

		/** @returns Forward iterator a lista utolsó eleme után. */
		inline forwardIt end() { return forwardIt(this->data + this->m_size); }
		/** @returns const Forward iterator a lista utolsó eleme után. */
		inline const forwardIt end() const { return forwardIt(this->data + this->m_size); }

		/** @returns Reverse iterator a lista első eleme előtt. */
		inline reverseIt rbegin() { return reverseIt(this->data + this->m_size - 1); }

		/** @returns Reverse iterator a lista utolsó elemén. */
		inline reverseIt rend() { return reverseIt(this->data - 1); }

		array& operator = (const array<T>& other) {
			this->m_size = other.m_size;
			delete[] this->data;
			this->data = new T[this->m_size];
			for (size_t i = 0; i < this->m_size; i++) {
				this->data[i] = other.data[i];
			}
		}

		T& operator [] (size_t idx) {
			const T& const_element = static_cast<const array*>(this)->operator[](idx);
			return const_cast<T&>(const_element);
		}

		const T& operator [] (size_t idx) const {
			return this->data[idx];
		}

		size_t size() const { return this->m_size; }

		void resize(size_t new_size) {
			T* temp = (T*)malloc(sizeof(T) * new_size);
			for (size_t i = 0; i < std::min(this->m_size, new_size); i++) {
				temp[i] = this->data[i];
			}
			free(this->data);
			this->data = temp;
			this->m_size = new_size;
		}
	private:
		size_t m_size;
		T* data;
	};
}
