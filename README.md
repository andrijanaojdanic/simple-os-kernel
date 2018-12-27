# simple-os-kernel
An implementation of simple but completely functional kernel for multithreaded os with support for time sharing. College project. C++
Environment : Eclipse with support for Borland C++

How to test: project needs to be run from cmd on a 32b version of Windows. Input arguments: buffer_capacity, number_of_producers, time_slice.

buffer_capacity - capacity of a buffer that takes in interrupts


number_of_producers - producers are threads that alternately display ciphers [0-9] on screen;


time_slice - number of time quantums that each of the producers has for producing their output before they yield processor to another producer 


------> interrupts are simulated by key events; every time when user presses a key, producers yield to interrupt routine which displays the character that corresponds to pressed key


------> producers simulate system and user tasks

