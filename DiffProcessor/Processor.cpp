#pragma once

#include "Processor.h"
#include <iostream>
#include <algorithm>  

namespace DiffProcessor
{
	Processor::Processor(long limit):_limit(limit)
	{
		// TODO: initialization.
	}
	Processor::~Processor()
	{
		// TODO: Free resources.
	}

	SortedLimitedList<double>::iterator Processor::_remove_if_greate(SortedLimitedList<double> &must_be_equal_to,SortedLimitedList<double>::iterator it, double value)
	{
		while(it != must_be_equal_to.end())
		{
			auto temp = it;
			--temp;
			if(*it > value)
			{
				must_be_equal_to.remove(it);
			}
			else
			{
				return it;
			}
			it = temp;
		}
		return it;
	}
	
	void Processor::_remove_to(SortedLimitedList<double> &must_be_equal_to,SortedLimitedList<double>::iterator &it)
	{
		while(must_be_equal_to.first() != it)
		{
			must_be_equal_to.remove(must_be_equal_to.first());
		}
	}
	
	SortedLimitedList<double>::iterator Processor::_clear_place(SortedLimitedList<double> &must_be_equal_to,SortedLimitedList<double>::iterator m_it,const SortedLimitedList<double> &expected_output, SortedLimitedList<double>::const_iterator	e_it)
	{
		
		auto reserve = m_it;
		while(m_it != must_be_equal_to.end())
		{
			while(e_it != expected_output.end())
			{
				if(*m_it>*e_it)
				{
					bool first = reserve == m_it ;
					auto temp = m_it;
					--temp;
					must_be_equal_to.remove(m_it);
					if(first)
						return temp;
					else
						return reserve;
				}
				else if (*m_it == *e_it)
				{
					--e_it;
					break;
				}
				else
				{
					--e_it;
				}
			}
			--m_it;
		}
		auto temp = reserve;
		--temp;
		must_be_equal_to.remove(reserve);
		return temp;
	}

	void Processor::do_process(SortedLimitedList<double> &must_be_equal_to, const SortedLimitedList<double> &expected_output)
	{
		// TODO: make "must_be_equal_to" list equal to "expected_output".
		// 0. Processor will be created once and then will be used billion times. 
		// 1. Use methods: AddFirst, AddLast, AddBefore, AddAfter, Remove to modify list.
		// 2. Do not change expected_output list.
		// 3. At any time number of elements in list could not exceed the "Limit". 
		// 4. "Limit" will be passed into Processor's constructor. All "must_be_equal_to" and "expected_output" lists will have the same "Limit" value.
		// 5. At any time list elements must be in non-descending order.
		// 6. Implementation must perform minimal possible number of actions (AddFirst, AddLast, AddBefore, AddAfter, Remove).
		// 7. Implementation must be fast and do not allocate excess memory.
		if(expected_output.first() == expected_output.end())
		{
			_remove_to(must_be_equal_to,must_be_equal_to.end());
			return;
		}
		//TODO: revers iterator is more suitable. But I not sure that allowed modified SortedLimitedList in test task 
		auto e = expected_output.last();
		auto m = must_be_equal_to.last();
		while(true)
		{
			if(m == must_be_equal_to.end())
			{
				// tail already matched and head empty
				must_be_equal_to.add_first(*e);
			}
			else
			{
				//remove all value greaten than in expected_output list
				m = _remove_if_greate(must_be_equal_to, m, *e);   // std::remove_if is not optimal in sorted list
				if(m == must_be_equal_to.end())
				{
					// all head in must_be_equal_to list was deleted
					must_be_equal_to.add_first(*e);
				}
				else  if(*m == *e)
				{
					//next element equivalent to expected_output element
					--m;
				}
				else
				{
					// need to add value in 'e' iterator from expected_output list
					if(must_be_equal_to.count() >= _limit)
					{
						//remove elements to avoid exceeding
						m = _clear_place(must_be_equal_to, m, expected_output, e);
						if(m == must_be_equal_to.end())
							must_be_equal_to.add_first(*e);
						else
							must_be_equal_to.add_after(m,*e);
					}
					else
						must_be_equal_to.add_after(m,*e);
				}
			}
			if(e == expected_output.first())
			{
				//remove all items from first to m iterator (including m) in must_be_equal_to
				if(m != must_be_equal_to.end())
				{
					++m;
					_remove_to(must_be_equal_to,m);
				}
				break;
			}
			--e;
		}
	}

};