from nora.physics.math import Vec3, Mat4


class Transform:
    """
    Represents the position, rotation, and scale of an entity.
    """
    local_position: Vec3
    """The local position of the entity."""

    local_rotation: Vec3
    """The local rotation (Euler angles in degrees) of the entity."""

    local_scale: Vec3
    """The local scale of the entity."""

    global_position: Vec3
    """The global position of the entity in world space (read-only)."""

    model_matrix: Mat4
    """The combined model matrix representing the entity's transformation (read-only)."""

    right: Vec3
    """The right vector of the entity in world space (read-only)."""

    up: Vec3
    """The up vector of the entity in world space (read-only)."""

    backward: Vec3
    """The backward vector of the entity in world space (read-only)."""

    forward: Vec3
    """The forward vector of the entity in world space (read-only)."""

    global_scale: Vec3
    """The global scale of the entity (read-only)."""

    is_dirty: bool
    """Returns True if the transform has been modified since the last computation."""

    def __init__(self) -> None: ...
      