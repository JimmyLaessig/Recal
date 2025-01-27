export module Engine.Application.Input;

//#include <Engine/System.hpp>


export namespace Reef
{

struct Vec2
{
	float x{ 0 };
	float y{ 0 };
};

enum class Key
{
	// Printable keys
	SPACE			= 32,
	APOSTROPHE		= 39, // '
	COMMA			= 44, // ,
	MINUS			= 45, // -
	PERIOD			= 46, // .
	SLASH			= 47, // /
	ZERO			= 48,
	ONE				= 49,
	TWO				= 50,
	THREE			= 51,
	FOUR			= 52,
	FIVE			= 53,
	SIX				= 54,
	SEVEN			= 55,
	EIGHT			= 56,
	NINE			= 57,
	SEMICOLON		= 59, // ;
	EQUAL			= 61, // =
	A				= 65,
	B				= 66,
	C				= 67,
	D				= 68,
	E				= 69,
	F				= 70,
	G				= 71,
	H				= 72,
	I				= 73,
	J				= 74,
	K				= 75,
	L				= 76,
	M				= 77,
	N				= 78,
	O				= 79,
	P				= 80,
	Q				= 81,
	R				= 82,
	S				= 83,
	T				= 84,
	U				= 85,
	V				= 86,
	W				= 87,
	X				= 88,
	Y				= 89,
	Z				= 90,
	LEFT_BRACKET	= 91, // [
	BACKSLASH		= 92, // /* \ */
	RIGHT_BRACKET	= 93, // ]
	GRAVE_ACCENT	= 96, // `
	WORLD_1			= 161, // non-US #1
	WORLD_2			= 162, // non-US #2

	// Function keys
	ESCAPE			= 256,
	ENTER			= 257,
	TAB				= 258,
	BACKSPACE		= 259,
	INSERT			= 260,
	DELETE			= 261,
	RIGHT			= 262,
	LEFT			= 263,
	DOWN			= 264,
	UP				= 265,
	PAGE_UP			= 266,
	PAGE_DOWN		= 267,
	HOME			= 268,
	END				= 269,
	CAPS_LOCK		= 280,
	SCROLL_LOCK		= 281,
	NUM_LOCK		= 282,
	PRINT_SCREEN	= 283,
	PAUSE			= 284,
	F1				= 290,
	F2				= 291,
	F3				= 292,
	F4				= 293,
	F5				= 294,
	F6				= 295,
	F7				= 296,
	F8				= 297,
	F9				= 298,
	F10				= 299,
	F11				= 300,
	F12				= 301,
	F13				= 302,
	F14				= 303,
	F15				= 304,
	F16				= 305,
	F17				= 306,
	F18				= 307,
	F19				= 308,
	F20				= 309,
	F21				= 310,
	F22				= 311,
	F23				= 312,
	F24				= 313,
	F25				= 314,
	KP_ZERO			= 320,
	KP_ONE			= 321,
	KP_TWO			= 322,
	KP_THREE		= 323,
	KP_FOUR			= 324,
	KP_FIVE			= 325,
	KP_SIX			= 326,
	KP_SEVEN		= 327,
	KP_EIGHT		= 328,
	KP_NINE			= 329,
	KP_DECIMAL		= 330,
	KP_DIVIDE		= 331,
	KP_MULTIPLY		= 332,
	KP_SUBTRACT		= 333,
	KP_ADD			= 334,
	KP_ENTER		= 335,
	KP_EQUAL		= 336,
	LEFT_SHIFT		= 340,
	LEFT_CONTROL	= 341,
	LEFT_ALT		= 342,
	LEFT_SUPER		= 343,
	RIGHT_SHIFT		= 344,
	RIGHT_CONTROL	= 345,
	RIGHT_ALT		= 346,
	RIGHT_SUPER		= 347,
	MENU			= 348,
	UNKNOWN			= -1,
};


enum class MouseButton
{
	BUTTON_1		= 0,
 	BUTTON_2		= 1,
 	BUTTON_3		= 2,
 	BUTTON_4		= 3,
 	BUTTON_5		= 4,
 	BUTTON_6		= 5,
 	BUTTON_7		= 6,
 	BUTTON_8		= 7,
	BUTTON_LEFT		= BUTTON_1,
	BUTTON_RIGHT	= BUTTON_2,
	BUTTON_MIDDLE	= BUTTON_3,
};


struct InputChord
{
	Key key;

	bool command;
	bool shift;
	bool control;
};


enum class KeyState
{
	PRESSED,
	REPEAT,
	RELEASED
};


class InputSystem
{
public:

	virtual KeyState getKeyState(Key key) const = 0;

	virtual KeyState getKeyState(const InputChord& chord) const = 0;

	virtual KeyState getMouseButtonState(MouseButton button) const = 0;

	virtual void setMousePosition(const Vec2& normalizedPosition) = 0;

	virtual Vec2 getMousePosition() const = 0;
};


class Input
{
public:

	static KeyState getKeyState(Key key);

	static KeyState getKeyState(const InputChord& chord);

	static KeyState getMouseButtonState(MouseButton button);

	static Vec2 getMousePosition();

	static void setMousePosition(const Vec2& normalizedPosition);

	static void setInputSystem(InputSystem& inputSystem);

	static void resetInputSystem();

private:

	static InputSystem* sCurrentInputSystem;
};

} // namespace Reef
