#pragma once

#include "SortedLimitedList.h"

namespace DiffProcessor
{
	class Processor
	{
	public:
		Processor(long limit);
		~Processor();
		void do_process(SortedLimitedList<double> &must_be_equal_to, const SortedLimitedList<double> &expected_output);
	private:

		/************************************************************************/
		/* Remove elements from must_be_equal_to.first() to it iterator         */
		/************************************************************************/
		void _remove_to(SortedLimitedList<double> &must_be_equal_to, SortedLimitedList<double>::iterator &it);

		/************************************************************************/
		/* Remove elements greaten then value                                   */
		/* from must_be_equal_to.first() to it iterator fo                      */
		/************************************************************************/
		SortedLimitedList<double>::iterator _remove_if_greate(SortedLimitedList<double> &must_be_equal_to,SortedLimitedList<double>::iterator it, double value);

		/************************************************************************/
		/* Find and delete element which not present in expected_output         */
		/************************************************************************/
		SortedLimitedList<double>::iterator  _clear_place(SortedLimitedList<double> &must_be_equal_to,SortedLimitedList<double>::iterator m_it,const SortedLimitedList<double> &expected_output, SortedLimitedList<double>::const_iterator	e_it);

		long _limit;
	};
};