//© Copyright 2010 Joseph Toppi and John Blackwood
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef PHYSEVENTUSERINPUT_H
#define PHYSEVENTUSERINPUT_H
///////////////////////////////////////////////////////////////////////////////
// This will expose all keyboard and mouse, joystick and other userinput events
// to developers, we are using the SDL keymap to get us started, large portions
// are directly copy/pasta'd, so we included their license too
///////////////////////////////////////

//From SDL
/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#include <vector>
#include "physevent.h"
#include "physdatatypes.h"

using namespace std;




///////////////////////////////////////////////////////////////////////////////
/// @class MetaCode
/// @headerfile physeventuserinput.h
/// @brief This Determines the kind of user input.
/// @details A Metacode contains the data that is passed around with an input
/// event. It stores one type of button press or analog representation (Mouse
/// move, joystick tilt, wheel spin, etc...). If it is an analog representation
/// it will also store how far or how it is pushed, pressed, rotated, or
/// whatever. Several of these can be used in combination to represent button
/// combinations, or complex input combination (like portions of fighter game
/// moves).
/// The first 127 character line up with Ascii, Currently upper lase are
/// omitted for brevity.
///////////////////////////////////////
class MetaCode
{
    public:
    /// @enum InputCode
    /// @brief The InputCode enum defines all the posible types of inputs.
    /// @details It has one entry for each key on a most keyboards. Then it
    /// has an entry for most mouse and joystick input methods.
    enum InputCode{
        KEY_UNKNOWN			= 0,    ///KEY_UNKNOWN This is used for unsupported keys or keys that are not in Unicode.
        KEY_FIRST			= 0,    ///KEY_FIRST Same Value as KEY_UNKOWN, is Guaranteed to be the lowest value of any key.
        KEY_BACKSPACE		= 8,
        KEY_TAB				= 9,
        KEY_CLEAR			= 12,
        KEY_RETURN			= 13,
        KEY_PAUSE			= 19,
        KEY_ESCAPE			= 27,
        KEY_SPACE			= 32,
        KEY_EXCLAIM			= 33,
        KEY_QUOTEDBL		= 34,
        KEY_HASH			= 35,
        KEY_DOLLAR			= 36,
        KEY_AMPERSAND		= 38,
        KEY_QUOTE			= 39,
        KEY_LEFTPAREN		= 40,
        KEY_RIGHTPAREN		= 41,
        KEY_ASTERISK		= 42,
        KEY_PLUS			= 43,
        KEY_COMMA			= 44,
        KEY_MINUS			= 45,
        KEY_PERIOD			= 46,
        KEY_SLASH			= 47,
        KEY_0				= 48,
        KEY_1				= 49,
        KEY_2				= 50,
        KEY_3				= 51,
        KEY_4				= 52,
        KEY_5				= 53,
        KEY_6				= 54,
        KEY_7				= 55,
        KEY_8				= 56,
        KEY_9				= 57,
        KEY_COLON			= 58,
        KEY_SEMICOLON		= 59,
        KEY_LESS			= 60,
        KEY_EQUALS			= 61,
        KEY_GREATER			= 62,
        KEY_QUESTION		= 63,
        KEY_AT				= 64,
        /*
           Skip uppercase letters
         */
        KEY_LEFTBRACKET		= 91,
        KEY_BACKSLASH		= 92,
        KEY_RIGHTBRACKET	= 93,
        KEY_CARET			= 94,
        KEY_UNDERSCORE		= 95,
        KEY_BACKQUOTE		= 96,
        KEY_a				= 97,
        KEY_b				= 98,
        KEY_c				= 99,
        KEY_d				= 100,
        KEY_e				= 101,
        KEY_f				= 102,
        KEY_g				= 103,
        KEY_h				= 104,
        KEY_i				= 105,
        KEY_j				= 106,
        KEY_k				= 107,
        KEY_l				= 108,
        KEY_m				= 109,
        KEY_n				= 110,
        KEY_o				= 111,
        KEY_p				= 112,
        KEY_q				= 113,
        KEY_r				= 114,
        KEY_s				= 115,
        KEY_t				= 116,
        KEY_u				= 117,
        KEY_v				= 118,
        KEY_w				= 119,
        KEY_x				= 120,
        KEY_y				= 121,
        KEY_z				= 122,
        KEY_DELETE			= 127,
        /* End of ASCII mapped keysyms */
        /* International keyboard syms */
        KEY_WORLD_0			= 160,		/* 0xA0 */
        KEY_WORLD_1			= 161,
        KEY_WORLD_2			= 162,
        KEY_WORLD_3			= 163,
        KEY_WORLD_4			= 164,
        KEY_WORLD_5			= 165,
        KEY_WORLD_6			= 166,
        KEY_WORLD_7			= 167,
        KEY_WORLD_8			= 168,
        KEY_WORLD_9			= 169,
        KEY_WORLD_10		= 170,
        KEY_WORLD_11		= 171,
        KEY_WORLD_12		= 172,
        KEY_WORLD_13		= 173,
        KEY_WORLD_14		= 174,
        KEY_WORLD_15		= 175,
        KEY_WORLD_16		= 176,
        KEY_WORLD_17		= 177,
        KEY_WORLD_18		= 178,
        KEY_WORLD_19		= 179,
        KEY_WORLD_20		= 180,
        KEY_WORLD_21		= 181,
        KEY_WORLD_22		= 182,
        KEY_WORLD_23		= 183,
        KEY_WORLD_24		= 184,
        KEY_WORLD_25		= 185,
        KEY_WORLD_26		= 186,
        KEY_WORLD_27		= 187,
        KEY_WORLD_28		= 188,
        KEY_WORLD_29		= 189,
        KEY_WORLD_30		= 190,
        KEY_WORLD_31		= 191,
        KEY_WORLD_32		= 192,
        KEY_WORLD_33		= 193,
        KEY_WORLD_34		= 194,
        KEY_WORLD_35		= 195,
        KEY_WORLD_36		= 196,
        KEY_WORLD_37		= 197,
        KEY_WORLD_38		= 198,
        KEY_WORLD_39		= 199,
        KEY_WORLD_40		= 200,
        KEY_WORLD_41		= 201,
        KEY_WORLD_42		= 202,
        KEY_WORLD_43		= 203,
        KEY_WORLD_44		= 204,
        KEY_WORLD_45		= 205,
        KEY_WORLD_46		= 206,
        KEY_WORLD_47		= 207,
        KEY_WORLD_48		= 208,
        KEY_WORLD_49		= 209,
        KEY_WORLD_50		= 210,
        KEY_WORLD_51		= 211,
        KEY_WORLD_52		= 212,
        KEY_WORLD_53		= 213,
        KEY_WORLD_54		= 214,
        KEY_WORLD_55		= 215,
        KEY_WORLD_56		= 216,
        KEY_WORLD_57		= 217,
        KEY_WORLD_58		= 218,
        KEY_WORLD_59		= 219,
        KEY_WORLD_60		= 220,
        KEY_WORLD_61		= 221,
        KEY_WORLD_62		= 222,
        KEY_WORLD_63		= 223,
        KEY_WORLD_64		= 224,
        KEY_WORLD_65		= 225,
        KEY_WORLD_66		= 226,
        KEY_WORLD_67		= 227,
        KEY_WORLD_68		= 228,
        KEY_WORLD_69		= 229,
        KEY_WORLD_70		= 230,
        KEY_WORLD_71		= 231,
        KEY_WORLD_72		= 232,
        KEY_WORLD_73		= 233,
        KEY_WORLD_74		= 234,
        KEY_WORLD_75		= 235,
        KEY_WORLD_76		= 236,
        KEY_WORLD_77		= 237,
        KEY_WORLD_78		= 238,
        KEY_WORLD_79		= 239,
        KEY_WORLD_80		= 240,
        KEY_WORLD_81		= 241,
        KEY_WORLD_82		= 242,
        KEY_WORLD_83		= 243,
        KEY_WORLD_84		= 244,
        KEY_WORLD_85		= 245,
        KEY_WORLD_86		= 246,
        KEY_WORLD_87		= 247,
        KEY_WORLD_88		= 248,
        KEY_WORLD_89		= 249,
        KEY_WORLD_90		= 250,
        KEY_WORLD_91		= 251,
        KEY_WORLD_92		= 252,
        KEY_WORLD_93		= 253,
        KEY_WORLD_94		= 254,
        KEY_WORLD_95		= 255,		/* 0xFF */

