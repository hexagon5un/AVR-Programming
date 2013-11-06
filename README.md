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
  in the book or [here](http://littlehacks.org/2013/11/programming_avr).
  
  
Repo Layout
-----------

All of the project code is organized by the chapters in the book.  So if
you're looking for an example of some SPI code, see the "Chapter" folder for 
SPI-related projects.



More!
-----

You've read the book, you've built the projects, you've worked through the code.
But still you hunger for more projects, more examples, more, more, more!
If I may toot my own horn, you should visit [LittleHacks.org](http://littlehacks.org/)
where I blog about whatever microcontroller projects I'm currently up to.  

In particular, if you're reading
 [Make: AVR Programming](http://shop.oreilly.com/product/0636920028161.do), and
you're interested in fully-elaborated versions of the projects with more
photos, videos, and explanation than could fit in a book, head on over to
 [LittleHacks.org's AVR-Programming Section](http://littlehacks.org/AVR-Programming/).  

If you haven't already, you should *definitely* head over to [The Cornell
University ECE 4760 Final
Projects](http://people.ece.cornell.edu/land/courses/ece4760/FinalProjects/)
list page.  It's an awe-inspiring collection of applications, and sure to spark
some creative thoughts.  It's all well-documented and there's tons of source
code in C.  [Professor Land's links section] (http://people.ece.cornell.edu/land/courses/ece4760/#links)
is also top-notch, and his lectures on YouTube are also worth a look if
you're getting serious about this whole AVR deal.  








