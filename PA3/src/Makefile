CC = gcc
CCFLAGS = 

#add C source files here
SRCS=main.c

TARGET=mips_sim

OBJS := $(patsubst %.c, %.o, $(SRCS))

all: $(TARGET)

%.o:%.c
	$(CC) $(CCFLAGS) $< -c -o $@

$(TARGET): $(OBJS)
	$(CC) $(CCFALGS) $^ -o $@

.PHONY=clean
.PHONY=run

run : $(TARGET)
		./$(TARGET)

clean: 
	rm -f $(OBJS) $(TARGET)
