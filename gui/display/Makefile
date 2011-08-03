#
# Makefile for libgepsystem.so
#

LIBRARY = Display
TARGET  = libGEP$(LIBRARY).so

SRC     = display_main_window.cpp \
          display_plane_widget.cpp \
          display_population_list.cpp \
          display_population_widget.cpp \
          display_status_widget.cpp

UI_SRC  = display_population_widget.ui \
          display_status_widget.ui

MOC_SRC = GEPDisplayMainWindow.hpp \
          GEPDisplayPopulationList.hpp \
          GEPDisplayPopulationWidget.hpp \
          GEPDisplayStatusWidget.hpp

INCS    = GEPDisplayMainWindow.hpp \
          GEPDisplayPlaneWidget.hpp \
          GEPDisplayPopulationList.hpp \
          GEPDisplayPopulationWidget.hpp \
          GEPDisplayStatusWidget.hpp

TEST_SRC = test_display.cpp
TEST_LIBS = -lGEPSystem -lGEPDisplay $(SYSTEMLIBS)

LIBS =

clean: library-clean

include ../../Makefile.include
-include .depend
