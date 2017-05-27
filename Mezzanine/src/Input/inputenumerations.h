// © Copyright 2010 - 2016 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/

#ifndef _inputenumerations_h
#define _inputenumerations_h

/// @todo This exists because somewhere in our chain of includes we have "winnt.h" (on windows) defining "KEY_EXECUTE", which causes the compile to fail at the line
/// declaring "KEY_EXECUTE".  I am reasonably certain this only exists when <Ogre.h> is included above this file in the include chain.
#ifdef KEY_EXECUTE
#undef KEY_EXECUTE
#endif

namespace Mezzanine
{
    namespace Input
    {
        /// @brief The InputCode enum defines all the possible types of inputs.
        /// @details It has one entry for each key on a most keyboards. Then it has an entry for most mouse and
        /// joystick input methods. @n
        /// @n
        /// The keyboard values must be explicit in order to match their internal representations.  There are too
        /// many keys with arbitrary values to sensically make and maintain a conversion function and have it be a
        /// slow down.
        enum InputCode
        {
            KEY_FIRST   = 0,    /**< KEY_FIRST Same Value as KEY_UNKOWN, is Guaranteed to be the lowest value of any key. */
            KEY_UNKNOWN = 0,    /**< KEY_UNKNOWN This is used for unsupported keys or keys that are not in Unicode. */

            KEY_A = 4,
            KEY_B = 5,
            KEY_C = 6,
            KEY_D = 7,
            KEY_E = 8,
            KEY_F = 9,
            KEY_G = 10,
            KEY_H = 11,
            KEY_I = 12,
            KEY_J = 13,
            KEY_K = 14,
            KEY_L = 15,
            KEY_M = 16,
            KEY_N = 17,
            KEY_O = 18,
            KEY_P = 19,
            KEY_Q = 20,
            KEY_R = 21,
            KEY_S = 22,
            KEY_T = 23,
            KEY_U = 24,
            KEY_V = 25,
            KEY_W = 26,
            KEY_X = 27,
            KEY_Y = 28,
            KEY_Z = 29,

            KEY_1 = 30,
            KEY_2 = 31,
            KEY_3 = 32,
            KEY_4 = 33,
            KEY_5 = 34,
            KEY_6 = 35,
            KEY_7 = 36,
            KEY_8 = 37,
            KEY_9 = 38,
            KEY_0 = 39,

            KEY_RETURN    = 40,
            KEY_ESCAPE    = 41,
            KEY_BACKSPACE = 42,
            KEY_TAB       = 43,
            KEY_SPACE     = 44,

            KEY_MINUS        = 45,
            KEY_EQUALS       = 46,
            KEY_LEFTBRACKET  = 47,
            KEY_RIGHTBRACKET = 48,
            KEY_BACKSLASH    = 49, /**< Located at the lower left of the return
                                    *   key on ISO keyboards and at the right end
                                    *   of the QWERTY row on ANSI keyboards.
                                    *   Produces REVERSE SOLIDUS (backslash) and
                                    *   VERTICAL LINE in a US layout, REVERSE
                                    *   SOLIDUS and VERTICAL LINE in a UK Mac
                                    *   layout, NUMBER SIGN and TILDE in a UK
                                    *   Windows layout, DOLLAR SIGN and POUND SIGN
                                    *   in a Swiss German layout, NUMBER SIGN and
                                    *   APOSTROPHE in a German layout, GRAVE
                                    *   ACCENT and POUND SIGN in a French Mac
                                    *   layout, and ASTERISK and MICRO SIGN in a
                                    *   French Windows layout.
                                    */
            KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                 *   instead of 49 for the same key, but all
                                 *   OSes I've seen treat the two codes
                                 *   identically. So, as an implementor, unless
                                 *   your keyboard generates both of those
                                 *   codes and your OS treats them differently,
                                 *   you should generate KEY_BACKSLASH
                                 *   instead of this code. As a user, you
                                 *   should not rely on this code because SDL
                                 *   will never generate it with most (all?)
                                 *   keyboards.
                                 */
            KEY_SEMICOLON  = 51,
            KEY_APOSTROPHE = 52,
            KEY_GRAVE      = 53, /**< Located in the top left corner (on both ANSI
                                  *   and ISO keyboards). Produces GRAVE ACCENT and
                                  *   TILDE in a US Windows layout and in US and UK
                                  *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                                  *   and NOT SIGN in a UK Windows layout, SECTION
                                  *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                                  *   layouts on ISO keyboards, SECTION SIGN and
                                  *   DEGREE SIGN in a Swiss German layout (Mac:
                                  *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                                  *   DEGREE SIGN in a German layout (Mac: only on
                                  *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                                  *   French Windows layout, COMMERCIAL AT and
                                  *   NUMBER SIGN in a French Mac layout on ISO
                                  *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                                  *   SIGN in a Swiss German, German, or French Mac
                                  *   layout on ANSI keyboards.
                                  */
            KEY_COMMA  = 54,
            KEY_PERIOD = 55,
            KEY_SLASH  = 56,

