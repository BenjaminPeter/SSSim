## -*- Makefile -*-
##
## User: peterb
## Time: Oct 17, 2012 11:33:32 AM
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -O -s
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# Define the target directories.
TARGETDIR_sssim=GNU-amd64-Linux


all: $(TARGETDIR_sssim)/sssim

## Target: sssim
OBJS_sssim =  \
	$(TARGETDIR_sssim)/TerminalLineage.o \
	$(TARGETDIR_sssim)/SequenceSimulator.o \
	$(TARGETDIR_sssim)/SEExpansion.o \
	$(TARGETDIR_sssim)/LineageTemplate.o \
	$(TARGETDIR_sssim)/SNPTable.o \
	$(TARGETDIR_sssim)/SFS.o \
	$(TARGETDIR_sssim)/InternalLineage.o \
	$(TARGETDIR_sssim)/IsolationByDistanceExpansion.o \
	$(TARGETDIR_sssim)/IsolationByDistance.o \
	$(TARGETDIR_sssim)/Simulator.o \
	$(TARGETDIR_sssim)/SEExpansionDiffK.o \
	$(TARGETDIR_sssim)/Parameters.o \
	$(TARGETDIR_sssim)/Sample.o \
	$(TARGETDIR_sssim)/FreqTable.o \
	$(TARGETDIR_sssim)/Coords.o \
	$(TARGETDIR_sssim)/SimulationResults.o \
	$(TARGETDIR_sssim)/main.o \
	$(TARGETDIR_sssim)/Event.o \
	$(TARGETDIR_sssim)/LocalLogisticGrowth.o \
	$(TARGETDIR_sssim)/utils.o \
	$(TARGETDIR_sssim)/Lineage.o \
	$(TARGETDIR_sssim)/StatCalculator.o \
	$(TARGETDIR_sssim)/Tree.o \
	$(TARGETDIR_sssim)/TreeSimulator.o
SYSLIBS_sssim = -lm -lgsl -lgslcblas -lboost_thread
USERLIBS_sssim =  $(SYSLIBS_sssim) 
DEPLIBS_sssim =    
LDLIBS_sssim = $(USERLIBS_sssim)


# Link or archive
$(TARGETDIR_sssim)/sssim: $(TARGETDIR_sssim) $(OBJS_sssim) $(DEPLIBS_sssim)
	$(LINK.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ $(OBJS_sssim) $(LDLIBS_sssim)


# Compile source files into .o files
$(TARGETDIR_sssim)/TreeSimulator.o: $(TARGETDIR_sssim) TreeSimulator.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ TreeSimulator.cpp

$(TARGETDIR_sssim)/TerminalLineage.o: $(TARGETDIR_sssim) TerminalLineage.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ TerminalLineage.cpp

$(TARGETDIR_sssim)/SequenceSimulator.o: $(TARGETDIR_sssim) SequenceSimulator.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ SequenceSimulator.cpp

$(TARGETDIR_sssim)/SEExpansion.o: $(TARGETDIR_sssim) SEExpansion.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ SEExpansion.cpp

$(TARGETDIR_sssim)/LineageTemplate.o: $(TARGETDIR_sssim) LineageTemplate.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ LineageTemplate.cpp

$(TARGETDIR_sssim)/SNPTable.o: $(TARGETDIR_sssim) SNPTable.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ SNPTable.cpp

$(TARGETDIR_sssim)/SFS.o: $(TARGETDIR_sssim) SFS.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ SFS.cpp

$(TARGETDIR_sssim)/InternalLineage.o: $(TARGETDIR_sssim) InternalLineage.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ InternalLineage.cpp

$(TARGETDIR_sssim)/IsolationByDistanceExpansion.o: $(TARGETDIR_sssim) IsolationByDistanceExpansion.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ IsolationByDistanceExpansion.cpp

$(TARGETDIR_sssim)/IsolationByDistance.o: $(TARGETDIR_sssim) IsolationByDistance.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ IsolationByDistance.cpp

$(TARGETDIR_sssim)/Simulator.o: $(TARGETDIR_sssim) Simulator.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ Simulator.cpp

$(TARGETDIR_sssim)/SEExpansionDiffK.o: $(TARGETDIR_sssim) SEExpansionDiffK.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ SEExpansionDiffK.cpp

$(TARGETDIR_sssim)/Parameters.o: $(TARGETDIR_sssim) Parameters.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ Parameters.cpp

$(TARGETDIR_sssim)/Sample.o: $(TARGETDIR_sssim) Sample.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ Sample.cpp

$(TARGETDIR_sssim)/FreqTable.o: $(TARGETDIR_sssim) FreqTable.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ FreqTable.cpp

$(TARGETDIR_sssim)/Coords.o: $(TARGETDIR_sssim) Coords.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ Coords.cpp

$(TARGETDIR_sssim)/SimulationResults.o: $(TARGETDIR_sssim) SimulationResults.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ SimulationResults.cpp

$(TARGETDIR_sssim)/main.o: $(TARGETDIR_sssim) main.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ main.cpp

$(TARGETDIR_sssim)/Event.o: $(TARGETDIR_sssim) Event.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ Event.cpp

$(TARGETDIR_sssim)/LocalLogisticGrowth.o: $(TARGETDIR_sssim) LocalLogisticGrowth.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ LocalLogisticGrowth.cpp

$(TARGETDIR_sssim)/utils.o: $(TARGETDIR_sssim) utils.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ utils.cpp

$(TARGETDIR_sssim)/Lineage.o: $(TARGETDIR_sssim) Lineage.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ Lineage.cpp

$(TARGETDIR_sssim)/StatCalculator.o: $(TARGETDIR_sssim) StatCalculator.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ StatCalculator.cpp

$(TARGETDIR_sssim)/Tree.o: $(TARGETDIR_sssim) Tree.cpp
	$(COMPILE.cc) $(CCFLAGS_sssim) $(CPPFLAGS_sssim) -o $@ Tree.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_sssim)/sssim \
		$(TARGETDIR_sssim)/TerminalLineage.o \
		$(TARGETDIR_sssim)/SequenceSimulator.o \
		$(TARGETDIR_sssim)/SEExpansion.o \
		$(TARGETDIR_sssim)/LineageTemplate.o \
		$(TARGETDIR_sssim)/SNPTable.o \
		$(TARGETDIR_sssim)/SFS.o \
		$(TARGETDIR_sssim)/InternalLineage.o \
		$(TARGETDIR_sssim)/IsolationByDistanceExpansion.o \
		$(TARGETDIR_sssim)/IsolationByDistance.o \
		$(TARGETDIR_sssim)/Simulator.o \
		$(TARGETDIR_sssim)/SEExpansionDiffK.o \
		$(TARGETDIR_sssim)/Parameters.o \
		$(TARGETDIR_sssim)/Sample.o \
		$(TARGETDIR_sssim)/FreqTable.o \
		$(TARGETDIR_sssim)/Coords.o \
		$(TARGETDIR_sssim)/SimulationResults.o \
		$(TARGETDIR_sssim)/main.o \
		$(TARGETDIR_sssim)/Event.o \
		$(TARGETDIR_sssim)/LocalLogisticGrowth.o \
		$(TARGETDIR_sssim)/utils.o \
		$(TARGETDIR_sssim)/Lineage.o \
		$(TARGETDIR_sssim)/StatCalculator.o \
		$(TARGETDIR_sssim)/Tree.o \
		$(TARGETDIR_sssim)/TreeSimulator.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_sssim)


# Create the target directory (if needed)
$(TARGETDIR_sssim):
	mkdir -p $(TARGETDIR_sssim)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

