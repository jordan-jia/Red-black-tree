#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

enum Color { RED, BLACK, DOUBLE_BLACK };

struct Node
{
	int data;
	int color;
	Node *left, *right, *parent;
	explicit Node(int);
};

class RBTree
{
private:
	Node *root;
protected:
	void rotateLeft(Node *&);
	void rotateRight(Node *&);
	void fixInsertRBTree(Node *&);
	void fixDeleteRBTree(Node *&);
	void inorderBST(Node *&, ofstream& fout);
	int getColor(Node *&);
	void setColor(Node *&, int);
	Node *minValueNode(Node *&);
	Node* insertBST(Node *&, Node *&);
	Node* deleteBST(Node *&, int);
public:
	RBTree();
	void insertValue(int);
	void deleteValue(int);
	void inorder(ofstream& fout);
};

int main() {
	int data, work,function;
	RBTree RBT;
	string s;

	ifstream fin;
	ofstream fout;
	fin.open("input.txt");
	fout.open("outpit.txt");
	fin >> work;
	for (int i = 0; i < work; i++)
	{
		fin >> function;

		if (function == 1)
		{
			getline(fin, s);
			getline(fin, s);
			fout << "Insert:"<<s<<endl;
			istringstream ins(s);
			while (ins >> data)
			{
				RBT.insertValue(data);
			}
			RBT.inorder(fout);
		}
		else if (function == 2)
		{
			getline(fin, s);
			getline(fin, s);
			fout << "Delete:" << s << endl;
			istringstream ins(s);
			while (ins >> data)
			{
				RBT.deleteValue(data);
			}
			RBT.inorder(fout);
		}
	}

	fin.close();
	fout.close();
	return 0;
}

Node::Node(int data) {
	this->data = data;
	color = RED;
	left = right = parent = nullptr;
}

RBTree::RBTree() {
	root = nullptr;
}

int RBTree::getColor(Node *&node) {
	if (node == nullptr)
		return BLACK;

	return node->color;
}

void RBTree::setColor(Node *&node, int color) {
	if (node == nullptr)
		return;

	node->color = color;
}

Node* RBTree::insertBST(Node *&root, Node *&ptr) {
	if (root == nullptr)
		return ptr;

	if (ptr->data <= root->data) {
		root->left = insertBST(root->left, ptr);
		root->left->parent = root;
	}
	else if (ptr->data > root->data) {
		root->right = insertBST(root->right, ptr);
		root->right->parent = root;
	}

	return root;
}

void RBTree::insertValue(int n) {
	Node *node = new Node(n);
	root = insertBST(root, node);
	fixInsertRBTree(node);
}

void RBTree::rotateLeft(Node *&ptr) {
	Node *right_child = ptr->right;
	ptr->right = right_child->left;

	if (ptr->right != nullptr)
		ptr->right->parent = ptr;

	right_child->parent = ptr->parent;

	if (ptr->parent == nullptr)
		root = right_child;
	else if (ptr == ptr->parent->left)
		ptr->parent->left = right_child;
	else
		ptr->parent->right = right_child;

	right_child->left = ptr;
	ptr->parent = right_child;
}

void RBTree::rotateRight(Node *&ptr) {
	Node *left_child = ptr->left;
	ptr->left = left_child->right;

	if (ptr->left != nullptr)
		ptr->left->parent = ptr;

	left_child->parent = ptr->parent;

	if (ptr->parent == nullptr)
		root = left_child;
	else if (ptr == ptr->parent->left)
		ptr->parent->left = left_child;
	else
		ptr->parent->right = left_child;

	left_child->right = ptr;
	ptr->parent = left_child;
}

void RBTree::fixInsertRBTree(Node *&ptr) {
	Node *parent = nullptr;
	Node *grandparent = nullptr;
	while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED) {
		parent = ptr->parent;
		grandparent = parent->parent;
		if (parent == grandparent->left) {
			Node *uncle = grandparent->right;
			if (getColor(uncle) == RED) {
				setColor(uncle, BLACK);
				setColor(parent, BLACK);
				setColor(grandparent, RED);
				ptr = grandparent;
			}
			else {
				if (ptr == parent->right) {
					rotateLeft(parent);
					ptr = parent;
					parent = ptr->parent;
				}
				rotateRight(grandparent);
				swap(parent->color, grandparent->color);
				ptr = parent;
			}
		}
		else {
			Node *uncle = grandparent->left;
			if (getColor(uncle) == RED) {
				setColor(uncle, BLACK);
				setColor(parent, BLACK);
				setColor(grandparent, RED);
				ptr = grandparent;
			}
			else {
				if (ptr == parent->left) {
					rotateRight(parent);
					ptr = parent;
					parent = ptr->parent;
				}
				rotateLeft(grandparent);
				swap(parent->color, grandparent->color);
				ptr = parent;
			}
		}
	}
	setColor(root, BLACK);
}

