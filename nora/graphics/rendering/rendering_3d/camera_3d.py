from nora.physics.math import Vec3
from nora.world.ecs import Component
from nora.graphics.color import Color


class Camera3D(Component):
    """
    A basic camera component.
    """
    front: Vec3
    right: Vec3
    yaw: float
    pitch: float
    zoom: float
    has_fog: bool = False
    fog_color: Color = Color()
    min_fog_dist: float = 0.1
    max_fog_dist: float = 8.0

    def __init__(self) -> None: ...
