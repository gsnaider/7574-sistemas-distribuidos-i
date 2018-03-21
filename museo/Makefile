CFLAGS := -g
VFLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes
ARCHIVOS = semaphore.o
PROGRAMA = main puerta

all: clean $(PROGRAMA)

main: $(ARCHIVOS) main.o
	gcc -o main $^

puerta: $(ARCHIVOS) puerta.o
	gcc -o puerta $^
	
run: clean $(PROGRAMA)
	./main
	ipcrm -a

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(PROGRAMA) *.o
	ipcrm -a

valgrind: clean $(PROGRAMA)
	valgrind $(VFLAGS) ./$(PROGRAMA)

.PHONY: clean


