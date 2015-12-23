################################################################################
GCPP := g++
RM   := rm -rf
OBJS := src/lib_code.o 				\
		src/lib_config.o 			\
		src/lib_datetime.o 			\
		src/lib_log.o 				\
		src/lib_md5c.o 				\
		src/lib_message.o 			\
		src/lib_monitor.o 			\
		src/lib_pattern_command.o 	\
		src/lib_serviceframe.o 		\
		src/lib_showerr.o 			\
		src/lib_sockets.o 			\
		src/lib_splitrecord.o 		\
		src/lib_thread.o 			\
		src/lib_util.o

.PHONY: all clean

CFLAGS += -I./include 

%.o:%.cpp
	$(GCPP) $(CFLAGS) -c $< -o $@

target_file := mylib.so
$(target_file) : $(OBJS)
	@$(GCPP) --shared --KPIC -o $@ $^
	@echo "====task completed !======"

all: $(target_file)

clean:
	@echo "clean useless files ..."
	@find . -name "*.so" -o -name "*.o" |xargs rm -f
################################################################################
