
#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
};

Node* insert(Node* root, int value) {
    if (root == NULL) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);

    return root;
}

void inorder(Node* root) {
    if (root == NULL)
        return;

    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

void findPredSucc(Node* root, int key) {
    Node* current = root;
    Node* pred = NULL;
    Node* succ = NULL;

    while (current != NULL) {
        if (key < current->data) {
            succ = current;
            current = current->left;
        }
        else if (key > current->data) {
            pred = current;
            current = current->right;
        }
        else {
            if (current->left != NULL) {
                Node* temp = current->left;
                while (temp->right != NULL)
                    temp = temp->right;
                pred = temp;
            }

            if (current->right != NULL) {
                Node* temp = current->right;
                while (temp->left != NULL)
                    temp = temp->left;
                succ = temp;
            }

            cout << "Predecessor: ";
            if (pred != NULL)
                cout << pred->data;
            else
                cout << "None";

            cout << "\nSuccessor: ";
            if (succ != NULL)
                cout << succ->data;
            else
                cout << "None";

            return;
        }
    }

    cout << "Key not found in the BST.\n";
}

void rangeRecursive(Node* root, int low, int high, int& comparisons) {
    if (root == NULL)
        return;

    comparisons++;

    if (root->data < low) {
        rangeRecursive(root->right, low, high, comparisons);
        return;
    }

    rangeRecursive(root->left, low, high, comparisons);

    comparisons++;

    if (root->data <= high) {
        cout << root->data << " ";
        rangeRecursive(root->right, low, high, comparisons);
    }
}

struct StackNode {
    Node* treeNode;
    StackNode* next;
};

void push(StackNode*& top, Node* treeNode) {
    StackNode* newNode = new StackNode;
    newNode->treeNode = treeNode;
    newNode->next = top;
    top = newNode;
}

Node* pop(StackNode*& top) {
    if (top == NULL)
        return NULL;

    StackNode* temp = top;
    Node* treeNode = temp->treeNode;
    top = top->next;
    delete temp;

    return treeNode;
}

void rangeIterative(Node* root, int low, int high, int& comparisons) {
    StackNode* top = NULL;
    Node* current = root;

    while (current != NULL || top != NULL) {
        while (current != NULL) {
            comparisons++;

            if (current->data < low) {
                current = current->right;
            }
            else {
                push(top, current);
                current = current->left;
            }
        }

        if (top != NULL) {
            current = pop(top);

            comparisons++;

            if (current->data <= high) {
                cout << current->data << " ";
                current = current->right;
            }
            else {
                break;
            }
        }
    }
}

void deleteTree(Node* root) {
    if (root == NULL)
        return;

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    Node* root = NULL;

    int values[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};

    for (int i = 0; i < 11; i++)
        root = insert(root, values[i]);

    cout << "Inorder Traversal: ";
    inorder(root);

    int key;
    cout << "\n\nEnter key to find predecessor and successor: ";
    cin >> key;

    findPredSucc(root, key);

    int low, high;
    cout << "\n\nEnter lower range: ";
    cin >> low;
    cout << "Enter upper range: ";
    cin >> high;

    if (low > high) {
        int temp = low;
        low = high;
        high = temp;
    }

    int recursiveComparisons = 0;
    int iterativeComparisons = 0;

    cout << "\nRecursive Range Query: ";
    rangeRecursive(root, low, high, recursiveComparisons);

    cout << "\nRecursive Comparisons: " << recursiveComparisons;

    cout << "\n\nIterative Range Query: ";
    rangeIterative(root, low, high, iterativeComparisons);

    cout << "\nIterative Comparisons: " << iterativeComparisons;

    deleteTree(root);

    return 0;
}
