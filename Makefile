TARGET = Assembler

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))
FLAGS = -fsanitize=address -Wall -Wextra -std=c++20

$(TARGET) : $(OBJ)
		g++ $(OBJ) $(FLAGS) -o $(TARGET)

%.o : %.cpp
		g++ $(FLAGS) -c $< -o $@

.PHONY : clean


clean :
		rm *.o $(TARGET)

run : $(TARGET)
		./$(TARGET)

draw :
		dot Tree_Graph.dot -T png > Tree_graph.png
