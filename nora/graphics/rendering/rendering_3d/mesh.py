from nora.world.ecs import Component
from nora.graphics.texture import Texture
from abc import ABC


class RenderComponent(ABC, Component):
    pass


class MeshedRenderComponent(ABC, RenderComponent):
    texture: Texture


class CuboidMesh(MeshedRenderComponent):
    def __init__(self): ...


class SphereMesh(MeshedRenderComponent):
    def __init__(self, sector_count: int = 36, stack_count: int = 18): ...


class CapsuleMesh(MeshedRenderComponent):
    def __init__(
        self, radius: float = 0.5, cylinder_height: float = 1.0, 
        sector_count: int = 36, hemisphere_stacks: int = 18, cylinder_stacks: int = 10
    ): ...


class Model(MeshedRenderComponent):
    def __init__(self, path: str = ""): ...

    path: str