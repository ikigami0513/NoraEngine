from nora import Component, Time, Text


class FPSDisplayComponent(Component):
    def start(self):
        self.last_fps = 0.0

    def update(self):
        if self.last_fps != Time.fps:
            self.owner.get_component(Text).text = f"{Time.fps} FPS"
            self.last_fps = Time.fps
            