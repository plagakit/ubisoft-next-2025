#include "matrix_test.h"

#include <engine.h>

void MatrixTest::RunTests()
{
	Logger::Info("Running Matrix tests...");

	// https://www.wolframalpha.com/input?i2d=true&i=%7B%7B0%2C1%2C2%2C3%7D%2C%7B4%2C5%2C6%2C7%7D%2C%7B8%2C9%2C10%2C11%7D%2C%7B12%2C13%2C14%2C15%7D%7D%7B%7B16%2C17%2C18%2C19%7D%2C%7B20%2C21%2C22%2C23%7D%2C%7B24%2C25%2C26%2C27%7D%2C%7B28%2C29%2C30%2C31%7D%7D
	
	Mat4 a = Mat4(
		0, 4, 8,  12,
		1, 5, 9,  13,
		2, 6, 10, 14,
		3, 7, 11, 15
	);

	Mat4 b = Mat4(
		16, 20, 24, 28,
		17, 21, 25, 29,
		18, 22, 26, 30,
		19, 23, 27, 31
	);

	Mat4 ab = a * b;
	Mat4 ba = b * a;

	//Logger::Info("%s", a.ToString().c_str());
	//Logger::Info("%s", ab.ToString().c_str());

	ASSERT_ERROR(
		ab[0] == 152 && ab[4] == 158 && ab[8] == 164 && ab[12] == 170 &&
		ab[1] == 504 && ab[5] == 526 && ab[9] == 548 && ab[13] == 570 &&
		ab[2] == 856 && ab[6] == 894 && ab[10] == 932 && ab[14] == 970 &&
		ab[3] == 1208 && ab[7] == 1262 && ab[11] == 1316 && ab[15] == 1370,
	"Wrong A * B matrix mult. Shucks");
	
	ASSERT_ERROR(
		ba[0] == 440 && ba[4] == 510 && ba[8] == 580 && ba[12] == 650 &&
		ba[1] == 536 && ba[5] == 622 && ba[9] == 708 && ba[13] == 794 &&
		ba[2] == 632 && ba[6] == 734 && ba[10] == 836 && ba[14] == 938 &&
		ba[3] == 728 && ba[7] == 846 && ba[11] == 964 && ba[15] == 1082,
		"Wrong B * A matrix mult. Shucks");

	// https://www.wolframalpha.com/input?i2d=true&i=%7B%7B0%2C1%2C2%2C3%7D%2C%7B4%2C5%2C6%2C7%7D%2C%7B8%2C9%2C10%2C11%7D%2C%7B12%2C13%2C14%2C15%7D%7D%7B%7B16%7D%2C%7B17%7D%2C%7B18%7D%2C%7B19%7D%7D
	Vec4 v = Vec4(16, 17, 18, 19);
	Vec4 av = a * v;
	
	ASSERT_ERROR(
		av.x == 110 &&
		av.y == 390 &&
		av.z == 670 &&
		av.w == 950,
	"Wrong Vec4 matrix mult. Shucks");

	Logger::Info("Matrix tests passed!");
}
