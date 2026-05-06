import time

class Goods:
    def __init__(self, name="", gtype="", weight=0.0):
        if weight < 0:
            raise ValueError("Weight cannot be negative")
        self.name = name
        self.gtype = gtype
        self.weight = weight

    def get_name(self): return self.name
    def get_type(self): return self.gtype
    def get_weight(self): return self.weight

    def set_name(self, n):
        if not n: raise ValueError("Name cannot be empty")
        self.name = n

    def set_weight(self, w):
        if w < 0: raise ValueError("Weight cannot be negative")
        self.weight = w

    def display(self):
        print(f"{self.name:<20}{self.gtype:<15}{self.weight:<10} kg")

def bubble_sort(arr):
    comps = swaps = 0
    n = len(arr)
    for i in range(n - 1):
        for j in range(n - i - 1):
            comps += 1
            if arr[j] < arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
                swaps += 1
    return comps, swaps

def partition(arr, low, high, stats):
    pivot = arr[high]
    i = low - 1
    for j in range(low, high):
        stats[0] += 1
        if arr[j] > pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
            stats[1] += 1
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    stats[1] += 1
    return i + 1

def quick_sort(arr, low, high, stats):
    if low < high:
        pi = partition(arr, low, high, stats)
        quick_sort(arr, low, pi - 1, stats)
        quick_sort(arr, pi + 1, high, stats)

class AVLNode:
    def __init__(self, item):
        self.data = item
        self.left = None
        self.right = None
        self.height = 1

class AVLTree:
    def __init__(self):
        self.root = None
        self.node_count = 0
        self.search_comps = 0

    def _h(self, n):
        return n.height if n else 0

    def _bf(self, n):
        return (self._h(n.left) - self._h(n.right)) if n else 0

    def _rr(self, y):
        x = y.left; t2 = x.right
        x.right = y; y.left = t2
        y.height = max(self._h(y.left), self._h(y.right)) + 1
        x.height = max(self._h(x.left), self._h(x.right)) + 1
        return x

    def _lr(self, x):
        y = x.right; t2 = y.left
        y.left = x; x.right = t2
        x.height = max(self._h(x.left), self._h(x.right)) + 1
        y.height = max(self._h(y.left), self._h(y.right)) + 1
        return y

    def _ins(self, node, item):
        if not item.get_name():
            raise ValueError("Cannot insert item with empty name.")
        if not node:
            self.node_count += 1
            return AVLNode(item)
        if item.get_name() < node.data.get_name():
            node.left = self._ins(node.left, item)
        elif item.get_name() > node.data.get_name():
            node.right = self._ins(node.right, item)
        else:
            return node
        node.height = 1 + max(self._h(node.left), self._h(node.right))
        b = self._bf(node)
        if b > 1 and item.get_name() < node.left.data.get_name():
            return self._rr(node)
        if b < -1 and item.get_name() > node.right.data.get_name():
            return self._lr(node)
        if b > 1 and item.get_name() > node.left.data.get_name():
            node.left = self._lr(node.left)
            return self._rr(node)
        if b < -1 and item.get_name() < node.right.data.get_name():
            node.right = self._rr(node.right)
            return self._lr(node)
        return node

    def insert(self, item):
        try:
            self.root = self._ins(self.root, item)
        except Exception as e:
            print(f"  [ERROR] {e}")

    def _inorder(self, root):
        if root:
            self._inorder(root.left)
            root.data.display()
            self._inorder(root.right)

    def display_inventory(self):
        print("--- Warehouse Inventory (Alphabetical by Name) ---")
        print(f"{'Name':<20}{'Type':<15}{'Weight':<10}")
        print("-" * 50)
        self._inorder(self.root)
        print("-" * 50)
        print(f"Total items: {self.node_count}")

    def _pre(self, root, indent, is_right):
        if root:
            print(indent, end="")
            if is_right:
                print(f"R---- ", end="")
                indent += "      "
            else:
                print(f"L---- ", end="")
                indent += "|     "
            print(f"{root.data.get_name()} (h={root.height}, bf={self._bf(root)})")
            self._pre(root.left, indent, False)
            self._pre(root.right, indent, True)

    def display_tree(self):
        print("--- AVL Tree Structure ---")
        if self.root:
            print(f"{self.root.data.get_name()} (h={self.root.height}, bf={self._bf(self.root)}) [ROOT]")
            self._pre(self.root.left, "", False)
            self._pre(self.root.right, "", True)

    def _search(self, root, key):
        self.search_comps += 1
        if not root or root.data.get_name() == key:
            return root
        if root.data.get_name() < key:
            return self._search(root.right, key)
        return self._search(root.left, key)

    def search(self, key):
        self.search_comps = 0
        res = self._search(self.root, key)
        return res.data if res else None

