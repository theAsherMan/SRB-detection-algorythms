-- Notes --

Hough Transform implemented using as both vote space and accumulator space
vote space counts each non-zero pixel as one.  accumulator space adds value of each pixel
no option to switch between implemented.  must be switched by modifying source
currently vote space is used.

Not finished.  produces (somewhat) accurate start time but DM cannot be confirmed

prints log information to screen.  also creates copies of data after each stage of processing and creates a file containing correct FRB and a file for detected FRBs

the files for showing FRBs (and correct FRBs) do not work

Where ever possible, this code utilises lazy initualisation

-- How To --

add .fits files to be analysed into a folder within fitsFiles folder
	(files can be created in fitsFiles to better organize data)
if there are known FRBs in file, include …
run "make App"
run "./App"

-- Documentation --

App
	contains main program and utility functions.  Once the project is polished this file should contain no components needed to process a fits file, only those needed for testing

csv
	used for reading CSV files.
	has no implementation for modifying files

fitsWriter
	writes fits files
	not required for core process, but useful for debugging

frbDrawer
	draws frbs onto VisualSpace objects, usually so a fitsWritter can save it
	=== DOSE NOT CURRENTLY WORK ===

Constants.hpp
	contains numerical constants
	as it stands, the only constant is K_DM

linearHoughTransformer
	preforms the actual analysis
	as the name suggests, it only recognises straight lines, so regridding should be used to process FRB before transform is applyied
	it is likly nessesary to preprocess the data before transforming it as the hough transform dose not handle high noise very well

preprocesor
	prepares the image for the Hough Transform, attempting to remove noise
	removes (sets to zero) the majority of pixles, leaving only those that are significantly brighter than the average

linearToSquareRegrider
	re scales the vertical axis of an image from y too y^-2
	as FRB's are quadratic sweeps, this transformes them into straight lines that the linear hough transformer can analyse

stopWatch
	used for timing the length of operations
	only used in debugging

visualSpace
	object for storing, accessing, and statisticaly analizing 2D data
	as it is intended to get data from a fits file, and CCfits reads data from a file as a flat array, visualSpace also stores data as a flat array
