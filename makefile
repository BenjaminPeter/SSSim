CXX = g++
CXXFLAGSRELEASE = -O3 -ffast-math -march=native
CXXFLAGSDEBUG = -g -Wall
COMPILEFLAGS = -c
LIBFLAGS = -lm -lgsl -lgslcblas -lboost_thread
TARGETDIR=maketest
EXENAME=sssim

CXXFLAGS = $(CXXFLAGSRELEASE)


all: $(TARGETDIR)/$(EXENAME)
	git commit -a

debug: $(TARGETDIR)/$(EXENAME)

debug : CXXFLAGS = $(CXXFLAGSDEBUG)

$(TARGETDIR):
		mkdir -p $(TARGETDIR)

TARGETOBJ = \
	$(TARGETDIR)/main.o \
	$(TARGETDIR)/Simulator.o \
	$(TARGETDIR)/TreeSimulator.o \
	$(TARGETDIR)/Parameters.o \
	$(TARGETDIR)/Sample.o \
	$(TARGETDIR)/Event.o \
	$(TARGETDIR)/Lineage.o \
	$(TARGETDIR)/TerminalLineage.o \
	$(TARGETDIR)/LineageTemplate.o \
	$(TARGETDIR)/InternalLineage.o \
	$(TARGETDIR)/SEExpansion.o \
	$(TARGETDIR)/SEExpansionDiffK.o \
	$(TARGETDIR)/IsolationByDistance.o \
	$(TARGETDIR)/IsolationByDistanceExpansion.o \
	$(TARGETDIR)/LocalLogisticGrowth.o \
	$(TARGETDIR)/SFS.o \
	$(TARGETDIR)/FreqTable.o \
	$(TARGETDIR)/SNPTable.o \
	$(TARGETDIR)/BootstrapResampler.o \
	$(TARGETDIR)/SimulationResults.o \
	$(TARGETDIR)/SequenceSimulator.o \
	$(TARGETDIR)/StatCalculator.o \
	$(TARGETDIR)/utils.o \
	$(TARGETDIR)/Tree.o \
	$(TARGETDIR)/Coords.o \

$(TARGETDIR)/$(EXENAME): $(TARGETDIR) $(TARGETOBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(TARGETOBJ) $(LIBFLAGS)

$(TARGETDIR)/main.o: main.cpp Simulator.h Parameters.h SimulationResults.h BootstrapResampler.h SNPTable.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ main.cpp

$(TARGETDIR)/Simulator.o: Simulator.cpp Simulator.h utils.h MigrationScheme.h SequenceSimulator.h Parameters.h SimulationResults.h TreeSimulator.h StatCalculator.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ Simulator.cpp
	
$(TARGETDIR)/TreeSimulator.o: TreeSimulator.cpp TreeSimulator.h Parameters.h SimulationResults.h Lineage.h Event.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ TreeSimulator.cpp

$(TARGETDIR)/Parameters.o: Parameters.cpp Parameters.h Sample.h MigrationScheme.h IsolationByDistance.h IsolationByDistanceExpansion.h SEExpansion.h SEExpansionDiffK.h SequenceSimulator.h LineageTemplate.h Lineage.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ Parameters.cpp

$(TARGETDIR)/Sample.o: Sample.cpp Sample.h Lineage.h utils.h TerminalLineage.h InternalLineage.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ Sample.cpp

$(TARGETDIR)/Event.o: Event.cpp Event.h Sample.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ Event.cpp

$(TARGETDIR)/Lineage.o: Lineage.cpp Lineage.h SFS.h FreqTable.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ Lineage.cpp

$(TARGETDIR)/TerminalLineage.o: TerminalLineage.cpp TerminalLineage.h Lineage.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ TerminalLineage.cpp

$(TARGETDIR)/LineageTemplate.o: LineageTemplate.cpp LineageTemplate.h Lineage.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ LineageTemplate.cpp

$(TARGETDIR)/InternalLineage.o: InternalLineage.cpp InternalLineage.h Lineage.h SFS.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ InternalLineage.cpp

$(TARGETDIR)/SEExpansion.o: SEExpansion.cpp SEExpansion.h MigrationScheme.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ SEExpansion.cpp

$(TARGETDIR)/SEExpansionDiffK.o: SEExpansionDiffK.cpp SEExpansionDiffK.h SEExpansion.h MigrationScheme.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ SEExpansionDiffK.cpp

$(TARGETDIR)/IsolationByDistance.o: IsolationByDistance.cpp IsolationByDistance.h MigrationScheme.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ IsolationByDistance.cpp

$(TARGETDIR)/IsolationByDistanceExpansion.o: IsolationByDistanceExpansion.cpp IsolationByDistanceExpansion.h IsolationByDistance.h MigrationScheme.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ IsolationByDistanceExpansion.cpp

$(TARGETDIR)/LocalLogisticGrowth.o: LocalLogisticGrowth.cpp LocalLogisticGrowth.h MigrationScheme.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ LocalLogisticGrowth.cpp

$(TARGETDIR)/SFS.o: SFS.cpp SFS.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ SFS.cpp

$(TARGETDIR)/FreqTable.o: FreqTable.cpp FreqTable.h utils.h SNPTable.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ FreqTable.cpp

$(TARGETDIR)/SNPTable.o: SNPTable.cpp SNPTable.h SFS.h Parameters.h utils.h StatCalculator.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ SNPTable.cpp

$(TARGETDIR)/SimulationResults.o: SimulationResults.cpp SimulationResults.h Parameters.h FreqTable.h SFS.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ SimulationResults.cpp

$(TARGETDIR)/SequenceSimulator.o: SequenceSimulator.cpp SequenceSimulator.h utils.h Lineage.h SFS.h Parameters.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ SequenceSimulator.cpp

$(TARGETDIR)/StatCalculator.o: StatCalculator.cpp StatCalculator.h SFS.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ StatCalculator.cpp

$(TARGETDIR)/utils.o: utils.cpp utils.h 
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ utils.cpp

$(TARGETDIR)/Tree.o: Tree.cpp Tree.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ Tree.cpp

$(TARGETDIR)/Coords.o: Coords.cpp Coords.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ Coords.cpp

$(TARGETDIR)/BootstrapResampler.o: BootstrapResampler.cpp BootstrapResampler.h SNPTable.h Parameters.h utils.h
	$(CXX) $(CXXFLAGS) $(COMPILEFLAGS) -o $@ BootstrapResampler.cpp

clean:
	rm -rf $(TARGETDIR)/*.o $(TARGETDIR)/$(EXENAME)

archive: 
	tar czvf $(EXENAME).tar.gz *cpp *h makefile
