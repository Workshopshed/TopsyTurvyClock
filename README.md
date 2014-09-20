TopsyTurvyClock
===============

The stepper motor class provides the interface to the motor and optosensors modified CustomStepper class from Igor Campos.

The clock class encapsulates the RTC and TimeZone classes with methods to set the time and date. 

The controller class glues everything together and provides the UI in the form of serial commands. The code uses the String object's abilities to parse the commands.

Setup and loop don't have much to do, simply repeatedly calling the controller's run method.


Thanks to
---------

CustomStepper class by Igor Campos

TimeZone class by Jack Christensen

Time by Michael Margolis

DS1307 based on work by DFRobot and Michael Margolis
