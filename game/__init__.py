from nora import *

class FPSDisplayComponent(Component):
    last_fps = 0.0

    def update(self):
        if self.last_fps != Time.fps:
            print(f"FPS: {int(Time.fps)}")
            self.last_fps = Time.fps

def initialize() -> None:
    Window.set_title("Hello Triangle - Nora Engine Example")
    Window.set_size(800, 600)
    Window.background_color = Color(0.2, 0.3, 0.3, 1.0)
    e = Entity()
    e.add_component(FPSDisplayComponent())
    Window.scene.add_entity(e)
