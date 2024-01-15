ReadMe.txt for ST STM32F429ZI prebuild binary

Start_STM32F429ZI_Nucleo.hex is a prebuild binary file of the OS_StartLEDBlink.c sample
application to run on a ST NUCLEO-F429ZI. It is prepared for download into
internal flash using J-Link without the need for an IDE.
An embOS stack-check and profiling library is used.
The application can be used with SEGGER SystemView.

For further information, refer to Application Note AN00020 ("Getting Started
with SEGGER Eval Software").

How to run the prebuild application:
====================================
1. Connect your target to your host PC via J-Link
2. Execute StartBinary.bat
