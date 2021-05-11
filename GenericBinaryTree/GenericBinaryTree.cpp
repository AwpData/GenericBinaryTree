#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

template <class T>
struct TreeNode {
	T value;
	TreeNode* left;
	TreeNode* right;

	TreeNode(T value, TreeNode* left = nullptr, TreeNode* right = nullptr) {
		this->value = value;
		this->left = left;
		this->right = right;
	}
};

template <class T>
class Tree {
private:
	TreeNode<T>* root;

public:
	Tree() {
		root = nullptr;
	}

	// These are all public methods that will use the first TreeNode "Root" (for encapsulation and efficiency) so the user can't enter it in 
	void insert(T value) {
		insert(value, root);
	}

	void displayInorder() {
		displayInorder(root);
	}

	void displayPreorder() {
		displayPreorder(root);
	}

	void displayPostorder() {
		displayPostorder(root);
	}

	bool search(T value, bool printAddress = true) {
		if (search(root, value, printAddress)) {
			return true;
		}
		else {
			return false;
		}
	}

	void remove(T value) {
		remove(root, value);
	}

private: // All private methods for encapsulation and so the user does not have to pass in the first TreeNode 

	// I used recursion because adding a treenode MUST either return that we added it or we didn't.
	// Notice how hitting one singular return statement will back out of the entire thing (because no other statements will execute) 

	void insert(T value, TreeNode<T>*& leaf) { // Inserts value into the tree (indirection operator used because we might modify address of treenode) 
		if (leaf == nullptr) { // This is either for the first node "root" or if we find a null TreeNode (as in pointing to nullptr) 
			cout << "Inserted " << value << " into tree" << endl;
			leaf = new TreeNode<T>(value); // Creates a treeNode here 
			return;
		}
		if (leaf->value == value) { // If we already have the value, don't add it 
			return;
		}
		if (value < leaf->value) { // If the value is less than the current treeNode value, we must go to the left of it 
			insert(value, leaf->left);
		}
		else if (value > leaf->value) { // If the value is greater than the current treeNode value, we must go to the right of it 
			insert(value, leaf->right);
		}
	}

	// Preorder consistently prints the leftmost values first 
	void displayPreorder(TreeNode<T>* leaf) { // Displays preorder (left, bottom, right) printing
		if (leaf != nullptr) {
			cout << leaf->value << " ";
			displayPreorder(leaf->left);
			displayPreorder(leaf->right);
		}
	}

	// Inorder consistently prints the bottom-most values first before moving to the right values
	void displayInorder(TreeNode<T>* leaf) { // Displays inorder (bottom, left, right) printing
		if (leaf != nullptr) {
			displayInorder(leaf->left);
			cout << leaf->value << " ";
			displayInorder(leaf->right);
		}
	}

	// Postorder consistently prints the right-most values (children) first before printing the actual parent node 
	void displayPostorder(TreeNode<T>* leaf) { // Displays preorder (right, left, bottom) printing 
		if (leaf != nullptr) {
			displayPostorder(leaf->left);
			displayPostorder(leaf->right);
			cout << leaf->value << " ";
		}
	}

	// Searches for a node by "binary searching" through the tree 
	bool search(TreeNode<T>* leaf, T value, bool printAddress) {
		if (leaf != nullptr) { // As long as our treenode is not null
			if (leaf->value == value) { // First we check if the desired value is the same as the treenode value 
				if (printAddress) { // I just have this here because I don't want to print the address when filling the tree! (only searching it) 
					cout << "Value: " << leaf->value << " found at address " << *(&leaf) << endl; // If it is, print out the address of this treenode
				}
				return true; // Stop doing recursion now, we found it. 
			}
			else if (value < leaf->value) { // Else, we go to the left if less than since that is how binary trees are built 
				search(leaf->left, value, printAddress);
			}
			else if (value > leaf->value) { // Vice versa of less than 
				search(leaf->right, value, printAddress);
			}
		}
		else { // Else statement needed for false because although recursion returns, it would have returned false instead (since I have no return statement after recursive function) 
			return false;
		}
	}

	void remove(TreeNode<T>*& tree, T value) { // To remove a value, we must pass in the indirection operator since we will be modifying addresses of our treenodes
		if (tree != nullptr) {
			if (tree->value == value) { // IF we find the value, then call the makeDeletion function with that treenode as a parameter 
				makeDeletion(tree);
			}
			else if (value < tree->value) { // Below two if-statements work similar to the search() function's traversal method 
				remove(tree->left, value);
			}
			else if (value > tree->value) {
				remove(tree->right, value);
			}
		}
	}

	void makeDeletion(TreeNode<T>*& tree) { // Deletes the desired node and moves our treeNode's around (if needed) to maintain binary tree standards 
		TreeNode* garbage = tree; // Set a deletion pointer to our desired deleted treenode 
		TreeNode* newAttachPoint; // This pointer will be used for moving treenodes around if we need it (the 2 children case) 
		if (tree->right == nullptr) { // First, if our right tree is nullptr, then we simply replace this tree node with the left tree (pass over it) 
			tree = tree->left;
		}
		else if (tree->left == nullptr) { // If our left tree is nullptr, then we simply replace this tree node with the right tree (pass over it) 
			tree = tree->right;
		}
		else { // We have 2 children 
			newAttachPoint = tree->right; // Set our 'moving' pointer to the right tree (because it is greater it will be easier to move to the left child) 
			while (newAttachPoint->left != nullptr) { // Finds the smallest value from the right tree's children
				newAttachPoint = newAttachPoint->left;
			}
			newAttachPoint->left = tree->left; // Once we find it, we will point that last node to the first tree->left node 
												// Every right treenode's value is greater than every left treenode's value!
			tree = tree->right; // Finally, assign our old tree to the right tree because now the right tree's left child's last value is attached to tree->left 
		}
		delete garbage; // Now we can delete the desired node because we reassigned tree to be tree->right 
	}
};

int main()
{
	Tree<string> t;
	t.insert("String");
	t.displayInorder();

	Tree<int> i;
	i.insert(3);
	i.displayInorder();

	Tree<double> d;
	d.insert(2.5);
	d.displayInorder();
}
