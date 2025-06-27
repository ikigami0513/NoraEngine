from nora.graphics.rendering.rendering_3d.mesh import RenderComponent
from typing import List


class Skybox(RenderComponent):
    faces: List[str]

    def __init__(self): ...
    