            KEY_CAPSLOCK = 57,

            KEY_F1  = 58,
            KEY_F2  = 59,
            KEY_F3  = 60,
            KEY_F4  = 61,
            KEY_F5  = 62,
            KEY_F6  = 63,
            KEY_F7  = 64,
            KEY_F8  = 65,
            KEY_F9  = 66,
            KEY_F10 = 67,
            KEY_F11 = 68,
            KEY_F12 = 69,

            KEY_PRINTSCREEN = 70,
            KEY_SCROLLLOCK  = 71,
            KEY_PAUSE       = 72,
            KEY_INSERT      = 73, /**< insert on PC, help on some Mac keyboards (but
                                            does send code 73, not 117) */
            KEY_HOME     = 74,
            KEY_PAGEUP   = 75,
            KEY_DELETE   = 76,
            KEY_END      = 77,
            KEY_PAGEDOWN = 78,
            KEY_RIGHT    = 79,
            KEY_LEFT     = 80,
            KEY_DOWN     = 81,
            KEY_UP       = 82,

            KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards */

            KEY_KP_DIVIDE   = 84,
            KEY_KP_MULTIPLY = 85,
            KEY_KP_MINUS    = 86,
            KEY_KP_PLUS     = 87,
            KEY_KP_ENTER    = 88,
            KEY_KP_1        = 89,
            KEY_KP_2        = 90,
            KEY_KP_3        = 91,
            KEY_KP_4        = 92,
            KEY_KP_5        = 93,
            KEY_KP_6        = 94,
            KEY_KP_7        = 95,
            KEY_KP_8        = 96,
            KEY_KP_9        = 97,
            KEY_KP_0        = 98,
            KEY_KP_PERIOD   = 99,

            KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                                *   keyboards have over ANSI ones,
                                                *   located between left shift and Y.
                                                *   Produces GRAVE ACCENT and TILDE in a
                                                *   US or UK Mac layout, REVERSE SOLIDUS
                                                *   (backslash) and VERTICAL LINE in a
                                                *   US or UK Windows layout, and
                                                *   LESS-THAN SIGN and GREATER-THAN SIGN
                                                *   in a Swiss German, German, or French
                                                *   layout. */
            KEY_APPLICATION = 101, /**< windows contextual menu, compose */
            KEY_POWER       = 102, /**< The USB document says this is a status flag,
                                    *   not a physical key - but some Mac keyboards
                                    *   do have a power key. */

