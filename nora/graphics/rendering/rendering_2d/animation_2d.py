from nora.world.ecs import Component


class Animation2D(Component):
    repeat: bool
    finish: bool
    current_frame: float
    frames_count: int

    def __init__(self, width: int, height: int, current_row: int, frames_count: int, animation_speed: int, repeat: bool = True): ...