from nora import *
from game.fps_display_component import FPSDisplayComponent


def init_fps_display():
    fps_entity = Entity()
    fps_entity.transform.local_position = Vec3(10.0, Window.get_size()[1] - 30, 0.0)
    fps_component = FPSDisplayComponent()
    fps_component.set_owner(fps_entity)
    fps_entity.add_component(fps_component)

    font = Font("../resources/fonts/Antonio-Regular.ttf", 16)
    fps_text = Text()
    fps_text.font = font
    fps_text.alignement = Alignment.Left
    fps_text.color = Color(1.0, 1.0, 1.0)
    fps_text.text = f"{Time.fps} FPS"
    fps_text.set_owner(fps_entity)
    fps_entity.add_component(fps_text)

    Window.scene.add_entity(fps_entity)
    