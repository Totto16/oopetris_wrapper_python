import os
from pathlib import Path
from typing import TYPE_CHECKING

from oopetris.recordings import get_information, is_recording_file

import pytest
from correct import correct_information
from pytest_subtests import SubTests

if TYPE_CHECKING:
    from oopetris.recordings import RecordingInformation


def get_file_path(name: str) -> Path:
    dir_path: Path = Path(os.path.realpath(__file__)).parent
    return dir_path / "files" / name


def test_is_recording_file(subtests: SubTests) -> None:
    with subtests.test("should return false for non existent file"):
        file: Path = get_file_path("NON-EXISTENT.rec")

        assert not file.exists()

        assert not is_recording_file(file)

    with subtests.test("should return false for incorrect file"):
        incorrect_file: Path = get_file_path("incorrect.rec")

        assert incorrect_file.exists()

        assert not is_recording_file(incorrect_file)

    with subtests.test("should return true for correct file"):
        correct_file: Path = get_file_path("correct.rec")

        assert correct_file.exists()

        assert is_recording_file(correct_file)


def test_get_information(subtests: SubTests) -> None:
    correct_file: Path = get_file_path(name="correct.rec")
    non_existent_file: Path = get_file_path("NON-EXISTENT.rec")
    incorrect_file: Path = get_file_path("incorrect.rec")

    with (  # noqa: PT012
        subtests.test("should raise an error for non existent file"),
        pytest.raises(
            FileNotFoundError,
            match=r"^File '.*NON-EXISTENT.rec' doesn't exist!$",
        ),
    ):
        assert not non_existent_file.exists()

        assert not get_information(non_existent_file)

    with (  # noqa: PT012
        subtests.test("should raise an error for incorrect file"),
        pytest.raises(
            RuntimeError,
            match=r"^An error occurred during parsing of the recording file '.*incorrect.rec': magic file bytes are not correct, this is either an old format or no recording at all$",
        ),
    ):
        assert incorrect_file.exists()

        assert not get_information(incorrect_file)

    with subtests.test("should return a dict, when the file exists and is valid"):
        assert correct_file.exists()

        information: RecordingInformation = get_information(correct_file)
        assert isinstance(information, dict)

    with subtests.test(
        "should return a correct dict, when the file exists and is valid",
    ):
        assert correct_file.exists()

        new_information: RecordingInformation = get_information(correct_file)
        assert new_information == correct_information
