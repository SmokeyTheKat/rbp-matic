all:
	gcc ./src/main.c -o ./rma -Wall
install:
	sudo cp ./rma /usr/bin/rma
tc: all
	./rma test.rma -o out
	xxd -b out
