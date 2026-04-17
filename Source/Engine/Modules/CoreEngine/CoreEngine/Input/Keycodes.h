#pragma once
#include <string>

namespace Eclipse
{
	struct Keycode
	{
		enum Scancode
		{
#pragma region |- MOUSE

			MOUSE_LEFT = 0,
			MOUSE_RIGHT = 1,
			MOUSE_MIDDLE = 2,
			MOUSE_BACK_BUTTON = 3,
			MOUSE_FORWARD_BUTTON = 4,

#pragma endregion


#pragma region |- Keyboard

			NONE = 0,

			ESCAPE = 256,

			PLUS = 45,

			N0 = 48,
			N1 = 49,
			N2 = 50,
			N3 = 51,
			N4 = 52,
			N5 = 53,
			N6 = 54,
			N7 = 55,
			N8 = 56,
			N9 = 57,

			ACUTE_ACCENT = 61,
			LESS_THAN = 162,
			SECTION_SIGN = 96,
			UMLAUT = 93,

			BACKSPACE = 259,

			INSERT = 260,
			HOME = 268,
			PG_UP = 266,
			DELETE_KEY = 261,
			END = 269,
			PG_DOWN = 267,

			LEFT = 263,
			UP = 265,
			RIGHT = 262,
			DOWN = 264,

			TAB = 258,
			CAPS_LOCK = 280,

			L_SHIFT = 340,
			R_SHIFT = 344,

			L_CTRL = 341,
			R_CTRL = 345,

			L_WINDOWS = 343,
			R_WINDOWS = 347,

			L_ALT = 342,
			R_ALT = 346,

			ENTER = 257,

			SPACE = 32,

			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,


			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,
			AA = 91,
			AAA = 39,
			OO = 59,

			COMMA = 44,
			DOT = 46,

			ABSTERIX = 92,

#pragma endregion
		};

		static std::string ToString(Scancode _scancode)
		{
			switch (_scancode)
			{
			case MOUSE_LEFT: return "MOUSE_LEFT";
			case MOUSE_RIGHT: return "MOUSE_RIGHT";
			case MOUSE_MIDDLE: return "MOUSE_MIDDLE";

			case MOUSE_BACK_BUTTON: return "MOUSE_BACK_BUTTON";
			case MOUSE_FORWARD_BUTTON: return "MOUSE_FORWARD_BUTTON";


			case ESCAPE: return "ESCAPE";
			case PLUS: return "PLUS";
			case N0: return "N0"; case N1: return "N1"; case N2: return "N2"; case N3: return "N3"; case N4: return "N4";
			case N5: return "N5"; case N6: return "N6"; case N7: return "N7"; case N8: return "N8"; case N9: return "N9";
			case ACUTE_ACCENT: return "ACUTE_ACCENT";
			case LESS_THAN: return "LESS_THAN";
			case SECTION_SIGN: return "SECTION_SIGN";
			case UMLAUT: return "UMLAUT";
			case BACKSPACE: return "BACKSPACE";
			case INSERT: return "INSERT";
			case HOME: return "HOME";
			case PG_UP: return "PG_UP";
			case DELETE_KEY: return "DELETE";
			case END: return "END";
			case PG_DOWN: return "PG_DOWN";
			case LEFT: return "LEFT";
			case UP: return "UP";
			case RIGHT: return "RIGHT";
			case DOWN: return "DOWN";
			case TAB: return "TAB";
			case CAPS_LOCK: return "CAPS_LOCK";
			case L_SHIFT: return "L_SHIFT";
			case R_SHIFT: return "R_SHIFT";
			case L_CTRL: return "L_CTRL";
			case R_CTRL: return "R_CTRL";
			case L_WINDOWS: return "L_WINDOWS";
			case R_WINDOWS: return "R_WINDOWS";
			case L_ALT: return "L_ALT";
			case R_ALT: return "R_ALT";
			case ENTER: return "ENTER";
			case SPACE: return "SPACE";
			case F1: return "F1"; case F2: return "F2"; case F3: return "F3"; case F4: return "F4"; case F5: return "F5"; case F6: return "F6";
			case F7: return "F7"; case F8: return "F8"; case F9: return "F9"; case F10: return "F10"; case F11: return "F11"; case F12: return "F12";
			case A: return "A"; case B: return "B"; case C: return "C"; case D: return "D"; case E: return "E"; case F: return "F"; case G: return "G";
			case H: return "H"; case I: return "I"; case J: return "J"; case K: return "K"; case L: return "L"; case M: return "M"; case N: return "N";
			case O: return "O"; case P: return "P"; case Q: return "Q"; case R: return "R"; case S: return "S"; case T: return "T"; case U: return "U";
			case V: return "V"; case W: return "W"; case X: return "X"; case Y: return "Y"; case Z: return "Z";
			case AA: return "AA";
			case AAA: return "AAA";
			case OO: return "OO";
			case COMMA: return "COMMA";
			case DOT: return "DOT";
			case ABSTERIX: return "ABSTERIX";
			}

			return "UNKNOWN";
		}
	};
}
