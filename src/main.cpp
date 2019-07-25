#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include "OperandFactory.hpp"

int main() {
	OperandFactory *of = OperandFactory::getInstance();
	const IOperand *f1 = of->createOperand(IOperand::eOperandType::Int16, "45"),
			*f2 = of->createOperand(IOperand::eOperandType::Int16, "20"),
			*f3 = *f1 + *f2;
	std::cout << f3->toString() <<std::endl;
	return 0;
}