#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include "OperandFactory.hpp"

int main() {
//	for (int a = std::numeric_limits<char>::min(); a <= std::numeric_limits<char>::max(); a++) {
//		for (int b = std::numeric_limits<char>::min();
//			 b <= std::numeric_limits<char>::max(); b++) {
//			std::cout << (int) a << "/" << (int) b << "=";
//			try {
//				const Int8* c = dynamic_cast<const Int8 *>(Int8(a) / Int8(b));
//				std::cout << (int)c->getValue() << std::endl;
//				delete c;
//			} catch (std::exception &ex) {
//				std::cout << ex.what() << std::endl;
//			}
//		}
//	}
//	try {
//		const Float *c = dynamic_cast<const Float *>(Float(std::numeric_limits<float>::max()) / Float(-0.5f));
//	} catch (std::exception &ex) {
//		std::cout << ex.what() << std::endl;
//	}
//	const Float	*f1 = new Float(std::numeric_limits<float>::max()),
//			*f2 = new Float(2),
//			*f3 = dynamic_cast<const Float *>(*f1 * *f2);
	std::stringstream s;
	s << std::numeric_limits<double>::max();
	double f;
	s >> f;
	std::cout << f << std::endl;
	system("leaks Abstract-VM");
	return 0;
}