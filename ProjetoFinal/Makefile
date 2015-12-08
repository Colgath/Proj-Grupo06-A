# Compilation variables. Compilation flags.
#for special includes copy this -I/path/for/somehting
CC=g++
CUDA_CC=nvcc
OPENCV_FLAGS= -lopencv_core -lopencv_highgui -lopencv_imgproc
NVIDIA_FLAGS=-L/usr/local/cuda/lib64 -lcuda -lcudart -rdc=true -gencode arch=compute_30,code=sm_30 -arch compute_30 -code sm_30 -m 64 -Xptxas=-dlcm=cg
FLAGS=-lm $(OPENCV_FLAGS)

#File sources
SOURCES= $(wildcard *.cu)
OBJECTS=$(SOURCES:.cu=.o)
TARGET_DIR=./
TARGET=$(TARGET_DIR)main
#Main
all: main.o
#	
main.o:
	nvcc `pkg-config --cflags opencv ` main.cu `pkg-config --libs opencv` -o main
	
run:
	./main

$(TARGET): $(OBJECTS)
	$(CUDA_CC) -o $@ $^ $(FLAGS) $(NVIDIA_FLAGS)

%.o: %.cu %.h
	$(CUDA_CC) $(FLAGS) -c $< -o $@ $(NVIDIA_FLAGS)

%.o: %.cu
	$(CUDA_CC) $(FLAGS) -c $< -o $@ $(NVIDIA_FLAGS)

#clean .o and main
clean:
	rm -f *.o $(TARGET)

#Quick clean+all
rebuild: clean all