        /* @name Numeric keypad */
        KEY_KP0				= 256,
        KEY_KP1				= 257,
        KEY_KP2				= 258,
        KEY_KP3				= 259,
        KEY_KP4				= 260,
        KEY_KP5				= 261,
        KEY_KP6				= 262,
        KEY_KP7				= 263,
        KEY_KP8				= 264,
        KEY_KP9				= 265,
        KEY_KP_PERIOD		= 266,
        KEY_KP_DIVIDE		= 267,
        KEY_KP_MULTIPLY		= 268,
        KEY_KP_MINUS		= 269,
        KEY_KP_PLUS			= 270,
        KEY_KP_ENTER		= 271,
        KEY_KP_EQUALS		= 272,

        /* @name Arrows + Home/End pad */
        KEY_UP				= 273,
        KEY_DOWN			= 274,
        KEY_RIGHT			= 275,
        KEY_LEFT			= 276,
        KEY_INSERT			= 277,
        KEY_HOME			= 278,
        KEY_END				= 279,
        KEY_PAGEUP			= 280,
        KEY_PAGEDOWN		= 281,

        /* Function keys */
        KEY_F1				= 282,
        KEY_F2				= 283,
        KEY_F3				= 284,
        KEY_F4				= 285,
        KEY_F5				= 286,
        KEY_F6				= 287,
        KEY_F7				= 288,
        KEY_F8				= 289,
        KEY_F9				= 290,
        KEY_F10				= 291,
        KEY_F11				= 292,
        KEY_F12				= 293,
        KEY_F13				= 294,
        KEY_F14				= 295,
        KEY_F15				= 296,

