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
    Window.set_title("Cuboid 3d space - Nora Engine Example")
    Window.set_size(1920, 1080)
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

    cube_positions = [
        Vec3(0.0, 0.0, 0.0),
        Vec3(2.0, 5.0, -15.0),
        Vec3(-1.5, -2.2, -2.5),
        Vec3(-3.8, -2.0, -12.3),
        Vec3(2.4, -0.4, -3.5),
        Vec3(-1.7, 3.0, -7.5),
        Vec3(1.3, -2.0, -2.5),
        Vec3(1.5,  2.0, -2.5),
        Vec3(1.5, 0.2, -1.5),
        Vec3(-1.3, 1.0, -1.5)
    ]

    t = Texture("../resources/textures/container.jpg")
    for i in range(len(cube_positions)):
        cube_entity = Entity()
        cube_entity.transform.local_position = cube_positions[i]
        cube_component = CuboidMesh()
        cube_component.texture = t
        cube_component.set_owner(cube_entity)
        cube_entity.add_component(cube_component)
        Window.scene.add_entity(cube_entity)
