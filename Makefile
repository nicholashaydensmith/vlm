#Makefile
all: clean gen_vol octree

gen_vol:
	gcc --std=c99 -g -Wall -Wextra -o$@ $@.c

octree:
	gcc --std=c99 -g -Wall -Wextra -o$@ $@.c

clean:
	rm -rf gen_vol
	rm -rf octree
