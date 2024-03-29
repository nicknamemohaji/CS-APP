all: libvector2.a prog2

libvector2.a:
	@echo "[*] create static library libvector2.a"
	gcc -c addvec.c multvec.c
	ar -rcs libvector2.a addvec.o multvec.o
	@echo

main2.o:
	@echo "[*] compile main2.c"
	gcc -c main2.c
	@echo

prog2: libvector2.a main2.o
	@echo ">>>> This will fail"
	-gcc -static -o prog2 ./libvector2.a main2.o
	@echo
	@echo
	@echo ">>>> This will succeed"
	gcc -static -o prog2 main2.o ./libvector2.a

clean:
	rm -f *.o *.a prog2

.PHONY: all clean
