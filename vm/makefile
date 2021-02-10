all:
	gcc ./src/main.c -o ./rmvm -Wall
install:
	sudo cp ./rmvm /usr/bin/rmvm
tc: all
	rma test.rma -o test
	./rmvm ./test
