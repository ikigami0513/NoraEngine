from nora import Window, Color

def initialize() -> None:
    Window.set_title("Hello Triangle - Nora Engine Example")
    Window.set_size(800, 600)
    Window.background_color = Color(0.2, 0.3, 0.3, 1.0)

def update() -> None:
    pass
