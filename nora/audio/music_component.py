from nora.world.ecs import Component


class MusicComponent(Component):
    music_path: str

    def __init__(self): ...
    