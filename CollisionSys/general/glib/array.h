#pragma once

namespace glib {
	template <typename T>
	class array {
	public:
		array(size_t capacity = 0) :
			capacity(capacity),
			size(0),
			data(operator new(sizeof(T) * capacity))
		{}

		array(const array& other) :
			capacity(other.capacity),
			size(other.size),
			data(operator new(sizeof(T) * other.capacity))
		{
			for (size_t i = 0; i < this->size; i++) {
				this->data[i] = other.data[i];
			}
		}

		array& operator = (const array<T>& other) {
			this->capacity = other.capacity;
			this->size = other.size;
			delete[] this->data;
			this->data = new T[size];
			for (size_t i = 0; i < this->size; i++) {
				this->data[i] = other.data[i];
			}
		}

		T& operator [] (size_t idx) {
			const T& const_element = static_cast<const array*>(this)->operator[](idx);
			return const_cast<T&>(const_element);
		}

		const T& operator [] (size_t idx) const {
			if (idx >= this->capacity) {
				throw std::out_of_range("invalid array subscript: tried to access element beyound capacity.");
			}
			return this->data[idx];
		}

		T& at(size_t idx) {
			const T& const_element = static_cast<const array*>(this)->at(idx);
			return const_cast<T&>(const_element);

		}

		const T& at(size_t idx) const {
			if (idx >= this->size) {
				throw std::out_of_range("invalid array subscript: tried to access element beyound size of array.");
			}
			return this->data[idx];
		}
	private:
		size_t capacity, size;
		T* data;
	};
}
