"""Part 2: Task 2 content - EXPANDED for distinction."""
from report_part1 import *

def write_task2(doc):
    doc.add_heading('Task 2: Operational Optimization and Efficiency', level=1)
    doc.add_paragraph(
        'This task addresses the theoretical and analytical aspects of resource management, '
        'process control, and routing optimization within the Swift-Load Logistics system. '
        'We examine how stack-based process control manages function execution, formally specify '
        'the stack ADT using imperative notation, analyze competing routing algorithms, and assess '
        'algorithm effectiveness through asymptotic analysis and empirical measurement.'
    )

    # P2
    doc.add_heading('P2: Stack Operations for Process Control', level=2)
    doc.add_paragraph(
        'A memory stack is a fundamental data structure that operates on a Last-In-First-Out (LIFO) '
        'principle. Unlike a queue where the first element added is the first removed, a stack ensures '
        'that the most recently added element is always the first to be removed. This behavior is '
        'analogous to a physical stack of plates: you can only add to or remove from the top.'
    )
    doc.add_paragraph(
        'In computer systems, the call stack is the primary mechanism for managing function execution. '
        'When a function is called, a stack frame is created containing: (1) the function\'s local '
        'variables, (2) the parameters passed to it, (3) the return address indicating where execution '
        'should resume after the function completes, and (4) the saved state of CPU registers. This '
        'frame is pushed onto the call stack. When the function returns, its frame is popped, restoring '
        'the previous execution context and allowing the program to continue exactly where it left off.'
    )
    bold_para(doc, 'Core Stack Operations:')
    t = doc.add_table(rows=1, cols=4)
    t.style = 'Table Grid'
    styled_header_row(t, ['Operation', 'Description', 'Time Complexity', 'Application in Swift-Load'])
    add_table_row(t, ['push(item)', 'Adds item to the top of the stack', 'O(1)',
        'When quickSort() calls partition(), a new frame is pushed'])
    add_table_row(t, ['pop()', 'Removes and returns the top item', 'O(1)',
        'When partition() returns, its frame is popped'])
    add_table_row(t, ['top()/peek()', 'Returns the top item without removing', 'O(1)',
        'Checking which function is currently executing'])
    add_table_row(t, ['isEmpty()', 'Returns true if stack has no elements', 'O(1)',
        'Checking if all functions have returned'])
    add_table_row(t, ['size()', 'Returns the number of elements', 'O(1)',
        'Monitoring recursion depth for stack overflow prevention'])
    doc.add_paragraph('Table 4: Stack Operations with Logistics Applications')

    bold_para(doc, 'Call Stack Diagram During QuickSort Execution:')
    st = doc.add_table(rows=6, cols=2)
    st.style = 'Table Grid'
    data = [
        ("Direction", "PUSH \u2193 down  |  POP \u2191 up"),
        ("TOP \u2192", "partition(arr, 0, 5) frame\n[local vars: pivot, i, j]"),
        ("", "quickSort(arr, 0, 5) frame\n[local vars: pi]"),
        ("", "loadTruck(truckId=42) frame\n[local vars: items, capacity]"),
        ("BOTTOM \u2192", "main() frame\n[local vars: inventory, weights]"),
    ]
    colors = ["FFFFFF", "F4B084", "8EA9DB", "C5E0B4", "D9D9D9"]
    for i, (label, content) in enumerate(data):
        st.rows[i].cells[0].text = label
        st.rows[i].cells[1].text = content
        if colors[i] != "FFFFFF":
            set_bg(st.rows[i].cells[1], colors[i])
    st.rows[5].cells[0].text = ""
    st.rows[5].cells[1].text = "[Stack Base - Memory Address 0x0000]"
    doc.add_paragraph('Figure 2: LIFO Call Stack During Recursive QuickSort Execution')

    bold_para(doc, 'C++ Stack Demonstration Code:')
    add_code_block(doc, '''stack<string> callStack;
callStack.push("main()");       // Frame 1 pushed
callStack.push("loadTruck()");  // Frame 2 pushed
callStack.push("quickSort()");  // Frame 3 pushed
callStack.push("partition()");  // Frame 4 pushed (TOP)

cout << "Current top: " << callStack.top();  // "partition()"
// Functions return in reverse order (LIFO):
while (!callStack.empty()) {
    cout << "POP: " << callStack.top();  // partition -> quickSort -> loadTruck -> main
    callStack.pop();
}''')
    doc.add_paragraph(
        'This LIFO mechanism is essential for recursion in our QuickSort implementation. Each recursive '
        'call to quickSort() creates a new stack frame, and the deepest recursive call (the base case) '
        'completes first. The maximum recursion depth for QuickSort is O(log N) in the average case, '
        'meaning for 1,000,000 items, the call stack would reach approximately 20 frames deep. If the '
        'stack exceeds its allocated memory, a stack overflow error occurs, which is why monitoring '
        'recursion depth is critical for production systems.'
    )

    # P3
    doc.add_heading('P3: Formal Specification for a Software Stack (Imperative Definition)', level=2)
    doc.add_paragraph(
        'A formal specification defines the behavior of an ADT using mathematical pre-conditions '
        'and post-conditions, completely independent of any programming language. This imperative '
        'definition describes the software stack used for LIFO cargo loading operations, where '
        'the last item loaded onto a truck is the first item unloaded at the delivery point.'
    )
    doc.add_paragraph(
        'The imperative approach specifies each operation as a state transformation: given a stack '
        'in state S, applying operation Op produces a new stack in state S\'. Pre-conditions define '
        'what must be true before the operation can execute, and post-conditions define what is '
        'guaranteed to be true after execution completes.'
    )
    ft = doc.add_table(rows=1, cols=3)
    ft.style = 'Table Grid'
    styled_header_row(ft, ['Operation Signature', 'Pre-conditions', 'Post-conditions'])
    add_table_row(ft, ['init(capacity: Integer) \u2192 Stack',
        'capacity > 0',
        'Returns Stack S where S.size = 0 and S.maxSize = capacity. Memory allocated for capacity elements.'])
    add_table_row(ft, ['push(S: Stack, item: T) \u2192 Stack',
        'S.size < S.maxSize\n(Stack is not full)',
        'item is placed at S.top position. S.size = S.size + 1. All previous elements unchanged.'])
    add_table_row(ft, ['pop(S: Stack) \u2192 T',
        'S.size > 0\n(Stack is not empty)',
        'Returns element at S.top. Removes that element. S.size = S.size - 1.'])
    add_table_row(ft, ['peek(S: Stack) \u2192 T',
        'S.size > 0\n(Stack is not empty)',
        'Returns element at S.top without any modification. S.size unchanged.'])
    add_table_row(ft, ['isEmpty(S: Stack) \u2192 Boolean',
        'None (always valid)',
        'Returns TRUE if S.size == 0, FALSE otherwise. Stack state unchanged.'])
    add_table_row(ft, ['isFull(S: Stack) \u2192 Boolean',
        'None (always valid)',
        'Returns TRUE if S.size == S.maxSize, FALSE otherwise. Stack state unchanged.'])
    doc.add_paragraph('Table 5: Formal Stack Specification Using Imperative Definition')
    doc.add_paragraph(
        'This formal specification is implementation-independent: it could be implemented using an '
        'array-based approach (fixed size, O(1) access) or a linked-list approach (dynamic size, '
        'additional pointer overhead). The specification only describes WHAT the operations do, not '
        'HOW they achieve it, which is the fundamental principle of abstraction in software engineering.'
    )

    # D1
    doc.add_heading('D1: Routing Analysis \u2013 Dijkstra\u2019s vs A* Algorithm', level=2)
    doc.add_paragraph(
        'Swift-Load Logistics operates across a network of warehouses, distribution centers, and '
        'delivery points connected by roads of varying distances and conditions. Finding the most '
        'efficient delivery route is critical for minimizing fuel costs, reducing delivery times, '
        'and maximizing the number of deliveries per day. We analyze two prominent shortest-path '
        'algorithms applied to the distribution network graph.'
    )
    bold_para(doc, "Dijkstra's Algorithm:")
    doc.add_paragraph(
        "Dijkstra's algorithm, published by Edsger Dijkstra in 1959, finds the shortest path from "
        "a single source node to all other nodes in a weighted graph with non-negative edge weights. "
        "It maintains a priority queue of unvisited nodes sorted by their tentative distances. At each "
        "step, it selects the unvisited node with the smallest tentative distance, marks it as visited, "
        "and updates the distances of its neighbors. Its evaluation function is f(n) = g(n), where "
        "g(n) is the actual accumulated cost from the source to node n. Because it explores uniformly "
        "in all directions, it is classified as an uninformed or blind search algorithm."
    )
    bold_para(doc, 'A* Search Algorithm:')
    doc.add_paragraph(
        'A* (A-Star), introduced by Hart, Nilsson, and Raphael in 1968, enhances Dijkstra by '
        'incorporating a heuristic function h(n) that estimates the remaining cost from node n to '
        'the goal. Its evaluation function is f(n) = g(n) + h(n), where g(n) is the actual cost '
        'from the source and h(n) is the heuristic estimate. Common heuristics for geographic routing '
        'include Euclidean distance (straight-line) and Manhattan distance. When the heuristic is '
        'admissible (never overestimates the true cost), A* is guaranteed to find the optimal shortest '
        'path while exploring significantly fewer nodes than Dijkstra, because it prioritizes nodes '
        'that appear to lead toward the goal rather than exploring uniformly in all directions.'
    )

    ct = doc.add_table(rows=1, cols=3)
    ct.style = 'Table Grid'
    styled_header_row(ct, ['Feature', "Dijkstra's Algorithm", 'A* Algorithm'])
    add_table_row(ct, ['Evaluation Function', 'f(n) = g(n)', 'f(n) = g(n) + h(n)'])
    add_table_row(ct, ['Search Type', 'Uninformed (blind expansion)', 'Informed (heuristic-guided)'])
    add_table_row(ct, ['Time Complexity', 'O(V\u00b2) or O((V+E) log V)', 'O(E) best case with good heuristic'])
    add_table_row(ct, ['Space Complexity', 'O(V)', 'O(V) for open/closed sets'])
    add_table_row(ct, ['Optimality', 'Always finds shortest path', 'Optimal if h(n) is admissible'])
    add_table_row(ct, ['Nodes Explored', 'All reachable nodes (worst case)', 'Only nodes along promising paths'])
    add_table_row(ct, ['Best Use Case', 'Pre-computing ALL shortest paths', 'Real-time single-destination routing'])
    add_table_row(ct, ['Swift-Load Verdict', 'Use for overnight route pre-computation', 'Use for live GPS truck navigation'])
    doc.add_paragraph('Table 6: Comprehensive Dijkstra vs A* Comparison for Delivery Routing')
    doc.add_paragraph(
        'For the Swift-Load GPS navigation system, A* is the recommended algorithm because it '
        'dramatically reduces computation time by focusing the search toward the delivery destination. '
        "However, Dijkstra's algorithm remains valuable for overnight batch processing where all "
        'possible routes between warehouses are pre-computed and cached for quick lookup during '
        'business hours.'
    )

    # P6
    doc.add_heading('P6: Effectiveness Assessment via Big O Notation', level=2)
    doc.add_paragraph(
        'Asymptotic analysis using Big O notation provides a hardware-independent mathematical '
        'framework for evaluating algorithm effectiveness. Big O describes the upper bound of an '
        "algorithm's growth rate as input size N approaches infinity, deliberately stripping away "
        'constant factors and lower-order terms to reveal the fundamental scaling behavior. This '
        'allows engineers to compare algorithms on a level playing field, regardless of whether '
        'one was tested on a modern server and another on an embedded truck computer.'
    )
    doc.add_paragraph(
        'The key insight of Big O is that constant factors become irrelevant at scale. An O(N) '
        'algorithm running on a slow computer will eventually outperform an O(N\u00b2) algorithm on '
        'a supercomputer as N grows sufficiently large. For Swift-Load, where N could represent '
        'millions of packages during holiday seasons, this mathematical guarantee is essential '
        'for system reliability.'
    )
    bt = doc.add_table(rows=1, cols=4)
    bt.style = 'Table Grid'
    styled_header_row(bt, ['Algorithm', 'Big O Complexity', 'Operations for N=100', 'Operations for N=1,000,000'])
    add_table_row(bt, ['AVL Tree Search', 'O(log N)', '~7', '~20'])
    add_table_row(bt, ['Linear Search', 'O(N)', '100', '1,000,000'])
    add_table_row(bt, ['Bubble Sort', 'O(N\u00b2)', '10,000', '1,000,000,000,000 (1 trillion)'])
    add_table_row(bt, ['QuickSort', 'O(N log N)', '~664', '~20,000,000'])
    add_table_row(bt, ["Dijkstra (priority queue)", 'O((V+E) log V)', '~1,400', '~40,000,000'])
    doc.add_paragraph('Table 7: Big O Scaling Comparison Across System Algorithms')

    # P7
    doc.add_heading('P7: Two Methods for Measuring Algorithm Efficiency', level=2)
    doc.add_paragraph(
        'Algorithm efficiency is measured through two complementary dimensions: time complexity '
        '(how long it takes to execute) and space complexity (how much memory it consumes). Both '
        'dimensions are critical for the Swift-Load system, where cargo processing must be fast '
        'enough for real-time operations and memory-efficient enough for embedded truck computers.'
    )
    bold_para(doc, 'Method 1: Execution Time Measurement (Time Complexity)')
    doc.add_paragraph(
        'In our C++ implementation, we measured wall-clock execution time using the <chrono> library '
        'with nanosecond precision. The high_resolution_clock captures timestamps immediately before '
        'and after algorithm execution, and duration_cast converts the elapsed time to nanoseconds. '
        'We also counted the exact number of comparisons and swaps performed by each algorithm, '
        'providing a hardware-independent metric alongside the wall-clock measurement.'
    )
    add_code_block(doc, '''auto start = high_resolution_clock::now();
bubbleSort(weights);  // Algorithm under test
auto stop = high_resolution_clock::now();
auto duration = duration_cast<nanoseconds>(stop - start);
cout << "Execution Time: " << duration.count() << " nanoseconds";
// Results: Bubble Sort: 66 comparisons, 34 swaps
//          QuickSort:   34 comparisons, 23 swaps''')

    bold_para(doc, 'Method 2: Memory Usage Analysis (Space Complexity)')
    doc.add_paragraph(
        'Space complexity is measured by analyzing the memory footprint of data structures using '
        'the C++ sizeof operator and by analyzing the auxiliary space required by algorithms. '
        'In our system, sizeof(Goods) returns approximately 80 bytes per object, while sizeof(AVLNode) '
        'returns approximately 104 bytes due to the additional left pointer, right pointer, and height '
        'integer field. An array of 8 Goods items uses ~640 bytes of contiguous memory, while 8 AVL nodes '
        'require ~832 bytes plus pointer overhead. This demonstrates the fundamental space-time trade-off: '
        'the AVL tree uses approximately 30% more memory per item but delivers O(log N) search performance '
        'versus O(N) for a linear array scan.'
    )

    # M5
    doc.add_heading('M5: Trade-off Interpretation in ADT Specification', level=2)
    doc.add_paragraph(
        'A trade-off in software engineering occurs when optimizing one property of a system '
        'necessarily degrades another property. In data structure design, the most fundamental '
        'and pervasive trade-off is between time efficiency (speed of operations) and space '
        'efficiency (memory consumption). Every design decision in the Swift-Load system involves '
        'navigating this trade-off to find the optimal balance for the specific operational requirements.'
    )
    bold_para(doc, 'Concrete Example: Choosing Between Hash Table, AVL Tree, and Linked List')
    doc.add_paragraph(
        'When designing the warehouse inventory system, we evaluated three candidate data structures, '
        'each representing a different position on the time-space trade-off spectrum:'
    )
    tt = doc.add_table(rows=1, cols=4)
    tt.style = 'Table Grid'
    styled_header_row(tt, ['Data Structure', 'Search Time', 'Memory Per Item', 'Trade-off Analysis'])
    add_table_row(tt, ['Hash Table', 'O(1) average', 'High (~200 bytes: data + bucket + hash)',
        'Fastest possible search but wastes significant memory on empty buckets and hash storage. '
        'Load factor management adds complexity.'])
    add_table_row(tt, ['AVL Tree (chosen)', 'O(log N)', 'Moderate (~104 bytes: data + 2 pointers + height)',
        'Excellent search speed with reasonable memory overhead. Self-balancing guarantees worst-case '
        'O(log N). Ordered traversal is free.'])
    add_table_row(tt, ['Linked List', 'O(N)', 'Low (~88 bytes: data + 1 pointer)',
        'Most memory-efficient per node but search time is linear. Unacceptable for real-time '
        'package tracking with thousands of items.'])
    doc.add_paragraph('Table 8: Space-Time Trade-off Comparison for Inventory Management')
    doc.add_paragraph(
        'For Swift-Load Logistics, we chose the AVL Tree as the optimal trade-off. The Hash Table\'s '
        'O(1) search time is attractive, but its memory waste from empty buckets and the complexity '
        'of hash collision resolution make it unsuitable for the memory-constrained embedded computers '
        'installed in delivery trucks. The Linked List\'s minimal memory footprint is appealing, but '
        'its O(N) search time would create multi-second delays when tracking packages in a warehouse '
        'with tens of thousands of items, violating the real-time performance requirements. The AVL '
        'Tree\'s O(log N) search with moderate memory overhead represents the ideal middle ground, '
        'providing sub-millisecond search times while fitting comfortably within the truck computer\'s '
        'available RAM.'
    )
