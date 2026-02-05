# # Определение ОС
# UNAME_S := $(shell uname -s)

# # Имена файлов
# TARGET = program
# SOURCES = main.cpp Fish.cpp Ocean.cpp Ship.cpp
# OBJECTS = $(SOURCES:.cpp=.o)
# HEADERS = Fish.h Ocean.h Ship.h globals.h log_folder.h fishmatrix.h

# # Компилятор и флаги
# CXX = g++
# CXXFLAGS = -Wall -Wextra -std=c++11 -fopenmp -I

# # Разные флаги линковки для разных ОС
# ifeq ($(UNAME_S),Linux)
#     # Для Linux
#     LDFLAGS = -Wl,-fopenmp
# else ifeq ($(UNAME_S),MINGW32_NT-6.2)
#     # Для Windows/MinGW
#     LDFLAGS = -fopenmp
# else
#     # Для других ОС
#     LDFLAGS =
# endif

# all: $(TARGET)

# $(TARGET): $(OBJECTS)
# 	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# %.o: %.cpp $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJECTS) $(TARGET)

# run: $(TARGET)
# 	@echo "Запуск программы..."
# 	@echo "Если нужен больший стек, выполните: ulimit -s unlimited"
# 	./$(TARGET)

# # Альтернативный запуск с увеличенным стеком
# run-with-big-stack: $(TARGET)
# 	@echo "Запуск с увеличенным стеком..."
# 	@bash -c 'ulimit -s unlimited && ./$(TARGET)'

# .PHONY: all clean run run-with-big-stack
# Определение ОС
UNAME_S := $(shell uname -s)

# Имена файлов
TARGET = program
SOURCES = main.cpp Fish.cpp Ocean.cpp Ship.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = Fish.h Ocean.h Ship.h globals.h log_folder.h fishmatrix.h

# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -fopenmp -Wno-unused-function -Wno-unused-variable -Wno-implicit-fallthrough
LDFLAGS = -fopenmp  # OpenMP нужно ТОЛЬКО здесь


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	@echo "Запуск программы..."
	./$(TARGET)

.PHONY: all clean run