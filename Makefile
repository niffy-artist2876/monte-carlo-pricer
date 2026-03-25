CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall

SRCS = main.cpp core/pricer.cpp core/simulation.cpp core/payoffs.cpp
TARGET = mc_pricer

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	del /f $(TARGET).exe 2>nul || rm -f $(TARGET)
