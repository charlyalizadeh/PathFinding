#pragma once
#include "EngineMazeSolver.h"

int main() {
	EngineMazeSolver* test = new EngineMazeSolver(50, 50, false, ASTAR, false,78940489420);
	test->Start();

	return 0;
}

