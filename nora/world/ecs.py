from nora.physics.transform import Transform
from nora.world.ecs import Entity
from typing import TypeVar


_T = TypeVar('_T')  


class Component:
    """
    Base class for all components.
    """
    owner: 'Entity'

    def __init__(self) -> None: ...
    def start(self) -> None: ...
    def update(self) -> None: ...
    def on_collision_enter(self, other: Entity) -> None: ...
    def set_owner(self, entity: 'Entity') -> None: ...



class Entity:
    """
    Represents an entity in the game world.
    Entities can have components attached to them.
    """
    transform: Transform

    def add_component(self, component: Component) -> None:
        """
        Adds a component to this entity.

        :param component: The component to add.
        """

    def get_component(self, type: type[_T]) -> _T | None:
        """
        Gets a component of the specified type attached to this entity.

        :param type: The type of the component to retrieve (e.g., Camera).
        :return: The component if found, otherwise None.
        """
        