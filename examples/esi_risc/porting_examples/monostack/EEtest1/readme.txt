test1

 A minimal EE demo that demonstrates how to create and activate a TASK.
 The compilation chain can start outside Eclipse environment from the Cygwin shell with the "make" 
 command inside the Debug folder of the project.
 The Debug folder can be created manually if you don't have the oil-based generator. 
 The Debug folder must contain at least the following files (suitably modified...):
 eecfg.h, eecfg.c and the makefile (see the examples of different architectures to begin...) 

The RT-DRUID project contains the following files:
"conf.oil": Configuration file in OSEK implementation language specific for the application.
"main.c": Main application file.
