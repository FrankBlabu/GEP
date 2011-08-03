#
# Makefile for libgepsystem.so
#

LIBRARY = System

SRC  = debug.cpp \
       exception.cpp \
       memory_logger.cpp \
       random.cpp \
       ring_buffer.cpp \
       trace_guard.cpp \
       vec2d.cpp

INCS = GEPDebug.hpp \
       GEPException.hpp \
       GEPMemoryLogger.hpp \
       GEPRandom.hpp \
       GEPRingBuffer.hpp \
       GEPTraceGuard.hpp \
       GEPVec2d.hpp
       
LIBS =

TEST_SRC = test_ring_buffer.cpp \
           test_random.cpp

clean: library-clean

include $(PWD)/Makefile.include
-include .depend
