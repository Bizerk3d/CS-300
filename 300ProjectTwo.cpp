//============================================================================
// Name        : 300ProjectTwo.cpp
// Author      : Brennan Reed
// Date        : 2/20/22
// Description : Binary Search Tree for Course Catalog, C++
// Description : Final project for CS300 @ SNHU
//============================================================================

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <istream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseNumber; // unique identifier
    string courseName;
    vector<string> preReqs;
    Course() {
    }
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    //default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with course
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

// creates vector of preReqs
void SplitString(string s, vector<string>& v) {

    string temp = "";
    for (int i = 0; i < s.length(); ++i) {

        if (s[i] == ',') {
            v.push_back(temp);
            temp = "";
        }
        else {
            temp.push_back(s[i]);
        }
    }
    v.push_back(temp);
}

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
    Node* removeNode(Node* node, string courseNumber);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseNumber);
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
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
    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if no root exists then root equals new node course
    if (root == nullptr) {
        root = new Node(course);
    }
    else {  // otherwise adds new node
        this->addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseNumber) {
    // remove node with root courseNumber
    this->removeNode(root, courseNumber);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    // set current node equal to root
    Node* current = root;
    // keep looping downwards until bottom reached or matching courseNumber found
    while (current != nullptr) {
        if (current->course.courseNumber.compare(courseNumber) == 0) {
            // if match found, return current course
            return current->course;
        }
        // if course is smaller than current node then traverse left
        if (courseNumber.compare(current->course.courseNumber) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
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
    // adds to left subtree if node greater than course
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            this->addNode(node->left, course);
        }
    }
    // adds to right subtree
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            this->addNode(node->right, course);
        }
    }
}
void BinarySearchTree::inOrder(Node* node) {
    // traverses tree and prints/displays courses if not empty, left then right inorder
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseName << endl;
        inOrder(node->right); 
    }
}

Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {
    // condition avoids crash by returning node if null
    if (node == nullptr) {
        return node;
    }
    // recurses left subtree
    if (courseNumber.compare(node->course.courseNumber) < 0) {
        node->left = removeNode(node->left, courseNumber);
    }
    else if (courseNumber.compare(node->course.courseNumber) > 0) {
        node->right = removeNode(node->right, courseNumber);
    }
    else {
        // no children
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        // one child to left
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        // one child to right
        else if (node->right != nullptr && node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        // two children
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->course = temp->course;
            node->right = removeNode(node->right, temp->course.courseNumber);
        }
    }
    return node;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the bid info
 */
void displayCourse(Course course) {
    cout << course.courseNumber << ", " << course.courseName << endl;

    // if course has preReqs prints those with appropriate formatting
    if (!course.preReqs.empty()) {
        cout << "Prerequisites: ";

        // format to display commas between each without trailing one
        bool first = true;
        for (auto const& e : course.preReqs) {
            if (first) { first = false; }
            else { cout << ", "; }
            cout << e;
        }
        cout << endl;
    }
    return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    string token;
    string line;
    ifstream myFile;
    try {
        myFile.open(csvPath);
        if (myFile.is_open()) {

            while (myFile.good()) {

                while (getline(myFile, line)) {

                    istringstream ss(line);
                    int tokenCount = 1;

                    // Create a data structure and add to the collection of courses
                    Course course;
                    while (getline(ss, token, ',')) {

                        // assigns first token from line as courseNumber
                        if (tokenCount == 1) {
                            course.courseNumber = token;
                            tokenCount++;
                        }
                        // assigns first token from line as courseName
                        else if (tokenCount == 2) {
                            course.courseName = token;
                            tokenCount++;
                        }
                        // add prereqs to vector
                        else if (tokenCount >= 3) {
                            SplitString(token, course.preReqs);
                        }
                        // error if not at least two fields for name and number
                        else {
                            cout << "Format Error: data must contain course name and course number." << endl;
                        }
                    }
                    // push this course to the end
                    bst->Insert(course);
                    ss.clear();
                }
            }
        }
        else {
            cout << "Failed to open " << csvPath << "." << endl;
        }
        myFile.close();
        cout << "File loaded successfully." << endl;
    }
    catch (const std::exception&e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
   
    // process command line file input name
    string csvPath;

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst{};

    Course course;
    // matched against coursenumber for search for specific course method
    string courseFind;

    cout << "Welcome to the course planning tool." << endl;
    int choice = 0;

    while (choice != 9) {        
        cout << endl;
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do?" << endl;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {

        case 1:
            bst = new BinarySearchTree();

            cout << endl;
            cout << "Please type the name of the data structure containing course information:" << endl;
            
            // name of file to load from user
            cin >> csvPath;

            // Complete the method call to load the courses
            loadCourses(csvPath, bst);
            break;

        case 2:
            if (bst == NULL) {
                cout << endl;
                cout << "Please load data first." << endl;
                break;
            }
            else {
                cout << endl;
                cout << "Here's a sample schedule: " << endl << endl;
                bst->InOrder();
                break;
            }
        case 3:
            if (bst == NULL) {
                cout << endl;
                cout << "Please load data first." << endl;
                break;
            }
            else {
                cout << endl;
                cout << "Enter the course number: " << endl;

                cin >> courseFind;
                // ignores case sensitivity for user input
                transform(courseFind.begin(), courseFind.end(), courseFind.begin(), ::toupper);
                // band-aid fix for mysterious leading quote marks
                courseFind = "\"" + courseFind;
                cout << endl;
                course = bst->Search(courseFind);

                if (!course.courseNumber.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "Course " << courseFind << " not found." << endl;
                }
                break;
            }
        case 9:
            break;

        default:
            cout << "Invalid selection - please try again." << endl;
        }
    }
    cout << endl;
    cout << "Thanks for using the course planner. Good-bye!" << endl;

    return 0;
}