            KEY_KP_EQUALS  = 103,
            KEY_F13        = 104,
            KEY_F14        = 105,
            KEY_F15        = 106,
            KEY_F16        = 107,
            KEY_F17        = 108,
            KEY_F18        = 109,
            KEY_F19        = 110,
            KEY_F20        = 111,
            KEY_F21        = 112,
            KEY_F22        = 113,
            KEY_F23        = 114,
            KEY_F24        = 115,
            KEY_EXECUTE    = 116,
            KEY_HELP       = 117,
            KEY_MENU       = 118,
            KEY_SELECT     = 119,
            KEY_STOP       = 120,
            KEY_AGAIN      = 121, /**< redo */
            KEY_UNDO       = 122,
            KEY_CUT        = 123,
            KEY_COPY       = 124,
            KEY_PASTE      = 125,
            KEY_FIND       = 126,
            KEY_MUTE       = 127,
            KEY_VOLUMEUP   = 128,
            KEY_VOLUMEDOWN = 129,
            /* not sure whether there's a reason to enable these */
            /*     KEY_LOCKINGCAPSLOCK = 130,  */
            /*     KEY_LOCKINGNUMLOCK = 131, */
            /*     KEY_LOCKINGSCROLLLOCK = 132, */
            KEY_KP_COMMA   = 133,
            KEY_KP_EQUALSAS400 = 134,

            KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see footnotes in USB doc */
            KEY_INTERNATIONAL2 = 136,
            KEY_INTERNATIONAL3 = 137, /**< Yen */
            KEY_INTERNATIONAL4 = 138,
            KEY_INTERNATIONAL5 = 139,
            KEY_INTERNATIONAL6 = 140,
            KEY_INTERNATIONAL7 = 141,
            KEY_INTERNATIONAL8 = 142,
            KEY_INTERNATIONAL9 = 143,

            KEY_LANG1 = 144, /**< Hangul/English toggle */
            KEY_LANG2 = 145, /**< Hanja conversion */
            KEY_LANG3 = 146, /**< Katakana */
            KEY_LANG4 = 147, /**< Hiragana */
            KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
            KEY_LANG6 = 149, /**< reserved */
            KEY_LANG7 = 150, /**< reserved */
            KEY_LANG8 = 151, /**< reserved */
            KEY_LANG9 = 152, /**< reserved */

            KEY_ALTERASE   = 153, /**< Erase-Eaze */
            KEY_SYSREQ     = 154,
            KEY_CANCEL     = 155,
            KEY_CLEAR      = 156,
            KEY_PRIOR      = 157,
            KEY_RETURN2    = 158,
            KEY_SEPARATOR  = 159,
            KEY_OUT        = 160,
            KEY_OPER       = 161,
            KEY_CLEARAGAIN = 162,
            KEY_CRSEL      = 163,
            KEY_EXSEL      = 164,

            KEY_KP_00             = 176,
            KEY_KP_000            = 177,
            KEY_THOUSANDSSEPARATOR= 178,
            KEY_DECIMALSEPARATOR  = 179,
            KEY_CURRENCYUNIT      = 180,
            KEY_CURRENCYSUBUNIT   = 181,
            KEY_KP_LEFTPAREN      = 182,
            KEY_KP_RIGHTPAREN     = 183,
            KEY_KP_LEFTBRACE      = 184,
            KEY_KP_RIGHTBRACE     = 185,
            KEY_KP_TAB            = 186,
            KEY_KP_BACKSPACE      = 187,
            KEY_KP_A              = 188,
            KEY_KP_B              = 189,
            KEY_KP_C              = 190,
            KEY_KP_D              = 191,
            KEY_KP_E              = 192,
            KEY_KP_F              = 193,
            KEY_KP_XOR            = 194,
            KEY_KP_POWER          = 195,
            KEY_KP_PERCENT        = 196,
            KEY_KP_LESS           = 197,
            KEY_KP_GREATER        = 198,
            KEY_KP_AMPERSAND      = 199,
            KEY_KP_DBLAMPERSAND   = 200,
            KEY_KP_VERTICALBAR    = 201,
            KEY_KP_DBLVERTICALBAR = 202,
            KEY_KP_COLON          = 203,
            KEY_KP_HASH           = 204,
            KEY_KP_SPACE          = 205,
            KEY_KP_AT             = 206,
            KEY_KP_EXCLAM         = 207,
            KEY_KP_MEMSTORE       = 208,
            KEY_KP_MEMRECALL      = 209,
            KEY_KP_MEMCLEAR       = 210,
            KEY_KP_MEMADD         = 211,
            KEY_KP_MEMSUBTRACT    = 212,
            KEY_KP_MEMMULTIPLY    = 213,
            KEY_KP_MEMDIVIDE      = 214,
            KEY_KP_PLUSMINUS      = 215,
            KEY_KP_CLEAR          = 216,
            KEY_KP_CLEARENTRY     = 217,
            KEY_KP_BINARY         = 218,
            KEY_KP_OCTAL          = 219,
            KEY_KP_DECIMAL        = 220,
            KEY_KP_HEXADECIMAL    = 221,

