/

// UMBC - CMSC 341 - Spring 2023 - Proj2
#include "show.h"

//Constructor: Initializes all the member variable
Show::Show(){
    m_root = nullptr;
}

//Destructor: Deletes all nodes in binary tree
Show::~Show(){
    //clear function deletes all the nodes and free the memory so we call clear function in destructor
    clear();
}

//This function inserts a Drone object into the tree in the proper position.
void Show::insert(const Drone& aDrone){
    //Check if aDrone's id is unique or not
    //If another drone with same id is not present in the tree
    //then only we add the drone to the tree
    if(!findDrone(aDrone.getID())){
        m_root = insertHelper(m_root, aDrone);
    }
}

//The clear function deallocates all memory in the tree and makes it an empty tree.
void Show::clear(){
    // Call the helper function to recursively delete nodes
    clearHelper(m_root);
    // Set the root to null to indicate an empty tree
    m_root = nullptr;
}


//The remove function traverses the tree to find a node with the id and removes it from the tree
void Show::remove(int id){
    if (m_root == nullptr){
        return;
    }

    if(!findDrone(id)){
        //calling helper function
            m_root = removeHelper(m_root, id);
            if (m_root == nullptr) {
                m_root->setHeight(0);
            }

    }
}

void Show::dumpTree() const {dump(m_root);}

void Show::dump(Drone* aDrone) const{
    if (aDrone != nullptr){
        cout << "(";
        dump(aDrone->m_left);//first visit the left child
        cout << aDrone->m_id << ":" << aDrone->m_height;//second visit the node itself
        dump(aDrone->m_right);//third visit the right child
        cout << ")";
    }
}

//This function prints a list of all drones in the tree to the standard output in the ascending order of IDs
void Show::listDrones() const {
    //calling helper function
    traverser(m_root);
}


//This function finds the node with id in the tree and sets its member variable to state.
bool Show::setState(int id, STATE state){
    if(!findDrone(id)) {
        return false;
    }

    Drone* drone = getDroneById(m_root, id);
    drone->setState(state);
    return true;
}

//This function traverses the tree, finds all drones with LIGHTOFF state and removes them from the tree.
void Show::removeLightOff(){
    //calling helper function
    lightOffRemoveHelper(m_root);
}


//This function returns the total number of drones of that color in the show.
int Show::countDrones(LIGHTCOLOR aColor) const{
    int count = 0;
    //calling helper function
    countHelper(m_root, count, aColor);
    count = 0;
}


//This drone find drone with particualr color and return id
bool Show::findDrone(int id) const {
    //calling helper function
    return finder(m_root, id);
}

const Show & Show::operator=(const Show & rhs){
    if (this != &rhs) {  // check for self-assignment
        // Delete the current tree
        clear();

        // Copy all nodes from rhs using insert function
        if (rhs.m_root != nullptr) {
            insert(*rhs.m_root);
        }
    }
    return *this;

}


// Helper functions

//This function helps to insert new node recursively
Drone* Show::insertHelper(Drone* drone, const Drone& aDrone){
    // If the current node is null, create a new Drone object
    if(drone == nullptr){
        return new Drone(aDrone);
    }
    // If the Drone ID to be inserted is less than the ID of the current node, traverse the left subtree
    if(aDrone.getID() < drone->getID()){
        drone->setLeft(insertHelper(drone->getLeft(), aDrone));// Recursively insert the Drone to the left subtree
    }
    else{
        // If the Drone ID to be inserted is less than the ID of the current node, traverse the right subtree
        drone->setRight(insertHelper(drone->getRight(), aDrone)); // Recursively insert the Drone to the right subtree
    }
    //update the height of the tree after insertion
    updateHeight(drone);
    int balanceFactor = getBalanceFactor(drone); // Calculate the balance factor of the current node

    // Perform a right rotation if the balance factor is greater than 1
    // and the ID of the Drone to be inserted is less than the ID of the left child.
    if(balanceFactor > 1 and aDrone.getID() < drone->getLeft()->getID()){
        return rightRotation(drone);
    }
    // Perform a left rotation followed by a right rotation if the balance factor is greater than 1
    // and the ID of the Drone to be inserted is greater than the ID of the left child.

    else if(balanceFactor > 1 and aDrone.getID() > drone->getLeft()->getID()){
        drone->setLeft(leftRotation(drone->getLeft()));
        return rightRotation(drone);
    }

    // Perform a left rotation if the balance factor is less than -1 and
    // the ID of the Drone to be inserted is greater than the ID of the right child.
    else if(balanceFactor < -1 and aDrone.getID() > drone->getRight()->getID()){
        return leftRotation(drone);
    }

    // Perform a right rotation followed by a left rotation if the balance factor is less than -1
    // and the ID of the Drone to be inserted is less than the ID of the right child.
    else if(balanceFactor < -1 and aDrone.getID() < drone->getRight()->getID()){
        drone->setRight(rightRotation(drone->getRight()));
        return leftRotation(drone);
    }
    return drone;
}

