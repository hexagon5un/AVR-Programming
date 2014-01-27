Code and Examples from "Make: AVR Programming"
==============================================

Welcome!
--------

Here you'll find all the code (and more!) for the Maker Media book "Make: AVR Programming".


Getting Started
---------------

* First, download the contents of this repository onto your hard-drive.  The easiest way
  is with the "Download ZIP" button above and on the right-hand side of this very web
  page.  Extract the zip file wherever is comfy.  (Feel free to clone the repo if you 
  are comfortable with Git.)

* Most of the projects share a common set of pin defines and a common simple
  USART serial library in the **AVR-Programming-Library** directory.  The
  makefiles I've included depend on the directory structure here by default, so
  don't go moving the folders around unless you also change the path to
  included files in the makefile.  
  
* If you're using the Arduino IDE, you'll want to copy the **AVR-Programming-Library** directory
  into your **sketchbook/libraries** folder.  If you don't know where this is, you can find out in the 
  "File...Settings" dialog within Arduino.  Now you can link your code to use this library simply
  using "Sketch...Import Library" and selecting the library from the the menu.

* Now you will be set to open the code, edit it, and flash it into the AVR following the directions
  in the book.
  
  
Repo Layout
-----------

All of the project code is organized by the chapters in the book.  So if
you're looking for an example of some SPI code, see the "Chapter16_SPI" folder for 
SPI-related projects.  That's obvious.

But a bunch of the projects are interesting in addition to the topic covered in
the chapter.  For instance, "Chapter05_Serial-IO" includes a project that uses
the serial communication between your desktop computer and the AVR to turn your
computer keyboard into a musical keyboard that plays notes generated on the
AVR, turning it into a serial-port-controlled organ.  You wouldn't think to 
go looking in the Serial I/O chapter unles you were following along in the book. 

So for an overview all the projects, the file [allProjectsList](https://github.com/hexagon5un/AVR-Programming/blob/master/allProjectsList) lists them all out by name.  

setupProject
------------

If you'd like a blank template to start writing your own AVR code, 
have a look in the **setupProject** directory that I've included here. Inside, you'll find 
**main.c** and **main.h** files that are essentially blank and ready to go.  **main.c** 
makes use of my simple USART library, which is also included an linked in by the **Makefile**. 
In short, you could copy this directory, rename files, and start using it in your own projects.

But you don't have to do that manually.  Running *python setupProject.py
myProjectName* will create a directory called **myProjectName** for you, copy
the blank main files, renaming them as appropriate, and set up the Makefile
accordingly.  All that's left for you to do is the hard part -- actually
coding.  

If you use this setup a lot, you'll want to personalize the **Makefile** and
the two **main** files to suit your own preferences.  That way, whenever you
start up a new project, it'll include a customized **Makefile** that has your
programmer, chip type, and favorite baud rate already set.   

Finally, if you like to map out your pin definitions in macro definitions, run
*python createPinDefines.py*.  The program will ask you what you'd like to call
each pin macro (e.g. "LED0") and then which pin on the AVR you'd like to
associate with it (e.g. "PB1").  When you're done entering your pin layout,
it'll create a "pinDefines.h" file with (I hope) nicely-named macros.  Move
this file into the right directory, and include it in your code.  Calling
LED0_SET_HIGH will turn your LED on.


More!
-----

You've read the book, you've built the projects, you've worked through the code.
But still you hunger for more projects, more examples, more, more, more!
If I may toot my own horn, you should visit [LittleHacks.org](http://littlehacks.org)
where I blog about whatever microcontroller projects I'm currently up to.  

In particular, if you're reading
 [Make: AVR Programming](http://shop.oreilly.com/product/0636920028161.do), and
you're interested in fully-elaborated versions of the projects with more
photos, videos, and explanation than could fit in a book, head on over to
 [LittleHacks.org's AVR-Programming Section](http://littlehacks.org/AVR-Programming).  

Once you've exhausted all of these resources, you should *definitely* head over
to [The Cornell University ECE 4760 Final
Projects](http://people.ece.cornell.edu/land/courses/ece4760/FinalProjects/)
list page.  It's an awe-inspiring collection of applications, and sure to spark
some creative thoughts.  It's all well-documented and there's tons of source
code in C.  [Professor Land's links section]
(http://people.ece.cornell.edu/land/courses/ece4760/#links) is also top-notch,
and his lectures on YouTube are also worth a look if you're getting serious
about this whole AVR deal.  






