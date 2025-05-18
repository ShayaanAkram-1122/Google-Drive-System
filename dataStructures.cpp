#include "dataStructures.h"

// Constructor
dataStructures::dataStructures() {
    topNode   = nullptr;
    front     = nullptr;
    rear      = nullptr;
    numOfItems = 0;
    head      = nullptr;
    size      = 0;
    root      = nullptr;
    vertices  = 0;
    adjList   = nullptr;
}

// Destructor
dataStructures::~dataStructures() {
    // Free graph memory
    if (adjList != nullptr) {
        for (int i = 0; i < vertices; i++) {
            GraphNode* current = adjList[i];
            while (current != nullptr) {
                GraphNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] adjList;
    }
}

// ---------------- STACK ----------------
void dataStructures::push(int d) {
    Node* newNode = new Node();
    newNode->data = d;
    newNode->next = topNode;
    newNode->prev = nullptr;
    topNode = newNode;
}

int dataStructures::pop() {
    if (topNode == nullptr) throw "Stack is empty!";
    int val = topNode->data;
    Node* temp = topNode;
    topNode = topNode->next;
    delete temp;
    return val;
}

// ---------------- QUEUE ----------------
void dataStructures::enqueue(int value) {
    Node* newNode = new Node();
    newNode->data = value;
    newNode->next = nullptr;
    newNode->prev = nullptr;

    if (rear != nullptr)
        rear->next = newNode;
    rear = newNode;
    if (front == nullptr)
        front = newNode;
    numOfItems++;
}

bool dataStructures::dequeue(int& value) {
    if (front == nullptr) return false;
    value = front->data;
    Node* temp = front;
    front = front->next;
    delete temp;
    if (front == nullptr)
        rear = nullptr;
    numOfItems--;
    return true;
}

// ---------------- DOUBLY LINKED LIST ----------------
bool dataStructures::insert(int data, int index) {
    // append if index == -1
    if (index == -1) index = size + 1;
    if (index < 1 || index > size + 1) return false;

    Node* newNode = new Node();
    newNode->data = data;
    newNode->next = nullptr;
    newNode->prev = nullptr;

    if (index == 1) {
        // insert at head
        newNode->next = head;
        if (head) head->prev = newNode;
        head = newNode;
    } else {
        // walk to node before the insertion point
        Node* curr = head;
        for (int i = 1; i < index - 1; i++) {
            curr = curr->next;
        }
        newNode->next = curr->next;
        newNode->prev = curr;
        if (curr->next) curr->next->prev = newNode;
        curr->next = newNode;
    }
    size++;
    return true;
}

bool dataStructures::deleteNode(int index) {
    if (index < 1 || index > size) return false;

    Node* curr = head;
    if (index == 1) {
        head = head->next;
        if (head) head->prev = nullptr;
        delete curr;
    } else {
        for (int i = 1; i < index; i++) {
            curr = curr->next;
        }
        curr->prev->next = curr->next;
        if (curr->next) curr->next->prev = curr->prev;
        delete curr;
    }
    size--;
    return true;
}

// ---------------- BINARY TREE ----------------
TreeNode* dataStructures::insertTreeNode(TreeNode* node, int data) {
    if (node == nullptr) {
        TreeNode* newNode = new TreeNode();
        newNode->data  = data;
        newNode->left  = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
    if (data < node->data)
        node->left  = insertTreeNode(node->left, data);
    else
        node->right = insertTreeNode(node->right, data);
    return node;
}

void dataStructures::insertTree(int data) {
    root = insertTreeNode(root, data);
}

// ---------------- GRAPH ----------------
void dataStructures::addVertex(int vertexName) {
    // allocate one more slot
    GraphNode** newArr = new GraphNode*[vertices + 1];
    for (int i = 0; i < vertices; i++)
        newArr[i] = adjList ? adjList[i] : nullptr;
    newArr[vertices] = nullptr;

    delete[] adjList;
    adjList = newArr;
    vertices++;
}

bool dataStructures::addEdge(int v1, int v2) {
    if (v1 >= vertices || v2 >= vertices) return false;

    GraphNode* edge1 = new GraphNode();
    edge1->vertex = v2;
    edge1->next   = adjList[v1];
    adjList[v1]   = edge1;

    GraphNode* edge2 = new GraphNode();
    edge2->vertex = v1;
    edge2->next   = adjList[v2];
    adjList[v2]   = edge2;

    return true;
}
