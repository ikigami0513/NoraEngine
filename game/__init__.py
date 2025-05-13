from nora import *
from nora_contrib.player_controller import PlayerController


class FPSDisplayComponent(Component):
    def start(self):
        self.last_fps = 0.0

    def update(self):
        if self.last_fps != Time.fps:
            print(f"FPS: {int(Time.fps)}")
            self.last_fps = Time.fps


def initialize() -> None:
    Window.set_title("Hello Triangle - Nora Engine Example")
    Window.set_size(800, 600)
    Window.background_color = Color(0.2, 0.3, 0.3, 1.0)

    e = Entity()
    e.transform.local_position = Vec3(0.0, 0.0, 3.0)

    fps_component = FPSDisplayComponent()
    fps_component.set_owner(e)
    e.add_component(fps_component)

    player_controller = PlayerController()
    player_controller.set_owner(e)
    e.add_component(player_controller)

    camera = Camera()
    camera.set_owner(e)
    e.add_component(camera)

    Window.scene.add_entity(e)
