from enum import Enum
from typing import TypeVar, List, Tuple
from abc import ABC


_T = TypeVar('_T')


class Debug:
    @staticmethod
    def info(message: str): ...

    @staticmethod
    def warning(message: str): ...

    @staticmethod
    def error(message: str): ...


class Vec3:
    """
    Represents a 3D vector.
    """
    x: float
    y: float
    z: float

    def __init__(self, x: float = 0.0, y: float = 0.0, z: float = 0.0) -> None: ...

    def __repr__(self) -> str: ...


class Mat4:
    """
    Represents a 4x4 matrix.
    """
    # Accessing elements might be tricky without further definition
    def __init__(self) -> None: ...

    def __repr__(self) -> str: ...
    def __getitem__(self, index: int) -> List[float]: ... # Assuming row access


class Transform:
    """
    Represents the position, rotation, and scale of an entity.
    """
    local_position: Vec3
    """The local position of the entity."""

    local_rotation: Vec3
    """The local rotation (Euler angles in degrees) of the entity."""

    local_scale: Vec3
    """The local scale of the entity."""

    global_position: Vec3
    """The global position of the entity in world space (read-only)."""

    model_matrix: Mat4
    """The combined model matrix representing the entity's transformation (read-only)."""

    right: Vec3
    """The right vector of the entity in world space (read-only)."""

    up: Vec3
    """The up vector of the entity in world space (read-only)."""

    backward: Vec3
    """The backward vector of the entity in world space (read-only)."""

    forward: Vec3
    """The forward vector of the entity in world space (read-only)."""

    global_scale: Vec3
    """The global scale of the entity (read-only)."""

    is_dirty: bool
    """Returns True if the transform has been modified since the last computation."""

    def __init__(self) -> None: ...
        


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


class Component:
    """
    Base class for all components.
    """
    owner: 'Entity'

    def __init__(self) -> None: ...
    def start(self) -> None: ...
    def update(self) -> None: ...
    def set_owner(self, entity: Entity) -> None: ...


class Camera(Component):
    """
    A basic camera component.
    """
    front: Vec3
    right: Vec3
    yaw: float
    pitch: float
    zoom: float

    def __init__(self) -> None: ...


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


class Texture:
    def __init__(self, path: str, flip_vertically: bool = False): ...


class Font:
    def __init__(self, font_path: str, font_size: int = 48): ...


class RenderComponent(ABC, Component):
    texture: Texture


class CuboidMesh(RenderComponent):
    def __init__(self): ...


class SphereMesh(RenderComponent):
    def __init__(self, sector_count: int = 36, stack_count: int = 18): ...


class CapsuleMesh(RenderComponent):
    def __init__(
        self, radius: float = 0.5, cylinder_height: float = 1.0, 
        sector_count: int = 36, hemisphere_stacks: int = 18, cylinder_stacks: int = 10
    ): ...


class Model(RenderComponent):
    def __init__(self, path: str = ""): ...

    path: str


class GuiComponent(ABC, Component): ...
    

class Text(GuiComponent):
    font: Font
    text: str
    color: Color

    def __init__(self): ...


class Sprite(RenderComponent):
    def __init__(self): ...
