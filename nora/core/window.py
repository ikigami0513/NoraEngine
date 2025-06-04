from enum import Enum, auto
from nora.world.scene import Scene
from nora.graphics.color import Color


class WindowContext(Enum):
    Context2D = auto()
    Context3D = auto()


class Window:
    """
    Static interface to the engine's main application window.
    """

    background_color: Color
    scene: Scene
    context: WindowContext

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