#include <Engine/Application/Window.hpp>
#include <Engine/Application/WindowImpl.hpp>

#include <memory>
#include <string_view>

using namespace Reef;


std::unique_ptr<Window>
Window::create(std::string_view title, uint32_t width, uint32_t height, bool fullscreen, bool showTitleBar)
{
	return WindowImpl::create(title, width, height, fullscreen, showTitleBar);
}