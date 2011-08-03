#
# Makefile for libgepcore.so
#


LIBRARY = Core

SRC  = core_controller.cpp \
       core_crossover.cpp \
       core_criterion.cpp \
       core_individual.cpp \
       core_mutation.cpp \
       core_operator.cpp \
       core_population.cpp \
       core_ring_individual.cpp \
       core_selection.cpp

INCS = GEPCoreController.hpp \
       GEPCoreCrossover.hpp \
       GEPCoreCriterion.hpp \
       GEPCoreIndividual.hpp \
       GEPCoreMutation.hpp \
       GEPCoreOperator.hpp \
       GEPCorePopulation.hpp \
       GEPCoreRingIndividual.hpp \
       GEPCoreSelection.hpp
       
LIBS =
       
TEST_SRC = test_ring_operators.cpp \
           test_selection_operators.cpp
           
UI_SRC    =
MOC_SRC   = GEPCoreController.hpp

clean: library-clean

include $(PWD)/Makefile.include
-include .depend
