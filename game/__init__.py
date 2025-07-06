import json
from nora import *
from typing import Dict, Any
from game.init_fps_display import init_fps_display
from game.stat_component import StatComponent
from game.player_component import PlayerComponent
from game.game_3D import create_3d_world


player_entity = None
camera_entity = None
button_entity = None


def button_on_click():
    global player_entity, camera_entity, button_entity

    if player_entity:
        camera = camera_entity.get_component(Camera2D)
        if camera.target == player_entity:
            camera.target = None
            button_entity.get_component(Text).text = "Target the player"
        else:
            camera.target = player_entity
            button_entity.get_component(Text).text = "Untarget the player"


def create_2d_world():
    init_fps_display()
    font = Font("../resources/fonts/Antonio-Regular.ttf", 16)

    global button_entity
    button_entity = Entity()
    button_entity.transform.local_position = Vec3(Window.get_size()[0] - 200, Window.get_size()[1] - 30, 0.0)
    rect = Rectangle(color=Color(0.0, 1.0, 1.0), width=300, height=50)
    rect.set_owner(button_entity)
    button_entity.add_component(rect)

    text = Text()
    text.text = "Target the player"
    text.font = font
    text.margin = 10.0
    text.alignement = Alignment.Center
    text.color = Color(0.0, 0.0, 0.0)
    text.set_owner(button_entity)
    button_entity.add_component(text)

    button = Button()
    button.hovered_color = Color(0.0, 0.85, 0.95)
    button.on_click_color = Color(0.0, 0.6, 0.7)
    button.on_click = button_on_click
    button.set_owner(button_entity)
    button_entity.add_component(button)

    Window.scene.add_entity(button_entity)

    music_entity = Entity()
    music = MusicComponent()
    music.music_path = "../resources/audio/music/contemplative.wav"
    music.set_owner(music_entity)
    music_entity.add_component(music)
    Window.scene.add_entity(music_entity)

    global camera_entity
    camera_entity = Entity()
    camera_entity.transform.local_position = Vec3(0.0, 0.0, 0.0)
    camera = Camera2D()
    camera.set_owner(camera_entity)
    camera_entity.add_component(camera)
    Window.scene.add_entity(camera_entity)

    global player_entity
    player_entity = Entity()
    size = Window.get_size()
    player_entity.transform.local_position = Vec3(0.0, 500.0, 0.0)
    player_entity.transform.local_scale = Vec3(2.5, 2.5, 0.0)
    sprite = Sprite()
    sprite.texture = Texture("../resources/textures/player/idle/idle_down.png")
    sprite.set_owner(player_entity)
    player_entity.add_component(sprite)

    rect_collider = RectCollider()
    rect_collider.offset = Offset(100, 100, 100, 100)
    rect_collider.set_owner(player_entity)
    player_entity.add_component(rect_collider)

    rigidbody = Rigidbody2D()
    rigidbody.set_owner(player_entity)
    player_entity.add_component(rigidbody)

    animation = Animation2D(96, 80, 1, 8, 2)
    animation.set_owner(player_entity)
    player_entity.add_component(animation)

    text_entity = Entity()
    text_entity.transform.local_position = Vec3(10.0, size[1] - 60.0)
    text = Text()
    text.font = font
    text.alignement = Alignment.Left
    text.color = Color(1.0, 1.0, 1.0)
    text.set_owner(text_entity)
    text_entity.add_component(text)
    Window.scene.add_entity(text_entity)

    health = StatComponent()
    health.set_owner(player_entity)
    player_entity.add_component(health)

    sword_whoosh = SoundComponent()
    sword_whoosh.sound_path = "../resources/audio/sword/whoosh.wav"
    sword_whoosh.set_owner(player_entity)
    player_entity.add_component(sword_whoosh)

    player = PlayerComponent()
    player.text = text
    player.set_owner(player_entity)
    player_entity.add_component(player)

    Window.scene.add_entity(player_entity)

    t_entity = Entity()
    t_entity.transform.local_position = Vec3(0.0, 0.0, -1.0)
    tilemap = Tilemap()
    tilemap.tilemap_path = "../resources/tilemaps/dev.xml"
    tilemap.set_owner(t_entity)
    t_entity.add_component(tilemap)
    Window.scene.add_entity(t_entity)


def initialize() -> None:
    with open("../settings.json", "rb") as f:
        settings: Dict[str, Any] = json.load(f)

    Window.set_title(settings.get("title"))
    size: Dict[str, int] = settings.get("size", {})
    Window.set_size(size.get("width", 600), size.get("height", 800))
    Window.context = getattr(WindowContext, f"Context{settings.get('context')}")
    Window.background_color = Color(0.2, 0.3, 0.3, 1.0)
    if Window.context == WindowContext.Context2D:
        create_2d_world()
    elif Window.context == WindowContext.Context3D:
        create_3d_world()
    else:
        raise Exception("Invalid Context")