            KEY_LCTRL  = 224,
            KEY_LSHIFT = 225,
            KEY_LALT   = 226, /**< alt, option */
            KEY_LSUPER = 227, /**< windows, command (apple), meta */
            KEY_RCTRL  = 228,
            KEY_RSHIFT = 229,
            KEY_RALT   = 230, /**< alt gr, option */
            KEY_RSUPER = 231, /**< windows, command (apple), meta */

            KEY_MODE   = 257,  /**< I'm not sure if this is really not covered
                                *   by any of the above, but since there's a
                                *   special KMOD_MODE for it I'm adding it here
                                */
            /*@}*//*Usage page 0x07*/

             /**
             *  \name Usage page 0x0C
             *
             *  These values are mapped from usage page 0x0C (USB consumer page).
             */
            /*@{*/

            KEY_AUDIONEXT    = 258,
            KEY_AUDIOPREV    = 259,
            KEY_AUDIOSTOP    = 260,
            KEY_AUDIOPLAY    = 261,
            KEY_AUDIOMUTE    = 262,
            KEY_MEDIASELECT  = 263,
            KEY_WWW          = 264,
            KEY_MAIL         = 265,
            KEY_CALCULATOR   = 266,
            KEY_COMPUTER     = 267,
            KEY_AC_SEARCH    = 268,
            KEY_AC_HOME      = 269,
            KEY_AC_BACK      = 270,
            KEY_AC_FORWARD   = 271,
            KEY_AC_STOP      = 272,
            KEY_AC_REFRESH   = 273,
            KEY_AC_BOOKMARKS = 274,

            /*@}*//*Usage page 0x0C*/

            /**
             *  \name Walther keys
             *
             *  These are values that Christian Walther added (for mac keyboard?).
             */
            /*@{*/

            KEY_BRIGHTNESSDOWN = 275,
            KEY_BRIGHTNESSUP   = 276,
            KEY_DISPLAYSWITCH  = 277, /**< display mirroring/dual display switch, video mode switch */
            KEY_KBDILLUMTOGGLE = 278,
            KEY_KBDILLUMDOWN   = 279,
            KEY_KBDILLUMUP     = 280,
            KEY_EJECT          = 281,
            KEY_SLEEP          = 282,
            KEY_APP1           = 283,
            KEY_APP2           = 284,

            /*@}*//*Walther Keys*/
            // Copying from SDL ends here

            KEY_LAST            = 285,      ///< The last Keyboard InputCode, all Keys values will be less than this, and all Events will be larger than that
            KEY_RESERVED_MAX    = 299,

            MOUSE_FIRST         = 300,      ///< The First Mouse event, all Mouse Event values will be more than this.

            MOUSEBUTTON_FIRST   = 301,               ///< This is the lowest mouse button value, all mice values will be larger of equal to this.
            MOUSEBUTTON_1       = MOUSEBUTTON_FIRST, ///< Most commonly left click.
            MOUSEBUTTON_2,                           ///< Most commonly right click.
            MOUSEBUTTON_3,                           ///< Most commonly middle click.
            MOUSEBUTTON_4,
            MOUSEBUTTON_5,
            MOUSEBUTTON_6,
            MOUSEBUTTON_7,
            MOUSEBUTTON_8,
            MOUSEBUTTON_9,
            MOUSEBUTTON_10,
            MOUSEBUTTON_11,
            MOUSEBUTTON_12,
            MOUSEBUTTON_13,
            MOUSEBUTTON_14,
            MOUSEBUTTON_15,
            MOUSEBUTTON_16,
            MOUSEBUTTON_17,
            MOUSEBUTTON_18,
            MOUSEBUTTON_19,
            MOUSEBUTTON_20,
            MOUSEBUTTON_LAST,      ///< The Last mouse button event, all mouse button event will be lower or equal to this.

