
#OBJS Specifies which files to compile
OBJS = img2x16.cpp labcolor.cpp

#OBJ_Name specifies the name of our executable
OBJ_NAME = img2x16

#This is the target
all: $(OBJS)
	g++ $(OBJS) -w -O2 -std=c++17 -o $(OBJ_NAME)

clean:
	rm -f $(OBJ_NAME)
