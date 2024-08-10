//#include "RenderingTestFixture.h"
//
//#include<Rendering/Context.h>
//#include<yaml-cpp/yaml.h>
//
//
//TEST_CASE("Rendering::BufferView tests")
//{
//	RenderingTestFixture fixture(Rendering::GraphicsAPI::OPENGL);
//	auto context = fixture.getContext();
//
//	std::vector<float> bufferData{ 0, 1, 2, 3, 4 };
//
//	Rendering::BufferConfig bufferConfig{};
//	bufferConfig.data = (std::byte*)bufferData.data();
//	bufferConfig.size = bufferData.size() * sizeof(float);
//	
//	auto buffer = context->createBuffer(bufferConfig);
//
//	REQUIRE(buffer);
//
//	SECTION("Create buffer view from invalid config - should fail")
//	{
//		Rendering::BufferViewConfig config{};
//
//		SECTION("Test default config")
//		{
//			auto view = context->createBufferView(config);
//			CHECK(view == nullptr);
//		}
//
//		SECTION("size is zero")
//		{
//			config.buffer = buffer;
//			config.size = 0;
//			config.attribute = Rendering::AttributeFormat::FLOAT;
//			auto view = context->createBufferView(config);
//			CHECK(view == nullptr);
//		}
//
//		SECTION("size does not match attribute format size")
//		{
//			config.buffer = buffer;
//			config.size = buffer->size();
//			config.attribute = Rendering::AttributeFormat::VEC4;
//
//			auto view = context->createBufferView(config);
//			CHECK(view == nullptr);
//		}
//
//		SECTION("size is larger than buffer size")
//		{
//			config.buffer	= buffer;
//			config.size		= buffer->size() + 1;
//			config.attribute = Rendering::AttributeFormat::FLOAT;
//
//			auto view = context->createBufferView(config);
//			CHECK(view == nullptr);
//		}
//
//		SECTION("offset is outside of buffer size")
//		{
//			config.buffer = buffer;
//			config.size = buffer->size();
//			config.offset = buffer->size() + 1;
//			config.attribute = Rendering::AttributeFormat::FLOAT;
//
//			auto view = context->createBufferView(config);
//			CHECK(view == nullptr);
//		}
//
//		SECTION("size + offset is outside of buffer size")
//		{
//			config.buffer = buffer;
//			config.size = buffer->size() * 0.5f;
//			config.offset = buffer->size() * 0.5f;
//			config.attribute = Rendering::AttributeFormat::FLOAT;
//
//			auto view = context->createBufferView(config);
//			CHECK(view == nullptr);
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
//
//
//TEST_CASE("YAML exploration")
//{
//	YAML::Node lineup = YAML::Load("{1B: Prince Fielder, 2B: Rickie Weeks, LF: Ryan Braun}");
//
//	int x = lineup["1B"].as<int>();
//}
