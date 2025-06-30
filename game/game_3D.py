from nora import *
from nora_contrib.player_controller_3d import PlayerController3D
from game.init_fps_display import init_fps_display


def init_cuboid(t: Texture):
    cube_positions = [
        Vec3(2.0, 5.0, -15.0),
        Vec3(-1.5, -2.2, -2.5),
        Vec3(-3.8, -2.0, -12.3),
        Vec3(2.4, -0.4, -3.5),
        Vec3(-1.7, 3.0, -7.5),
        Vec3(1.3, -2.0, -2.5),
        Vec3(1.5,  2.0, -2.5),
    ]

    for i in range(len(cube_positions)):
        cube_entity = Entity()
        cube_entity.transform.local_position = cube_positions[i]
        angle = 20 * i
        cube_entity.transform.local_rotation = Vec3(angle, angle, angle)
        cube_component = CuboidMesh()
        cube_component.texture = t
        cube_component.set_owner(cube_entity)
        cube_entity.add_component(cube_component)
        Window.scene.add_entity(cube_entity)


def init_sphere(t: Texture):
    s_entity = Entity()
    s_entity.transform.local_position = Vec3(-1.3, 1.0, -1.5)
    sphere_component = SphereMesh()
    sphere_component.texture = t
    sphere_component.set_owner(s_entity)
    s_entity.add_component(sphere_component)
    Window.scene.add_entity(s_entity)


def init_capsule(t: Texture):
    ce = Entity()
    ce.transform.local_position = Vec3(1.5, 0.2, -1.5)
    capsule = CapsuleMesh()
    capsule.texture = t
    capsule.set_owner(ce)
    ce.add_component(capsule)
    Window.scene.add_entity(ce)


def init_camera_and_controller():
    e = Entity()
    e.transform.local_position = Vec3(0.0, 0.0, 3.0)

    player_controller = PlayerController3D()
    player_controller.set_owner(e)
    e.add_component(player_controller)

    camera = Camera3D()
    camera.has_fog = False
    camera.fog_color = Color(0.7, 0.7, 0.7, 1.0)
    camera.set_owner(e)
    e.add_component(camera)

    Window.scene.add_entity(e)


def create_3d_world():
    Mouse.mode = MouseMode.Disabled
    init_camera_and_controller()
    init_fps_display()

    t = Texture("../resources/textures/container.jpg")
    init_cuboid(t)
    init_sphere(t)
    init_capsule(t)

    rock = Entity()
    rock.transform.local_position = Vec3(0.0, 0.0, 0.0)
    rock.transform.local_scale = Vec3(0.1, 0.1, 0.1)
    model = Model("../resources/models/rock/rock.obj")
    model.set_owner(rock)
    rock.add_component(model)
    Window.scene.add_entity(rock)

    skybox_entity = Entity()
    skybox = Skybox()
    skybox.faces = [
        "../resources/textures/skybox/right.jpg",
        "../resources/textures/skybox/left.jpg",
        "../resources/textures/skybox/top.jpg",
        "../resources/textures/skybox/bottom.jpg",
        "../resources/textures/skybox/front.jpg",
        "../resources/textures/skybox/back.jpg",
    ]
    skybox.set_owner(skybox_entity)
    skybox_entity.add_component(skybox)
    Window.scene.add_entity(skybox_entity)
