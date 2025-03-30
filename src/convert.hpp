


#include <pybind11/pybind11.h>

#include <core/core.hpp>
#include <recordings/recordings.hpp>

pybind11::dict recording_reader_to_python(const recorder::RecordingReader& reader);