        /* Key state modifier keys */
        KEY_NUMLOCK			= 300,
        KEY_CAPSLOCK		= 301,
        KEY_SCROLLOCK		= 302,
        KEY_RSHIFT			= 303,
        KEY_LSHIFT			= 304,
        KEY_RCTRL			= 305,
        KEY_LCTRL			= 306,
        KEY_RALT			= 307,
        KEY_LALT			= 308,
        KEY_RMETA			= 309,
        KEY_LMETA			= 310,
        KEY_LSUPER			= 311,		/**< Left "Windows" key */
        KEY_RSUPER			= 312,		/**< Right "Windows" key */
        KEY_MODE			= 313,		/**< "Alt Gr" key */
        KEY_COMPOSE			= 314,		/**< Multi-key compose key */

        /* Miscellaneous function keys */
        KEY_HELP			= 315,
        KEY_PRINT			= 316,
        KEY_SYSREQ			= 317,
        KEY_BREAK			= 318,
        KEY_MENU			= 319,
        KEY_POWER			= 320,		/**< Power Macintosh power key */
        KEY_EURO			= 321,		/**< Some European keyboards */
        KEY_UNDO			= 322,		/**< Atari keyboard has Undo */

        //From here down they do not match directly anymore
        KEYMOD_NONE  	= 323,
        KEYMOD_LSHIFT	= 324,
        KEYMOD_RSHIFT	= 325,
        KEYMOD_LCTRL 	= 326,
        KEYMOD_RCTRL 	= 327,
        KEYMOD_LALT  	= 328,
        KEYMOD_RALT 	= 329,
        KEYMOD_LMETA 	= 330,
        KEYMOD_RMETA 	= 331,
        KEYMOD_NUM   	= 332,
        KEYMOD_CAPS  	= 333,
        KEYMOD_MODE  	= 334,
        KEYMOD_RESERVED = 335,

        KEY_LAST        = 479,  /// The last KeyCode, all Keys values will be less than this, and all Events will be larger than that

        INPUTEVENT_FIRST = 480, /// The First non-event, all Keys values will be more than this

