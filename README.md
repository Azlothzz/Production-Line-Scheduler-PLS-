# Production-Line-Scheduler-PLS-
PROJECT TITLE: 
Steel-making Production Line Scheduler (PLS)

DESCRIPTION:
This production line scheeduler is a tool designed to optimize the scheduling of steel production across various lines in a manufacturing plant. It aims to maximize efficiency and reduce downtime by automating the scheduling process. It contains scheduling algorithms like FCFS and SJF.

USAGE:
This project is divided into 5 functions:
>addPERIOD where the user enters the production period. addPERIOD [start date] [end date]
>addORDER is for the user to add an ordeer and give somme details about the product. addORDER [Order Number] [Due Date] [Quantity] [Product Name]
>addBATCH is to add a batch file in text which contains multiple lines of addORDER. addBATCH [Orders in a batch file]
>runPLS is to generate and execute the schedule with a selected scheduling algorithm, then printREPORT is to print a report of tthat schedule with the analysis details. runPLS [Algorithm] | printREPORT > [Report file name]
>exitPLS is to terminate the program. exitPLS
NOTE: The production schedule and analysis report will be saved in the files.

HOW TO RUN THE PROGRAM:
To run the program in Apollo:
STEP1: It is neccesary to import all the fundamental files, in this case all the files inside the folder called "PLS_G25". 
STEP2: Once every file is imported to apollo, use the command "make all", then "gcc -g -o pls main.c scheduler.c order.h util.h -lm" 
STEP3: To finally run the code input "./pls".

---------------------------------------END----------------------------------------------
