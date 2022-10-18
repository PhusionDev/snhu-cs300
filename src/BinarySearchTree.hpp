//
//  BinarySearchTree.hpp
//  CS300-Project1
//
//  Created by Jared Hodgkins on 9/23/22.
//

#ifndef BinarySearchTree_hpp
#define BinarySearchTree_hpp

#include <iostream>
#include <string>
#include <unordered_map>
#include "Course.hpp"

struct TreeNode
{
    Course course;
    TreeNode *left;
    TreeNode *right;

    // default constructor
    TreeNode()
    {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    TreeNode(Course aCourse) : TreeNode()
    {
        course = aCourse;
    }
};

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree
{
private:
    TreeNode *root;
    unsigned int _size;
    std::unordered_map<std::string, int> prereqWeights; // map course number to prereq weight
    
    void addNode(TreeNode *node, Course course);
    void removeNode(TreeNode *node);
    void deleteNode(TreeNode *node);
    void deleteTreeRecursive(TreeNode *node);
    void inOrder(TreeNode *node);
    void preOrder(TreeNode *node);
    void postOrder(TreeNode *node);
    TreeNode *searchNode(std::string courseNumber);
    TreeNode *searchNodeRecursive(TreeNode *node, std::string courseNumber);
    TreeNode *getParent(std::string bidId);
    TreeNode *getParentRecursive(TreeNode *subtreeRoot, std::string courseNumber);
    void printNode(TreeNode *node);
    void printSize();

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void Remove(std::string bidId);
    void InOrder();
    void PreOrder();
    void PostOrder();
    int Size();
    Course *Search(std::string courseNumber);
};
#endif /* BinarySearchTree_hpp */
