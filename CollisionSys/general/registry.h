#pragma once

#include "list.h"

namespace glib {
	template <typename Key, typename T>
	struct pair {
		Key first;
		T second;

		pair() : first(), second() {}

		pair(const Key& key, const T& data) :
			first(key),
			second(data)
		{}

		pair& operator = (const pair& other) {
			this->first = other.first;
			this->second = other.second;
			return *this;
		}

		bool operator == (const pair& other) const {
			return this->first == other.first;
		}

		bool operator != (const pair& other) const {
			return this->first != other.first;
		}
	};

	template <typename Key, typename T>
	class registry
	{
	public:
		using PairType = pair<Key, T >;
		using ListType = list<PairType>;
		using forwardIt = typename ListType::forwardIt;
	private:
		ListType reg_list;
	public:
		forwardIt begin() {
			return this->reg_list.begin();
		}

		forwardIt end() {
			return this->reg_list.end();
		}

		void add(const PairType& reg) {
			this->reg_list.push_back(reg);
		}

		void add(Key key, T data) {
			this->add(PairType(key, data));
		}

		forwardIt get(const Key& key) {
			for (forwardIt it = this->begin(); it != this->end(); it++) {
				if (it->first == key) {
					return it;
				}
			}
			return this->end();
		}
	};
}
