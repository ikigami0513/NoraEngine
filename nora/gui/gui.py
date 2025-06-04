from nora.world.ecs import Component
from nora.graphics.color import Color
from abc import ABC



class GuiComponent(ABC, Component): ...


class Font:
    def __init__(self, font_path: str, font_size: int = 48): ...

    

class Text(GuiComponent):
    font: Font
    text: str
    color: Color

    def __init__(self): ...
    