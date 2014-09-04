TopsyTurvyClock
===============

Work in progress code for the Topsy Turvy Clock



The stepper motor class provides the interface to the motor and optosensors.

The clock class encapsulates the RTC and TimeZone classes with methods to set the time and date.

The controller class glues everything together and provides the UI in the form of serial commands. The code uses the String object's abilities to parse the commands.

Setup and loop don't have much to do, simply repeatedly calling the controller's run method.