        MOUSE_FIRST             = 480,  /// The First Mouse event, all Mouse Event values will be more than this
        MOUSEBUTTON             = 481,
        MOUSEABSOLUTEVERTICAL   = 482,
        MOUSEABSOLUTEHORIZONTAL = 483,
        MOUSEVERTICAL           = 484,
        MOUSEHORIZONTAL         = 485,
        MOUSEWHEELVERTICAL      = 485,
        MOUSEWHEELHORIZONTAL    = 489,
        MOUSE_LAST              = 490,  /// The last MouseEvent Code, all Mouse events will be less than this

        JOYSTICK_FIRST          = 499,  /// The First JoyStick event, all Joystick Event values will be more than this
        JOYSTICKBUTTON          = 500,
        JOYSTICKMOTIONAXIS      = 501,
        JOYSTICKBALLVERTICAL    = 502,
        JOYSTICKBALLHORIZONTAL  = 503,
        JOYSTICKHATVERTICAL     = 504,
        JOYSTICKHATHORIZONTAL   = 505,
        JOYSTICK_LAST           = 506,  /// The last JoyStick Event Code, all JoyStick events will be less than this.
        INPUTEVENT_LAST = 512           /// The last Event Code, all event codes will be less than this.
    };

    /// @enum ButtonState
    /// @brief An Optional listing of value that can be used in a metacode to represent the information of a button press
    /// @details This is optional set of values that can make working with buttons easier. The values the engine pass
    /// via the the event manager will all use these whereever appropriate.
    enum ButtonState{
        BUTTON_LIFTING = -1,    /**< Used when the key stops being pressed. */
        BUTTON_UP = 0,          /**< The default state of a key. */
        BUTTON_PRESSING = 1,    /**< This is used at the exact point in time that a key goes from unpressed to pressed. */
        BUTTON_DOWN = 2         /**< This is used the entire time a key is down.  */
    };

    private:
        int MetaValue;
        short unsigned int ID;
        MetaCode::InputCode Code;
        void Construct(const RawEvent &RawEvent_);
        void Construct(const int &MetaValue_, const short unsigned int &ID_, const MetaCode::InputCode &Code_);

        static MetaCode::InputCode GetInputCodeFromSDL_KEY(const RawEvent &RawEvent_);

    public:
        /// @brief Default constructor
        /// @details This sets nothing on the MetaCode and leaves it completely unassigned. Accessing a data member could cause problems
        MetaCode();

        /// @brief Descriptive Constructor
        /// @details This sets all values in the MetaCode, leaving it in completely ready state. This is the ideal constructor for simulating user input.
        /// @param MetaValue_ How much is something moving, tilting, rotating or whatever. For buttons a positive value is pushed, and a negative value is becoming unpressed, and 0 is unpressed.
        /// @param ID_ Which input is being activated. For everything except Keyboards codes, this selects which button, which joystick, which mouse which item.
        /// @param Code_ Which key or which type of input was pressed. Sqeaky, thinks this has partial unicode support.
        MetaCode(const int &MetaValue_, const short unsigned int &ID_, const MetaCode::InputCode &Code_);

        /// @brief The Heavy Lifting Consctructor
        /// @details This contructor accepts a RawEvent from the input event subsystem internal to the engine. This converts all the required information
        /// from the lower level format and store what is needed in the event that is created. This is used heavily by engine internals.
        /// @warning We recomend against using this Constructor, because the binary format of RawEvent could change if the input event SubSystem Changes. In
        /// that event you would have to recompile your application to get it working with a new version of physgame.
        MetaCode(const RawEvent &RawEvent_);

        /// @brief This Returns the Inputcode
        /// @details This Value can be use to determine what keyboard button has been pressed, or what specific kind of Joystick or mouse event has occurred.
        /// This value can be set with @ref SetCode .
        /// @return This returns the input code for this MetaCode
        MetaCode::InputCode GetCode() const;

        /// @brief This Sets The InputCode
        /// @details See @ref GetCode to see exactly what the Code is. This will Set the code stored in this MetaCode. This value can be retrieved with @ref GetCode .
        /// @param Code_ Teh value you want the stored code to become.
        void SetCode(const MetaCode::InputCode &Code_);

