from nora import *

class FPSDisplayComponent(Component):
    last_fps = 0.0

    def update(self):
        self.owner.get_component(SimpleComponent).greet()
        if self.last_fps != Time.fps:
            print(f"FPS: {int(Time.fps)}")
            self.last_fps = Time.fps

class SimpleComponent(Component):
    def start(self):
        print("Hello i'm a simple test component")

    def greet(self):
        print("Hello Nora Engine")

def initialize() -> None:
    Window.set_title("Hello Triangle - Nora Engine Example")
    Window.set_size(1080, 720)
    Window.background_color = Color(0.2, 0.3, 0.3, 1.0)

    e = Entity()

    fps_component = FPSDisplayComponent()
    fps_component.set_owner(e)
    e.add_component(fps_component)

    simple_component = SimpleComponent()
    simple_component.set_owner(e)
    e.add_component(simple_component)

    Window.scene.add_entity(e)