            MOUSEMOTION_FIRST       = 330,
            MOUSEABSOLUTEVERTICAL   = MOUSEMOTION_FIRST,
            MOUSEABSOLUTEHORIZONTAL,
            MOUSEVERTICAL,
            MOUSEHORIZONTAL,
            MOUSEWHEELVERTICAL,
            MOUSEWHEELHORIZONTAL,
            MOUSEMOTION_LAST,

            MOUSE_LAST,            ///< The last MouseEvent Code, all Mouse events will be less than this.

            MOTION_FIRST            = 400,  ///< The first Motion event.
            MOTION_LAST,                    ///< The last Motion event.

            MULTITOUCH_FIRST        = 450,  ///< The first Multi Touch event.
            MULTITOUCH_ACTION,
            MULTITOUCH_GESTURE,
            MULTITOUCH_PINCH,
            MULTITOUCH_STRETCH,
            MULTITOUCH_LAST,                ///< The last Multi Touch event.

            JOYSTICK_FIRST        = 500,  ///< The First JoyStick event, all Joystick Event values will be more than this.

            JOYSTICKBUTTON_FIRST  = 501,  ///< This is the lowest Joystick button value, all joystick button values will be larger or equal to this.
            JOYSTICKBUTTON_1      = JOYSTICKBUTTON_FIRST,
            JOYSTICKBUTTON_2,
            JOYSTICKBUTTON_3,
            JOYSTICKBUTTON_4,
            JOYSTICKBUTTON_5,
            JOYSTICKBUTTON_6,
            JOYSTICKBUTTON_7,
            JOYSTICKBUTTON_8,
            JOYSTICKBUTTON_9,
            JOYSTICKBUTTON_10,
            JOYSTICKBUTTON_11,
            JOYSTICKBUTTON_12,
            JOYSTICKBUTTON_13,
            JOYSTICKBUTTON_14,
            JOYSTICKBUTTON_15,
            JOYSTICKBUTTON_16,
            JOYSTICKBUTTON_17,
            JOYSTICKBUTTON_18,
            JOYSTICKBUTTON_19,
            JOYSTICKBUTTON_20,
            JOYSTICKBUTTON_LAST,  ///< The Last Joystick button event, all Joystick button event will be lower or equal to this.

            JOYSTICKAXIS_FIRST    = 530,  ///< This is the lowest Joystick axis value, all joystick values will be larger of equal to this.
            JOYSTICKAXIS_1        = JOYSTICKAXIS_FIRST,
            JOYSTICKAXIS_2,
            JOYSTICKAXIS_3,
            JOYSTICKAXIS_4,
            JOYSTICKAXIS_5,
            JOYSTICKAXIS_6,
            JOYSTICKAXIS_7,
            JOYSTICKAXIS_8,
            JOYSTICKAXIS_9,
            JOYSTICKAXIS_10,
            JOYSTICKAXIS_11,
            JOYSTICKAXIS_12,
            JOYSTICKAXIS_13,
            JOYSTICKAXIS_14,
            JOYSTICKAXIS_15,
            JOYSTICKAXIS_16,
            JOYSTICKAXIS_17,
            JOYSTICKAXIS_18,
            JOYSTICKAXIS_19,
            JOYSTICKAXIS_20,
            JOYSTICKAXIS_LAST,  ///< The last Joystick axis event, all joystick axis event will be lower or equal to this.

            JOYSTICKBALL_FIRST        = 560,
            JOYSTICKBALL_1_VERTICAL   = JOYSTICKBALL_FIRST,
            JOYSTICKBALL_1_HORIZONTAL,
            JOYSTICKBALL_2_VERTICAL,
            JOYSTICKBALL_2_HORIZONTAL,
            JOYSTICKBALL_LAST,

