all: libvector.so prog2l

libvector.so:
	@echo "[*] create dynamic library libvector.so"
	gcc -fpic -shared -o libvector.so addvec.c multvec.c
	@echo

main2.o:
	@echo "[*] compile main2.c"
	gcc -c main2.c
	@echo

prog2l: libvector.so main2.o
	@echo ">>>> This will fail"
	@echo "(동적 라이브러리를 링크할 때에도 심볼 참조에 따른 순서가 중요)"
	-gcc -o prog2l ./libvector.so main2.o
	@echo
	@echo
	@echo ">>>> This will succeed"
	gcc -o prog2l main2.o ./libvector.so

clean:
	rm -f *.o *.so prog2l

.PHONY: all clean
