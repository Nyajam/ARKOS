/*
  Name: ARKOS core
  Author: Ildefonso Macarro Pueyo
  Mail: fusm7id@gmail.com
  Date: 16/08/2022
  Comments: Framenwork for multithreading on arduino without context switching.
  Library header.
*/

#ifndef ARKOS_H
#define ARKOS_H 1

//Consts
#define ARKOS_SIZE_LIST_PCB 8 //Number of process
#define ARKOS_TYPE_DATA_PCB int //Tipe of storage data
//States of the process
#define ARKOS_PRUN 0 //Process in execution
#define ARKOS_PSLEEP 1 //Process wait to execute
#define ARKOS_PNULL 2 //Process not create

#include <Arduino.h>

//Structs
typedef struct PCB_t
{
  int id; //id
  int stat; //status of process (run, sleep, not create...)
  unsigned long actionTime; //time when wake
  ARKOS_TYPE_DATA_PCB* data; //private storage of process
  void (*function)(); //the code of process
  struct PCB_t* next; //next process for the process list
} PCB;

//Functions - headers
void voidPcbs(); //initialize the table of process
void chargePrograms(void (**programas)()); //Charge the programs of the table of process
void planner(PCB* pcb); //process scheduler
void run(); //execute the process
PCB* myPCB(); //return the PCB of the process
void sleep(unsigned long time); //set the time to wake the process

#endif
