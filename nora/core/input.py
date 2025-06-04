from enum import Enum
from typing import Tuple


class Key(Enum):
    """
    Enumeration of keyboard keys used for input detection.
    Corresponds to GLFW key codes.
    """
    Unknown = -1
    Space = 32
    Apostrophe = 39
    Comma = 44
    Minus = 45
    Period = 46
    Slash = 47
    Num0 = 48
    Num1 = 49
    Num2 = 50
    Num3 = 51
    Num4 = 52
    Num5 = 53
    Num6 = 54
    Num7 = 55
    Num8 = 56
    Num9 = 57
    A = 65
    B = 66
    C = 67
    D = 68
    E = 69
    F = 70
    G = 71
    H = 72
    I = 73
    J = 74
    K = 75
    L = 76
    M = 77
    N = 78
    O = 79
    P = 80
    Q = 81
    R = 82
    S = 83
    T = 84
    U = 85
    V = 86
    W = 87
    X = 88
    Y = 89
    Z = 90
    Escape = 256
    Enter = 257
    Tab = 258
    Backspace = 259
    Insert = 260
    Delete = 261
    Right = 262
    Left = 263
    Down = 264
    Up = 265


class MouseCode(Enum):
    Button1 = 0
    Button2 = 1
    Button3 = 2
    Button4 = 3
    Button5 = 4
    Button6 = 5
    Button7 = 6
    Button8 = 7
    Left = 0
    Right = 1
    Middle = 2


class Input:
    """
    Input handling class that provides static methods to query user input.
    """
    mouse_position: Tuple[float, float]
    mouse_delta: Tuple[float, float]
    scroll_delta: Tuple[float, float]

    @staticmethod
    def is_key_pressed(key: Key) -> bool:
        """
        Checks if the specified key is currently pressed.

        :param key: A value from the Key enum
        :return: True if the key is pressed, False otherwise
        """

    @staticmethod
    def is_just_pressed(key: Key) -> bool:
        """
        Check if the specified key is just pressed.

        :param key: A value from the Key enum
        :return: True if the key is just pressed, False otherwise
        """

    @staticmethod
    def is_just_released(key: Key) -> bool:
        """
        Check if the specified key is just released.
        :parem key: A value from the key enum
        :return: True if the key is just released, False otherwise
        """

    @staticmethod
    def is_mouse_button_pressed(mouse_code: MouseCode) -> bool: ...

    @staticmethod
    def is_mouse_button_just_pressed(mouse_code: MouseCode) -> bool: ...

    @staticmethod
    def is_mouse_button_just_released(mouse_code: MouseCode) -> bool: ...
