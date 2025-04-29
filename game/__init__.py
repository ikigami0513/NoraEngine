from nora import Window, Color, Input, Key

def initialize() -> None:
    Window.set_title("Hello Triangle - Nora Engine Example")
    Window.set_size(800, 600)
    Window.background_color = Color(0.2, 0.3, 0.3, 1.0)

def update() -> None:
    if Input.is_key_pressed(Key.S):
        print("coucou !")
    elif Input.is_just_released(Key.S):
        print("au revoir !")

    if Input.is_just_pressed(Key.D):
        print("Hello !")
