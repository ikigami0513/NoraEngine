from typing import List


class Vec3:
    """
    Represents a 3D vector.
    """
    x: float
    y: float
    z: float

    def __init__(self, x: float = 0.0, y: float = 0.0, z: float = 0.0) -> None: ...

    def __repr__(self) -> str: ...


class Mat4:
    """
    Represents a 4x4 matrix.
    """
    # Accessing elements might be tricky without further definition
    def __init__(self) -> None: ...

    def __repr__(self) -> str: ...
    def __getitem__(self, index: int) -> List[float]: ... # Assuming row access
