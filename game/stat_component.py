from nora import Component


class StatComponent(Component):
    def start(self):
        self._max: int = 50
        self._current: int = self._max

    def current(self) -> int:
        return self._current

    def _check(self):
        if self._current > self._max:
            self._current = self._max

    def set(self, value: int):
        self._current = value
        self._check()
        
    def add(self, value: int):
        self._current += value
        self._check()

    def remove(self, value: int):
        self.add(-value)
    