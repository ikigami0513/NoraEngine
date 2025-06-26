from nora.world.ecs import Component
from nora.physics.math import Vec2


class Rigidbody2D(Component):
    velocity: Vec2

    def __init__(self): ...
    