//This function helps to delete all nodes
void Show::clearHelper(Drone* aDrone) {
    if (aDrone == nullptr) {
        return;
    }
    // Recursively delete the left and right subtrees
    clearHelper(aDrone->m_left);
    clearHelper(aDrone->m_right);
    // Delete the current node and set the pointer to null
    delete aDrone;
    aDrone = nullptr;
}


//This function recursively traverse to each node and print its info
void Show::traverser(Drone* drone) const{
    //if current node is null, return
    if(drone == nullptr) {
        return;
    }
    //recursively traverse left subtree
    traverser(drone->getLeft());
    //print according to output file
    cout << drone->getID() << ":" << drone->getStateStr() << ":" << drone->getTypeStr() << endl;
    //recursively traverse right subtree
    traverser(drone->getRight());
}


//This function finds the drone in tree and return true if found
bool Show::finder(Drone* drone, int id) const {
    //if node is null, return false
    if(drone == nullptr) {
        return false;
    }
    //if id matches, return true
    else if (drone->getID() == id) {
        return true;
    }
    //if id is less, recursively traverse left and search
    else if(id < drone->getID()) {
        return finder(drone->getLeft(), id);
        //cout << "found left" << endl;

    }
    else {
        //if id is less, recursively traverse right and search
        return finder(drone->getRight(), id);
        //cout << "found right" << endl;
    }
}


//This function returns the id of the drone
Drone *Show::getDroneById(Drone* drone, int id) {
    //if id matches return current node
    if (drone->getID() == id) {
        return drone;
    }
    //if id is less than in current node, recursively search left subtree
    else if(id < drone->getID()){
        finder(drone->getLeft(), id);}
    //if id is less than in current node, recursively search right subtree
    else {
        finder(drone->getRight(), id);
    }
}


// This function counts the number of drones in a given subtree with a specified color
void Show::countHelper(Drone* drone, int& count, LIGHTCOLOR aColor) const {
    //if node is null, return
    if(drone == nullptr) {
        return;
    }
    //traverse to left subtree
    countHelper(drone->getLeft(), count, aColor);
    if(drone->getType() == aColor) {
        //increment counter

        count++;

    }
    //traverse to right subtree
    countHelper(drone->getRight(), count, aColor);
}

// This function updates the height of a given drone
void Show::updateHeight(Drone* drone) {
    //get the height of left subtree and set it to 1 if its nullptr
    int left_child_height = (drone->getLeft() == nullptr) ? -1 : drone->getLeft()->getHeight();

    //get the height of right subtree and set it to 1 if its nullptr
    int right_child_height = (drone->getRight() == nullptr) ?  -1 : drone->getRight()->getHeight();

    // Set the height of the current drone to the maximum height of its children plus 1
    drone->setHeight((left_child_height>right_child_height) ? left_child_height + 1 : right_child_height + 1);
}

// This function calculates the balance factor of a given drone,
// which is the difference between the heights of its left and right child drones
int Show::getBalanceFactor(Drone* drone) const {
    // Get the heights of the left child drones, or set them to 0 if they are null
    int left_child_factor = (drone->getLeft() == nullptr) ? 0 : drone->getLeft()->getHeight()+1;

    // Get the heights of the right child drones, or set them to 0 if they are null
    int right_child_factor = (drone->getRight() == nullptr) ?  0 : drone->getRight()->getHeight()+1;

    //return the difference
    return left_child_factor - right_child_factor;
}

// This function performs a left rotation of a drone node in the AVL tree.
Drone *Show::leftRotation(Drone *drone) {
    // Get the right child of the drone as the new root
    Drone* newRoot = drone->getRight();
    // Get the left child of the new root as temporary variable
    Drone* temp = newRoot->getLeft();

    newRoot->setLeft(drone); // Set the current drone as the left child of the new root
    drone->setRight(temp);// Set the temporary variable as the right child of the current drone

    //updating the height
    updateHeight(drone);
    updateHeight(newRoot);

    //return new root
    return newRoot;
}

// This function performs a right rotation of a drone node in the AVL tree.
Drone *Show::rightRotation(Drone *drone) {
    // Get the left child of the drone as the new root
    Drone* newRoot = drone->getLeft();
    // Get the right child of the new root as temporary variable
    Drone* temp = newRoot->getRight();

    newRoot->setRight(drone);// Set the current drone as the right child of the new root
    drone->setLeft(temp);// Set the temporary variable as the left child of the current drone

    //updating the height
    updateHeight(drone);
    updateHeight(newRoot);

    //return new root
    return newRoot;


}


