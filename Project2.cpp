//============================================================================
// Name        : Project2.cpp
// Author      : Tyler Lafrance
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> prerequisites;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {

    // Root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {

    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    
    // Call inOrder fuction and pass root
    inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    
    // If root equal to nullptr
    if(root == nullptr) {

        // Root is equal to new node course
        root = new Node(course);
    } else {

        // Add Node root and course
        addNode(root, course);
    }

}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    
    // Set current node equal to root
    Node* currentNode = root;

    // Keep looping downwards until bottom reached or matching courseId found
    while(currentNode != nullptr) {

        // If match found, return current course
        if(currentNode->course.courseId == courseId) {
            return currentNode->course;
        }

        // If course is smaller than current node then traverse left
        if(currentNode->course.courseId > courseId) {

        currentNode = currentNode->left;
        } else {

        // Else larger so traverse right
        currentNode = currentNode->right;
        }
    }

    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    
    // If node is larger then add to left
    if(node->course.courseId > course.courseId) {

        // If no left node
        if(node->left == nullptr) {

            // This node becomes left
            node->left = new Node(course);

        } else {

            // Else recurse down the left node
            addNode(node->left, course);
        }
        
    } else {

        // If no right node
        if(node->right == nullptr) {

            // This node becomes right
            node->right = new Node(course);

        } else {

            // Else recurse down the right node
            addNode(node->right, course);
        }
    }

}

void BinarySearchTree::inOrder(Node* node) {
      
    // If node is not equal to nullptr
    if(node != nullptr) {

        // inOrder node left
        inOrder(node->left);

        // Output courseID and title
        cout << node->course.courseId << ", " << node->course.title << endl;

        // inOrder right
        inOrder(node->right);

    }

}

/**
 * Shows the users options for the program
 */
void showMenu() {
    cout << endl;
    cout << "   1. Load Data Structure." << endl;
    cout << "   2. Print Course List." << endl;
    cout << "   3. Print Course." << endl;
    cout << "   9. Exit" << endl;
    cout << endl;
    cout << "What would you like to do? ";
}

/**
 * Load a text file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* courseTree) {

    ifstream file(csvPath);

    // Check to see if file was opened
    if(file.is_open()) {

        string line;

        // Read CSV line by line and add course information
        while (getline(file, line)) {
            
            // Break up line by comma and put in splitLine
            stringstream s_stream(line);
            vector<string> splitLine;

            while(s_stream.good()) {
                string substring;
                getline(s_stream, substring, ',');
                splitLine.push_back(substring);
            }

            // If not enough parameters for course
            if (splitLine.size() < 2) {
                cout << "Course is missing parameters, cancelling file read";
                return;
            }

            // Create new course object and add course information
            Course newCourse;
            newCourse.courseId = splitLine[0];
            newCourse.title = splitLine[1];

            // Add prerequisites
            if(splitLine.size() > 2) {
                for(int i = 2; i < splitLine.size(); i++) {
                    
                    if (splitLine[i] != ""){
                    newCourse.prerequisites.push_back(splitLine[i]);
                    }
                }
            }

            // Add new course to tree
            courseTree->Insert(newCourse);

        }



    } else {

        // Else file was not opened
        cout << "File could not be opened" << endl;
    }

}


/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // Define a binary search tree to hold all courses
    BinarySearchTree* courseTree;
    courseTree = new BinarySearchTree();
    Course course;

    // Prompt user for CSV name
    string csvPath, courseKey;
    cout << "What is the name of the file you'll be working with? ";
    cin >> csvPath;

    cout << "Welcome to the course planner." << endl;

    int choice = 0;
    while (choice != 9) {

        // Show menu and get user choice
        showMenu();
    
        cin >> choice;

        switch (choice) {

        case 1:

            // Complete the method call to load the courses
            loadCourses(csvPath, courseTree);
            break;

        case 2:
            // Print sample schedule in order
            cout << "Here is a sample schedule:" << endl << endl;
            courseTree->InOrder();
            break;

        case 3:

            cout << "What course do you want to know about? ";         

            cin >> courseKey;

            course = courseTree->Search(courseKey);

            if (!course.courseId.empty()) {

                // Output courseID, title
                cout << course.courseId << ", " << course.title << endl;

                // Print prerequisites
                cout << "Prerequisites: ";
                
                const char* delim = "";
                for (auto const& course : course.prerequisites) {
                    std::cout << delim << course;
                    delim = ", ";
                }

                cout << endl;

            } else {
            	cout << "Course ID " << courseKey << " not found." << endl;
            }

            break;

        case 9:
           cout << "Thank you for using the course planner!" << endl;
           break;

        default: 
            cout << choice << " is not a valid option.";
        }
    }

	return 0;
}
