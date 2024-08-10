//#include "RenderingTestFixture.h"
//
//#include<Rendering/Context.h>
//
//
//TEST_CASE("Rendering::Buffer tests")
//{
//	RenderingTestFixture fixture(Rendering::GraphicsAPI::OPENGL);
//	auto context = fixture.getContext();
//	SECTION("Create buffer from invalid config - should fail")
//	{
//		SECTION("Test default config")
//		{
//			Rendering::BufferConfig config{};
//
//			auto buffer = context->createBuffer(config);
//			CHECK(buffer == nullptr);
//		}
//
//		SECTION("nullptr as data, valid size")
//		{
//			Rendering::BufferConfig config{};
//			config.data = nullptr;
//			config.size = 16;
//			auto buffer = context->createBuffer(config);
//			CHECK(buffer == nullptr);
//		}
//
//		SECTION("Valid data pointer, zero size")
//		{
//			float value = 1;
//			Rendering::BufferConfig config{};
//			config.data = (std::byte*)&value;
//			config.size = 0;
//			auto buffer = context->createBuffer(config);
//			CHECK(buffer == nullptr);
//		}
//
//		SECTION("From empty vector")
//		{
//			std::vector<std::byte> data;
//			data.reserve(10);
//
//			Rendering::BufferConfig config{};
//			config.data = data.data();
//			config.size = data.size();
//			auto buffer = context->createBuffer(config);
//			CHECK(buffer == nullptr);
//		}
//	}
//
//	SECTION("Create buffer from valid configs - should succeed")
//	{
//		size_t size = GENERATE(range(1u, 128u, 1024u));
//			
//		std::vector<std::byte> data(size);
//		REQUIRE(data.size() == size);
//
//		Rendering::BufferConfig config{};
//		config.data = (std::byte*)data.data();
//		config.size = data.size();
//
//		auto buffer = context->createBuffer(config);
//		REQUIRE(buffer);
//		CHECK(buffer->size() == data.size());
//	}
//}