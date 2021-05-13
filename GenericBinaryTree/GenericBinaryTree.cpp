#include <iostream>
#include <stack> // STL
#include <vector> // STL 
#include <fstream>
#include <string>

#pragma warning(disable : 4996) 

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
		if (root == nullptr) {
			cout << "Tree is empty!" << endl;
			return;
		}
		displayInorder(root);
	}

	void removeAllPostorder() {
		removeAllPostorder(root);
		this->root = nullptr;
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


	void displayInorder(TreeNode<T>* leaf) {
		stack<TreeNode<T>*> treeNodes; // Create a stack of generic TreeNodes
		while (leaf != nullptr || !treeNodes.empty()) { // While our tree exists or the stack isn't empty
			while (leaf != nullptr) { // This will traverse to the left-most treenode 
				treeNodes.push(leaf); // First we must push the parent node to remember to go back to it 
				leaf = leaf->left; // Then we keep going left 
			}
			leaf = treeNodes.top(); // We hit the left-most pointer, so now I must get it by getting the top 
			cout << leaf->value << " "; // Print out that value 
			treeNodes.pop(); // Then remove it 
			leaf = leaf->right; // Finally, we rinse and repeat with right
			// If there are no more left nodes, then the stack will keep popping until we get to another right node that exists (and then we check for left nodes and so on)
		}
	}

	void removeAllPostorder(TreeNode<T>* leaf) { // Displays preorder (right, left, bottom) printing 
		if (leaf != nullptr) {
			removeAllPostorder(leaf->left);
			removeAllPostorder(leaf->right);
			cout << "Removing: " << leaf->value << endl;
			delete leaf;
		}
	}
};

vector<string> split(string s, char delimiter) {
	vector<string> returned;
	int pos = 0;
	while (true) { // While the delimieter can still find a comma (it will stop after it can't go any more) 
		string substring = s.substr(pos, s.find(delimiter, pos) - pos); // Starts at pos 0 and then gets the substring of pos to the next comma minus pos (because it is the length, not the index) 
		returned.push_back(substring); // Pushes the substring into our vector 
		if (s.find(delimiter, pos) == -1) { // Once the finder cannot find any more commas it is time to break out of it 
			break;
		}
		pos = s.find(delimiter, pos) + 1; // else it will set position to that position of the comma + 1 for the next element 
	}
	for (int i = 0; i < returned.size(); i++) {
		cout << returned[i] << " ";
	}
	cout << endl;
	return returned;

}

int main()
{
	Tree<string> stringTree;
	Tree<double> doubleTree;
	Tree<int> intTree;

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
	// WORK ON ERROR PARSING FOR STOD AND STOI 
	string parser = "";
	vector<string> substrings;
	while (getline(file, parser)) {
		try {
			substrings = split(parser, ',');
			if (substrings.size() == 0) {
				throw;
			}
			for (int i = 0; i < substrings.size(); i++) {
				if ((substrings[i][0] >= 'A' && substrings[i][0] <= 'Z') || (substrings[i][0] >= 'a' && substrings[i][0] <= 'z')) { // First it gets the string at vector element i, then it checks the first char to test for letter
					stringTree.insert(substrings[i]);
				}
				else if (substrings[i].find('.') != string::npos) {
					try {
						double d = stod(substrings[i]);
						doubleTree.insert(d);
					}
					catch (exception e) {
						cout << "Could not convert " << substrings[i] << " due to formatting issues, skipping..." << endl;
					}
				}
				else {
					try {
						int num = stod(substrings[i]);
						intTree.insert(i);
					}
					catch (exception e) {
						cout << "Could not convert " << substrings[i] << " due to formatting issues, skipping..." << endl;
					}
				}
			}
		}
		catch (exception e) {
			cout << "Vector was empty, moving on to next line" << endl;
		}
	}
	stringTree.displayInorder();
	cout << endl;
	doubleTree.displayInorder();
	cout << endl;
	intTree.displayInorder();
	cout << endl;

	cout << "\nDeleting all trees: " << endl;
	stringTree.removeAllPostorder();
	cout << endl;
	doubleTree.removeAllPostorder();
	cout << endl;
	intTree.removeAllPostorder();

	cout << endl;
	stringTree.displayInorder();
	cout << endl;
	doubleTree.displayInorder();
	cout << endl;
	intTree.displayInorder();
}
