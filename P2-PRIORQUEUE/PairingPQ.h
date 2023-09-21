// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>


// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
	// This is a way to refer to the base class object.
	using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
	// Each node within the pairing heap
	class Node {
	public:
		// TODO: After you add add one extra pointer (see below), be sure to
		// initialize it here.
		explicit Node(const TYPE &val)
			: elt{ val }, child{ nullptr }, sibling{ nullptr }, previous{ nullptr }
		{}

		// Description: Allows access to the element at that Node's position.
		// There are two versions, getElt() and a dereference operator, use
		// whichever one seems more natural to you.
		// Runtime: O(1) - this has been provided for you.
		const TYPE &getElt() const { return elt; }
		const TYPE &operator*() const { return elt; }

		// The following line allows you to access any private data members of this
		// Node class from within the PairingPQ class. (ie: myNode.elt is a legal
		// statement in PairingPQ's add_node() function).
		friend PairingPQ;

	private:
		TYPE elt;
		Node *child;
		Node *sibling;
		Node *previous; // Actually previous
		// TODO: Add one extra pointer (parent or previous) as desired.
	}; // Node


	// Description: Construct an empty priority_queue with an optional comparison functor.
	// Runtime: O(1)
	explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
		BaseClass{ comp }, sizes{ 0 }, root{ nullptr } {
        // TODO: Implement this function.
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, sizes{ 0 }, root{ nullptr } {
        // TODO: Implement this function.
		while (start != end)
		{
			push(*start);
			++start;
		}
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
        BaseClass{ other.compare } {
		
        // TODO: Implement this function.
		this->sizes = 0;
		
		std::deque<Node*> temp;

		temp.push_back(other.root);
		//temp1.push_back(this->root);
		while (!temp.empty())
		{
			// Check if the roots node is not a nullptr
			if (!temp.empty())
			{
				push(temp.front()->elt);
				if (temp.front()->child != nullptr)
				{
					temp.push_back(temp.front()->child);
				}
				if (temp.front()->sibling != nullptr)
				{
					temp.push_back(temp.front()->sibling);
				}
				temp.pop_front();
			}
		}
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ& operator=(const PairingPQ& rhs) {
        // TODO: Implement this function.
		PairingPQ temp(rhs);

		// Maybe have to go continuously swap everything??
		std::swap(sizes, temp.sizes);
		std::swap(root, temp.root);

        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
		
		// Deque to go through 
		// Add children, siblings, delete the node
		std::deque<Node*> temp;

		
		if (root == nullptr)
		{
			return;
		}

		temp.push_back(root);

		while (!temp.empty())
		{
			if (temp.front()->child != nullptr)
			{
				temp.push_back(temp.front()->child);
				if (temp.front()->child != nullptr)
				{
					temp.front()->child->previous = nullptr;
				}
				temp.front()->child = nullptr;
			}
			if (temp.front()->sibling != nullptr)
			{
				temp.push_back(temp.front()->sibling);
				if (temp.front()->sibling != nullptr)
				{
					temp.front()->sibling->previous = nullptr;
				}
				temp.front()->sibling = nullptr;
			}
			delete temp.front();
			temp.front() = nullptr;
			temp.pop_front();
			sizes -= 1;
		}
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.

		// Use the deque, similar to the copy constructor
		// Start at the root and move down
		// So look at the current node's child and siblings, add them in by melding
		std::deque<Node*> temp;

		if (root == nullptr) {
			return;
		}

		temp.push_back(root->child);
		root->child = nullptr;
		while (!temp.empty())
		{
			if (temp.front()->child != nullptr)
			{
				temp.push_back(temp.front()->child);
				if (temp.front()->child != nullptr)
				{
					temp.front()->child->previous = nullptr;
				}
				temp.front()->child = nullptr;
			}
			if (temp.front()->sibling != nullptr)
			{
				temp.push_back(temp.front()->sibling);
				if (temp.front()->sibling != nullptr)
				{
					temp.front()->sibling->previous = nullptr;
				}
				temp.front()->sibling = nullptr;
			}
			if (!temp.empty())
			{
				root = meld(root, temp.front());
			}
			temp.pop_front();
			
		}

    } // updatePriorities()


    // Description: Add a new element to the priority_queue. This is almost done,
    //              in that you should implement push functionality in the addNode()
    //              function, and this function should call addNode().
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        // TODO: Implement this function.
		addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {

		// Another deque to loop through
		// Meld everything back together afterwards
		// Meld the childs and the siblings to the new pairing heap

        // TODO: Implement this function.

		std::deque<Node*> temp;

		// Delete the past root
		Node *tempoo = root;

		if (root->child != nullptr)
		{
			root->child->previous = nullptr;
		}

		root = root->child;
		delete tempoo;
		tempoo = nullptr;

		// First detach the root node from the entire heap
		// If it is already empty, no point in popping
		if (root == nullptr) {
			sizes -= 1;
			return;
		}
			
		// Likely push in the next element
		// Maybe just pass in the root
		temp.push_back(root);
		
		// Put in all the trees into the queue
		while (temp.back()->sibling != nullptr)
		{
			if (temp.back()->sibling != nullptr)
			{
				Node *temp1 = temp.back()->sibling;
				// Clean up 5(current temp.back())
				temp.back()->previous = nullptr;
				temp.back()->sibling = nullptr;
				
				// Now push back the sibling
				temp.push_back(temp1);
			}
		}
		// Clean up the last element
		if (!temp.empty())
		{
			temp.back()->previous = nullptr;
			temp.back()->sibling = nullptr;
		}
		
		while (temp.size() != 1)
		{
			// Extract the first two trees and then meld them
			Node *temp1 = temp.front();
			temp.pop_front();
			Node *temp2 = temp.front();
			temp.pop_front();
			root = meld(temp1, temp2);
			temp.push_back(root);
		}

		sizes -= 1;

    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        // TODO: Implement this function

        // These lines are present only so that this provided file compiles.
        return root->elt;      
    } // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function
        return sizes;
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function
		if (size() == 0)
		{
			return true;
		}
		return false; 
    } // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE & new_value) {
        // TODO: Implement this function

		// If the node is equivalent to the root node, then simply update the root
		if (root == nullptr || node == nullptr)
		{
			return;
		}
		if (node == root)
		{
			root->elt = new_value;
		}
		// That means its much further in and we have to find that node
		else
		{
			if (node->previous->sibling == node && node->sibling == nullptr)
			{
				node->previous->sibling = nullptr;
			}
			else if (node->previous->child == node && node->sibling == nullptr)
			{
				node->previous->child = nullptr;
			}
			else if (node->sibling != nullptr)
			{
				if (node->previous->child == node)
				{
					node->previous->child = node->sibling;
				}
				else if (node->previous->sibling == node)
				{
					node->previous->sibling = node->sibling;
				}
			}
			node->previous = nullptr;
			node->elt = new_value;
			root = meld(node, root);
		}
		
    } // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE & val) {
        // TODO: Implement this function

		// If it is empty, set our root to it
		if (sizes == 0)
		{
			Node *temp = new Node(val);
			root = temp;
			sizes += 1; // Size should become 1 now
			return root;
		}
		// Else there are already 1 nodes in the structure
		// Create a new single node 
		Node *temp = new Node(val);
		// The root is returned by meld
		root = meld(root, temp);
		sizes += 1;
        return temp; 

    } // addNode()


