from nora.physics.math import Vec3
from nora.world.ecs import Component


class Camera3D(Component):
    """
    A basic camera component.
    """
    front: Vec3
    right: Vec3
    yaw: float
    pitch: float
    zoom: float

    def __init__(self) -> None: ...
