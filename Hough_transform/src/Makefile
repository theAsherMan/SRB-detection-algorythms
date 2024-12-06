
bin := ./bin/
VPATH := $(shell find build -type d -print | tr '\012' ':' | sed 's/:$$//'):$(shell find test -type d -print | tr '\012' ':' | sed 's/:$$//'):$(shell find bin -type d -print | tr '\012' ':' | sed 's/:$$//')

help :
	@echo ""
	@echo =====================================================================
	@echo ""
	@echo type in the name of the executable you whish to compile, or clean to empty the bin
	@echo ""
	@echo =====================================================================
	@echo ""

clean :
	rm -r bin
	mkdir bin
	rm *.fits

App : $(bin)App.o $(bin)VisualSpace.o $(bin)LinearHoughTransformer.o $(bin)LinearToSquareRegrider.o $(bin)StopWatch.o $(bin)Preprocesor.o $(bin)FitsWriter.o $(bin)Csv.o $(bin)FrbDrawer.o
	g++ -o $@ $^ -I /usr/include/CCfits -L /usr/lib -l CCfits -l cfitsio

testRegrid : $(bin)testRegrid.o $(bin)LinearToSquareRegrider.o $(bin)VisualSpace.o
	g++ -o $@ $^

testLinearHoughTransform : $(bin)testLinearHoughTransform.o $(bin)LinearHoughTransformer.o $(bin)VisualSpace.o
	g++ -o $@ $^

testFitsIO : $(bin)testFitsIO.o $(bin)VisualSpace.o $(bin)FitsWriter.o
	g++ -o $@ $^ -I /usr/include/CCfits -L /usr/lib -l CCfits -l cfitsio

testThetaDmConvertion : $(bin)testThetaDmConvertion.o $(bin)LinearToSquareRegrider.o $(bin)VisualSpace.o
	g++ -o $@ $^

testCSV : $(bin)testCsv.o $(bin)Csv.o
	g++ -o $@ $^

testExpansionConcept : $(bin)testExpansionConcept.o $(bin)VisualSpace.o $(bin)FitsWriter.o
	g++ -o $@ $^ -I /usr/include/CCfits -L /usr/lib -l CCfits -l cfitsio

$(bin)testRegrid.o : testRegrid.cpp
	g++ -c $< -o $@

$(bin)testLinearHoughTransform.o : testLinearHoughTransform.cpp
	g++ -c $< -o $@

$(bin)testFitsIO.o : testFitsIO.cpp
	g++ -c $< -o $@

$(bin)testThetaDmConvertion.o : testThetaDmConversion.cpp
	g++ -c $< -o $@

$(bin)testCsv.o : testCsv.cpp
	g++ -c $< -o $@

$(bin)testExpansionConcept.o : testExpansionConcept.cpp
	g++ -c $< -o $@

$(bin)App.o : App.cpp App.hpp
	g++ -c $< -o $@

$(bin)LinearToSquareRegrider.o : LinearToSquareRegrider.cpp LinearToSquareRegrider.hpp
	g++ -c $< -o $@

$(bin)LinearHoughTransformer.o : LinearHoughTransformer.cpp LinearHoughTransformer.hpp
	g++ -c $< -o $@

$(bin)VisualSpace.o : VisualSpace.cpp VisualSpace.hpp
	g++ -c $< -o $@

$(bin)StopWatch.o : StopWatch.cpp StopWatch.hpp
	g++ -c $< -o $@

$(bin)Preprocesor.o : Preprocesor.cpp Preprocesor.hpp
	g++ -c $< -o $@

$(bin)FitsWriter.o : FitsWriter.cpp FitsWriter.hpp
	g++ -c $< -o $@

$(bin)Csv.o : Csv.cpp Csv.hpp
	g++ -c $< -o $@

$(bin)FrbDrawer.o : FrbDrawer.cpp FrbDrawer.hpp
	g++ -c $< -o $@