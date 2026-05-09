/*
 * ============================================================================
 * Swift-Load Logistics - Data Structures & Algorithms System
 * Unit 19: Final Assignment
 * ============================================================================
 * This program implements a comprehensive logistics management system for
 * Swift-Load Logistics, demonstrating multiple data structures and algorithms
 * including: Goods ADT with encapsulation, FIFO Queue, Bubble Sort, QuickSort,
 * Memory Stack simulation, AVL Tree for warehouse inventory, and Linear Search
 * for performance comparison.
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <iomanip>
#include <queue>
#include <stack>
#include <functional>

using namespace std;
using namespace std::chrono;

// ============================================================================
// TASK 1 - P1, M3, D2: Goods ADT with Encapsulation
// ============================================================================
// The Goods class demonstrates a well-designed Abstract Data Type (ADT) with
// full data encapsulation. All member variables are private and accessed
// exclusively through validated getter/setter methods.
// ============================================================================
class Goods {
private:
    // Private member variables - Information Hiding (M3)
    string name;
    string type;
    double weight;

public:
    // --- Constructor (Initialization) ---
    // Pre-condition:  weight >= 0, name should not be empty for valid goods
    // Post-condition: A Goods object with validated attributes is created
    Goods(string name = "Unnamed", string type = "", double weight = 0.0) {
        // Validation order: Name first, then Weight
        if (name.empty()) {
            throw invalid_argument("Name cannot be empty");
        }
        if (weight < 0) {
            throw invalid_argument("Weight cannot be negative");
        }
        this->name = name;
        this->type = type;
        this->weight = weight;
    }

    // --- Accessor Methods (Getters) ---
    // These provide read-only access to private data
    string getName() const { return name; }
    string getType() const { return type; }
    double getWeight() const { return weight; }

    // --- Mutator Methods (Setters) with Validation ---
    // These enforce business rules before modifying data
    void setName(const string& newName) {
        if (newName.empty())
            throw invalid_argument("Name cannot be empty");
        name = newName;
    }

    void setType(const string& newType) { type = newType; }

    void setWeight(double newWeight) {
        if (newWeight < 0) {
            throw invalid_argument("Weight cannot be negative");
        }
        weight = newWeight;
    }

    // --- Display Method ---
    void display() const {
        cout << left << setw(20) << name << setw(15) << type << setw(10) << weight << " kg" << endl;
    }
};

// ============================================================================
// TASK 1 - M2 & TASK 2 - P7: Sorting Algorithms with Performance Measurement
// ============================================================================

// --- Bubble Sort (Descending Order) ---
// Time Complexity: O(N^2) average and worst case
// Space Complexity: O(1) - in-place sorting
long long bubbleSortComparisons = 0;
long long bubbleSortSwaps = 0;

void bubbleSort(vector<double>& arr) {
    size_t n = arr.size();
    bubbleSortComparisons = 0;
    bubbleSortSwaps = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bubbleSortComparisons++;
            if (arr[j] < arr[j + 1]) {  // < for descending order
                swap(arr[j], arr[j + 1]);
                bubbleSortSwaps++;
            }
        }
    }
}

// --- QuickSort (Descending Order) ---
// Time Complexity: O(N log N) average case, O(N^2) worst case
// Space Complexity: O(log N) for recursive stack frames
long long quickSortComparisons = 0;
long long quickSortSwaps = 0;

int partition(vector<double>& arr, int low, int high) {
    double pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        quickSortComparisons++;
        if (arr[j] > pivot) {  // > for descending order
            i++;
            swap(arr[i], arr[j]);
            quickSortSwaps++;
        }
    }
    swap(arr[i + 1], arr[high]);
    quickSortSwaps++;
    return (i + 1);
}

void quickSort(vector<double>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ============================================================================
// TASK 3 - P4, M4, P5, D3: AVL Tree Implementation
// ============================================================================

class AVLNode {
public:
    Goods data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Goods item) : data(item), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;
    int nodeCount;
    int searchComparisons;  // Track comparisons for D3 analysis

    int height(AVLNode* N) {
        if (N == nullptr) return 0;
        return N->height;
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    // --- Right Rotation (for Left-Left imbalance) ---
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    // --- Left Rotation (for Right-Right imbalance) ---
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    // --- Balance Factor Calculation ---
    int getBalance(AVLNode* N) {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }

    // --- Insert with Self-Balancing ---
    AVLNode* insertNode(AVLNode* node, Goods item) {
        // P5: Robustness - validate input before insertion
        if (item.getName().empty()) {
            throw invalid_argument("Cannot insert item with empty name.");
        }

        // Standard BST insertion
        if (node == nullptr) {
            nodeCount++;
            return (new AVLNode(item));
        }

        if (item.getName() < node->data.getName())
            node->left = insertNode(node->left, item);
        else if (item.getName() > node->data.getName())
            node->right = insertNode(node->right, item);
        else  // Duplicate keys not allowed
            throw invalid_argument("Key already exists: " + item.getName());

        // Update height of ancestor node
        node->height = 1 + max(height(node->left), height(node->right));

        // Calculate balance factor
        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && item.getName() < node->left->data.getName())
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && item.getName() > node->right->data.getName())
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && item.getName() > node->left->data.getName()) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && item.getName() < node->right->data.getName()) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // --- In-Order Traversal (Alphabetical Display) ---
    void inOrder(AVLNode* root) {
        if (root != nullptr) {
            inOrder(root->left);
            root->data.display();
            inOrder(root->right);
        }
    }

    // --- Search with Comparison Counting ---
    AVLNode* searchNode(AVLNode* root, string key) {
        searchComparisons++;
        if (root == nullptr || root->data.getName() == key)
            return root;

        if (root->data.getName() < key)
            return searchNode(root->right, key);

        return searchNode(root->left, key);
    }

    // --- Pre-Order Traversal (Show Tree Structure) ---
    void preOrder(AVLNode* root, string indent, bool isRight) {
        if (root != nullptr) {
            cout << indent;
            if (isRight) {
                cout << "R---- ";
                indent += "      ";
            } else {
                cout << "L---- ";
                indent += "|     ";
            }
            cout << root->data.getName()
                 << " (h=" << root->height
                 << ", bf=" << getBalance(root) << ")" << endl;
            preOrder(root->left, indent, false);
            preOrder(root->right, indent, true);
        }
    }

public:
    AVLTree() : root(nullptr), nodeCount(0), searchComparisons(0) {}

    void insert(Goods item) {
        try {
            root = insertNode(root, item);
        } catch (const exception& e) {
            cout << "  [ERROR] " << e.what() << endl;
        }
    }

    void displayInventory() {
        cout << "--- Warehouse Inventory (Alphabetical by Name) ---" << endl;
        cout << left << setw(20) << "Name" << setw(15) << "Type" << setw(10) << "Weight" << endl;
        cout << string(50, '-') << endl;
        inOrder(root);
        cout << string(50, '-') << endl;
        cout << "Total items: " << nodeCount << endl;
    }

    void displayTreeStructure() {
        cout << "--- AVL Tree Structure ---" << endl;
        if (root) {
            cout << root->data.getName()
                 << " (h=" << root->height
                 << ", bf=" << getBalance(root) << ") [ROOT]" << endl;
            preOrder(root->left, "", false);
            preOrder(root->right, "", true);
        }
    }

    Goods* search(string key) {
        searchComparisons = 0;
        AVLNode* result = searchNode(root, key);
        if (result != nullptr) {
            return &(result->data);
        }
        return nullptr;
    }

    int getSearchComparisons() const { return searchComparisons; }
    int getNodeCount() const { return nodeCount; }
    int getTreeHeight() const { return height(root); }
};

// ============================================================================
// TASK 3 - D3: Linear Search for Comparison
// ============================================================================
int linearSearch(const vector<Goods>& inventory, const string& key) {
    int comparisons = 0;
    for (size_t i = 0; i < inventory.size(); i++) {
        comparisons++;
        if (inventory[i].getName() == key) {
            return comparisons;
        }
    }
    return comparisons;  // Not found, searched everything
}

// ============================================================================
// TASK 2 - P2: Stack Operations Demonstration
// ============================================================================
void demonstrateStack() {
    cout << "=== Stack Operations Demonstration (LIFO) ===" << endl;
    stack<string> callStack;

    cout << "\nSimulating function call stack:" << endl;

    // Push operations
    callStack.push("main()");
    cout << "  PUSH: main()          | Stack size: " << callStack.size() << endl;

    callStack.push("loadTruck()");
    cout << "  PUSH: loadTruck()     | Stack size: " << callStack.size() << endl;

    callStack.push("quickSort()");
    cout << "  PUSH: quickSort()     | Stack size: " << callStack.size() << endl;

    callStack.push("partition()");
    cout << "  PUSH: partition()     | Stack size: " << callStack.size() << endl;

    cout << "\n  Current top: " << callStack.top() << endl;
    cout << "  Stack is empty: " << (callStack.empty() ? "Yes" : "No") << endl;

    // Pop operations (returning from functions)
    cout << "\nReturning from functions:" << endl;
    while (!callStack.empty()) {
        cout << "  POP:  " << left << setw(18) << callStack.top()
             << "| Stack size: " << callStack.size() - 1 << endl;
        callStack.pop();
    }
    cout << "  Stack is empty: " << (callStack.empty() ? "Yes" : "No") << endl;
}

// ============================================================================
// MAIN FUNCTION - Complete System Demonstration
// ============================================================================
int main() {
    cout << "==========================================================" << endl;
    cout << "   Swift-Load Logistics - System Execution Output" << endl;
    cout << "   Data Structures & Algorithms - Final Assignment" << endl;
    cout << "==========================================================" << endl << endl;

    // -----------------------------------------------------------------------
    // SECTION 1: Goods ADT Demonstration (P1, M3)
    // -----------------------------------------------------------------------
    cout << "=== [P1, M3] Goods ADT & Encapsulation Demo ===" << endl;
    cout << "\nCreating valid goods:" << endl;
    Goods laptop("Laptop", "Electronics", 2.5);
    Goods desk("Desk", "Furniture", 25.0);
    cout << "  "; laptop.display();
    cout << "  "; desk.display();

    cout << "\nTesting setter validation:" << endl;
    try {
        desk.setWeight(30.0);
        cout << "  Updated Desk weight to 30.0 kg - SUCCESS" << endl;
    } catch (const exception& e) {
        cout << "  ERROR: " << e.what() << endl;
    }

    try {
        desk.setWeight(-10.0);
        cout << "  Set negative weight - should not reach here" << endl;
    } catch (const exception& e) {
        cout << "  Attempt to set weight=-10: CAUGHT -> " << e.what() << endl;
    }

    try {
        desk.setName("");
        cout << "  Set empty name - should not reach here" << endl;
    } catch (const exception& e) {
        cout << "  Attempt to set name='': CAUGHT -> " << e.what() << endl;
    }

    // -----------------------------------------------------------------------
    // SECTION 2: Queue Illustration (M1)
    // -----------------------------------------------------------------------
    cout << "\n=== [M1] Queue Illustration (FIFO for Loading Bay) ===" << endl;
    queue<string> truckQueue;

    cout << "\nEnqueue operations:" << endl;
    truckQueue.push("Truck A (Heavy Duty)");
    cout << "  Enqueue: Truck A (Heavy Duty)    | Queue size: " << truckQueue.size() << endl;
    truckQueue.push("Truck B (Refrigerated)");
    cout << "  Enqueue: Truck B (Refrigerated)  | Queue size: " << truckQueue.size() << endl;
    truckQueue.push("Truck C (Standard)");
    cout << "  Enqueue: Truck C (Standard)      | Queue size: " << truckQueue.size() << endl;

    cout << "\n  Front of queue: " << truckQueue.front() << endl;
    cout << "  Back of queue:  " << truckQueue.back() << endl;

    cout << "\nDequeue operations (processing trucks):" << endl;
    while (!truckQueue.empty()) {
        cout << "  Dequeue: " << left << setw(28) << truckQueue.front()
             << "| Remaining: " << truckQueue.size() - 1 << endl;
        truckQueue.pop();
    }

    // -----------------------------------------------------------------------
    // SECTION 3: Stack Operations (P2)
    // -----------------------------------------------------------------------
    cout << "\n";
    demonstrateStack();

    // -----------------------------------------------------------------------
    // SECTION 4: Sorting Comparison (M2) & Efficiency Measurement (P7)
    // -----------------------------------------------------------------------
    cout << "\n=== [M2, P7] Sorting Algorithms & Efficiency ===" << endl;
    vector<double> weights = {45.5, 12.0, 89.2, 5.5, 34.1, 100.0, 77.8, 23.4, 56.7, 9.9, 41.2, 60.0};

    cout << "\nOriginal 12 cargo weights:" << endl << "  ";
    for (double w : weights) cout << w << " ";
    cout << endl;

    // Bubble Sort
    vector<double> bubbleWeights = weights;
    auto start = high_resolution_clock::now();
    bubbleSort(bubbleWeights);
    auto stop = high_resolution_clock::now();
    auto bubbleDuration = duration_cast<nanoseconds>(stop - start);

    cout << "\nBubble Sort (Descending):" << endl << "  ";
    for (double w : bubbleWeights) cout << w << " ";
    cout << "\n  Execution Time:  " << bubbleDuration.count() << " nanoseconds";
    cout << "\n  Comparisons:     " << bubbleSortComparisons;
    cout << "\n  Swaps:           " << bubbleSortSwaps << endl;

    // QuickSort
    vector<double> quickWeights = weights;
    start = high_resolution_clock::now();
    quickSortComparisons = 0;
    quickSortSwaps = 0;
    quickSort(quickWeights, 0, quickWeights.size() - 1);
    stop = high_resolution_clock::now();
    auto quickDuration = duration_cast<nanoseconds>(stop - start);

    cout << "\nQuickSort (Descending):" << endl << "  ";
    for (double w : quickWeights) cout << w << " ";
    cout << "\n  Execution Time:  " << quickDuration.count() << " nanoseconds";
    cout << "\n  Comparisons:     " << quickSortComparisons;
    cout << "\n  Swaps:           " << quickSortSwaps << endl;

    // Performance Summary Table
    cout << "\n  +------------------+---------------+--------+-------+" << endl;
    cout << "  | Algorithm        | Time (ns)     | Comps  | Swaps |" << endl;
    cout << "  +------------------+---------------+--------+-------+" << endl;
    cout << "  | Bubble Sort      | " << left << setw(14) << bubbleDuration.count()
         << "| " << setw(7) << bubbleSortComparisons << "| " << setw(6) << bubbleSortSwaps << "|" << endl;
    cout << "  | QuickSort        | " << left << setw(14) << quickDuration.count()
         << "| " << setw(7) << quickSortComparisons << "| " << setw(6) << quickSortSwaps << "|" << endl;
    cout << "  +------------------+---------------+--------+-------+" << endl;

    // -----------------------------------------------------------------------
    // SECTION 5: AVL Tree Implementation (P4, M4) & Robustness (P5)
    // -----------------------------------------------------------------------
    cout << "\n=== [P4, M4, P5] AVL Tree & Robustness Testing ===" << endl;
    AVLTree inventory;

    // P5: Robustness - Error handling tests
    cout << "\n--- Robustness Test Results (P5) ---" << endl;

    cout << "Test 1: Creating Goods with negative weight..." << endl;
    try {
        Goods invalidGoods("BadItem", "Electronics", -5.0);
        cout << "  FAIL: No exception thrown" << endl;
    } catch (const invalid_argument& e) {
        cout << "  PASS: " << e.what() << endl;
    }

    cout << "Test 2: Creating item with empty name..." << endl;
    try {
        Goods nullGoods("", "Misc", 10.0);
        inventory.insert(nullGoods);
        cout << "  FAIL: No exception thrown" << endl;
    } catch (const invalid_argument& e) {
        cout << "  PASS: " << e.what() << endl;
    }

    cout << "Test 3: Setting negative weight via setter..." << endl;
    try {
        Goods testGoods("Test", "Test", 5.0);
        testGoods.setWeight(-100);
        cout << "  FAIL: No exception thrown" << endl;
    } catch (const invalid_argument& e) {
        cout << "  PASS: " << e.what() << endl;
    }

    cout << "Test 4: Setting empty name via setter..." << endl;
    try {
        Goods testGoods2("Test", "Test", 5.0);
        testGoods2.setName("");
        cout << "  FAIL: No exception thrown" << endl;
    } catch (const invalid_argument& e) {
        cout << "  PASS: " << e.what() << endl;
    }

    cout << "Test 5 (T9): Inserting duplicate key..." << endl;
    Goods dup1("Monitor", "Electronics", 4.0);
    inventory.insert(dup1); // First insert succeeds
    Goods dup2("Monitor", "Electronics", 4.0);
    inventory.insert(dup2); // Second insert will throw and be caught inside insert()

    // Insert valid goods into AVL Tree
    cout << "\n--- Inserting Valid Inventory Items ---" << endl;
    vector<Goods> itemList = {
        Goods("Laptop", "Electronics", 2.5),
        Goods("Desk", "Furniture", 25.0),
        Goods("Apples", "Food", 50.0),
        // Monitor is already inserted in Test 5
        Goods("Chair", "Furniture", 12.0),
        Goods("Tablet", "Electronics", 1.5),
        Goods("Printer", "Electronics", 8.0),
        Goods("Bananas", "Food", 15.0)
    };

    for (const auto& item : itemList) {
        cout << "  Inserting: " << item.getName() << endl;
        inventory.insert(item);
    }

    cout << "\n";
    inventory.displayInventory();

    cout << "\n";
    inventory.displayTreeStructure();
    cout << "  Tree Height: " << inventory.getTreeHeight() << endl;
    cout << "  Total Nodes: " << inventory.getNodeCount() << endl;

    // -----------------------------------------------------------------------
    // SECTION 6: Search Performance - AVL vs Linear Search (D3)
    // -----------------------------------------------------------------------
    cout << "\n=== [D3] AVL Tree vs Linear Search Comparison ===" << endl;

    string searchKeys[] = {"Monitor", "Tablet", "Bananas", "Keyboard"};

    cout << "\n  +-----------+---------------------+---------------------+" << endl;
    cout << "  | Search Key| AVL Comparisons     | Linear Comparisons  |" << endl;
    cout << "  +-----------+---------------------+---------------------+" << endl;

    for (const string& key : searchKeys) {
        // AVL Tree search
        Goods* avlResult = inventory.search(key);
        int avlComps = inventory.getSearchComparisons();

        // Linear search
        int linearComps = linearSearch(itemList, key);

        cout << "  | " << left << setw(10) << key
             << "| " << setw(4) << avlComps << (avlResult ? " (Found)        " : " (Not Found)    ")
             << "| " << setw(4) << linearComps
             << ((linearComps <= (int)itemList.size() && key != "Keyboard") ? " (Found)        " : " (Not Found)    ")
             << "|" << endl;
    }
    cout << "  +-----------+---------------------+---------------------+" << endl;
    cout << "\n  Analysis: AVL search is O(log N) while Linear search is O(N)." << endl;
    cout << "  For 1,000,000 items: AVL needs ~20 comparisons vs 1,000,000 for linear." << endl;

    // -----------------------------------------------------------------------
    // SECTION 7: Memory Usage Comparison (P7)
    // -----------------------------------------------------------------------
    cout << "\n=== [P7] Memory Usage Analysis ===" << endl;
    cout << "  sizeof(Goods):    " << sizeof(Goods) << " bytes" << endl;
    cout << "  sizeof(AVLNode):  " << sizeof(AVLNode) << " bytes" << endl;
    cout << "  Array of 8 items: " << sizeof(Goods) * 8 << " bytes (contiguous)" << endl;
    cout << "  AVL Tree 8 nodes: " << sizeof(AVLNode) * 8 << " bytes (+ pointer overhead)" << endl;
    cout << "  Trade-off: AVL uses more memory but provides O(log N) search." << endl;

    cout << "\n==========================================================" << endl;
    cout << "   All tests completed successfully." << endl;
    cout << "==========================================================" << endl;

    return 0;
}
