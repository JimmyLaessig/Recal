module;

#include <Engine/System.h>

#include <cstdint>
#include <string_view>
#include <memory>

export module Engine.Application.Window;

//import Engine.Application.Input;

export namespace Reef
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

export ENGINE_API class Window //: public InputSystem
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
