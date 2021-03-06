// Trevor Tang 5/14/21
#include <iostream>
#include <stack> // STL
#include <vector> // STL 
#include <fstream>
#include <string>
#include <windows.h>  // Text coloring (Just for fun) 

#pragma warning(disable : 4996) // This is so I can use stod and stoi

using namespace std;

template <class T>
struct TreeNode { // TreeNode Struct that is templated for all data types 
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
class Tree { // Templated tree class 
private:
	TreeNode<T>* root;

public: // These public methods don't contain the treenode because the user shouldn't have to put in the root node to start (its inefficient and redundant) 
	Tree() {
		root = nullptr;
	}

	// These are all public methods that will use the first TreeNode "Root" (for encapsulation and efficiency) so the user can't enter it in 
	void insert(T value, string datatype = "") {
		insert(value, root, datatype); // ** 'datatype' parameter is just for output so the user can see which tree I exactly inserted it in. 
	}

	void displayInorder() { // Displays the tree in ascended order 
		displayInorder(root); // I display the tree first (if there is nothing, nothing will print)
		if (root == nullptr) { // After deletion methods are called, this should be true as the tree is empty (it is) 
			cout << "Tree is empty!";
			return;
		}
	}

	void removeAllPostorder() { // Removes all nodes with postorder traversal 
		removeAllPostorder(root);
		this->root = nullptr; // After we delete every node, we set the root to nullptr since nothing exists anymore 
	}

private: // All private methods for encapsulation and so the user does not have to pass in the first TreeNode 

	// I used recursion because adding a treenode MUST either return that we added it or we didn't.
	// Notice how hitting one singular return statement will back out of the entire thing (because no other statements will execute) 

	void insert(T value, TreeNode<T>*& leaf, string datatype) { // Inserts value into the tree (indirection operator used because we might modify address of treenode) 
		if (leaf == nullptr) { // This is either for the first node "root" or if we find a null TreeNode (as in pointing to nullptr) 
			cout << "Inserted '" << value << "' into " << datatype << " tree" << endl;
			leaf = new TreeNode<T>(value); // Creates a treeNode here 
			return;
		}
		if (leaf->value == value) { // If we already have the value, don't add it 
			cout << "Error, cannot insert " << value << " because it already exists in the " << datatype << " tree" << endl;
			return;
		}
		if (value < leaf->value) { // If the value is less than the current treeNode value, we must go to the left of it 
			insert(value, leaf->left, datatype);
		}
		else if (value > leaf->value) { // If the value is greater than the current treeNode value, we must go to the right of it 
			insert(value, leaf->right, datatype);
		}
	}

	void displayInorder(TreeNode<T>* leaf) {
		stack<TreeNode<T>*> treeNodes; // Create a stack of generic TreeNodes
		while (leaf != nullptr || !treeNodes.empty()) { // While our tree exists or the stack isn't empty
			while (leaf != nullptr) { // This will traverse to a null node after the left-most treenode 
				treeNodes.push(leaf); // First we must push the parent node to remember to go back to it 
				leaf = leaf->left; // Then we keep going left 
			}
			leaf = treeNodes.top(); // We hit the null node after the left-most pointer, so now I must get the left-most node by getting the top of the stack
			cout << leaf->value << " "; // Print out that value 
			treeNodes.pop(); // Then remove it 
			leaf = leaf->right; // Finally, we rinse and repeat with the right node of the left-most node (if it exists) 
			// If there are no more left nodes, then the stack will keep popping until we get to another right node that exists at the root node (and then we check for left nodes in the right node and so on)
		}
	}

