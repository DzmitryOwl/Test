#include <iostream>
#include "../DiffProcessor/Processor.h"

using namespace DiffProcessor;

Processor* diffProcessor;
SortedLimitedList<double> * sortedList1 = new SortedLimitedList<double>(10);
SortedLimitedList<double> * sortedList2 = new SortedLimitedList<double>(10);
SortedLimitedList<double> * sortedList1_ = new SortedLimitedList<double>(10);
SortedLimitedList<double> * sortedList2_ = new SortedLimitedList<double>(10);
long test_index = 0;

void do_test(int operations)
{
	try
	{
		diffProcessor->do_process(*sortedList1, *sortedList2);
		if (*sortedList1 != *sortedList2 || *sortedList1 != *sortedList2_ || sortedList1->performed_operations() != operations)
		{
			std::cout << "Test case: " << (++test_index) << " Failed. Inputs: mustBeEqual: [" << sortedList1_ << "] expected: ["  << sortedList2_ << "]" << std::endl ;
			std::cout << "Your output " << sortedList1 << std::endl;
			std::cout << "Performed operations: " << sortedList1->performed_operations() << ", expected " << operations << std::endl << std::endl;;
		}
		else
		{
			std::cout << "Test case: " << (++test_index) << " Succeed. Inputs: mustBeEqual: [" << sortedList1_ << "] expected: [" << sortedList2_ << "]" << std::endl << std::endl;;
		}
	}
	catch (...)
	{
		std::cout << "Test case: " << (++test_index) << " Exception. Inputs: mustBeEqual: [" << sortedList1_ << "] expected: [" << sortedList2_ << "]" << std::endl << std::endl;;
	}
}


void test(double array1[], size_t size1, double array2[], size_t size2, int operations)
{
	sortedList1->from_array(array1, size1);
	sortedList2->from_array(array2, size2);
	sortedList1_->from_array(array1, size1);
	sortedList2_->from_array(array2, size2);
	do_test(operations);
}

int main(int argc, const char * argv[])
{
	diffProcessor = new Processor(10);

	{
		double needToBeEqual_10[] = { 0, 1, 2, 3, 3, 3, 4, 5, 5, 10 };
		double expected_10[] =      { 0, 1, 2, 3, 3, 3, 4, 5, 5, 10 };
		test(needToBeEqual_10, sizeof(needToBeEqual_10) / sizeof(double), expected_10, sizeof(expected_10) / sizeof(double), 0);
	}
	{
		double needToBeEqual_10[] = { 0, 1, 2, 3, 3, 3, 4, 5, 5, 10 };
		double expected_10[] =      { 0, 1, 2, 3, 5, 5, 6, 7, 9, 10 };
		test(needToBeEqual_10, sizeof(needToBeEqual_10) / sizeof(double), expected_10, sizeof(expected_10) / sizeof(double), 6);
	}
	{
		double needToBeEqual_10[] = { 1, 1, 2, 3, 3, 3, 4, 5, 5, 10 };
		double expected_10[] =      { 0, 1, 2, 3, 5, 5, 6, 7, 9, 10 };
		test(needToBeEqual_10, sizeof(needToBeEqual_10) / sizeof(double), expected_10, sizeof(expected_10) / sizeof(double), 8);
	}

	{
		double needToBeEqual_10[] = { 1, 1, 2, 3, 3, 3, 4, 5, 5, 10 };
		double expected_10[] = { 1};
		test(needToBeEqual_10, sizeof(needToBeEqual_10) / sizeof(double), expected_10, sizeof(expected_10) / sizeof(double), 9);
	}
	{
		double needToBeEqual_10[] = { 1 };
		double expected_10[] =      { 0, 1, 2, 3, 5, 5, 6, 7, 9, 10 };
		test(needToBeEqual_10, sizeof(needToBeEqual_10) / sizeof(double), expected_10, sizeof(expected_10) / sizeof(double), 9);
	}
	{
		double needToBeEqual_10[] = { 0 };
		double expected_10[] =      { 0, 1, 2, 3, 5, 5, 6, 7, 9, 10 };
		test(needToBeEqual_10, sizeof(needToBeEqual_10) / sizeof(double), expected_10, sizeof(expected_10) / sizeof(double), 9);
	}
	double needToBeEqual_0[] = { 0, 1, 2, 3, 4, 6, 7 };
	double expected_0[] =      { 0, 1, 2, 3, 4, 5, 6, 7 };
	test(needToBeEqual_0, sizeof(needToBeEqual_0) / sizeof(double), expected_0, sizeof(expected_0) / sizeof(double), 1);

	double needToBeEqual_1[] = { 0, 1, 2, 3, 4, 6, 7 };
	double expected_1[] = { 1, 2, 3, 3 };
	test(needToBeEqual_1, sizeof(needToBeEqual_1) / sizeof(double), expected_1, sizeof(expected_1) / sizeof(double), 5);

	double needToBeEqual_2[] = { 0 };
	double expected_2[] = { 7 };
	test(needToBeEqual_2, sizeof(needToBeEqual_2) / sizeof(double), expected_2, sizeof(expected_2) / sizeof(double), 2);

	double needToBeEqual_3[] = { 0, 1, 1, 1, 1, 1, 1 };
	double expected_3[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
	test(needToBeEqual_3, sizeof(needToBeEqual_3) / sizeof(double), expected_3, sizeof(expected_3) / sizeof(double), 17);

	double needToBeEqual_4[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 3 };
	double expected_4[] = { 0, 2, 2, 2, 2, 2, 2, 2, 2, 3 };
	test(needToBeEqual_4, sizeof(needToBeEqual_4) / sizeof(double), expected_4, sizeof(expected_4) / sizeof(double), 16);

	double needToBeEqual_5[] = { 0, 1, 2, 3, 4, 6, 7 };
	test(needToBeEqual_5, sizeof(needToBeEqual_5) / sizeof(double), NULL, 0, 7);

	double needToBeEqual_6[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	double expected_6[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	test(needToBeEqual_6, sizeof(needToBeEqual_6) / sizeof(double), expected_6, sizeof(expected_6) / sizeof(double), 2);

	double needToBeEqual_7[] = { 0, 0 };
	double expected_7[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	test(needToBeEqual_7, sizeof(needToBeEqual_7) / sizeof(double), expected_7, sizeof(expected_7) / sizeof(double), 10);

	delete diffProcessor;

	return 0;
}