            JOYSTICKHAT_FIRST     = 570,
            JOYSTICKHAT_1         = JOYSTICKHAT_FIRST,
            JOYSTICKHAT_2,
            JOYSTICKHAT_3,
            JOYSTICKHAT_4,
            JOYSTICKHAT_5,
            JOYSTICKHAT_LAST,

            JOYSTICK_LAST,  ///< The last Joystick Input Code, all Joystick events will be less than this.

            CONTROLLER_FIRST        = 600,  ///< The First Controller event, all Controller Event values will be more than this.

            CONTROLLERBUTTON_FIRST  = 601,
            CONTROLLERBUTTON_A      = CONTROLLERBUTTON_FIRST,
            CONTROLLERBUTTON_B,
            CONTROLLERBUTTON_X,
            CONTROLLERBUTTON_Y,
            CONTROLLERBUTTON_BACK,
            CONTROLLERBUTTON_GUIDE,
            CONTROLLERBUTTON_START,
            CONTROLLERBUTTON_LEFTSTICK,
            CONTROLLERBUTTON_RIGHTSTICK,
            CONTROLLERBUTTON_LEFTSHOULDER,
            CONTROLLERBUTTON_RIGHTSHOULDER,
            CONTROLLERBUTTON_DPAD_UP,
            CONTROLLERBUTTON_DPAD_DOWN,
            CONTROLLERBUTTON_DPAD_LEFT,
            CONTROLLERBUTTON_DPAD_RIGHT,
            CONTROLLERBUTTON_RESERVED_1,
            CONTROLLERBUTTON_RESERVED_2,
            CONTROLLERBUTTON_RESERVED_3,
            CONTROLLERBUTTON_RESERVED_4,
            CONTROLLERBUTTON_RESERVED_5,
            CONTROLLERBUTTON_LAST,

            CONTROLLERAXIS_FIRST    = 630,  ///< This is the lowest Controller axis value, all Controller values will be larger of equal to this.
            CONTROLLERAXIS_LEFTX    = CONTROLLERAXIS_FIRST,
            CONTROLLERAXIS_LEFTY,
            CONTROLLERAXIS_RIGHTX,
            CONTROLLERAXIS_RIGHTY,
            CONTROLLERAXIS_TRIGGERLEFT,
            CONTROLLERAXIS_TRIGGERRIGHT,
            CONTROLLERAXIS_RESERVED_1,
            CONTROLLERAXIS_RESERVED_2,
            CONTROLLERAXIS_RESERVED_3,
            CONTROLLERAXIS_RESERVED_4,
            CONTROLLERAXIS_LAST,  ///< The last Controller axis event, all Controller axis event will be lower than this.

            CONTROLLER_LAST,  ///< The last Controller Input Code, all Controller events will be less than this.

            // Compound Inputs
            COMPOUNDINPUT_FIRST                 = 700,

            COMPOUNDINPUT_MOUSEMULTICLICK_FIRST = 701,
            COMPOUNDINPUT_MOUSELEFTMULTICLICK   = COMPOUNDINPUT_MOUSEMULTICLICK_FIRST,
            COMPOUNDINPUT_MOUSERIGHTMULTICLICK,
            COMPOUNDINPUT_MOUSEMULTICLICK_LAST,
            COMPOUNDINPUT_CUSTOMSEQUENCE,

            COMPOUNDINPUT_LAST,

            OSTEXTINPUT             = 900,   /**  This represents text as detected by the OS as it is supposed to be presented when printed.
                                              *   OS text events are useful for non-english languages where a keystroke can represent multiple characters,
                                              *   or multiple keystrokes can represent a single character.  A MetaCode with this type of input code will
                                              *   always represent only one character, but the event that creates it may make multiple MetaCodes.
                                              */
        };//InputCode

        /// @brief An enum listing containing the different kind of input devices, useful for some query functions.
        enum InputDevice
        {
            DEVICE_UNKNOWN    = 0,
            DEVICE_KEYBOARD   = 1,
            DEVICE_MOUSE      = 2,
            DEVICE_JOYSTICK   = 3,
            DEVICE_CONTROLLER = 4,
            DEVICE_MULTITOUCH = 5
        };//InputDevice

