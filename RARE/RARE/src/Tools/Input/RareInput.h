#pragma once
#include <stdint.h>

namespace Rare {
	typedef uint32_t MouseKey;
	typedef uint32_t KeyCode;
	namespace MouseKeys {
		constexpr Rare::MouseKey MOUSE_BUTTON_1 = 0;
		constexpr Rare::MouseKey MOUSE_BUTTON_2 = 1;
		constexpr Rare::MouseKey MOUSE_BUTTON_3 = 2;
		constexpr Rare::MouseKey MOUSE_BUTTON_4 = 3;
		constexpr Rare::MouseKey MOUSE_BUTTON_5 = 4;
		constexpr Rare::MouseKey MOUSE_BUTTON_6 = 5;
		constexpr Rare::MouseKey MOUSE_BUTTON_7 = 6;
		constexpr Rare::MouseKey MOUSE_BUTTON_8 = 7;
		constexpr Rare::MouseKey MOUSE_BUTTON_LEFT =   MOUSE_BUTTON_1;
		constexpr Rare::MouseKey MOUSE_BUTTON_RIGHT =  MOUSE_BUTTON_2;
		constexpr Rare::MouseKey MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3;

	}

	namespace KeyCodes {
		constexpr Rare::KeyCode KEY_SPACE = 32;
		constexpr Rare::KeyCode KEY_APOSTROPHE = 39;  /* ' */
		constexpr Rare::KeyCode KEY_COMMA = 44;  /* , */
		constexpr Rare::KeyCode KEY_MINUS = 45;  /* - */
		constexpr Rare::KeyCode KEY_PERIOD = 46;  /* . */
		constexpr Rare::KeyCode KEY_SLASH = 47;  /* / */
		constexpr Rare::KeyCode KEY_0 = 48;
		constexpr Rare::KeyCode KEY_1 = 49;
		constexpr Rare::KeyCode KEY_2 = 50;
		constexpr Rare::KeyCode KEY_3 = 51;
		constexpr Rare::KeyCode KEY_4 = 52;
		constexpr Rare::KeyCode KEY_5 = 53;
		constexpr Rare::KeyCode KEY_6 = 54;
		constexpr Rare::KeyCode KEY_7 = 55;
		constexpr Rare::KeyCode KEY_8 = 56;
		constexpr Rare::KeyCode KEY_9 = 57;
		constexpr Rare::KeyCode KEY_SEMICOLON = 59;  /* ; */
		constexpr Rare::KeyCode KEY_EQUAL = 61;  /* = */
		constexpr Rare::KeyCode KEY_A = 65;
		constexpr Rare::KeyCode KEY_B = 66;
		constexpr Rare::KeyCode KEY_C = 67;
		constexpr Rare::KeyCode KEY_D = 68;
		constexpr Rare::KeyCode KEY_E = 69;
		constexpr Rare::KeyCode KEY_F = 70;
		constexpr Rare::KeyCode KEY_G = 71;
		constexpr Rare::KeyCode KEY_H = 72;
		constexpr Rare::KeyCode KEY_I = 73;
		constexpr Rare::KeyCode KEY_J = 74;
		constexpr Rare::KeyCode KEY_K = 75;
		constexpr Rare::KeyCode KEY_L = 76;
		constexpr Rare::KeyCode KEY_M = 77;
		constexpr Rare::KeyCode KEY_N = 78;
		constexpr Rare::KeyCode KEY_O = 79;
		constexpr Rare::KeyCode KEY_P = 80;
		constexpr Rare::KeyCode KEY_Q = 81;
		constexpr Rare::KeyCode KEY_R = 82;
		constexpr Rare::KeyCode KEY_S = 83;
		constexpr Rare::KeyCode KEY_T = 84;
		constexpr Rare::KeyCode KEY_U = 85;
		constexpr Rare::KeyCode KEY_V = 86;
		constexpr Rare::KeyCode KEY_W = 87;
		constexpr Rare::KeyCode KEY_X = 88;
		constexpr Rare::KeyCode KEY_Y = 89;
		constexpr Rare::KeyCode KEY_Z = 90;
		constexpr Rare::KeyCode KEY_LEFT_BRACKET = 91;  /* [ */
		constexpr Rare::KeyCode KEY_BACKSLASH = 92;  /* \ */
		constexpr Rare::KeyCode KEY_RIGHT_BRACKET = 93;  /* ] */
		constexpr Rare::KeyCode KEY_GRAVE_ACCENT = 96;  /* ` */
		constexpr Rare::KeyCode KEY_WORLD_1 = 161; /* non-US #1 */
		constexpr Rare::KeyCode KEY_WORLD_2 = 162; /* non-US #2 */
		constexpr Rare::KeyCode KEY_ESCAPE = 256;
		constexpr Rare::KeyCode KEY_ENTER = 257;
		constexpr Rare::KeyCode KEY_TAB = 258;
		constexpr Rare::KeyCode KEY_BACKSPACE = 259;
		constexpr Rare::KeyCode KEY_INSERT = 260;
		constexpr Rare::KeyCode KEY_DELETE = 261;
		constexpr Rare::KeyCode KEY_RIGHT = 262;
		constexpr Rare::KeyCode KEY_LEFT = 263;
		constexpr Rare::KeyCode KEY_DOWN = 264;
		constexpr Rare::KeyCode KEY_UP = 265;
		constexpr Rare::KeyCode KEY_PAGE_UP = 266;
		constexpr Rare::KeyCode KEY_PAGE_DOWN = 267;
		constexpr Rare::KeyCode KEY_HOME = 268;
		constexpr Rare::KeyCode KEY_END = 269;
		constexpr Rare::KeyCode KEY_CAPS_LOCK = 280;
		constexpr Rare::KeyCode KEY_SCROLL_LOCK = 281;
		constexpr Rare::KeyCode KEY_NUM_LOCK = 282;
		constexpr Rare::KeyCode KEY_PRINT_SCREEN = 283;
		constexpr Rare::KeyCode KEY_PAUSE = 284;
		constexpr Rare::KeyCode KEY_F1 = 290;
		constexpr Rare::KeyCode KEY_F2 = 291;
		constexpr Rare::KeyCode KEY_F3 = 292;
		constexpr Rare::KeyCode KEY_F4 = 293;
		constexpr Rare::KeyCode KEY_F5 = 294;
		constexpr Rare::KeyCode KEY_F6 = 295;
		constexpr Rare::KeyCode KEY_F7 = 296;
		constexpr Rare::KeyCode KEY_F8 = 297;
		constexpr Rare::KeyCode KEY_F9 = 298;
		constexpr Rare::KeyCode KEY_F10 = 299;
		constexpr Rare::KeyCode KEY_F11 = 300;
		constexpr Rare::KeyCode KEY_F12 = 301;
		constexpr Rare::KeyCode KEY_F13 = 302;
		constexpr Rare::KeyCode KEY_F14 = 303;
		constexpr Rare::KeyCode KEY_F15 = 304;
		constexpr Rare::KeyCode KEY_F16 = 305;
		constexpr Rare::KeyCode KEY_F17 = 306;
		constexpr Rare::KeyCode KEY_F18 = 307;
		constexpr Rare::KeyCode KEY_F19 = 308;
		constexpr Rare::KeyCode KEY_F20 = 309;
		constexpr Rare::KeyCode KEY_F21 = 310;
		constexpr Rare::KeyCode KEY_F22 = 311;
		constexpr Rare::KeyCode KEY_F23 = 312;
		constexpr Rare::KeyCode KEY_F24 = 313;
		constexpr Rare::KeyCode KEY_F25 = 314;
		constexpr Rare::KeyCode KEY_KP_0 = 320;
		constexpr Rare::KeyCode KEY_KP_1 = 321;
		constexpr Rare::KeyCode KEY_KP_2 = 322;
		constexpr Rare::KeyCode KEY_KP_3 = 323;
		constexpr Rare::KeyCode KEY_KP_4 = 324;
		constexpr Rare::KeyCode KEY_KP_5 = 325;
		constexpr Rare::KeyCode KEY_KP_6 = 326;
		constexpr Rare::KeyCode KEY_KP_7 = 327;
		constexpr Rare::KeyCode KEY_KP_8 = 328;
		constexpr Rare::KeyCode KEY_KP_9 = 329;
		constexpr Rare::KeyCode KEY_KP_DECIMAL = 330;
		constexpr Rare::KeyCode KEY_KP_DIVIDE = 331;
		constexpr Rare::KeyCode KEY_KP_MULTIPLY = 332;
		constexpr Rare::KeyCode KEY_KP_SUBTRACT = 333;
		constexpr Rare::KeyCode KEY_KP_ADD = 334;
		constexpr Rare::KeyCode KEY_KP_ENTER = 335;
		constexpr Rare::KeyCode KEY_KP_EQUAL = 336;
		constexpr Rare::KeyCode KEY_LEFT_SHIFT = 340;
		constexpr Rare::KeyCode KEY_LEFT_CONTROL = 341;
		constexpr Rare::KeyCode KEY_LEFT_ALT = 342;
		constexpr Rare::KeyCode KEY_LEFT_SUPER = 343;
		constexpr Rare::KeyCode KEY_RIGHT_SHIFT = 344;
		constexpr Rare::KeyCode KEY_RIGHT_CONTROL = 345;
		constexpr Rare::KeyCode KEY_RIGHT_ALT = 346;
		constexpr Rare::KeyCode KEY_RIGHT_SUPER = 347;
		constexpr Rare::KeyCode KEY_MENU = 348;
	}
}