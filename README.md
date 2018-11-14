# MNXB01-project

Complete instructions, datasets and code templates at: 
http://www.hep.lu.se/courses/MNXB01/2018/MNXB01-project.tgz

We write anything else helpful here - links to read, example data, etc.
Communication should be done primarily through some other means, though - facebook, email, etc. As this is instanced via email we can start there. We should organize who starts on what parts, and set some deadlines for them - I recommend a soft deadline for everything on the 9th of November. //Alex W


The final code is found in the Calle folder with the basically all code in Calle/temptrender.h

########################################

The description of hos to use the method DataView3D is written in Calle/project.cpp and below:

The DataView3D(Int_t startYear, Int_t endYear, bool isUppsala, Int_t startTime=666, Int_t endTime=666)
plots a 2D/3D graph that shows the data points in a window specified by the user.
The first two arguments specify between which year to look, the third argument is true if
you want to look at the uppsala data, else its false.
The two last arguments are only for the CSV files. There you can specify which timewindow
of the day you want to view datapoints from. Put time window to 0,24 if you want the full day.

There is no error catching for when asking for years that are not in the data file.

There seems to be some very mysterious problem with how the range of the axis are set, sometimes
it works and sometimes it doesnt. I have not been able to figure out why this is.

#######################################

A short docstring of what tempOnDay does and which input parameters it accepts is found on lines 38-48 in tempTrender.h.
Some further comments on what specific code blocks in the function can be found throughout the function.
