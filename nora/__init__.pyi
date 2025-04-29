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
