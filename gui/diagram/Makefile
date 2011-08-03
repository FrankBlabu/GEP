#
# Makefile for libgepdiagram.so
#

LIBRARY = Diagram

SRC     = diagram_widget.cpp \
          diagram_distribution_widget.cpp \
          diagram_fitness_widget.cpp

UI_SRC  =
MOC_SRC = GEPDiagramWidget.hpp \
          GEPDiagramDistributionWidget.hpp \
          GEPDiagramFitnessWidget.hpp

INCS    = GEPDiagramWidget.hpp \
          GEPDiagramDistributionWidget.hpp \
          GEPDiagramFitnessWidget.hpp

LIBS =

TEST_SRC = test_display.cpp
TEST_LIBS = -lGEPSystem -lGEPDisplay $(SYSTEMLIBS)

clean: library-clean

include ../../Makefile.include
-include .depend
