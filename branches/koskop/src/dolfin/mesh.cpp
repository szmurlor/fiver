//============================================================================
// Name        : mesh.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <mesh/Mesh.h>
using namespace dolfin;

int main() {
    dolfin::Mesh mesh("walec88.xml");

	std::cout << "geometry" << mesh.geometry().str(true) << std::endl;
	std::cout << "topology" << mesh.topology().str(true) << std::endl;
	std::cout << "data" << mesh.data().str(true) << std::endl;
	return 0;
}
