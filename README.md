# Circular Queue in C (Ring Buffer Architecture)

[![CI](https://github.com/i7modes/Queues/actions/workflows/ci.yml/badge.svg)](https://github.com/i7modes/Queues/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![C Standard](https://img.shields.io/badge/C-C99-blue.svg)
![Memory Status](https://img.shields.io/badge/Valgrind-0%20Leaks-brightgreen.svg)

A high-performance, cache-friendly implementation of a **Circular Queue (Ring Buffer)** in C99. Operates in strict $\mathcal{O}(1)$ time without memory shifting, featuring modulo wraparound, safe boundary handling, automated test validation, sliding-window algorithms, and zero memory leaks.

---

## Architecture: Circular Ring Buffer

Standard linear array queues require shifting all remaining elements left by one position upon every dequeue ($\mathcal{O}(n)$ time). A **Circular Ring Buffer** solves this by wrapping `front` and `rear` pointers modulo capacity.

### Initial State:
```text
      [0]        [1]        [2]        [3]        [4]
   +----------+----------+----------+----------+----------+
   |    10    |    20    |    30    |  (empty) |  (empty) |
   +----------+----------+----------+----------+----------+
        ^                                  ^
        |                                  |
      Front                               Rear (Next insertion index)
```

### After Dequeueing 2 Elements & Enqueueing 4 More (Wraparound):
```text
      [0]        [1]        [2]        [3]        [4]
   +----------+----------+----------+----------+----------+
   |    60    |    70    |  (empty) |    30    |    40    |
   +----------+----------+----------+----------+----------+
                             ^             ^
                             |             |
                            Rear         Front
```

### Why use a Circular Ring Buffer?
1. **Zero Element Shifting**: Dequeuing merely increments `front = (front + 1) % capacity` in $\mathcal{O}(1)$ time.
2. **Contiguous Memory**: Maximum CPU cache line locality compared to pointer-heavy node-based queues.
3. **Graceful Overflow/Underflow**: Rejects full enqueues and empty dequeues with boolean return codes without aborting the process.

---

## Features

- **$\mathcal{O}(1)$ Core Operations**: `Queue_Enqueue`, `Queue_Dequeue`, `Queue_Peek`, `Queue_IsEmpty`, `Queue_IsFull`, `Queue_Size`.
- **Circular Wraparound**: Flawlessly wraps around buffer boundaries under heavy throughput.
- **Sliding Window Moving Average (`Queue_MovingAverage`)**: Efficiently tracks moving averages across streaming data.
- **Robust Memory Management**: Complete teardown via `Queue_Destroy(&q)` with 0 memory leaks (verified by Valgrind).
- **Automated Unit Testing & CI**: 39 unit test assertions tested across Ubuntu and Windows with Clang AddressSanitizer and Valgrind.
- **Legacy Compatibility**: Full backward compatibility for classic Weiss-style signatures (`CreateQueue`, `Enqueue`, `Dequeue`, `Front`, `FrontAndDequeue`, `DisposeQueue`, etc.).

---

## Complexity Analysis

| Operation | Time Complexity | Space Complexity | Description |
| :--- | :---: | :---: | :--- |
| `Queue_Enqueue` | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ | Inserts item at rear with modulo wrap |
| `Queue_Dequeue` | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ | Removes item at front with modulo wrap |
| `Queue_Peek` | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ | Inspects front item without removal |
| `Queue_IsEmpty` | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ | Compares size to 0 |
| `Queue_IsFull` | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ | Compares size to capacity |
| `Queue_Size` / `Capacity` | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ | Returns cached metrics |
| `Queue_MovingAverage` | $\mathcal{O}(n)$ | $\mathcal{O}(w)$ | Calculates rolling window average ($w$ = window size) |
| `Queue_Destroy` | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ | Deallocates array buffer and handle |

---

## Project Structure

```text
Queues/
├── include/
│   └── queue.h                  # Public API, types, and legacy aliases
├── src/
│   └── queue.c                  # Circular queue implementation & moving average
├── examples/
│   └── demo.c                   # Terminal showcase demonstrating wraparound
├── tests/
│   └── test_queue.c             # Automated unit test suite (39 assertions)
├── .github/
│   └── workflows/
│       └── ci.yml               # GitHub Actions CI matrix
├── Makefile                     # Cross-platform build script
├── LICENSE                      # MIT License
├── .gitignore                   # Ignore binaries and temporary files
└── README.md                    # Project documentation
```

---

## Quick Start

### 1. Clone the Repository
```bash
git clone https://github.com/i7modes/Queues.git
cd Queues
```

### 2. Build and Run Demo
```bash
make demo
make run-demo
```

### 3. Run Automated Unit Tests
```bash
make check
```

Expected output:
```text
========================================
  Circular Queue Automated Tests
========================================

--- Test: Initialization & Empty Safety ---
  [PASS] Queue_Create returns non-NULL
  [PASS] New queue is empty
  [PASS] New queue is not full
...
========================================
  Results: 39/39 assertions passed
========================================
```

### 4. Run Memory Leak Check (Linux / WSL)
```bash
make test
valgrind --leak-check=full --show-leak-kinds=all ./bin/test_runner
```

---

## Code Example

```c
#include "queue.h"
#include <stdio.h>

int main(void) {
    // 1. Create ring buffer with capacity 4
    Queue *q = Queue_Create(4);

    // 2. Enqueue elements
    Queue_Enqueue(q, 10);
    Queue_Enqueue(q, 20);
    Queue_Enqueue(q, 30);
    Queue_Print(q); // [Front: 10] -> [20] -> [Rear: 30] (Size: 3/4)

    // 3. Dequeue and wrap around
    int val;
    Queue_Dequeue(q, &val); // Removes 10
    Queue_Enqueue(q, 40);
    Queue_Enqueue(q, 50);   // Wraps around index 0
    Queue_Print(q); // [Front: 20] -> [30] -> [40] -> [Rear: 50] (Size: 4/4)

    // 4. Moving Average Algorithm
    int stream[] = {1, 10, 3, 5, 2};
    double avgs[5];
    Queue_MovingAverage(stream, 5, 3, avgs);

    // 5. Clean up memory
    Queue_Destroy(&q); // q is now NULL, 0 memory leaks!
    return 0;
}
```

---

## License

Distributed under the [MIT License](file:///c:/Users/i7mod/OneDrive/GitHub/Queues/LICENSE). Created by [i7modes](https://github.com/i7modes).
