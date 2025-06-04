from nora.world.ecs import Component


class Animation2D(Component):
    def __init__(self, width: int, height: int, current_row: int, frames_count: int, animation_speed): ...