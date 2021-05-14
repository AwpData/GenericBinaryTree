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
class Tree {
private:
	TreeNode<T>* root;

public:
	Tree() {
		root = nullptr;
	}

	// These are all public methods that will use the first TreeNode "Root" (for encapsulation and efficiency) so the user can't enter it in 
	void insert(T value, string datatype = "") {
		insert(value, root, datatype); // ** Datatype parameter is just for output so the user can see which tree I exactly inserted it in. 
	}

	void displayInorder() {
		displayInorder(root); // I display the tree first (if there is nothing, nothing will print)
		if (root == nullptr) { // After deletion, this should be called to say that the tree is empty (it is) 
			cout << "Tree is empty!";
			return;
		}	
	}

	void removeAllPostorder() {
		removeAllPostorder(root);
		this->root = nullptr; // After we delete every node, we set the root to nullptr since nothing exists anymore 
	}

	void remove(T value) {
		remove(root, value);
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
			removeAllPostorder(leaf->left);
			removeAllPostorder(leaf->right);
			cout << "Removing: " << leaf->value << endl;
			delete leaf; // Because this is the right-most node of the left-most node, it should have no children and we are safe to delete it 
			leaf = nullptr; // Set to nullptr to avoid dangling pointers 
		}
	}
};

vector<string> split(string s, char delimiter) {
	vector<string> returned;
	int pos = 0;

	HANDLE  hConsole; // handler for console so I can change the text color for readability 
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	while (true) { // While the delimiter can still find a comma (it will stop after it can't go any more) 
		string substring = s.substr(pos, s.find(delimiter, pos) - pos); // Starts at pos 0 and then gets the substring of pos to the next comma minus pos (because it is the length, not the index) 
		returned.push_back(substring); // Pushes the substring into our vector 
		if (s.find(delimiter, pos) == -1) { // Once the finder cannot find any more commas it is time to break out of it 
			break;
		}
		pos = s.find(delimiter, pos) + 1; // else it will set position to that position of the comma + 1 for the next element 
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

int main()
{

	Tree<string> stringTree; // String tree
	Tree<double> doubleTree; // Double tree
	Tree<int> intTree; // Int tree 

	bool fileFound = false;
	string fileName;
	ifstream file;
	while (!fileFound) {
		cout << "Please enter the name of your file " << endl;
		cin >> fileName;
		try {
			file.open(fileName);
			if (file.fail()) {
				throw string("Cannot find file, try again");
			}
			cout << "Success! File found" << endl;
			fileFound = true;
		}
		catch (string s) {
			cout << s << endl;
		}
	}
	cout << "\n";
	string parser = "";
	vector<string> substrings;
	while (getline(file, parser)) {
		try {
			substrings = split(parser, ',');
			if (substrings.size() == 0) {
				throw;
			}
			for (int i = 0; i < substrings.size(); i++) {
				if ((substrings[0][0] >= 'A' && substrings[0][0] <= 'Z') || (substrings[0][0] >= 'a' && substrings[0][0] <= 'z')) { // First it gets the string at vector element i, then it checks the first char to test for letter
					try {
						if (substrings[i][0] >= '0' && substrings[i][0] <= '9') {
							throw string("Error, the string '" + substrings[i] + "' is an integer/double, but the line should be made of letters, skipping...");
						}
						stringTree.insert(substrings[i], "string");
					}
					catch (string s) {
						cout << s << endl;
					}
				}
				else if (substrings[0].find('.') != string::npos) { // This is if the first character was a double value, thus we compare others for 
					try {
						if (substrings[i][0] >= 'A' && substrings[i][0] <= 'z') {
							throw string("Error, the string '" + substrings[i] + "' has letters in it, but the line should be made of doubles, skipping...");
						}
						if (substrings[i].find('.') == string::npos) {
							throw string("Error, the string '" + substrings[i] + "' is an integer, but the line should be made of doubles, skipping...");
						}
						double d = stod(substrings[i]);
						doubleTree.insert(d, "double");
					}
					catch (string s) {
						cout << s << endl;
					}
				}
				else {
					try {
						if (substrings[i][0] >= 'A' && substrings[i][0] <= 'z') {
							throw string("Error, the string '" + substrings[i] + "' has letters in it, but the line should be made of integers, skipping...");
						}
						if (substrings[i].find('.') == true) {
							throw string("Error, the string '" + substrings[i] + "' is a double, but the line should be made of integers, skipping...");
						}
						int num = stoi(substrings[i]);
						intTree.insert(num, "integer");
					}
					catch (string s) {
						cout << s << endl;
					}
				}
			}
		}
		catch (exception e) {
			cout << "Line is empty, moving on to next line" << endl;
		}
		cout << "\n";
	}

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
	cout << "\nDeleting all trees: " << endl;
	stringTree.removeAllPostorder();
	cout << endl;
	doubleTree.removeAllPostorder();
	cout << endl;
	intTree.removeAllPostorder();
	cout << endl;

	// Then I print the trees again to prove that it actually deleted everything 
	cout << "Printing trees (post-deletion): " << endl;
	cout << "String tree: ";
	stringTree.displayInorder();
	cout << endl;

	cout << "Double tree: ";
	doubleTree.displayInorder();
	cout << endl;

	cout << "Integer tree: ";
	intTree.displayInorder();
	cout << endl;

	file.close();
}
