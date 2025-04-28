from nora_api import Time
from nora.color import Color
import colorsys

__title__ = "Nora Engine"
__width__ = 800
__height__ = 600

background_color = Color(0.0, 0.0, 0.0, 1.0)

# Internal hue tracker
_hue: float = 0.0  # Ranges from 0.0 to 1.0

def update():
    print(Time.fps)
    global _hue
    # Increase hue over time
    _hue = (_hue + 0.1 * Time.delta_time) % 1.0

    # Convert HSV to RGB
    r, g, b = colorsys.hsv_to_rgb(_hue, 1.0, 1.0)  # Full saturation and brightness
    background_color.r = r
    background_color.g = g
    background_color.b = b
