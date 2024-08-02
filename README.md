# Concurrent Programming and Synchronization Mechanisms

This project is a simulation of a news broadcasting system implemented using concurrent programming and synchronization mechanisms in C++. It involves multiple producers generating news stories, a dispatcher sorting these stories, co-editors processing them, and a screen manager displaying them.

## Table of Contents
- [General Description](#general-description)
- [System Design](#system-design)
- [Bounded Buffer](#bounded-buffer)
- [Configuration File](#configuration-file)
- [Compilation and Execution](#compilation-and-execution)
- [Running the Tests](#running-the-tests)
  - [Output Files](#output-files)

## General Description

This project features a simulation of a news broadcasting system. Different types of stories (SPORTS, NEWS, WEATHER) are generated, sorted, processed, and displayed in real-time. The implementation showcases the performance impact of various synchronization constructs under different workloads, highlighting the challenges and solutions in concurrent programming.

## System Design

The system consists of the following active participants:

#### Producer

Each producer creates several strings in the following format:

```
“producer <i> <type> <j>”
```

- `<i>`: Producer's ID
- `<type>`: A random type chosen from `SPORTS`, `NEWS`, `WEATHER`
- `<j>`: The number of strings of type `<type>` this producer has already produced

The number of products a producer makes is specified via its constructor.

For example, if producer 2 needs to create 3 strings, a possible outcome could be:

```
Producer 2 SPORTS 0
Producer 2 SPORTS 1
Producer 2 WEATHER 0
```

Each producer sends its information to the Dispatcher (introduced below) via its own private queue. Each producer's private queue is shared between the Producer and the Dispatcher. Each of the string products is inserted by the Producer into its 'producer queue'. After inserting all the products, the Producer sends a 'DONE' string through its queue.

#### Dispatcher

The Dispatcher continuously accepts messages from the producers' queues. It scans the producers' queues using a Round Robin algorithm. The Dispatcher does not block when the queues are empty. Each message is "sorted" by the Dispatcher and inserted into one of the Dispatcher queues, which include strings of a single type. When the Dispatcher receives a "DONE" message from all producers, it sends a "DONE" message through each of its queues.

- **SPORTS**: Inserted into the "S dispatcher queue"
- **NEWS**: Inserted into the "N dispatcher queue"
- **WEATHER**: Inserted into the "W dispatcher queue"

#### Co-Editors

For each type of possible message, there is a Co-Editor that receives the message through the Dispatcher’s queue, "edits" it, and passes it to the screen manager via a single shared queue. The editing process is simulated by the Co-Editors by blocking for one-tenth (0.1) of a second. When a Co-Editor receives a "DONE" message, it passes it without waiting through the shared queue.

#### Screen Manager

The Screen Manager displays the strings it receives via the Co-Editors' queue to the screen (standard output). After printing all messages to the screen and receiving three "DONE" messages, the Screen Manager displays a 'DONE' statement.

The design can be visualized as follows:

![image (1)](https://github.com/user-attachments/assets/04ad58e7-6804-459f-9418-f05c5fe19a5f)

Three producers communicate with the dispatcher via their Producer queues. The Dispatcher communicates with the Co-Editors
via three queues corresponding to the three types of messages. The Co-Editors communicate with the
Screen-Manager via a single shared queue, and the Screen-Manager displays the system's output.

## Bounded Buffer

The producer queues and the co-editors' shared queue are implemented as bounded buffers. The buffer supports the following operations:
- `Bounded_Buffer (int size)`: Constructor that creates a bounded buffer of specified size.
- `void insert (char * s)`: Inserts an object into the buffer.
- `char * remove ()`: Removes and returns the first object from the buffer.

To solve the critical section problem in the bounded buffer implementation, I used a combination of binary semaphores (mutexes) and counting semaphores. This ensures thread-safe access to the shared buffer by multiple producers and consumers.

#### Key Components:
- **Binary Semaphore (Mutex)**: Ensures mutual exclusion so that only one thread can access the buffer at a time.
- **Counting Semaphores**: Track the number of empty and full slots in the buffer.
- **Buffer Queue**: Stores the items.
- **Mutex (mtx)**: Ensures thread-safe access to the buffer.

### Configuration File

The configuration file should have the following format:

```
PRODUCER 1
[number of products]
queue size = [size]

PRODUCER 2
[number of products]
queue size = [size]

...
...

PRODUCER n
[number of products]
queue size = [size]

Co-Editor queue size = [size]
```

For example, the following are legal configuration files:

```
PRODUCER 1
30
queue size = 5

PRODUCER 2
25
queue size = 3

PRODUCER 3
16
queue size = 30

Co-Editor queue size = 17
```

## Compilation and Execution

To compile and run the program, follow these steps:

1. **Compilation**:
   ```sh
   make
   ```
2. **Execution**:
   ```sh
   ./ProdCons config.txt
   ```
   
## Running the Tests

The tests are located in the `Tests` folder and include 4 configuration files, a shell script to check message counts, and a Python script to automate the testing process.

1. **Navigate to the Tests directory**:
   The shell script checks the message counts.
   ```sh
   cd Tests
   ```
3. **Running the Shell Script:**:
   ```sh
   chmod +x message_count_tester.sh
   ./message_count_tester.sh
   ```   
4. **Running the Python Test Script**:
   The Python script automates the testing process and checks the program's behavior under various configurations.
   ```sh
   python3 prodcons_tester.py
   ```   
### Output Files

For each test configuration, the script generates an output file containing the results of the program's execution. These files are named according to the configuration (e.g., `config1_output.txt`, `config2_output.txt`). Additionally, a summary of the test results is written to `test_results.txt`, which includes pass/fail statuses and any encountered errors.

   
