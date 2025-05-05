from enum import Enum
from typing import TypeVar, List


_T = TypeVar('_T')


class Time:
    """
    Provides timing information from the engine
    """

    delta_time: float
    """
    The elapsed time (in seconds) between the current frame and the previous frame.
    Typically used for frame-independant movement or updates.
    """

    fps: float
    """
    The number of frames per second calculated over the last second.
    Useful for performance monitoring and profiling.
    """
    

class Window:
    """
    Static interface to the engine's main application window.
    """

    background_color: Color
    scene: Scene

    @staticmethod
    def set_title(title: str) -> None:
        """
        Sets the window title to the specified string.

        :param title: The new title for the window
        """

    @staticmethod
    def get_title() -> str:
        """
        Gets the current window title.
        """

    @staticmethod
    def get_size() -> tuple[int, int]:
        """
        Returns the current window size as a (width, height) tuple.
        """

    @staticmethod
    def set_size(width: int, height: int) -> None:
        """
        Sets the window size to the specified dimensions.
        """

class Color:
    r: float
    g: float
    b: float
    alpha: float

    def __init__(self, r: float = 0.0, g: float = 0.0, b: float = 0.0, alpha: float = 1.0) -> None: ...


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


class Input:
    """
    Input handling class that provides static methods to query user input.
    """

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


class Component:
    """
    Base class for all components.
    """
    owner: 'Entity'

    def __init__(self) -> None: ...
    def start(self) -> None: ...
    def update(self) -> None: ...


class Camera(Component):
    """
    A basic camera component.
    """
    def __init__(self) -> None: ...


class Transform:
    """
    Represents the position, rotation, and scale of an entity.
    """
    # Assuming Transform has members like position, rotation, scale
    # You'll need to add the actual members based on your C++ Transform class
    ...

class Entity:
    """
    Represents an entity in the game world.
    Entities can have components attached to them.
    """
    transform: Transform

    def add_component(self, component: Component) -> None:
        """
        Adds a component to this entity.

        :param component: The component to add.
        """

    def get_component(self, type: type[_T]) -> _T | None:
        """
        Gets a component of the specified type attached to this entity.

        :param type: The type of the component to retrieve (e.g., Camera).
        :return: The component if found, otherwise None.
        """
    

class Scene:
    def __init__(self): ...

    def add_entity(entity: Entity) -> None: ...
    def get_root_entities() -> List[Entity]: ...