void RBTree::fixDeleteRBTree(Node *&node) {
	if (node == nullptr)
		return;

	if (node == root) {
		root = nullptr;
		return;
	}

	if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED) {
		Node *child = node->left != nullptr ? node->left : node->right;

		if (node == node->parent->left) {
			node->parent->left = child;
			if (child != nullptr)
				child->parent = node->parent;
			setColor(child, BLACK);
			delete (node);
		}
		else {
			node->parent->right = child;
			if (child != nullptr)
				child->parent = node->parent;
			setColor(child, BLACK);
			delete (node);
		}
	}
	else {
		Node *sibling = nullptr;
		Node *parent = nullptr;
		Node *ptr = node;
		setColor(ptr, DOUBLE_BLACK);
		while (ptr != root && getColor(ptr) == DOUBLE_BLACK) {
			parent = ptr->parent;
			if (ptr == parent->left) {
				sibling = parent->right;
				if (getColor(sibling) == RED) {
					setColor(sibling, BLACK);
					setColor(parent, RED);
					rotateLeft(parent);
				}
				else {
					if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
						setColor(sibling, RED);
						if (getColor(parent) == RED)
							setColor(parent, BLACK);
						else
							setColor(parent, DOUBLE_BLACK);
						ptr = parent;
					}
					else {
						if (getColor(sibling->right) == BLACK) {
							setColor(sibling->left, BLACK);
							setColor(sibling, RED);
							rotateRight(sibling);
							sibling = parent->right;
						}
						setColor(sibling, parent->color);
						setColor(parent, BLACK);
						setColor(sibling->right, BLACK);
						rotateLeft(parent);
						break;
					}
				}
			}
			else {
				sibling = parent->left;
				if (getColor(sibling) == RED) {
					setColor(sibling, BLACK);
					setColor(parent, RED);
					rotateRight(parent);
				}
				else {
					if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
						setColor(sibling, RED);
						if (getColor(parent) == RED)
							setColor(parent, BLACK);
						else
							setColor(parent, DOUBLE_BLACK);
						ptr = parent;
					}
					else {
						if (getColor(sibling->left) == BLACK) {
							setColor(sibling->right, BLACK);
							setColor(sibling, RED);
							rotateLeft(sibling);
							sibling = parent->left;
						}
						setColor(sibling, parent->color);
						setColor(parent, BLACK);
						setColor(sibling->left, BLACK);
						rotateRight(parent);
						break;
					}
				}
			}
		}
		if (node == node->parent->left)
			node->parent->left = nullptr;
		else
			node->parent->right = nullptr;
		delete(node);
		setColor(root, BLACK);
	}
}

Node* RBTree::deleteBST(Node *&root, int data) {
	if (root == nullptr)
		return root;

	if (data < root->data)
		return deleteBST(root->left, data);

	if (data > root->data)
		return deleteBST(root->right, data);

	if (root->left == nullptr || root->right == nullptr)
		return root;

	Node *temp = minValueNode(root->right);
	root->data = temp->data;
	return deleteBST(root->right, temp->data);
}

void RBTree::deleteValue(int data) {
	Node *node = deleteBST(root, data);
	fixDeleteRBTree(node);
}

void RBTree::inorderBST(Node *&ptr, ofstream& fout) {
	if (ptr == nullptr)
		return;

	inorderBST(ptr->left,fout);
	fout << "key: " << ptr->data<<" ";

	if (ptr->parent==nullptr)
		fout << "parent:   " ;
	else
		fout << "parent: " << ptr->parent->data << " ";

	if (ptr->color == 0)
		fout << "color: red"<<endl;
	else
		fout << "color: black" << endl;
	inorderBST(ptr->right,fout);
}

void RBTree::inorder(ofstream& fout) {
	inorderBST(root,fout);
}


Node *RBTree::minValueNode(Node *&node) {

	Node *ptr = node;

	while (ptr->left != nullptr)
		ptr = ptr->left;

	return ptr;
}

