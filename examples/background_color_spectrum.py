from nora import Time, Window
import colorsys

# Internal hue tracker
_hue: float = 0.0  # Ranges from 0.0 to 1.0

def initialize() -> None:
    Window.set_title("Background Color Spectrum - Nora Engine Example")
    Window.set_size(800, 600)

def update() -> None:
    global _hue
    # Increase hue over time
    _hue = (_hue + 0.1 * Time.delta_time) % 1.0

    # Convert HSV to RGB
    r, g, b = colorsys.hsv_to_rgb(_hue, 1.0, 1.0)  # Full saturation and brightness
    Window.background_color.r = r
    Window.background_color.g = g
    Window.background_color.b = b
