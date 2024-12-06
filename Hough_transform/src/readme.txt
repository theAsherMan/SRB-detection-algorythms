-- Notes --

Hough Transform implemented using as both vote space and accumulator space
vote space counts each non-zero pixel as one.  accumulator space adds value of each pixel
no option to switch between implemented.  must be switched by modifying source
currently vote space is used.

Not finished.  produces (somewhat) accurate start time but DM cannot be confirmed

prints log information to screen.  also creates copies of data after each stage of processing and creates a file containing correct FRB and a file for detected FRBs

the files for showing FRBs (and correct FRBs) do not work

-- How To --

add .fits files to be analysed into a folder within fitsFiles folder
	(files can be created in fitsFiles to better organize data)
if there are known FRBs in file, include …
run "make App"
run "./App"

-- Documentation --