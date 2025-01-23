#ifndef REEF_APPLICATION_WINDOW_HPP
#define REEF_APPLICATION_WINDOW_HPP

#include <Engine/System.hpp>

#include <cstdint>
#include <memory>
#include <string_view>

namespace Reef
{

/*enum class WindowEvent
{
	SEND_TO_TRAY,
	RESTORED_FROM_TRAY,
	MAXIMIZED,
	RESTORED,
	FOCUSED,
	FOCUS_LOST,
	OPENED,
	CLOSED
};*/

class ENGINE_API Window //: public InputSystem
{
public:

	virtual ~Window() = default;

	virtual void show() = 0;

	virtual void hide() = 0;

	virtual bool isShowing() const = 0;

	virtual bool close() = 0;

	virtual bool shouldClose() const = 0;

	virtual void resize(uint32_t width, uint32_t height) = 0;

	virtual uint32_t width() const = 0;

	virtual uint32_t height() const = 0;

	virtual void setTitle(std::string_view title) = 0;

	virtual std::string_view title() const = 0;

	virtual bool isFullscreen() const = 0;

	virtual bool setFullscreen() = 0;

	virtual bool setWindowed() = 0;

	virtual bool showTitleBar() = 0;

	virtual bool hideTitleBar() = 0;

	virtual void processEvents() = 0;

	virtual void present() = 0;

	virtual void* nativeWindowHandle() = 0;

	static std::unique_ptr<Window> create(std::string_view title, uint32_t width, uint32_t height, bool fullscreen, bool showTitleBar);

};

} // namespace Reef

#endif // !REEF_APPLICATION_WINDOW_HPP
