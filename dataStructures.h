#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <iostream>
using namespace std;

// Node used for Stack, Queue, and Doubly Linked List
struct Node {
    int data;
    Node* next;
    Node* prev;
};

// Node for Binary Tree
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
};

// Node for Graph
struct GraphNode {
    int vertex;
    GraphNode* next;
};

class dataStructures {
private:
    Node* topNode;
    Node* front;
    Node* rear;
    int numOfItems;

    Node* head;
    int size;

    TreeNode* root;

    GraphNode** adjList;
    int vertices;

    TreeNode* insertTreeNode(TreeNode* node, int data);

public:
    dataStructures();
    ~dataStructures();

    // Stack
    void push(int d);
    int pop();

    // Queue
    void enqueue(int value);
    bool dequeue(int& value);

    // Doubly Linked List
    bool insert(int data, int index = -1);
    bool deleteNode(int index);

    // Binary Tree
    void insertTree(int data);

    // Graph
    void addVertex(int vertexName);
    bool addEdge(int vertex1, int vertex2);
};

#endif
