NAME = scop

CXX = c++
CC = cc
CXXFLAGS = -I . -I lib/ -I lib/glad/include/
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
OBJDIR = obj

SRC = main.cpp \
	lib/glad/src/glad.c \
	Shader/Shader.cpp \
	Shader/ShaderProgram.cpp \
	Camera/Camera.cpp \
	Input/Input.cpp \
	Renderer/Renderer.cpp \


OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(SRC))) $(patsubst %.c,$(OBJDIR)/%.o,$(filter %.c,$(SRC)))

all: $(NAME)
	@echo "All compiled successfully"

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@

run: all
	./$(NAME)

clean:
	rm -f $(OBJ)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: clean all