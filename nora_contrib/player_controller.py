from nora import Component, Camera, Time, Input, Key


class PlayerController(Component):
    last_x: float
    last_y: float
    first_mouse: bool
    movement_speed: float
    mouse_sensitivity: float
    constrain_pitch: bool
    min_pitch: float
    max_pitch: float
    min_zoom: float
    max_zoom: float

    def start(self) -> None:
        self.last_x = 0.0
        self.last_y = 0.0
        self.first_mouse = True
        self.movement_speed = 2.5
        self.mouse_sensitivity = 0.1
        self.constrain_pitch = True
        self.min_pitch = -89.0
        self.max_pitch = 89.0
        self.min_zoom = 1.0
        self.max_zoom = 45.0
        self.camera = self.owner.get_component(Camera)

    def update(self):
        self.process_keyboard()
        self.process_mouse_movement()
        self.process_mouse_scroll()

    def process_keyboard(self):
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

    def process_mouse_movement(self):
        xpos, ypos = Input.mouse_position
        if self.first_mouse:
            self.last_x = xpos
            self.last_y = ypos
            self.first_mouse = False

        xoffset = xpos - self.last_x
        yoffset = self.last_y - ypos

        xoffset *= self.mouse_sensitivity
        yoffset *= self.mouse_sensitivity

        self.camera.yaw += xoffset
        self.camera.pitch += yoffset

        if self.constrain_pitch:
            if self.camera.pitch > self.max_pitch:
                self.camera.pitch = self.max_pitch
            if self.camera.pitch < self.min_pitch:
                self.camera.pitch = self.min_pitch

    def process_mouse_scroll(self):
        self.camera.zoom -= Input.scroll_delta[1]
        if self.camera.zoom < self.min_zoom:
            self.camera.zoom = self.min_zoom
        if self.camera.zoom > self.max_zoom:
            self.camera.zoom = self.max_zoom
