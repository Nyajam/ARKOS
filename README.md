# ARKOS

## About

This code is a framework for concurrent programming in Arduino (including basics
boards) like a operating system.


The concept of ARKOS is that programs are functions in the main code, this
functions have to be programmed in sections that do not run all in a row in one
time. Therefore, the sections are parts of the code of the function and one
section is executed (one at a time) depending of context of process. The key of
sections is doing them small (small but worth it).
The function uses as memory global variables for data and shared variables while
the function's local variables that don't get lost will have a small memory in
the process.

The functions are executed in ARKOS in one loop and ARKOS is the one who sort
this executions. It is concurrent programming but it is a non-expulsive context
switch.

The current version initializes many programs but don't end them or load any
more after the initialization.

### Version

1.0

### License

GPL3

### Author

Ildefonso Macarro Pueyo <fusm7id@gmail.com>

## Install

Download the project and unzip it to the "libraries" folder of the arduino IDE.
The result would be something like:

``` sh
Arduino
└── libraries
    └── ARKOS
        ├── examples
        │   └── ARKOS
        │       └── ARKOS.ino
        ├── library.properties
        ├── LICENSE
        ├── README.md
        └── src
            ├── ARKOS.c
            └── ARKOS.h

```

## How to use

### Code

The key is to divide the functions into sections that do what we want:

``` C
#include <ARKOS.h>

void myFunction()
{
	PCB* pcb = myPCB();
	if(pcb == NULL) //ERROR
		return;
	if(pcb->data == NULL) //First time execute
	{
		pcb->data=(ARKOS_TYPE_DATA_PCB*)alloc(1*sizeof(ARKOS_TYPE_DATA_PCB)); //Reserve data memory
		pcb->data[0]=0;
	}
	switch(pcb->data[0])
	{
		case 0:
			digitalPrint(LED_BUILTIN, LOW);
			pcb->data[0] = 1;
			break;
		case 1:
			digitalPrint(LED_BUILTIN, HIGH);
			pcb->data[0] = 0;
			break;
	}
	/*
	"sleep()" Does not change or interrupt the process by this function, only
	sets the sleep time when the function ends (in this iteration).
	*/
	sleep(1000);
}
```

The PCB is Process Control Block, it is a struct. The function's own PCB is
obtained with function **myPCB**, in this PCB we will store our data in the
element of struct, "**data**".

The type of PCB.data is by default **int**, but the define is in
**ARKOS_TYPE_DATA_PCB**.

The function does not have arguments and returns void.

### Start ARKOS

It is very simple:

``` C
#include <ARKOS.h>

void setup()
{
	void (*programs[])()={
		myFunction,
		NULL}; //List of the "programs", the functions.
	voidPcbs(); //initialize the table of process
	chargePrograms(programs); //charge the list of programs
}
void loop()
{
	run(); //Execute ARKOS (is a loop, no exit to loop() function
}
```

First create a list of programs that is an array to reference all concurrent
functions that will be executed in ARKOS.
Then initialize the table of PCBs (to zero) with **voidPcbs**.
Finally load programs list in ARKOS with **chargePrograms**.

And to execute ARKOS, launch ARKOS with **run**.

### Points of interest

- The function does not have arguments and returns void.
- The define ARKOS_TYPE_DATA_PCB is the type of variable in PCB (PCB.data).
- The PCB is obtained with **myPCB()**.
- The process start again at the time set with **sleep()**.
- The sleep **does not interrupt** the code flow.
- ARKOS is a framework for concurrency but ARKOS isn't concurrent (for
interrupts).
- The define ARKOS_SIZE_LIST_PCB is the number of processes.
- The PCB has an array of data for the internal working of the process:
**PCB.data**