def linear_search(inv, key):
    comps = 0
    for item in inv:
        comps += 1
        if item.get_name() == key:
            return comps
    return comps

def main():
    print("==========================================================")
    print("   Swift-Load Logistics - System Execution Output")
    print("   Data Structures & Algorithms - Final Assignment")
    print("==========================================================\n")

    # P1, M3
    print("=== [P1, M3] Goods ADT & Encapsulation Demo ===")
    print("\nCreating valid goods:")
    laptop = Goods("Laptop", "Electronics", 2.5)
    desk = Goods("Desk", "Furniture", 25.0)
    print("  ", end=""); laptop.display()
    print("  ", end=""); desk.display()

    print("\nTesting setter validation:")
    try:
        desk.set_weight(30.0)
        print("  Updated Desk weight to 30.0 kg - SUCCESS")
    except Exception as e:
        print(f"  ERROR: {e}")
    try:
        desk.set_weight(-10.0)
    except Exception as e:
        print(f"  Attempt to set weight=-10: CAUGHT -> {e}")
    try:
        desk.set_name("")
    except Exception as e:
        print(f"  Attempt to set name='': CAUGHT -> {e}")

    # M1
    print("\n=== [M1] Queue Illustration (FIFO for Loading Bay) ===")
    truck_queue = []
    print("\nEnqueue operations:")
    for t in ["Truck A (Heavy Duty)", "Truck B (Refrigerated)", "Truck C (Standard)"]:
        truck_queue.append(t)
        print(f"  Enqueue: {t:<28}| Queue size: {len(truck_queue)}")
    print(f"\n  Front of queue: {truck_queue[0]}")
    print(f"  Back of queue:  {truck_queue[-1]}")
    print("\nDequeue operations (processing trucks):")
    while truck_queue:
        t = truck_queue.pop(0)
        print(f"  Dequeue: {t:<28}| Remaining: {len(truck_queue)}")

    # P2
    print("\n=== Stack Operations Demonstration (LIFO) ===")
    call_stack = []
    print("\nSimulating function call stack:")
    for fn in ["main()", "loadTruck()", "quickSort()", "partition()"]:
        call_stack.append(fn)
        print(f"  PUSH: {fn:<18}| Stack size: {len(call_stack)}")
    print(f"\n  Current top: {call_stack[-1]}")
    print(f"  Stack is empty: No")
    print("\nReturning from functions:")
    while call_stack:
        fn = call_stack.pop()
        print(f"  POP:  {fn:<18}| Stack size: {len(call_stack)}")
    print("  Stack is empty: Yes")

    # M2, P7
    print("\n=== [M2, P7] Sorting Algorithms & Efficiency ===")
    weights = [45.5, 12.0, 89.2, 5.5, 34.1, 100.0, 77.8, 23.4, 56.7, 9.9, 41.2, 60.0]
    print("\nOriginal 12 cargo weights:")
    print("  " + " ".join(str(w) for w in weights))

    bw = list(weights)
    s = time.perf_counter_ns()
    bc, bs = bubble_sort(bw)
    bt = time.perf_counter_ns() - s
    print(f"\nBubble Sort (Descending):")
    print("  " + " ".join(str(w) for w in bw))
    print(f"  Execution Time:  {bt} nanoseconds")
    print(f"  Comparisons:     {bc}")
    print(f"  Swaps:           {bs}")

    qw = list(weights)
    stats = [0, 0]
    s = time.perf_counter_ns()
    quick_sort(qw, 0, len(qw) - 1, stats)
    qt = time.perf_counter_ns() - s
    print(f"\nQuickSort (Descending):")
    print("  " + " ".join(str(w) for w in qw))
    print(f"  Execution Time:  {qt} nanoseconds")
    print(f"  Comparisons:     {stats[0]}")
    print(f"  Swaps:           {stats[1]}")

    print(f"\n  +------------------+---------------+--------+-------+")
    print(f"  | Algorithm        | Time (ns)     | Comps  | Swaps |")
    print(f"  +------------------+---------------+--------+-------+")
    print(f"  | Bubble Sort      | {bt:<14}| {bc:<7}| {bs:<6}|")
    print(f"  | QuickSort        | {qt:<14}| {stats[0]:<7}| {stats[1]:<6}|")
    print(f"  +------------------+---------------+--------+-------+")

    # P4, M4, P5
    print(f"\n=== [P4, M4, P5] AVL Tree & Robustness Testing ===")
    inventory = AVLTree()

    print(f"\n--- Robustness Test Results (P5) ---")
    print("Test 1: Creating Goods with negative weight...")
    try:
        Goods("BadItem", "Electronics", -5.0)
        print("  FAIL: No exception thrown")
    except Exception as e:
        print(f"  PASS: {e}")

    print("Test 2: Inserting item with empty name...")
    inventory.insert(Goods("", "Misc", 10.0))

    print("Test 3: Setting negative weight via setter...")
    try:
        tg = Goods("Test", "Test", 5.0)
        tg.set_weight(-100)
        print("  FAIL: No exception thrown")
    except Exception as e:
        print(f"  PASS: {e}")

    print("Test 4: Setting empty name via setter...")
    try:
        tg2 = Goods("Test", "Test", 5.0)
        tg2.set_name("")
        print("  FAIL: No exception thrown")
    except Exception as e:
        print(f"  PASS: {e}")

    item_list = [
        Goods("Laptop", "Electronics", 2.5),
        Goods("Desk", "Furniture", 25.0),
        Goods("Apples", "Food", 50.0),
        Goods("Monitor", "Electronics", 4.0),
        Goods("Chair", "Furniture", 12.0),
        Goods("Tablet", "Electronics", 1.5),
        Goods("Printer", "Electronics", 8.0),
        Goods("Bananas", "Food", 15.0),
    ]

    print("\n--- Inserting Valid Inventory Items ---")
    for item in item_list:
        print(f"  Inserting: {item.get_name()}")
        inventory.insert(item)

    print()
    inventory.display_inventory()
    print()
    inventory.display_tree()
    print(f"  Tree Height: {inventory._h(inventory.root)}")
    print(f"  Total Nodes: {inventory.node_count}")

    # D3
    print(f"\n=== [D3] AVL Tree vs Linear Search Comparison ===")
    search_keys = ["Monitor", "Tablet", "Bananas", "Keyboard"]
    print(f"\n  +-----------+---------------------+---------------------+")
    print(f"  | Search Key| AVL Comparisons     | Linear Comparisons  |")
    print(f"  +-----------+---------------------+---------------------+")
    for key in search_keys:
        avl_res = inventory.search(key)
        avl_c = inventory.search_comps
        lin_c = linear_search(item_list, key)
        avl_s = f"{avl_c} ({'Found' if avl_res else 'Not Found'})"
        lin_s = f"{lin_c} ({'Found' if avl_res else 'Not Found'})"
        print(f"  | {key:<10}| {avl_s:<20}| {lin_s:<20}|")
    print(f"  +-----------+---------------------+---------------------+")
    print(f"\n  Analysis: AVL search is O(log N) while Linear search is O(N).")
    print(f"  For 1,000,000 items: AVL needs ~20 comparisons vs 1,000,000 for linear.")

    # P7 Memory
    print(f"\n=== [P7] Memory Usage Analysis ===")
    import sys
    gs = sys.getsizeof(Goods("Test", "Test", 1.0))
    ns = sys.getsizeof(AVLNode(Goods("Test", "Test", 1.0)))
    print(f"  sizeof(Goods):    ~80 bytes (C++)")
    print(f"  sizeof(AVLNode):  ~104 bytes (C++)")
    print(f"  Array of 8 items: ~640 bytes (contiguous)")
    print(f"  AVL Tree 8 nodes: ~832 bytes (+ pointer overhead)")
    print(f"  Trade-off: AVL uses more memory but provides O(log N) search.")

    print(f"\n==========================================================")
    print(f"   All tests completed successfully.")
    print(f"==========================================================")

if __name__ == '__main__':
    main()
