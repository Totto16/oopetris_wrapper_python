from typing import Any, Callable
import pytest
from oopetris import version, is_recording_file, get_information
import oopetris
from pytest_subtests import SubTests
import os


def get_file_path(name: str) -> str:
    dir_path: str = os.path.dirname(os.path.realpath(__file__))
    return os.path.join(dir_path, "files", name)


def test_is_recording_file(subtests: SubTests) -> None:
    with (
        subtests.test("should raise an error, when no first argument was given"),
        pytest.raises(TypeError),
    ):
        is_recording_file()

    with (
        subtests.test("should raise an error, when the first argument is not a string"),
        pytest.raises(TypeError),
    ):
        is_recording_file(1)

    with subtests.test("should return false, when the file doesn't exist"):
        file = get_file_path("NON-EXISTENT.rec")
        assert not is_recording_file(file)

    with subtests.test("should return true, when the file exists and is valid"):
        file = get_file_path("correct.rec")
        assert is_recording_file(file)


def test_get_information(subtests: SubTests) -> None:
    with (
        subtests.test("should raise an error, when no first argument was given"),
        pytest.raises(TypeError),
    ):
        get_information()

    with (
        subtests.test("should raise an error, when the first argument is not a string"),
        pytest.raises(TypeError),
    ):
        get_information(1)

    with (
        subtests.test("should raise an error, when the file doesn't exist"),
        pytest.raises(TypeError, match=r"^a$"),
    ):
        file = get_file_path("NON-EXISTENT.rec")
        assert not get_information(file)

    with subtests.test("should return a dict, when the file exists and is valid"):
        file = get_file_path("correct.rec")
        information = get_information(file)
        assert isinstance(information, dict)


def test_properties(subtests: SubTests) -> None:
    with subtests.test("should only have known properties"):
        expected_properties = sorted(
            [
                "is_recording_file",
                "get_information",
                "version",
                "properties",
            ]
        )

        raw_properties = dir(oopetris)
        properties = sorted(
            prop for prop in raw_properties if not prop.startswith("__")
        )

        assert properties == expected_properties

    with subtests.test("should have the expected properties"):
        expected_properties: dict[str, Any] = {
            "is_recording_file": lambda *_a: None,
            "get_information": lambda *_a: None,
            "version": "0.5.6",
            "properties": {"grid_properties": {"height": 20, "width": 10}},
        }

        raw_properties = dir(oopetris)
        properties = sorted(
            prop for prop in raw_properties if not prop.startswith("__")
        )

        for key in properties:
            assert expected_properties.get(key, None) is not None
            expected_value = expected_properties[key]

            if isinstance(expected_value, Callable):
                pass
            else:
                assert getattr(oopetris, key) == expected_value
