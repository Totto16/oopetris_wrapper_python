import os
from pathlib import Path
from typing import TYPE_CHECKING, Any

from oopetris import recordings
from oopetris.recordings import get_information, is_recording_file

import pytest
from pytest_subtests import SubTests

if TYPE_CHECKING:
    from oopetris.recordings import RecordingInformation


def get_file_path(name: str) -> Path:
    dir_path: Path = Path(os.path.realpath(__file__)).parent
    return dir_path / "files" / name


def test_is_recording_file(subtests: SubTests) -> None:
    with (
        subtests.test("should raise an error, when no first argument was given"),
        pytest.raises(TypeError),
    ):
        is_recording_file()  # type: ignore[call-arg]

    with (
        subtests.test("should raise an error, when the first argument is not a string"),
        pytest.raises(TypeError),
    ):
        is_recording_file(1)  # type: ignore[arg-type]

    with subtests.test("should accept 'string' and 'pathlib.Path' as argument"):
        file: Path = get_file_path("NON-EXISTENT.rec")
        assert not is_recording_file(file)
        assert not is_recording_file(str(file))

    with subtests.test("should return false, when the file doesn't exist"):
        non_existent_file: Path = get_file_path("NON-EXISTENT.rec")
        assert not is_recording_file(non_existent_file)

    with subtests.test("should return true, when the file exists and is valid"):
        correct_file: Path = get_file_path("correct.rec")
        assert is_recording_file(correct_file)


def test_get_information(subtests: SubTests) -> None:
    with (
        subtests.test("should raise an error, when no first argument was given"),
        pytest.raises(TypeError),
    ):
        get_information()  # type: ignore[call-arg]

    with (
        subtests.test("should raise an error, when the first argument is not a string"),
        pytest.raises(TypeError),
    ):
        get_information(1)  # type: ignore[arg-type]

    correct_file: Path = get_file_path(name="correct.rec")

    with subtests.test("should accept 'string' and 'pathlib.Path' as argument"):
        get_information(correct_file)
        get_information(str(correct_file))

    non_existent_file: Path = get_file_path("NON-EXISTENT.rec")

    with (
        subtests.test("should raise an error, when the file doesn't exist"),
        pytest.raises(
            FileNotFoundError,
            match=r"^File '.*NON-EXISTENT.rec' doesn't exist!$",
        ),
    ):
        assert not get_information(non_existent_file)

    with subtests.test("should return a dict, when the file exists and is valid"):
        information: RecordingInformation = get_information(correct_file)
        assert isinstance(information, dict)


def test_properties(subtests: SubTests) -> None:
    raw_properties: list[str] = dir(recordings)
    properties: list[str] = sorted(
        prop for prop in raw_properties if not prop.startswith("__")
    )

    with subtests.test("should only have known properties"):
        expected_properties_list: list[str] = sorted(
            [
                "is_recording_file",
                "get_information",
                "version",
                "properties",
            ],
        )

        assert properties == expected_properties_list

    with subtests.test("should have the expected properties"):
        expected_properties_dict: dict[str, Any] = {
            "is_recording_file": lambda *_a: None,
            "get_information": lambda *_a: None,
            "version": "0.5.6",
            "properties": {"grid_properties": {"height": 20, "width": 10}},
        }

        for key in properties:
            assert expected_properties_dict.get(key) is not None
            expected_value = expected_properties_dict[key]

            if callable(expected_value):
                pass
            else:
                assert getattr(recordings, key) == expected_value