        /// @brief This Returns the MetaValue
        /// @details The MetaValue can be use to determine how far something is tilted, pushed, rotated, or other analog value.
        /// This value can be set with @ref SetMetaValue .
        /// @return This returns the input code for this MetaCode. For keyboard Buttons this will be
        /// 0 if not pressed, 1 if pressed, and -1 if it was pressed and just released. This could return any number inside a range (depending on hardware and configuration)
        /// to represent how tilted a joystick or how much a mouse moved.
        int GetMetaValue() const;                     //How much is being done? How far has the mouse moved, how much is the throttle pushed.

        /// @brief This Sets The MetaValue
        /// @details See @ref GetMetaValue to see exactly what the MetaValue is. This will set the MetaValue stored in this MetaCode. This value can be retrieved with @ref GetMetaValue .
        /// @param MetaValue_ Teh value you want the stored MetaValue to become. No bounds checking will be done. You can supply a completely invalid value if you choose to.
        void SetMetaValue(const int &MetaValue_);      //For button presses a positive value is pushed and a zero means unpushed

        /// @brief This Returns the Input ID
        /// @details The Input ID can be used to differentiate between which Joystick axis is being manipulated, or which mouse button is being pushed.
        /// On systems that support multiple keyboards this will even differentiate between those.
        /// This value can be set with @ref SetID .
        /// @return This returns the input ID, which (on a normal system) can help Identify which Mouse Button, Joystick Button, Joystick Axis,
        /// JoystickBall (Horizontal and Vertical), Joystick Hat Axis (those little joysticks on your joystick), but if the system can handle it this
        /// can identify from unique input sources and InputCode.
        short unsigned int GetID() const;             //Which input is doing it? If this the input was one of may, like which mouse button, which joystick axis.

        /// @brief This Sets The input ID
        /// @details See @ref GetID to see exactly what the input ID is. This will set the ID stored in this MetaCode. This value can be retrieved with @ref GetID .
        /// @param ID_ The value you want the stored MetaValue to become. No bounds checking will be done. You can supply a completely invalid value if you choose to.
        void SetID(const short unsigned int &ID_);

        /// @brief Compares two MetaCodes for equality
        /// @details This returns true if the MetaValue and Code are the Same, this ignores ID.
        bool operator==(const MetaCode &other) const;
};

/// @brief Allows for streaming of MetaCodes
/// @details If it can be streamed, then it can be logged" Holds true for the MetaCode.
std::ostream& operator << (std::ostream& stream, const MetaCode& x);


///////////////////////////////////////////////////////////////////////////////
/// @class PhysEventUserInput
/// @headerfile physeventuserinput.h
/// @brief This is a container for MetaCodes that is used in the physEventManager
/// @details The PhysEventUserInput is the container for information about how a
/// user enters data and commands into a program. By Default one is inserted into
/// event manager the with all the user input from the last run of the main loop.
/// These can be manually inserted into the EventManager to simulate input from
/// other sources. If setup properly this can allow computer controlled
/// characters to use the same interface players, allowing for more realistic
/// response from them. This is not limited to the tricks discussed here.
///////////////////////////////////////////////////////////////////////////////
class PhysEventUserInput : public PhysEvent
{
    private:
        vector<MetaCode> Code;

	public:
        /// @brief Default constructor
        /// @details This creates a perfectly functional, but empty physevent.
        PhysEventUserInput();
        PhysEventUserInput(const MetaCode &Code_);
        PhysEventUserInput(const vector<MetaCode> &Code_);
        virtual ~PhysEventUserInput();

        //code managment functions
        const MetaCode GetMetaCode(const unsigned int &Index);
        unsigned int GetMetaCodeCount();

        void AddCode(const MetaCode &Code_);
        void EraseCode(const MetaCode &Code_);
        void EraseCode(const unsigned int &Index);
        void ToggleCode(const MetaCode &Code_);
        void ToggleCode(const unsigned int &Index);

		virtual EventType getEventType();
};


#endif
