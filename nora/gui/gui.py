from nora.world.ecs import Component
from nora.graphics.color import Color
from nora.physics.math import Vec2
from abc import ABC
from enum import Enum, auto
from typing import Callable



class GuiComponent(ABC, Component): ...


class Font:
    def __init__(self, font_path: str, font_size: int = 48): ...

    
class Alignment(Enum):
    Left = auto()
    Center = auto()
    Right = auto()


class Text(GuiComponent):
    font: Font
    text: str
    color: Color
    alignement: Alignment
    margin: float

    def __init__(self): ...

    def get_text_bounds(scale: float = 1.0) -> Vec2: ...
    

class Rectangle(GuiComponent):
    color: Color
    width: float
    height: float

    def __init__(self, color: Color = Color(1.0, 1.0, 1.0, 1.0), width: float = 100, height: float = 50): ...


class Button(Component):
    hovered_color: Color
    on_click_color: Color
    on_click: Callable[[], None]
