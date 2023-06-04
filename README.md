## Table of Contents

- [Introduction](#introduction)
- [Usage](#usage)
- [Active Object Implementation](#active-object-implementation)
- [Task Creation](#task-creation)
- [Queue Implementation](#queue-implementation)
- [Example Tasks](#example-tasks)
- [Compiling and Running the Program](#compiling-and-running-the-program)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The Active Object is a software design pattern that enables concurrent and asynchronous execution of tasks. It decouples the execution of tasks from their invocation, allowing tasks to be performed concurrently in separate threads. This implementation provides a framework for creating and managing active objects, as well as a set of example tasks that can be executed by the active objects.

## Usage

To use the Active Object framework, you need to define the tasks you want to execute and their corresponding task functions. The provided example includes four tasks:

1. **ActiveObjectTask1**: Generates random numbers and enqueues tasks for other active objects.
2. **ActiveObjectTask2**: Checks if a number is prime and enqueues a new task with an incremented number.
3. **ActiveObjectTask3**: Checks if a number is prime and enqueues a new task with a decremented number.
4. **ActiveObjectTask4**: Checks if a number is prime and prints it along with its next consecutive number.

You can define your own tasks by creating new task functions that perform the desired operations.

## Active Object Implementation

The Active Object implementation consists of the following files:

- **Active_Object.h**: Header file containing the Active Object and Task structures, as well as function prototypes.
- **Active_Object.c**: Source file implementing the Active Object functions, such as object creation, execution, and termination.

The Active Object is created using the `CreateActiveObject` function, which takes a task function pointer as an argument. The Active Object runs the specified task function in a separate thread, executing tasks from its associated queue.

The Active Object can be stopped using the `stopActiveObject` function, which terminates the thread and frees associated resources.

## Task Creation

Tasks are created using the `createTask` function, which takes the number of tasks to be performed and optional data as arguments. Each task is defined by the number of iterations to be executed and any additional data required by the task function.

Tasks are destroyed using the `destroyTask` function, which frees the allocated memory.

## Queue Implementation

The Queue implementation consists of the following files:

- **queue.h**: Header file containing the Queue structure and function prototypes.
- **queue.c**: Source file implementing the Queue functions, such as creation, destruction, enqueue, dequeue, and other utility functions.

The Queue provides a thread-safe data structure for storing tasks. It uses a linked list to maintain the order of tasks and allows for efficient enqueue and dequeue operations.

## Example Tasks

The provided example includes four tasks that demonstrate the usage of the Active Object framework:

1. **ActiveObjectTask1**: Generates random numbers and enqueues tasks for other active objects.
2. **ActiveObjectTask2**: Checks if a number is prime and enqueues a new task with an incremented number.
3. **ActiveObjectTask3**: Checks if a number is prime and enqueues a new task with a decremented number.
4. **ActiveObjectTask4**: Checks if a number is prime and prints it along with its next consecutive number.

You can modify or add your own tasks by defining new task functions and registering them in the `Functions_Array` in the `main` function.

## Compiling and Running the Program

To compile and run the program using the provided Makefile, follow these steps:

1. Create a

file named `Makefile` in the same directory as your source code files.
2. Copy and paste the contents of the provided Makefile into your `Makefile` file.
3. Open a terminal or command prompt and navigate to the directory containing your source code and the Makefile.
4. Run the following command to compile the program:

   ```
   make
   ```

   This command will execute the `Makefile` and compile the source files into an executable named `st_pipeline`.

5. Once the compilation is successful, run the following command to execute the program:

   ```
   ./st_pipeline <number> 
   ```

   Replace `<number>` with the desired number of times to execute. Optionally, you can provide a seed for the random number generator.

6. The program will run and display the output based on the provided command-line arguments.

To clean up the generated executable and object files, you can run the following command:

```
make clean
```

This command will remove the `st_pipeline` executable from the directory.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvement, please open an issue or a pull request on the GitHub repository.

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code for both commercial and non-commercial purposes.