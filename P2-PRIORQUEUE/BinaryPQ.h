// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
    } // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
		BaseClass{ comp }, data{ start, end } {
        // TODO: Implement this function.
		updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
		for (size_t i = data.size(); i >= 1; i--)
		{
			fixDown(data.size(), i);
		}
    } // updatePriorities()


    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        // TODO: Implement this function.
		data.resize(data.size() + 1);
		data.back() = val;
		fixUp(data.size());
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
		getElement(1) = data.back();
		data.resize(data.size() - 1);
		fixDown(data.size(), 1);
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        // TODO: Implement this function.

        // These lines are present only so that this provided file compiles.
        return getElement(1);      // TODO: Delete or change this line
    } // top()


    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function.  Might be very simple,
        // depending on your implementation.
        return data.size(); // TODO: Delete or change this line
    } // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function.  Might be very simple,
        // depending on your implementation.
        return data.empty(); // TODO: Delete or change this line
    } // empty()


private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;

    // TODO: Add any additional member functions or data you require here.
    // For instance, you might add fixUp() and fixDown().
	// Translate 1-based indexing into a 0-based vector
	TYPE &getElement(std::size_t i)
	{
		return data[i - 1];
	}

	const TYPE &getElement(std::size_t i) const
	{
		return data[i - 1];
	}

	void fixUp(std::size_t k) {
		while (k > 1 && this->compare(getElement(k / 2), getElement(k))) {
			std::swap(getElement(k), getElement(k / 2));
			k /= 2; // move up to parent
		} // while
	} // fixUp

	void fixDown(std::size_t heapsize, std::size_t k) {
		while (2 * k <= heapsize) {
			std::size_t j = 2 * k; // start with left child
			if (j < heapsize && this->compare(getElement(j), getElement(j + 1)))
			{
				++j;
			}
			if (this->compare(getElement(j), getElement(k)))
			{
				break; // heap restored
			}
			std::swap(getElement(k), getElement(j));
			k = j; // move down		
		} // while
	} // fixDown
	
}; // BinaryPQ


#endif // BINARYPQ_H
