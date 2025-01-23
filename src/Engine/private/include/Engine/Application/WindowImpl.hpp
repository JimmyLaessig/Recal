#ifndef REEF_APPLICATION_WINDOWIMPL_HPP
#define REEF_APPLICATION_WINDOWIMPL_HPP

#include <Engine/Application/Window.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>


// Foward declaration of the windowing toolkit used
class GLFWwindow;

namespace Reef
{

class WindowImpl : public Window
{
	
public:

	~WindowImpl();

	void show() override;

	void hide() override;

	bool isShowing() const override;

	bool close() override;

	bool shouldClose() const override;

	void resize(uint32_t width, uint32_t height) override;

	uint32_t width() const override;

	uint32_t height() const override;

	void setTitle(std::string_view title) override;

	std::string_view title() const override;

	bool isFullscreen() const override;

	bool setFullscreen() override;

	bool setWindowed() override;

	bool showTitleBar() override;

	bool hideTitleBar() override;

	void processEvents() override;

	void present() override;

	void* nativeWindowHandle() override;

	//KeyState getKeyState(Key key) const override;

	//KeyState getKeyState(const InputChord& chord) const override;

	//KeyState getMouseButtonState(MouseButton button) const override;

	//Vec2 getMousePosition() const override;

	//void setMousePosition(const Vec2& normalizedPosition) override;

	static std::unique_ptr<WindowImpl> create(std::string_view title, uint32_t width, uint32_t height, bool fullscreen, bool showTitleBar);

private:
	
	WindowImpl() = default;

	GLFWwindow* mWindowHandle{ nullptr };

	std::string mTitle;
};

} // namespace Reef

#endif // !REEF_APPLICATION_WINDOWIMPL_HPP