        /// @brief An Optional listing of value that can be used in a metacode to represent the information of a button press
        /// @details This is optional set of values that can make working with buttons easier. The values the engine pass
        /// via the the event manager will all use these where ever appropriate.
        enum ButtonState
        {
            BUTTON_LIFTING = 1,     /**< Used when the key stops being pressed. */
            BUTTON_UP = 2,          /**< The default state of a key. */
            BUTTON_PRESSING = 4,    /**< This is used at the exact point in time that a key goes from unpressed to pressed. */
            BUTTON_DOWN = 8         /**< This is used the entire time a key is down.  */
        };//ButtonState

        /// @brief A convenience enum that stores the maximum number of states for various input devices.
        /// @details This is intended to be used with containers wanting to store the maximum number of
        /// states for each input type on a given device.
        enum DeviceMaxStates
        {
            KEYBOARD_BUTTONMAX = Input::KEY_LAST - Input::KEY_FIRST,
            MOUSE_BUTTONMAX = Input::MOUSEBUTTON_LAST - Input::MOUSEBUTTON_FIRST,
            CONTROLLER_BUTTONMAX = Input::CONTROLLERBUTTON_LAST - Input::CONTROLLERBUTTON_FIRST,
            CONTROLLER_AXISMAX = Input::CONTROLLERAXIS_LAST - Input::CONTROLLERAXIS_FIRST,
            JOYSTICK_BUTTONMAX = Input::JOYSTICKBUTTON_LAST - Input::JOYSTICKBUTTON_FIRST,
            JOYSTICK_AXISMAX = Input::JOYSTICKAXIS_LAST - Input::JOYSTICKAXIS_FIRST,
            JOYSTICK_TRACKBALLMAX = Input::JOYSTICKBALL_LAST - Input::JOYSTICKBALL_FIRST,
            JOYSTICK_HATMAX = Input::JOYSTICKHAT_LAST - Input::JOYSTICKHAT_FIRST
        };//DeviceMaxStates

        /// @brief An Optional listing of values that can be used in a metacode Indicate spin, digital or binary travel in a directional
        /// @details This is optional set of values that can make working with the MouseWheel and other items that can be moved along 1 axis in two directions easier. The values the engine pass
        /// via the the event manager will all use these where ever appropriate.
        enum DirectionalMotionState
        {
            DIRECTIONALMOTION_UPLEFT = 1,          /**< Optionally Used when the device is spun/moved up or to the left as a MetaValue */
            DIRECTIONALMOTION_UNCHANGED = 0,       /**< This really isn't used in normal situations, but if a mouse wheel or joyhat event is ever needed when the device is unmoved */
            DIRECTIONALMOTION_DOWNRIGHT = -1       /**< Optionally Used when the MouseWheel/Joyhat is spun Down/Right as a Meta Code */
        };//DirectionalMotionState

        /// @brief An optional listing of the possible states a hat on a Joystick can take.
        /// @details This is optional set of values that can make working with Joystick hats easier. The values the engine pass
        /// via the the event manager will all use these where ever appropriate.
        enum HatState
        {
            JOYSTICKHAT_CENTERED  = 0,
            JOYSTICKHAT_UP        = 1,
            JOYSTICKHAT_RIGHT     = 2,
            JOYSTICKHAT_DOWN      = 4,
            JOYSTICKHAT_LEFT      = 8,
            JOYSTICKHAT_RIGHTUP   = ( JOYSTICKHAT_RIGHT | JOYSTICKHAT_UP ),
            JOYSTICKHAT_RIGHTDOWN = ( JOYSTICKHAT_RIGHT | JOYSTICKHAT_DOWN ),
            JOYSTICKHAT_LEFTUP    = ( JOYSTICKHAT_LEFT | JOYSTICKHAT_UP ),
            JOYSTICKHAT_LEFTDOWN  = ( JOYSTICKHAT_LEFT | JOYSTICKHAT_DOWN )
        };//JoystickHatState
    }//Input
}//Mezzanine

#endif