//This function removes a drone with given id from BST
Drone* Show::removeHelper(Drone *drone, int id) {
    if (drone == nullptr) {
        return nullptr;
    }
    // If the ID is less than the current drone's ID, traverse the left subtree recursively
    if(id < drone->getID()) {
        drone->setLeft(removeHelper(drone->getLeft(), id));
    }
        // If the ID is less than the current drone's ID, traverse the right subtree recursively
    else if(id > drone->getID()){
        drone->setRight(removeHelper(drone->getRight(), id));
    }
    else{
        // If the ID matches the current drone's ID, remove the drone from the BST
        if(drone->getLeft() == nullptr or drone->getRight() == nullptr){ //if none or one children
            Drone* temp = drone->getLeft() ? drone->getLeft() : drone->getRight();
            if(temp == nullptr){
                temp = drone;
                drone = nullptr;
            }
            else{
                *drone = *temp;
            }
              delete temp;

        }
        else{ //if two children
            //Getting inorder successor
            Drone* temp = drone->getRight();
            while(temp->getLeft() != nullptr){
                temp = temp->getLeft();
            }
            //updating id, state, type
            drone->setID(temp->getID());
            drone->setState(temp->getState());
            drone->setType(temp->getType());
            // Remove the inorder successor from the subtree recursively
            drone->setRight(removeHelper(drone->getRight(), temp->getID()));
        }
        //if nothing in tree
        if(drone == nullptr){
            return drone;
        }

        //updating height and balance factor
        updateHeight(drone);
        int balanceFactor = getBalanceFactor(drone);

        // Perform rotations based on the balance factor to balance the BST
        if(balanceFactor > 1 and getBalanceFactor(drone->getLeft()) >=0){
            return rightRotation(drone);
        }
        else if(balanceFactor > 1 and getBalanceFactor(drone->getLeft()) < 0){
            drone->setLeft(leftRotation(drone->getLeft()));
            return rightRotation(drone);
        }
        else if(balanceFactor < -1 and getBalanceFactor(drone->getRight()) <= 0){
            return leftRotation(drone);
        }
        else if(balanceFactor < -1 and getBalanceFactor(drone->getRight()) > 0){
            drone->setRight(rightRotation(drone->getRight()));
            return leftRotation(drone);
        }
        //return the root of modified BST (balanced)
        return drone;
    }
}


//This function removes the drone with lightoff
void Show::lightOffRemoveHelper(Drone *drone) {
    //if drone is null, return
    if(drone == nullptr)
        return;
    //recursively traverse left subtree
    lightOffRemoveHelper(drone->getLeft());

    //if drone with lightoff found, remove that drone
    if(drone->getState() == STATE::LIGHTOFF) {
        remove(drone->getID());
    }

    //recursively traverse right subtree
    lightOffRemoveHelper(drone->getRight());
}


//testing funtions

bool Show::isBalanced(Drone* drone) const {
    // Base case: If the drone is null, return true
    if (drone == nullptr) {
        return true;
    }
    // Recursively check the left and right subtree to see if they are balanced
    bool leftSubtreeBalanced = isBalanced(drone->getLeft());
    bool rightSubtreeBalanced = isBalanced(drone->getRight());

    // If either subtree is not balanced, return false
    if (!leftSubtreeBalanced or !rightSubtreeBalanced) {
        return false;
    }
    // Calculate the balance factor of the current node
    int balanceFactor = getBalanceFactor(drone);
    // If the balance factor is not -1, 0, or 1, return false
    if (balanceFactor < -1 or balanceFactor > 1) {
        return false;
    }
    // If the balance factor is within the range, return true
    return true;
}


// This function is a function for isBST() and recursively checks if the binary search tree property is preserved or not
bool Show::isBST(Drone* drone, int minValue, int maxValue) const {
    // Base case: If the drone is null, return true
    if (drone == nullptr) {
        return true;
    }
    // Check if the current node is within the given range
    if (drone->getID() < minValue or drone->getID() > maxValue) {
        return false;
    }

    // Recursively check the left and right subtree
    bool leftSubtreeBST = isBST(drone->getLeft(), minValue, drone->getID() - 1);
    bool rightSubtreeBST = isBST(drone->getRight(), drone->getID() + 1, maxValue);

    // If either subtree is not a binary search tree, return false
    if (!leftSubtreeBST or !rightSubtreeBST) {
        return false;
    }

    // If the current node and both subtrees are binary search tree, return true
    return true;
}


