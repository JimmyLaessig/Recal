
#include <catch2/catch.hpp>

import Math.Matrix;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool areIdentical(const Math::Mat44F& m1, const glm::mat4& m2)
{
	for (uint32_t c = 0; c < 4; ++c)
	{
		for (uint32_t r = 0; r < 4; ++r)
		{
			if (m1[c][r] != m2[c][r])
			{
				return false;
			}
		}
	}

	return true;
}


TEST_CASE("Test Matrix")
{
	SECTION("Test identity")
	{
		Math::Mat44F m(1.0f);
		glm::mat4 gt(1.0f);
		CHECK(areIdentical(m, gt));
	}

	SECTION("Test translate")
	{
		Math::Mat44F m = Math::createTranslationMatrix({ 1.0f, 2.0f, 3.0f });
		glm::mat4 gt = glm::translate(glm::mat4(1.0f), { 1.0f, 2.0f, 3.0f });
		CHECK(areIdentical(m, gt));
	}

	SECTION("Test scale")
	{
		Math::Mat44F m = Math::createScaleMatrix({ 1.0f, 2.0f, 3.0f });
		glm::mat4 gt = glm::scale(glm::mat4(1.0f), { 1.0f, 2.0f, 3.0f });
		CHECK(areIdentical(m, gt));
	}

	SECTION("Test rotate")
	{
		////glm::quat rotGT(0.5f, 0.5f, 0.5f, 0.5f);
		//
		//Math::Mat44F m = Math::createScaleMatrix<float>({ 1.0f, 2.0f, 3.0f });
		//glm::mat4 gt = glm::scale(glm::mat4(1.0f), { 1.0f, 2.0f, 3.0f });
		//CHECK(areIdentical(m, gt));
	}
}