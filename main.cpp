/* Swift-Load Logistics System */

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

// Note: using namespace std is acceptable for small scripts, but in production
// C++ code it is discouraged to prevent namespace pollution and naming collisions.
using namespace std;
using namespace std::chrono;

// Goods class to represent logistics items securely
class Goods {
private:
    // internal data 
    string name;
    string type;
    double weight;

public:
    // constructor setup
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
    // getters to safely read data
    string getName() const { return name; }
    string getType() const { return type; }
    double getWeight() const { return weight; }

    // --- Mutator Methods (Setters) with Validation ---
    // making sure no one inputs a negative weight or empty string
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

// --- Bubble Sort (Descending Order) ---
// slow but simple sort
void bubbleSort(vector<double>& arr, long long& comps, long long& swaps) {
    size_t n = arr.size();
    comps = 0;
    swaps = 0;
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            comps++;
            if (arr[j] < arr[j + 1]) {  // < for descending order
                swap(arr[j], arr[j + 1]);
                swaps++;
            }
        }
    }
}

// --- QuickSort (Descending Order) ---
// fast recursive sort

int partition(vector<double>& arr, int low, int high, long long& comps, long long& swaps) {
    double pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        comps++;
        if (arr[j] > pivot) {  // > for descending order
            i++;
            swap(arr[i], arr[j]);
            swaps++;
        }
    }
    swap(arr[i + 1], arr[high]);
    swaps++;
    return (i + 1);
}

