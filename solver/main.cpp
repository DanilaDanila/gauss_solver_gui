#include "fract.hpp"
#include "tools.hpp"
#include "vector.hpp"
#include "slau.hpp"

#include <iostream>
int main()
{
	/*
	 * y + 3z = -1
	 * 2x + 3y + 5z = 3
	 * 3x + 5y + 7z = 6
	 * (1, 2, -1)
	 *
	 * 2x + 3y + 4z + 5t = 30
	 * 3x + 3y + 4z + 5t = 34
	 * 4x + 4y + 4z + 5t = 41
	 * x + y + z + t = 10
	 * (4, 3, 2, 1)
	 *
	 * ----------------------
	 *
	 * 2x + y + z = 4
	 * 3x + z = 4
	 *
	 * 3x1 + x2 + x3 + 2x4 = -2
	 * 5x1 + 2x3 + 5x4 = -2
	 * 6x1 + x2 + 5x3 + 7x4 = -4
	 * 2x1 + x2 + 2x3 + 2x4 = -2
	 */	
	
	Vector<Vector<int>> slau0={{0, 1, 3, -1},
						  	   {2, 3, 5, 3},
						  	   {3, 5, 7, 6}};

	Vector<Vector<int>> slau1={{2, 3, 4, 5, 30},
							   {3, 3, 4, 5, 34},
							   {4, 4, 4, 5, 41},
							   {1, 1, 1, 1, 10}};

	Vector<Vector<int>> slau2={{2, 1, 1, 4},
							   {3, 0, 1, 4}};

	Vector<Vector<int>> slau3={{3, 1, 1, 2, -2},
						   	   {5, 0, 2, 5, -2},
						   	   {6, 1, 5, 7, -4},
						   	   {2, 1, 2, 2, -2}};

	std::cout<<as_slau(slau0)<<"\n";
	std::cout<<as_matrix(get_solution(slau0))<<"\n\n";

	return 0;
}
