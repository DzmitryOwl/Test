#include <vector>

namespace DiffProcessor
{
	template<typename T> class SortedLimitedList
	{
	protected:
		struct Entry
		{
			friend class SortedLimitedList;
			friend struct iterator;
		protected:
			T value;
			Entry* previous;
			Entry* next;
		};

	public:
		struct iterator
		{
			friend class SortedLimitedList;
			typedef iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef Entry* pointer;
		protected:
			pointer _ptr;
			iterator(pointer ptr) : _ptr(ptr) { }
		public:
			self_type operator++() { self_type i = *this; _ptr = _ptr->next; return i; }
			self_type operator++(int junk) { _ptr = _ptr->next; return *this; }
			self_type operator--() { self_type i = *this; _ptr = _ptr->previous; return i; }
			self_type operator--(int junk) { _ptr = _ptr->previous; return *this; }
			reference operator*() { return _ptr->value; }
			bool operator==(const self_type& rhs) { return _ptr == rhs._ptr; }
			bool operator!=(const self_type& rhs) { return _ptr != rhs._ptr; }
		};

		struct const_iterator
		{
			friend class SortedLimitedList;
			typedef const_iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef Entry* pointer;
		protected:
			pointer _ptr;
			const_iterator(pointer ptr) : _ptr(ptr) { }
		public:
			self_type operator++() { self_type i = *this; _ptr = _ptr->next; return i; }
			self_type operator++(int junk) { _ptr = _ptr->next; return *this; }
			self_type operator--() { self_type i = *this; _ptr = _ptr->previous; return i; }
			self_type operator--(int junk) { _ptr = _ptr->previous; return *this; }
			reference operator*() { return _ptr->value; }
			bool operator==(const self_type& rhs) { return _ptr == rhs._ptr; }
			bool operator!=(const self_type& rhs) { return _ptr != rhs._ptr; }
		};

	protected:
		long _limit;
		long _count;
		long _performed_operations;
		Entry* _first;
		Entry* _last;
		Entry* _free_pool;

		Entry* allocate()
		{
			if (_free_pool == nullptr)
				throw std::bad_alloc();
			Entry* result = _free_pool;
			_free_pool = _free_pool->next;

			return result;
		}
		void free_entry(Entry* entry)
		{
			entry->next = _free_pool;
			_free_pool = entry;
		}
		void check(Entry* entry)
		{
			Entry* before = entry->previous;
			if (before != nullptr && before->value > entry->value)
				throw std::invalid_argument("Incorrect order");

			Entry* after = entry->next;
			if (after != nullptr && after->value < entry->value)
				throw std::invalid_argument("Incorrect order");
		}

	public:

		SortedLimitedList(long limit) :
			_limit(limit),
			_count(0),
			_performed_operations(0)
		{
			_first = nullptr;
			_last = nullptr;
			_free_pool = nullptr;
			for (long i = 0; i < limit; ++i)
			{
				Entry* current = new Entry();
				free_entry(current);
			}
		}

		~SortedLimitedList()
		{
			Entry* e = _first, next;
			while (e != nullptr)
			{
				next = e->Next;
				delete e;
				entry = next;
			}
			e = _free_pool;
			while (e != nullptr)
			{
				next = e->Next;
				delete e;
				entry = next;
			}
		}

		void from_array(T a[], size_t size)
		{
			clear();
			for (size_t i = 0; i < size; ++i)
				add_last(a[i]);
			_performed_operations = 0;
		}


		void to_stream(std::ostream& stream)
		{
			Entry* e = _first;
			while (e != nullptr)
			{
				stream << e->value;
				if (e->next != nullptr)
					stream << ", ";
				e = e->next;
			}
		}

		void from_vector(std::vector<T>& vector)
		{
			clear();
			for (size_t i = 0; i < vector.size(); ++i)
				add_last(vector[i]);
			_performed_operations = 0;
		}

		void add_after(iterator& after, T value)
		{
			Entry* afteri = after._ptr;
			if (afteri == nullptr)
				add_last(value);
			else
			{
				Entry* entry = allocate();

				entry->value = value;
				entry->previous = afteri;
				entry->next = afteri->next;
				++_count;

				if (afteri->next != nullptr)
					afteri->next->previous = entry;
				else
					_last = entry;

				afteri->next = entry;
				++_performed_operations;
				check(entry);
			}
		}

		void add_before(iterator& before, T value)
		{
			Entry* beforei = before._ptr;
			if (beforei == nullptr)
				add_first(value);
			else
			{
				Entry* entry = allocate();

				entry->value = value;
				entry->previous = beforei->previous;
				entry->next = beforei;
				++_count;

				if (beforei->previous != nullptr)
					beforei->previous->next = entry;
				else
					_first = entry;

				beforei->previous = entry;
				++_performed_operations;
				check(entry);
			}
		}

		void add_last(T value)
		{
			Entry* entry = allocate();

			entry->value = value;
			entry->previous = _last;
			entry->next = nullptr;
			++_count;

			if (_last != nullptr)
				_last->next = entry;
			else
				_first = entry;

			_last = entry;
			++_performed_operations;
			check(entry);
		}

		void add_first(T value)
		{
			Entry* entry = allocate();

			entry->value = value;
			entry->previous = nullptr;
			entry->next = _first;
			++_count;

			if (_first != nullptr)
				_first->previous = entry;
			else
				_last = entry;

			_first = entry;
			++_performed_operations;
			check(entry);
		}

		void remove(iterator& iterator)
		{
			Entry* entry = iterator._ptr;
			if (entry->previous != nullptr)
				entry->previous->next = entry->next;
			else
				_first = entry->next;

			if (entry->next != nullptr)
				entry->next->previous = entry->previous;
			else
				_last = entry->previous;

			++_performed_operations;
			--_count;
			free_entry(entry);
		}

		void clear()
		{
			Entry* entry = _first, *next;
			while (entry != nullptr)
			{
				next = entry->next;
				remove(iterator(entry));
				entry = next;
			}
			_performed_operations = 0;
			_count = 0;
		}

		bool operator == (const SortedLimitedList<T>&  other)
		{
			Entry* e1 = _first;
			Entry* e2 = other._first;
			while (e1 != nullptr && e2 != nullptr)
			{
				if (e1->value != e2->value)
					return false;
				e1 = e1->next;
				e2 = e2->next;
			}

			return e1 == nullptr && e2 == nullptr;
		}

		bool operator != (const SortedLimitedList<T>&  other)
		{
			return !(*this == other);
		}

		const_iterator first() const { return (const_iterator(_first)); }
		const_iterator last() const { return (const_iterator(_last)); }

		iterator first() { return (iterator(_first)); }
		iterator last() { return (iterator(_last)); }
		long count() { return _count; }
		long limit() { return _limit; }
		long performed_operations() { return _performed_operations; }

		iterator end() { return (iterator(nullptr)); }
		const_iterator end() const { return (const_iterator(nullptr)); }
	};


	template <typename T >
	std::ostream& operator<<(std::ostream& stream, SortedLimitedList<T>* list)
	{
		list->to_stream(stream);
		return stream;
	}

	template <typename T >
	std::ostream& operator<<(std::ostream& stream, SortedLimitedList<T>& list)
	{
		list.to_stream(stream);
		return stream;
	}

};