from typing import Literal, TypedDict

# types

# int aliases

type U64 = int
type U32 = int
type U8 = int

# custom types

type AdditionalInformationValue = (
    str | U8 | U32 | U64 | bool | list[AdditionalInformationValue]
)

type AdditionalInformation = dict[str, AdditionalInformationValue]

type InputEvent = Literal[
    "RotateLeftPressed",
    "RotateRightPressed",
    "MoveLeftPressed",
    "MoveRightPressed",
    "MoveDownPressed",
    "DropPressed",
    "HoldPressed",
    "RotateLeftReleased",
    "RotateRightReleased",
    "MoveLeftReleased",
    "MoveRightReleased",
    "MoveDownReleased",
    "DropReleased",
    "HoldReleased",
]

class TetrionRecord(TypedDict):
    event: InputEvent
    simulation_step_index: U64
    tetrion_index: U8

class MinoPosition(TypedDict):
    x: U8
    y: U8

type TetrominoType = Literal["I", "J", "L", "O", "S", "T", "Z"]

class Mino(TypedDict):
    position: MinoPosition
    type: TetrominoType

class TetrionSnapshot(TypedDict):
    level: U32
    lines_cleared: U32
    mino_stack: list[Mino]
    score: U64
    simulation_step_index: U64
    tetrion_index: U8

class TetrionHeader(TypedDict):
    seed: U64
    starting_level: U32

class RecordingInformation(TypedDict):
    information: list[AdditionalInformation]
    records: list[TetrionRecord]
    snapshots: list[TetrionSnapshot]
    tetrion_headers: list[TetrionHeader]
    version: U8

class GridProperties(TypedDict):
    height: U8
    width: U8

class RecordingsProperties(TypedDict):
    grid_properties: GridProperties

# actual functions

def is_recording_file(path: str) -> bool: ...
def get_information(path: str) -> RecordingInformation: ...

properties: RecordingsProperties

version: str