	void removeAllPostorder(TreeNode<T>* leaf) { // Removes all nodes through postorder traversal (left, right, then value) 
		if (leaf != nullptr) {
			removeAllPostorder(leaf->left); // Will keep recursively go left until leaf is not nullptr 
			removeAllPostorder(leaf->right); // Then it will see if there is any right value at that left node (and again, see if that right node has a left node and so on) 
			cout << "Removing: " << leaf->value << endl;
			delete leaf; // Because this is the right-most node of the left-most node, it should have no children and we are safe to delete it 
			leaf = nullptr; // Set to nullptr to avoid dangling pointers 
		}
	}
};

vector<string> split(string s, char delimiter, boolean isDigit = false) { // Splits the string into substrings with delimiter being ','
	vector<string> returned;
	int pos = 0;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // handler for console so I can change the text color for readability 

	while (true) { // While the delimiter can still find a comma (it will stop after it can't go any more) 
		string substring = s.substr(pos, s.find(delimiter, pos) - pos); // Starts at pos 0 and then gets the substring of pos to the next comma minus pos (because it is the length, not the index) 
		if (substring[0] == ' ') { // This checks if the first index of substring as a space (because comma separated lists can have spaces between each value) 
			substring = substring.substr(1); // Therefore, I get rid of it by substringing the substring to the next character
		}
		returned.push_back(substring); // Pushes the substring into our vector 
		if (s.find(delimiter, pos) == -1) { // Once the finder cannot find any more commas it is time to break out of it 
			break;
		}
		pos = s.find(delimiter, pos) + 1; // else it will set position to that position of the comma + 1 to make the next substring
	}
	SetConsoleTextAttribute(hConsole, 11); // Sets console text color to LIGHT BLUE 
	cout << "Line: ";
	for (int i = 0; i < returned.size(); i++) { // Just prints out the line of text (the vector's elements) that was parsed 
		cout << returned[i] << " ";
	}
	cout << endl;
	SetConsoleTextAttribute(hConsole, 15); // Sets console text color to WHITE 
	return returned;
}

int main(int argc, char* argv[])
{
	Tree<string> stringTree; // String tree
	Tree<double> doubleTree; // Double tree
	Tree<int> intTree; // Int tree 

	string parser = "";
	ifstream file;

	try {
		string fileName = argv[1]; // Gets the filename from command arguments 
		file.open(fileName); // Attempts to open the file 
		if (file.fail()) { // If file doesn't exist 
			throw string("Cannot find file '" + fileName + "', restart program"); // Throw point for an unknown file 
		}
		cout << "Success! File '" << fileName << "' found" << endl; // If the file does exist 
	}
	catch (exception e) {
		cout << "Error, no file name" << endl;
	}
	catch (string s) {
		cout << s << endl;
	}


	// File parsing (with invalid file exception handling) 
	cout << "\n";
	vector<string> substrings;
	while (getline(file, parser)) { // While there is a line available to assign to our parser variable 
		try {
			substrings = split(parser, ',');
			// First, I use the split() method to split the line into substrings in order to make up the vector's elements 
			if (substrings.size() == 0) { // However, if the line is blank, then it will show an error message and move on to the next line 
				throw;
			}
			for (int i = 0; i < substrings.size(); i++) {
				if ((substrings[0][0] >= 'A' && substrings[0][0] <= 'Z') || (substrings[0][0] >= 'a' && substrings[0][0] <= 'z')) { // First it gets the string at vector element i, then it checks the first char to test for letter
					try {
						if (substrings[i][0] >= '0' && substrings[i][0] <= '9') { // If we have a character that is a number, throw an exception!
							throw string("Error, the string '" + substrings[i] + "' is an integer/double, but the line should be made of letters, skipping...");
						}
						stringTree.insert(substrings[i], "string"); // Else, add the string to the string tree 
					}
					catch (string s) {
						cout << s << endl;
					}
				}
				else if (substrings[0].find('.') != string::npos) { // This is if the first character was a double value
					try {
						double d = stod(substrings[i]); // conversion of string to double 
						doubleTree.insert(d, "double"); // inserting value into double tree 
					}
					catch (invalid_argument) // Catches stod exception: invalid_argument
					{
						cout << "Stod conversion could not be completed on invalid argument '" << substrings[i] << "'" << endl;
					}
					catch (out_of_range) { // Catches stod exception: out_of_range
						cout << "The double value '" << substrings[i] << "' is out of range of all possible double values" << endl;
					}
				}
				else { // The value is an integer value (because first character is a number and there is no decimal point) 
					try {
						int num = stoi(substrings[i]); // Converts string to integer 
						intTree.insert(num, "integer"); // Adds value to integer tree 
					}
					catch (invalid_argument) // Catches stoi exception: invalid_argument
					{
						cout << "Stoi conversion could not be completed on invalid argument '" << substrings[i] << "'" << endl;
					}
					catch (out_of_range) { // Catches stoi exception: out_of_range
						cout << "The integer value '" << substrings[i] << "' is out of range of all possible integer values (-2,147,483,648 to 2,147,483,647)" << endl;
					}
				}
			}
			cout << "\n";
			// Time to print every tree in ascending order (hence "inorder" traversal)
			cout << "Printing trees (Inorder): " << endl;
			cout << "String tree: ";
			stringTree.displayInorder();
			cout << endl;

			cout << "Double tree: ";
			doubleTree.displayInorder();
			cout << endl;

			cout << "Integer tree: ";
			intTree.displayInorder();
			cout << endl;

			// After printing, I will now remove all nodes from each tree with postorder traversal 
			cout << "\nDeleting tree: " << endl;
			stringTree.removeAllPostorder();
			cout << endl;
			doubleTree.removeAllPostorder();
			cout << endl;
			intTree.removeAllPostorder();
			cout << endl;
		}
		catch (exception e) { // This catch is the blank line exception handler 
			cout << "Line is empty, moving on to next line" << endl;
		}
	}
	file.close();
}