private:
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
	
	std::size_t sizes;
	Node *root;

	// They are root nodes
	Node *meld(Node *parent_one, Node *parent_two)
	{
		if (parent_one == nullptr && parent_two != nullptr)
		{
			return parent_two;
		}
		else if (parent_one != nullptr && parent_two == nullptr)
		{
			return parent_one;
		}
		else if (parent_one == nullptr && parent_two == nullptr)
		{
			return nullptr;
		}
		// Compare the two nodes, true if parent_one < parent_two
		if (this->compare(parent_one->elt, parent_two->elt))
		{
			// Whichever one is smaller becomes the child of the other(parent_one is now child of parent_two)
			parent_one->sibling = parent_two->child;
			if (parent_two->child != nullptr)
			{
				parent_two->child->previous = parent_one;
			}
			parent_two->child = parent_one;
			parent_one->previous = parent_two;
			parent_two->previous = nullptr;
			return parent_two;
		}
		// True if parent_two < parent_one
		else
		{
			parent_two->sibling = parent_one->child;
			if (parent_one->child != nullptr)
			{
				parent_one->child->previous = parent_two;
			}
			parent_one->child = parent_two;
			parent_two->previous = parent_one;
			parent_one->previous = nullptr;
			return parent_one;
		}
	}
};


#endif // PAIRINGPQ_H
