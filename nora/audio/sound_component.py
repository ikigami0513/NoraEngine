from nora.world.ecs import Component


class SoundComponent(Component):
    sound_path: str

    def __init__(self): ...

    def play(self): ...
