from nora import *
from typing import Dict
from enum import StrEnum


class PlayerState(StrEnum):
    IDLE = "idle"
    RUN = "run"


class PlayerDirection(StrEnum):
    UP = "up"
    DOWN = "down"
    LEFT = "left"
    RIGHT = "right"


class PlayerComponent(Component):
    text: Text

    def start(self) -> None:
        self.state = PlayerState.IDLE
        self.direction = PlayerDirection.DOWN
        self.sprite = self.owner.get_component(Sprite)
        self.animation = self.owner.get_component(Animation2D)
        self.velocity = Vec2(0.0, 0.0)
        self.speed = 50.0
        self.textures: Dict[str, Texture] = {
            "idle_up": Texture("../resources/textures/player/idle/idle_up.png"),
            "idle_down": Texture("../resources/textures/player/idle/idle_down.png"),
            "idle_left": Texture("../resources/textures/player/idle/idle_left.png"),
            "idle_right": Texture("../resources/textures/player/idle/idle_right.png"),

            "run_up": Texture("../resources/textures/player/run/run_up.png"),
            "run_down": Texture("../resources/textures/player/run/run_down.png"),
            "run_left": Texture("../resources/textures/player/run/run_left.png"),
            "run_right": Texture("../resources/textures/player/run/run_right.png")
        }

    def update(self) -> None:
        self.velocity = Vec2(0.0, 0.0)
        new_direction = self.direction
        new_state = self.state

        if Input.is_key_pressed(Key.W):
            new_direction = PlayerDirection.UP
            self.velocity.y = 1
        elif Input.is_key_pressed(Key.S):
            new_direction = PlayerDirection.DOWN
            self.velocity.y = -1

        if Input.is_key_pressed(Key.A):
            new_direction = PlayerDirection.LEFT
            self.velocity.x = -1
        elif Input.is_key_pressed(Key.D):
            new_direction = PlayerDirection.RIGHT
            self.velocity.x = 1

        if self.velocity.x != 0 or self.velocity.y != 0:
            self.velocity = self.velocity.normalized()
            displacement = self.velocity * self.speed * Time.delta_time
            self.owner.transform.local_position += Vec3(displacement.x, displacement.y, 0.0)
            new_state = PlayerState.RUN
        else:
            new_state = PlayerState.IDLE

        if new_direction != self.direction or new_state != self.state:
            self.direction = new_direction
            self.state = new_state
            self.sprite.texture = self.textures[f"{self.state.value}_{self.direction.value}"]

        pos = self.owner.transform.local_position
        self.text.text = f"X: {pos.x:.2f} Y: {pos.y:.2f}"
