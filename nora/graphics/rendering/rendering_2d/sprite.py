from nora.world.ecs import Component
from nora.graphics.texture import Texture


class Sprite(Component):
    texture: Texture

    def __init__(self): ...