/*
  Name: ARKOS core
  Author: Ildefonso Macarro Pueyo
  Mail: fusm7id@gmail.com
  Date: 16/08/2022
  Comments: Example of use with 3 "processes" of blinks.

  This is an example of one function (with three invokes) in role of program like a process.
  In the mayority of the operating systems, the process access to their PCB (Process Control Block),
  but in this case, Arduino (for ATmega) does not support the context switch, this solution makes one
  type of struct (PCB) for the order of the processes. To make the functions concurrent, they are made
  iterable.
  For example, in this case the blink sleeps a specific time, the function saves the state of led to
  print in this specific time, notifies this time to ARKOS (with sleep() in milli seconds) and goes to
  the end of the function, finishing the iteration. Then, ARKOS calls the function when the time of start
  has arrived.
*/

#include <ARKOS.h>

void blinking(); //The functions in role of programs, have a void return and the 

void setup()
{
  //initialize of ARKOS
  void (*programs[])()={
    blinking,
    blinking,
    blinking,
    NULL}; //List of the "programs", the functions. IMPORTANT: ->THE LIST HAS END WIHT NULL<-
  voidPcbs(); //initialize the table of process
  chargePrograms(programs); //charge the list of programs
  
  //setup for blink
  Serial.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN-1, OUTPUT);
  pinMode(LED_BUILTIN-2, OUTPUT);
  Serial.println("Start system");
  delay(1000);
}

void loop()
{
  run(); //execute ARKOS
}

void blinking()
{
  PCB* pcb = myPCB(); //get the Process Control Block of this process
  const long timeStop=(pow(2,pcb->id+1))*1000; //the time for sleep (calculating whit id of process)
  const int pin=LED_BUILTIN-pcb->id; //the pin out for the led (determinate whit id of process)

  if(pcb==NULL) //Error case
  {
    Serial.println("HELP");
    delay(1000);
    return;
  }

  if(pcb->data==NULL) //First execution
  {
    Serial.print("Start blink:"); //For debug in terminal
    Serial.println(pcb->id+1); //For debug in terminal
    pcb->data=(ARKOS_TYPE_DATA_PCB*)malloc(1*sizeof(ARKOS_TYPE_DATA_PCB)); //the space of save variables in the PCB
    pcb->data[0]=LOW; //start state of led
  }

  Serial.print("Execute:"); //For debug in terminal
  Serial.println(pcb->id+1); //For debug in terminal
  digitalWrite(pin, pcb->data[0]);
  if(pcb->data[0]==LOW)
    pcb->data[0]=HIGH;
  else
    pcb->data[0]=LOW;
  sleep(timeStop); //"sleep()" Does not change or interrupt the process by this function, only sets the sleep time when the function ends (in this iteration).
}
