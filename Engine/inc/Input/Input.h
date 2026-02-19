#pragma once
#include <cstdint>

// an sdl and XInput independant set of Input Buttons/Ranges
namespace REC
{
	namespace Input
	{
		namespace Keyboard
		{
			enum class Button : uint8_t
			{
				Keyboard_A = 4,
				Keyboard_B = 5,
				Keyboard_C = 6,
				Keyboard_D = 7,
				Keyboard_E = 8,
				Keyboard_F = 9,
				Keyboard_G = 10,
				Keyboard_H = 11,
				Keyboard_I = 12,
				Keyboard_J = 13,
				Keyboard_K = 14,
				Keyboard_L = 15,
				Keyboard_M = 16,
				Keyboard_N = 17,
				Keyboard_O = 18,
				Keyboard_P = 19,
				Keyboard_Q = 20,
				Keyboard_R = 21,
				Keyboard_S = 22,
				Keyboard_T = 23,
				Keyboard_U = 24,
				Keyboard_V = 25,
				Keyboard_W = 26,
				Keyboard_X = 27,
				Keyboard_Y = 28,
				Keyboard_Z = 29,

				Keyboard_1 = 30,
				Keyboard_2 = 31,
				Keyboard_3 = 32,
				Keyboard_4 = 33,
				Keyboard_5 = 34,
				Keyboard_6 = 35,
				Keyboard_7 = 36,
				Keyboard_8 = 37,
				Keyboard_9 = 38,
				Keyboard_0 = 39,

				Keyboard_Return = 40,
				Keyboard_Escape = 41,
				Keyboard_Backspace = 42,
				Keyboard_Tab = 43,
				Keyboard_Space = 44,

				Keyboard_Minus = 45,		 //  -
				Keyboard_Equals = 46,		 //  =
				Keyboard_LeftBracket = 47,	 //  [
				Keyboard_RightBracket = 48,	 //  ]
				Keyboard_Backslash = 49,	 // '\'
				Keyboard_Semicolon = 51,	 //  ;
				Keyboard_Apostrophe = 52,	 //  '
				Keyboard_Grave = 53,		 //  `
				Keyboard_Comma = 54,		 //  ,
				Keyboard_Period = 55,		 //	.
				Keyboard_Slash = 56,		 // '/'
				Keyboard_Capslock = 57,

				Keyboard_F1 = 58,
				Keyboard_F2 = 59,
				Keyboard_F3 = 60,
				Keyboard_F4 = 61,
				Keyboard_F5 = 62,
				Keyboard_F6 = 63,
				Keyboard_F7 = 64,
				Keyboard_F8 = 65,
				Keyboard_F9 = 66,
				Keyboard_F10 = 67,
				Keyboard_F11 = 68,
				Keyboard_F12 = 69,

				Keyboard_Printscreen = 70,
				Keyboard_ScrollLock = 71,
				Keyboard_Pause = 72,
				Keyboard_Insert = 73,

				Keyboard_Home = 74,
				Keyboard_Pageup = 75,
				Keyboard_Delete = 76,
				Keyboard_End = 77,
				Keyboard_PageDown = 78,
				Keyboard_Right = 79,
				Keyboard_Left = 80,
				Keyboard_Down = 81,
				Keyboard_Up = 82,

				Keyboard_NumLock = 83,

				Keypad_Divide = 84,
				Keypad_Multiply = 85,
				Keypad_Minus = 86,
				Keypad_Plus = 87,
				Keypad_Enter = 88,
				Keypad_1 = 89,
				Keypad_2 = 90,
				Keypad_3 = 91,
				Keypad_4 = 92,
				Keypad_5 = 93,
				Keypad_6 = 94,
				Keypad_7 = 95,
				Keypad_8 = 96,
				Keypad_9 = 97,
				Keypad_0 = 98,
				Keypad_Period = 99,
				Keypad_Equals = 103,

				Keyboard_Windows = 101,
				Keyboard_LCTRL = 242,
				Keyboard_LSHIFT = 225,
				Keyboard_LALT = 226,
				Keyboard_RCTRL = 228,
				Keyboard_RSHIFT = 229,
				Keyboard_RALT = 230
			};
		}

		namespace Controller
		{
			enum class Range
			{
				Gamepad_LeftStick, // 2 axis
				Gamepad_RightStick, // 2 axis
				Gamepad_LeftTrigger, // 1 axis
				Gamepad_RightTrigger // 1 axis
			};

			enum class Button : uint16_t
			{
				Gamepad_A = 0x1000,
				Gamepad_B = 0x2000,
				Gamepad_X = 0x4000,
				Gamepad_Y = 0x8000,

				Gamepad_LeftBumber = 0x0100,
				Gamepad_RightBumber = 0x0200,

				Gamepad_Start = 0x0020,
				Gamepad_Back = 0x0010,

				GamePad_DPad_Up = 0x0001,
				GamePad_DPad_Down = 0x0002,
				GamePad_DPad_Left = 0x0004,
				GamePad_DPad_Right = 0x0008,

				GamePad_LeftStick = 0x0040,
				GamePad_RightStick = 0x0080,
			};
		}
	}
}

constexpr uint16_t operator&(uint16_t lhs, REC::Input::Controller::Button rhs)
{
	return lhs & static_cast<uint16_t>(rhs);
}
