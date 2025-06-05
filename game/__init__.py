import json
from nora import *
from typing import Dict, Any
from game.init_fps_display import init_fps_display
from game.player_component import PlayerComponent


def create_2d_world():
    init_fps_display()

    e = Entity()
    e.transform.local_position = Vec3(0.0, 0.0, 0.0)
    camera = Camera2D()
    camera.set_owner(e)
    e.add_component(camera)
    Window.scene.add_entity(e)

    e = Entity()
    size = Window.get_size()
    e.transform.local_position = Vec3(0.0, 0.0, 0.0)
    e.transform.local_scale = Vec3(2.5, 2.5, 0.0)
    sprite = Sprite()
    sprite.texture = Texture("../resources/textures/player/idle/idle_down.png")
    sprite.set_owner(e)
    e.add_component(sprite)

    animation = Animation2D(96, 80, 1, 8, 5)
    animation.set_owner(e)
    e.add_component(animation)

    text_entity = Entity()
    text_entity.transform.local_position = Vec3(10.0, size[1] - 60.0)
    text = Text()
    text.font = Font("../resources/fonts/Antonio-Regular.ttf", 16)
    text.color = Color(1.0, 1.0, 1.0)
    text.set_owner(text_entity)
    text_entity.add_component(text)
    Window.scene.add_entity(text_entity)

    player = PlayerComponent()
    player.text = text
    player.set_owner(e)
    e.add_component(player)

    tree = Entity()
    tree.transform.local_position = Vec3(100.0, 95.0, 0.0)
    tree.transform.local_scale = Vec3(3.0, 3.0, 0.0)
    tree_sprite = Sprite()
    tree_sprite.texture = Texture("../resources/textures/trees/tree1/tree1_green.png")
    tree_sprite.set_owner(tree)
    tree.add_component(tree_sprite)
    Window.scene.add_entity(tree)

    camera.target = e

    Window.scene.add_entity(e)


def initialize() -> None:
    with open("../settings.json", "rb") as f:
        settings: Dict[str, Any] = json.load(f)

    Window.set_title(settings.get("title"))
    size: Dict[str, int] = settings.get("size", {})
    Window.set_size(size.get("width", 600), size.get("height", 800))
    Window.context = getattr(WindowContext, f"Context{settings.get('context')}")
    Window.background_color = Color(0.2, 0.3, 0.3, 1.0)
    create_2d_world()
