from nora import *
from game.stat_component import StatComponent
from typing import Dict
from enum import StrEnum
from typing import Optional


class PlayerState(StrEnum):
    IDLE = "idle"
    RUN = "run"
    ATTACK = "attack"


class PlayerDirection(StrEnum):
    UP = "up"
    DOWN = "down"
    LEFT = "left"
    RIGHT = "right"


class PlayerComponent(Component):
    text: Optional[Text]

    def start(self) -> None:
        self.state = PlayerState.IDLE
        self.direction = PlayerDirection.DOWN
        self.sprite = self.owner.get_component(Sprite)
        self.animation = self.owner.get_component(Animation2D)
        self.rigidbody = self.owner.get_component(Rigidbody2D)
        self.sword_whoosh_sound = self.owner.get_component(SoundComponent)
        self.velocity = Vec2(0.0, 0.0)
        self.speed = 50.0
        self.attack_cycle = 1
        self.repeat_attack = False
        self.textures: Dict[str, Texture] = {
            "idle_up": Texture("../resources/textures/player/idle/idle_up.png"),
            "idle_down": Texture("../resources/textures/player/idle/idle_down.png"),
            "idle_left": Texture("../resources/textures/player/idle/idle_left.png"),
            "idle_right": Texture("../resources/textures/player/idle/idle_right.png"),

            "run_up": Texture("../resources/textures/player/run/run_up.png"),
            "run_down": Texture("../resources/textures/player/run/run_down.png"),
            "run_left": Texture("../resources/textures/player/run/run_left.png"),
            "run_right": Texture("../resources/textures/player/run/run_right.png"),

            "attack1_up": Texture("../resources/textures/player/attack1/attack1_up.png"),
            "attack1_down": Texture("../resources/textures/player/attack1/attack1_down.png"),
            "attack1_left": Texture("../resources/textures/player/attack1/attack1_left.png"),
            "attack1_right": Texture("../resources/textures/player/attack1/attack1_right.png"),

            "attack2_up": Texture("../resources/textures/player/attack2/attack2_up.png"),
            "attack2_down": Texture("../resources/textures/player/attack2/attack2_down.png"),
            "attack2_left": Texture("../resources/textures/player/attack2/attack2_left.png"),
            "attack2_right": Texture("../resources/textures/player/attack2/attack2_right.png"),
        }

        self.health = self.owner.get_component(StatComponent)

    def update(self) -> None:
        self.velocity = Vec2(0.0, 0.0)
        new_direction = self.direction
        new_state = self.state

        if self.state != PlayerState.ATTACK:
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

            if Input.is_key_pressed(Key.Space):
                new_state = PlayerState.ATTACK
                self.direction = new_direction
                self.state = new_state
                self.sprite.texture = self.textures[f"{self.state.value}{self.attack_cycle}_{self.direction.value}"]
                self.animation.current_frame = 0.0
                self.animation.repeat = False
                self.sword_whoosh_sound.play()
            else:
                if self.velocity.x != 0 or self.velocity.y != 0:
                    self.velocity = self.velocity.normalized()
                    displacement = self.velocity * self.speed
                    self.rigidbody.velocity = displacement
                    new_state = PlayerState.RUN
                else:
                    self.rigidbody.velocity = Vec2(0.0, 0.0)
                    new_state = PlayerState.IDLE

                if new_direction != self.direction or new_state != self.state:
                    self.direction = new_direction
                    self.state = new_state
                    self.sprite.texture = self.textures[f"{self.state.value}_{self.direction.value}"]

        else:
            if self.animation.finish:
                if self.repeat_attack and self.attack_cycle == 1:
                    self.attack_cycle = 2
                    self.sprite.texture = self.textures[f"{self.state.value}{self.attack_cycle}_{self.direction.value}"]
                    self.sword_whoosh_sound.play()
                else:
                    self.repeat_attack = False
                    self.attack_cycle = 1
                    self.state = PlayerState.IDLE
                    self.sprite.texture = self.textures[f"{self.state.value}_{self.direction.value}"]
                    self.animation.repeat = True

                self.animation.finish = False
                self.animation.current_frame = 0.0

            if (Input.is_just_pressed(Key.Space)) and self.animation.current_frame >= self.animation.frames_count / 2:
                self.repeat_attack = True

        if self.text:
            pos = self.owner.transform.local_position
            self.text.text = f"X: {pos.x:.2f} Y: {pos.y:.2f} Health: {self.health.current()}"

    def on_collision_enter(self, other: Entity):
        pass
