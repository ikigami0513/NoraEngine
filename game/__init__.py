from nora import *


class PlayerController(Component):
    movement_speed: float
    mouse_sensitivity: float
    constrain_pitch: bool
    min_pitch: float
    max_pitch: float
    min_zoom: float
    max_zoom: float

    def start(self):
        self.movement_speed = 2.5
        self.mouse_sensitivity = 0.1
        self.constrain_pitch = True
        self.min_pitch = -89.0
        self.max_pitch = 89.0
        self.min_zoom = 1.0
        self.max_zoom = 45.0
        self.camera = self.owner.get_component(Camera)

    def update(self):
        transform = self.owner.transform
        velocity = self.movement_speed * Time.delta_time

        if Input.is_key_pressed(Key.W):
            transform.local_position += self.camera.front * velocity
        if Input.is_key_pressed(Key.S):
            transform.local_position -= self.camera.front * velocity
        if Input.is_key_pressed(Key.A):
            transform.local_position -= self.camera.right * velocity
        if Input.is_key_pressed(Key.D):
            transform.local_position += self.camera.right * velocity

        xoffset, yoffset = Input.mouse_delta
        self.camera.yaw += xoffset
        self.camera.pitch += yoffset

        if self.constrain_pitch:
            if self.camera.pitch > self.max_pitch:
                self.camera.pitch = self.max_pitch
            if self.camera.pitch < self.min_pitch:
                self.camera.pitch = self.min_pitch

        self.camera.zoom -= Input.scroll_delta[1]
        if self.camera.zoom < self.min_zoom:
            self.camera.zoom = self.min_zoom
        if self.camera.zoom > self.max_zoom:
            self.camera.zoom = self.max_zoom


class FPSDisplayComponent(Component):
    last_fps = 0.0

    def update(self):
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
    Window.set_size(800, 600)
    Window.background_color = Color(0.2, 0.3, 0.3, 1.0)

    e = Entity()
    e.transform.local_position = Vec3(0.0, 0.0, 3.0)

    fps_component = FPSDisplayComponent()
    fps_component.set_owner(e)
    e.add_component(fps_component)

    simple_component = SimpleComponent()
    simple_component.set_owner(e)
    e.add_component(simple_component)

    camera = Camera()
    camera.set_owner(e)
    e.add_component(camera)

    player_controller = PlayerController()
    player_controller.set_owner(e)
    e.add_component(player_controller)

    Window.scene.add_entity(e)
