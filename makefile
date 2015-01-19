CC=gcc
#FG=-I core/ -pedantic -Werror -Wall -ggdb -g
FG=-I core/ -pedantic -Werror -Wall -O2
OUT=build/

driver: main.c $(OUT)muTraZ.o $(OUT)parser.o $(OUT)vm.o \
	$(OUT)tree.o $(OUT)compiler.o $(OUT)library.o \
	$(OUT)utils.o $(OUT)htbl.o
	$(CC) $(FG) main.c \
	$(OUT)muTraZ.o $(OUT)parser.o $(OUT)vm.o \
	$(OUT)tree.o $(OUT)compiler.o $(OUT)utils.o \
	$(OUT)htbl.o $(OUT)library.o \
	-o mutraz

$(OUT)muTraZ.o: core/muTraZ.c core/muTraZ.h
	$(CC) $(FG) -c core/muTraZ.c -o $(OUT)muTraZ.o

$(OUT)parser.o: core/parser.c core/parser.h
	$(CC) $(FG) -c core/parser.c -o $(OUT)parser.o

$(OUT)vm.o: core/vm.c core/vm.h
	$(CC) $(FG) -c core/vm.c -o $(OUT)vm.o

$(OUT)tree.o: core/tree.c core/tree.h
	$(CC) $(FG) -c core/tree.c -o $(OUT)tree.o

$(OUT)compiler.o: core/compiler.c core/compiler.h
	$(CC) $(FG) -c core/compiler.c -o $(OUT)compiler.o

$(OUT)utils.o: core/utils.c core/utils.h
	$(CC) $(FG) -c core/utils.c -o $(OUT)utils.o

$(OUT)htbl.o: core/htbl.c core/htbl.h
	$(CC) $(FG) -c core/htbl.c -o $(OUT)htbl.o
	
$(OUT)library.o: lib/libs.c lib/io.c lib/datetime.c \
	lib/string.c
	$(CC) $(FG) -c lib/datetime.c -o $(OUT)datetime.o
	$(CC) $(FG) -c lib/io.c -o $(OUT)io.o
	$(CC) $(FG) -c lib/libs.c -o $(OUT)libs.o
	$(CC) $(FG) -c lib/string.c -o $(OUT)string.o
	ld -r $(OUT)io.o $(OUT)libs.o $(OUT)datetime.o \
	$(OUT)string.o \
	-o $(OUT)library.o

clean:
	rm -f $(OUT)*.o
