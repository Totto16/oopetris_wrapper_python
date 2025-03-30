#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <core/core.hpp>
#include <recordings/recordings.hpp>

#include "./convert.hpp"

bool is_recording_file(std::string path) {

    if (not std::filesystem::exists(path)) {

        return false;
    }

    auto parsed = recorder::RecordingReader::from_path(path);

    return parsed.has_value();
}

class FileNotFoundError : public std::runtime_error {
    std::string m_file;

public:
    explicit FileNotFoundError(std::string file)
        : std::runtime_error{ "File Not Found Exception" },
          m_file{ std::move(file) } { }

    [[nodiscard]] std::string file() const {
        return m_file;
    }
};


pybind11::dict get_information(std::string path) {


    if (not std::filesystem::exists(path)) {
        throw FileNotFoundError(path);
    }

    auto parsed = recorder::RecordingReader::from_path(path);

    if (not parsed.has_value()) {

        std::string error = "An error occurred during parsing of the recording file '";
        error += path;
        error += "': ";
        error += parsed.error();

        throw std::runtime_error(error);
    }

    auto recording_reader = std::move(parsed.value());


    pybind11::dict val = recording_reader_to_python(recording_reader);

    return val;
}

PYBIND11_MODULE(oopetris, module) {
    module.doc() = "oopetris wrapper plugin"; // optional module docstring

    module.def("is_recording_file", &is_recording_file, "Checks if a file is a recording file", pybind11::arg("path"));

    module.def("get_information", &get_information, "Get Information from a recording file", pybind11::arg("path"));

    module.attr("version") = pybind11::str(utils::version());


    std::vector<std::pair<std::string, u32>> properties_vector{
        { "height", grid::height_in_tiles },
        {  "width",  grid::width_in_tiles }
    };

    pybind11::dict grid_properties{};

    for (const auto& [key, value] : properties_vector) {
        grid_properties[pybind11::str(key)] = pybind11::int_(value);
    }

    pybind11::dict properties{};


    properties[pybind11::str("grid_properties")] = grid_properties;

    module.attr("properties") = properties;


    pybind11::register_local_exception_translator([](std::exception_ptr p) -> void {
        try {
            if (p)
                std::rethrow_exception(p);
        } catch (const std::system_error& err) {
            const int errornum = err.code().value();
            PyErr_SetObject(PyExc_IOError, pybind11::make_tuple(errornum, err.what()).ptr());
            return;
        } catch (const FileNotFoundError& err) {

            std::string error = "File '";
            error += err.file();
            error += "' doesn't exist!";

            PyErr_SetObject(PyExc_FileNotFoundError, pybind11::str(error).ptr());
            return;
        }
    });
}