void quickSort(vector<double>& arr, int low, int high, long long& comps, long long& swaps) {
    if (low < high) {
        int pi = partition(arr, low, high, comps, swaps);
        quickSort(arr, low, pi - 1, comps, swaps);
        quickSort(arr, pi + 1, high, comps, swaps);
    }
}

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

    int height(AVLNode* N) const {
        if (N == nullptr) return 0;
        return N->height;
    }

    // using built-in max

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
    int getBalance(AVLNode* N) const {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }

    // --- Insert with Self-Balancing ---
    AVLNode* insertNode(AVLNode* node, Goods item) {
        // validate input before insertion
        if (item.getName().empty()) {
            throw invalid_argument("Cannot insert item with empty name.");
        }

        // normal insert first
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

        // heavy on the left
        if (balance > 1 && item.getName() < node->left->data.getName())
            return rightRotate(node);

        // heavy on the right
        if (balance < -1 && item.getName() > node->right->data.getName())
            return leftRotate(node);

        // zigzag left-right
        if (balance > 1 && item.getName() > node->left->data.getName()) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // zigzag right-left
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

    // --- Post-Order Traversal for Memory Cleanup ---
    void destroyTree(AVLNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr), nodeCount(0), searchComparisons(0) {}

    ~AVLTree() {
        destroyTree(root);
    }

    void insert(Goods item) {
        try {
            root = insertNode(root, item);
        } catch (const exception& e) {
            cout << "  " << e.what() << endl;
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

pair<int, bool> linearSearch(const vector<Goods>& inventory, const string& key) {
    int comparisons = 0;
    for (size_t i = 0; i < inventory.size(); i++) {
        comparisons++;
        if (inventory[i].getName() == key) {
            return {comparisons, true};
        }
    }
    return {comparisons, false};  // Not found, searched everything
}

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

// MAIN FUNCTION - Complete System Demonstration
int main() {
    cout << "==========================================================" << endl;
    cout << "   Swift-Load Logistics - System Execution Output" << endl;
    cout << "   Data Structures & Algorithms - Final Assignment" << endl;
    cout << "==========================================================" << endl << endl;

        
        cout << "=== Goods ADT & Encapsulation Demo ===" << endl;
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

        
        cout << "\n=== Queue Illustration (FIFO for Loading Bay) ===" << endl;
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

        
        cout << "\n";
    demonstrateStack();

        
        cout << "\n=== Sorting Algorithms & Efficiency ===" << endl;
    vector<double> weights = {45.5, 12.0, 89.2, 5.5, 34.1, 100.0, 77.8, 23.4, 56.7, 9.9, 41.2, 60.0};

    cout << "\nOriginal 12 cargo weights:" << endl << "  ";
    for (double w : weights) cout << w << " ";
    cout << endl;

    // Bubble Sort
    vector<double> bubbleWeights = weights;
    auto start = high_resolution_clock::now();
    long long bubbleComps = 0, bubbleSwaps = 0;
    bubbleSort(bubbleWeights, bubbleComps, bubbleSwaps);
    auto stop = high_resolution_clock::now();
    auto bubbleDuration = duration_cast<nanoseconds>(stop - start);

    cout << "\nBubble Sort (Descending):" << endl << "  ";
    for (double w : bubbleWeights) cout << w << " ";
    cout << "\n  Execution Time:  " << bubbleDuration.count() << " nanoseconds";
    cout << "\n  Comparisons:     " << bubbleComps;
    cout << "\n  Swaps:           " << bubbleSwaps << endl;

    // QuickSort
    vector<double> quickWeights = weights;
    start = high_resolution_clock::now();
    long long quickComps = 0, quickSwaps = 0;
    quickSort(quickWeights, 0, quickWeights.size() - 1, quickComps, quickSwaps);
    stop = high_resolution_clock::now();
    auto quickDuration = duration_cast<nanoseconds>(stop - start);

    cout << "\nQuickSort (Descending):" << endl << "  ";
    for (double w : quickWeights) cout << w << " ";
    cout << "\n  Execution Time:  " << quickDuration.count() << " nanoseconds";
    cout << "\n  Comparisons:     " << quickComps;
    cout << "\n  Swaps:           " << quickSwaps << endl;

    // Performance Summary Table
    cout << "\n  +------------------+---------------+--------+-------+" << endl;
    cout << "  | Algorithm        | Time (ns)     | Comps  | Swaps |" << endl;
    cout << "  +------------------+---------------+--------+-------+" << endl;
    cout << "  | Bubble Sort      | " << left << setw(14) << bubbleDuration.count()
         << "| " << setw(7) << bubbleComps << "| " << setw(6) << bubbleSwaps << "|" << endl;
    cout << "  | QuickSort        | " << left << setw(14) << quickDuration.count()
         << "| " << setw(7) << quickComps << "| " << setw(6) << quickSwaps << "|" << endl;
    cout << "  +------------------+---------------+--------+-------+" << endl;

        
        cout << "\n=== AVL Tree & Robustness Testing ===" << endl;
    AVLTree inventory;

    // testing bad inputs
    cout << "\n--- Robustness Test Results  ---" << endl;

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

    cout << "Test 5: Inserting duplicate key..." << endl;
    Goods dup1("Monitor", "Electronics", 4.0);
    inventory.insert(dup1); // First insert succeeds
    Goods dup2("Monitor", "Electronics", 4.0);
    inventory.insert(dup2); // Second insert will throw and be caught inside insert()

    // Insert valid goods into AVL Tree
    cout << "\n--- Inserting Valid Inventory Items ---" << endl;
    vector<Goods> itemList = {
        Goods("Monitor", "Electronics", 4.0), // Added to vector so linear search dataset syncs with AVL
        Goods("Laptop", "Electronics", 2.5),
        Goods("Desk", "Furniture", 25.0),
        Goods("Apples", "Food", 50.0),
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

        
        cout << "\n=== AVL Tree vs Linear Search Comparison ===" << endl;

    string searchKeys[] = {"Monitor", "Tablet", "Bananas", "Keyboard"};

    cout << "\n  +-----------+---------------------+---------------------+" << endl;
    cout << "  | Search Key| AVL Comparisons     | Linear Comparisons  |" << endl;
    cout << "  +-----------+---------------------+---------------------+" << endl;

    for (const string& key : searchKeys) {
        // AVL Tree search
        Goods* avlResult = inventory.search(key);
        int avlComps = inventory.getSearchComparisons();

        // Linear search
        pair<int, bool> linearResult = linearSearch(itemList, key);
        int linearComps = linearResult.first;
        bool linearFound = linearResult.second;

        cout << "  | " << left << setw(10) << key
             << "| " << setw(4) << avlComps << (avlResult ? " (Found)        " : " (Not Found)    ")
             << "| " << setw(4) << linearComps
             << (linearFound ? " (Found)        " : " (Not Found)    ")
             << "|" << endl;
    }
    cout << "  +-----------+---------------------+---------------------+" << endl;
    cout << "\n  Analysis: AVL search is O(log N) while Linear search is O(N)." << endl;
    cout << "  For 1,000,000 items: AVL needs ~20 comparisons vs 1,000,000 for linear." << endl;

        
        cout << "\n=== Memory Usage Analysis ===" << endl;
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
