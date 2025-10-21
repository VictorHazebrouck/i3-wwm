TARGET = run.exe
SRC_DIR = .
LIBS_DIR = core
APP_DIR = app

CC = g++
CFLAGS = -std=c++23 -Wall -Wextra  -I.

CORE_SRCS := $(shell find $(LIBS_DIR) -name "*.cpp")
APP_SRCS := $(shell find $(APP_DIR) -name "*.cpp")
SOURCES := $(CORE_SRCS) $(APP_SRCS)
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all clean fclean re

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(TARGET)

re: fclean all
