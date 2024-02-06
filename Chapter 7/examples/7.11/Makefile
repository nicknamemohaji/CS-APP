prog2r:
	@make -C ../7.10 libvector.so
	gcc -rdynamic -o prog2r dll.c -ldl

clean:
	rm -f prog2r
	@make -C ../7.10 clean

.PHONY: prog2r clean
