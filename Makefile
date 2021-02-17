simulation: utils.c input.c simulation.c stats.c output.c log.c
	gcc -g utils.c input.c simulation.c stats.c output.c log.c -lm -o simulation
