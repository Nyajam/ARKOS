/*
  Name: ARKOS core
  Author: Ildefonso Macarro Pueyo
  Mail: fusm7id@gmail.com
  Date: 16/08/2022
  Comments: Framenwork for multithreading on arduino without context switching.
  Library core.
*/

#include <Arduino.h>
#include "ARKOS.h"

//Variables globals
PCB ARKOS_PCB_LIST[ARKOS_SIZE_LIST_PCB]; //Table of processes
PCB* ARKOS_PCB_RUN; //List of processes pending execution, first is executing

//Functions - implementations
void voidPcbs()
{
  for(int i=0;i<ARKOS_SIZE_LIST_PCB;i++)
  {
    ARKOS_PCB_LIST[i].id=i;
    ARKOS_PCB_LIST[i].stat=ARKOS_PNULL;
    ARKOS_PCB_LIST[i].actionTime=millis();
    ARKOS_PCB_LIST[i].data=NULL;
    ARKOS_PCB_LIST[i].function=NULL;
    ARKOS_PCB_LIST[i].next=NULL;    
  }
  ARKOS_PCB_RUN=NULL;  
}

void chargePrograms(void (**programas)())
{
  //ignore the more programs that ARKOS_SIZE_LIST_PCB says
  for(int i=0;i<ARKOS_SIZE_LIST_PCB;i++)
  {
    if(programas[i]==NULL)
      break;
    ARKOS_PCB_LIST[i].function=programas[i];
    ARKOS_PCB_LIST[i].stat=ARKOS_PSLEEP;
    planner(&ARKOS_PCB_LIST[i]); //sort and charge the programs
  }
}

void planner(PCB* pcb)
{
  if(ARKOS_PCB_RUN==NULL) //no programs to execute case
  {
    ARKOS_PCB_RUN=pcb;
    pcb->next=NULL;
  }
  else if(ARKOS_PCB_RUN==pcb) //this program is move to new position at last execute
  {
    ARKOS_PCB_RUN=pcb->next;
    pcb->next=NULL;
    planner(pcb);
  }
  else //sort of sleep time (less is first)
  {
    PCB* ptr = ARKOS_PCB_RUN;
    PCB* aux = NULL;
    do
    {
      /*
        A considerar: El tiempo de activacion tiene que ser menor y NO menor o igual por starvation.
        At considering: The activation time has to be less than and NOT less than or equal to for starvation.
      */      
      if(pcb->actionTime<ptr->actionTime) //If the process to insert wakeup before the list position
      {
        if(ptr==ARKOS_PCB_RUN) //Insert first
        {
          pcb->next=ARKOS_PCB_RUN;
          ARKOS_PCB_RUN=pcb;
          break;
        }
        else if(ptr->next==NULL) //Insert end (penultimate)
        {
          pcb->next=ptr;
          aux->next=pcb;
          break;
        }
        else //Insert in the middle
        {
          pcb->next=ptr;
          aux->next=pcb;
          break;
        }
      }
      else if(ptr->next==NULL) //Insert last
      {
        ptr->next=pcb;
        pcb->next=NULL;
        break;
      }
      aux=ptr;
      ptr=ptr->next;
    } while(ptr!=NULL);
  }
}

void run()
{
  while(ARKOS_PCB_RUN!=NULL) //while process wait to execute
  {
    if(millis() >= ARKOS_PCB_RUN->actionTime) //if is it is the time to weke
    {
      ARKOS_PCB_RUN->stat=ARKOS_PRUN; //change of process state
      ARKOS_PCB_RUN->function(); //call to execute function
    }
    ARKOS_PCB_RUN->stat=ARKOS_PSLEEP;
    planner(ARKOS_PCB_RUN);  //call process scheduler
  }
}

PCB* myPCB()
{
  return ARKOS_PCB_RUN;
}

void sleep(unsigned long time)
{
  if(myPCB()!=NULL)
  {
    myPCB()->actionTime=millis()+time;
  }
}
