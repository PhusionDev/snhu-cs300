//
//  BinarySearchTree.cpp
//  CS300-Project1
//
//  Created by Jared Hodgkins on 9/23/22.
//

#include <iostream>
#include <string>
#include "BinarySearchTree.hpp"
#include "Utils.hpp"

//============================================================================
// Constructor(s) & Destructor
//============================================================================

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree()
{
    root = nullptr; // root is equal to nullptr
    _size = 0;      // set initial size to 0
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree()
{
    std::cout << "Deleting Binary Search Tree...\n";
    BinarySearchTree::deleteTreeRecursive(root); // recurse from root deleting every node
}

//============================================================================
// Private Functions
//============================================================================

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course course to be added
 */
void BinarySearchTree::addNode(TreeNode *node, Course course)
{ // parent, node to insert
    // if node is larger then add to left
    if (course.courseNumber < node->course.courseNumber)
    {
        if (node->left == nullptr)
        {                                      // if no left node
            node->left = new TreeNode(course); // this node becomes left
        }
        else
        { // else recurse down the left node
            BinarySearchTree::addNode(node->left, course);
        }
    }
    else
    { // node is smaller, add to right
        if (node->right == nullptr)
        {                                       // if no right node
            node->right = new TreeNode(course); // this node becomes right
        }
        else
        { // else recurse down the left node
            BinarySearchTree::addNode(node->right, course);
        }
    }
}

/**
 * Heavy lifting function to remove a node from the
 * BST. Checks whether the node to be removed has
 * children or not and then manipulates the tree
 * accordingly.
 *
 * @param node the node to remove
 */
void BinarySearchTree::removeNode(TreeNode *node)
{
    if (node == nullptr)
    { // default case, node is null, nothing to remove
        return;
    }

    // get parent node of node
    TreeNode *parent = BinarySearchTree::getParent(node->course.courseNumber);

    // case 1: internal node with 2 children
    if (node->left != nullptr && node->right != nullptr)
    {
        // find successor and successor's parent
        TreeNode *succNode = node->right;
        TreeNode *succParent = node;
        while (succNode->left != nullptr)
        {                              // iterate to find final successor node
            succParent = succNode;     // set parent to current node before iteration
            succNode = succNode->left; // set next node by traversing left through tree
        }
        // final successor node reached
        std::cout << "Replacing node's course at " << node << " which currently contains course titled: \"" << node->course.title << "\" with course titled: \"" << succNode->course.title << "\" at " << succNode << " and then deleting successor node\n";
        node->course = succNode->course; // copy course from successor to node
        succParent->left = nullptr;      // clear pointer from successor's parent
        node = succNode;                 // set node to successor to be deleted
    }
    else if (node == root)
    { // case 2: root node (with 1 or 0 children)
        if (node->left != nullptr)
        {                      // root has a left child
            root = node->left; // redirect root to the node's left (next node)
        }
        else
        {                       // root has a right child
            root = node->right; // redirect root to the node's right (next node)
        }
    }
    else if (node->left != nullptr)
    { // case 3: internal with left child only
        if (parent->left == node)
        {                              // current node is the parent's left node
            parent->left = node->left; // redirect parent's left to current node's left
        }
        else
        {                               // current node is the parent's right node
            parent->right = node->left; // redirect parent's right to current node's left
        }
    }
    else
    { // case 4: internal with right child only or is leaf
        if (parent->left == node)
        {                               // current node is the parent's left node
            parent->left = node->right; // redirect parent's left to current node's right
        }
        else
        {                                // current node is the parent's right node
            parent->right = node->right; // redirect parent's right to current node's right
        }
    }

    BinarySearchTree::deleteNode(node); // delete the node from the tree (free memory)
}

/**
 * A method to delete a node and free up its memory
 * and displays some debugging information.
 *
 * @param node the node to remove from the BST
 */
void BinarySearchTree::deleteNode(TreeNode *node)
{
    if (node != nullptr)
    {
        std::cout << "Deleting Node at: " << node << " Title: \"" << node->course.title << "\"\n";
        node->left = nullptr;  // clear node's left pointer
        node->right = nullptr; // clear node's right pointer
        delete node;           // delete node from memory
        node = nullptr;        // point node to null
        _size--;               // decrement size
    }
}

/**
 * Recursive function to delete nodes in
 * a binary search tree starting from a
 * specified node
 *
 * @param node the node in which to start deleting from
 */
void BinarySearchTree::deleteTreeRecursive(TreeNode *node)
{
    if (node == nullptr)
    { // default case, nothing to delete
        return;
    }

    BinarySearchTree::deleteTreeRecursive(node->left);  // recursively delete from node's left
    BinarySearchTree::deleteTreeRecursive(node->right); // recursively delete from node's right

    delete node;    // delete node
    node = nullptr; // assign node pointer to null
    _size--;        // decrement size
}

/**
 * Recursively traverse a BST in order
 *
 * @param node the node to start or continue traversal from
 */
void BinarySearchTree::inOrder(TreeNode *node)
{
    if (node == nullptr)
    {
        return;
    }
    BinarySearchTree::inOrder(node->left); // traverse left through tree
    utils::printCourse(&(node->course));
    BinarySearchTree::inOrder(node->right); // traverse right through tree
}

/**
 * Root method that utilizes a recursive function
 * to search for a node with given course number
 *
 * @param courseNumber the course number to search for
 * @return a node pointer to a matching node if found, otherwise nullptr
 */
TreeNode *BinarySearchTree::searchNode(std::string courseNumber)
{
    // recursively search through BST for node with matching course number
    return BinarySearchTree::searchNodeRecursive(root, courseNumber);
}

/**
 * Recursive method to search through a tree
 * to find a node containing a course with matching
 * course number.
 *
 * @param node the node to search
 * @param courseNumber the course number to match
 * @return a node pointer to a matching node if found, or nullptr if not found
 */
TreeNode *BinarySearchTree::searchNodeRecursive(TreeNode *node, std::string courseNumber)
{
    if (node != nullptr)
    { // make sure current node is not null
        if (node->course.courseNumber == courseNumber)
        {
            return node; // current node is a match, return it
        }
        else if (node->course.courseNumber > courseNumber)
        {
            // current node's course number is larger, search left through tree
            return BinarySearchTree::searchNodeRecursive(node->left, courseNumber);
        }
        else
        {
            // current node's course number is smaller, search right through tree
            return BinarySearchTree::searchNodeRecursive(node->right, courseNumber);
        }
    }
    return nullptr; // match not found
}

/**
 * Method to get a course's parent node
 * This method assumes that the child node
 * already exists and has been verified
 * prior to calling.
 *
 * @param courseNumber the course number to search for
 * @return a node pointer to the course's parent node if it exists
 */
TreeNode *BinarySearchTree::getParent(std::string courseNumber)
{
    return BinarySearchTree::getParentRecursive(root, courseNumber);
}

/**
 * Recursively search through a tree to find a course's
 * parent node. This function assumes that the child node
 * already exists and has been verified prior to calling
 *
 * @param subtreeRoot the node from which to begin searching
 * @param courseNumber the course number to search for
 * @return a node pointer to the parent node of the course
 */
TreeNode *BinarySearchTree::getParentRecursive(TreeNode *subtreeRoot, std::string courseNumber)
{
    if (subtreeRoot == nullptr)
    { // reached end of tree
        return nullptr;
    }

    if (subtreeRoot->left->course.courseNumber == courseNumber || subtreeRoot->right->course.courseNumber == courseNumber)
    {
        // the current node is the parent of the node with given course number
        return subtreeRoot; // return current node
    }

    if (courseNumber < subtreeRoot->course.courseNumber)
    { // current node is greater than course number
        // search left in the tree
        return BinarySearchTree::getParentRecursive(subtreeRoot->left, courseNumber);
    }
    // current node is less than the course number -- search right in the tree
    return BinarySearchTree::getParentRecursive(subtreeRoot->right, courseNumber);
}

/**
 * Helper function to print information
 * about the tree's current size
 */
void BinarySearchTree::printSize()
{
    std::cout << "There are " << _size << " entries in tree.\n";
}

//============================================================================
// Public Functions
//============================================================================

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course)
{
    _size++; // insert should always add a node, increment size
    if (root == nullptr)
    {                                // check if root node is null
        root = new TreeNode(course); // root is null, make course the root node
    }
    else
    {
        // recursively search for node to insert course into
        BinarySearchTree::addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(std::string courseNumber)
{
    // get the node to be removed containing course with given course number
    TreeNode *node = BinarySearchTree::searchNode(courseNumber);
    if (node != nullptr)
    {
        BinarySearchTree::removeNode(node);
        return;
    }
    // node doesn't exist
    std::cout << "course number: " << courseNumber << " not found, nothing to remove...\n";
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder()
{
    BinarySearchTree::inOrder(root); // call inOrder fuction and pass root
    BinarySearchTree::printSize();
}

/**
 * Report the number of nodes in the
 * Binary search tree.
 *
 * @return the number of nodes in tree
 */
int BinarySearchTree::Size()
{
    return _size;
}

/**
 * Search for a course
 */
Course *BinarySearchTree::Search(std::string courseNumber)
{
    Course *course = nullptr; // create pointer to course starting as nullptr

    TreeNode *current = root; // set current node equal to root
    if (current == nullptr)
    { // default case, root is empty
        return course;
    }

    // keep iterating through tree until bottom reached or matching courseId found
    while (current != nullptr)
    {
        if (current->course.courseNumber == courseNumber)
        {
            // if match found, return current course
            return &(current->course);
        }
        // if course is smaller than current node then traverse left
        if (courseNumber < current->course.courseNumber)
        {
            current = current->left;
        }
        else
        { // else larger so traverse right
            current = current->right;
        }
    }

    // course not found, return empty course
    return course;
}
