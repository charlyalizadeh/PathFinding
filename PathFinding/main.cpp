#pragma once
#include "EngineMazeSolver.h"

int main() {
	EngineMazeSolver* test = new EngineMazeSolver(50, 50, true, DIJKSTRA, true,time(NULL));
	test->Start();

	return 0;
}

