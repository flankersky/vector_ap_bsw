# OS Abstraction Layer

This component defines an OS Abstraction Layer which hides OS specific APIs and functionality.

The OS Abstraction Layer presents a common interface for all OSes and can be compiled and built against any of the
target OS. All other production code is prohibited from accessing the OS directly; rather it must use the OS
abstraction wrapper instead.


## Provided Abstraction

- Process

  A process is an instance of a program image that is being executed. It has its own address space
  and a set of system resources.

- Message queue

  Message queues allow unidirectional communication between two processes.

- I/O

  Input/Output related functionality.

- OS Controller
  Allows the shutdown and restart of the OS


## Driver

For some operating systems there might be kernel driver needed to provide the required functionality.
Driver are located in the driver folder.

