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
    