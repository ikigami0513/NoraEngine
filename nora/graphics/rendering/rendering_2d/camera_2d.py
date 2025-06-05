from nora.world.ecs import Component, Entity
from typing import Optional


class Camera2D(Component):
    target: Optional[Entity]

    def __init__(self) -> None: ...