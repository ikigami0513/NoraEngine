from nora.world.ecs import Component
from nora.physics.offset import Offset


class RectCollider(Component):
    offset: Offset

    def __init__(self): ...
    