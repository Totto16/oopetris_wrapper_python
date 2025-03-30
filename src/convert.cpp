

#include "./convert.hpp"

#include <limits>


static pybind11::object information_value_to_python(const recorder::InformationValue& information_value) {

    return std::visit(
            helper::Overloaded{ [](const std::string& value) -> pybind11::object { return pybind11::str(value); },
                                [](const float& value) -> pybind11::object { return pybind11::float_(value); },
                                [](const double& value) -> pybind11::object { return pybind11::float_(value); },
                                [](const bool& value) -> pybind11::object { return pybind11::bool_(value); },
                                [](const u8& value) -> pybind11::object { return pybind11::int_(value); },
                                [](const i8& value) -> pybind11::object { return pybind11::int_(value); },
                                [](const u32& value) -> pybind11::object { return pybind11::int_(value); },
                                [](const i32& value) -> pybind11::object { return pybind11::int_(value); },
                                [](const u64& value) -> pybind11::object { return pybind11::int_(value); },
                                [](const i64& value) -> pybind11::object { return pybind11::int_(value); },
                                [](const std::vector<recorder::InformationValue>& values) -> pybind11::object {
                                    pybind11::list array{};

                                    for (auto& value : values) {
                                        array.append(value);
                                    }

                                    return array;
                                } },
            information_value.underlying()
    );
}

static pybind11::dict information_to_python(const recorder::AdditionalInformation& information) {

    pybind11::dict result{};

    for (const auto& [key, raw_value] : information) {
        auto value = information_value_to_python(raw_value);
        result[pybind11::str(key)] = value;
    }

    return result;
}

const char* event_to_string(InputEvent event) {
    switch (event) {
        case InputEvent::RotateLeftPressed:
            return "RotateLeftPressed";
        case InputEvent::RotateRightPressed:
            return "RotateRightPressed";
        case InputEvent::MoveLeftPressed:
            return "MoveLeftPressed";
        case InputEvent::MoveRightPressed:
            return "MoveRightPressed";
        case InputEvent::MoveDownPressed:
            return "MoveDownPressed";
        case InputEvent::DropPressed:
            return "DropPressed";
        case InputEvent::HoldPressed:
            return "HoldPressed";
        case InputEvent::RotateLeftReleased:
            return "RotateLeftReleased";
        case InputEvent::RotateRightReleased:
            return "RotateRightReleased";
        case InputEvent::MoveLeftReleased:
            return "MoveLeftReleased";
        case InputEvent::MoveRightReleased:
            return "MoveRightReleased";
        case InputEvent::MoveDownReleased:
            return "MoveDownReleased";
        case InputEvent::DropReleased:
            return "DropReleased";
        case InputEvent::HoldReleased:
            return "HoldReleased";
        default:
            throw new std::runtime_error("UNREACHABLE");
    }
}

static inline pybind11::str event_to_python_string(InputEvent event) {
    pybind11::str(event_to_string(event));
}


static pybind11::dict record_to_python(const recorder::Record& record) {

    pybind11::dict result{};

    auto python_event = event_to_python_string(record.event);

    auto python_simulation_step_index = pybind11::int_(record.simulation_step_index);

    auto python_tetrion_index = pybind11::int_(record.tetrion_index);

    std::vector<std::pair<std::string, pybind11::object>> properties_vector{
        {                 "event",                 python_event },
        { "simulation_step_index", python_simulation_step_index },
        {         "tetrion_index",         python_tetrion_index }
    };

    for (const auto& [key, value] : properties_vector) {
        result[pybind11::str(key)] = value;
    }

    return result;
}

static pybind11::list records_to_python(const std::vector<recorder::Record>& records) {
    pybind11::list array{};

    for (auto& record : records) {
        array.append(record_to_python(record));
    }

    return array;
}


static pybind11::dict header_to_python(const recorder::TetrionHeader& header) {

    pybind11::dict result{};

    auto python_seed = pybind11::int_(header.seed);

    auto python_starting_level = pybind11::int_(header.starting_level);

    std::vector<std::pair<std::string, pybind11::object>> properties_vector{
        {           "seed",           python_seed },
        { "starting_level", python_starting_level },
    };

    for (const auto& [key, value] : properties_vector) {
        result[pybind11::str(key)] = value;
    }

    return result;
}


static pybind11::list headers_to_python(const std::vector<recorder::TetrionHeader>& headers) {
    pybind11::list array{};

    for (auto& header : headers) {
        array.append(header_to_python(header));
    }

    return array;
}


static pybind11::dict mino_position_to_python(const grid::GridPoint& mino_position) {

    pybind11::dict result{};

    auto mino_pos = mino_position.cast<uint8_t>();

    auto python_x = pybind11::int_(mino_pos.x);

    auto python_y = pybind11::int_(mino_pos.y);

    std::vector<std::pair<std::string, pybind11::object>> properties_vector{
        { "x", python_x },
        { "y", python_y }
    };

    for (const auto& [key, value] : properties_vector) {
        result[pybind11::str(key)] = value;
    }

    return result;
}


const char* tetromino_type_to_string(helper::TetrominoType type) {
    switch (type) {
        case helper::TetrominoType::I: {
            return "I";
        }
        case helper::TetrominoType::J: {
            return "J";
        }
        case helper::TetrominoType::L: {
            return "L";
        }
        case helper::TetrominoType::O: {
            return "O";
        }
        case helper::TetrominoType::S: {
            return "S";
        }
        case helper::TetrominoType::T: {
            return "T";
        }
        case helper::TetrominoType::Z: {
            return "Z";
        }
        default:
            throw new std::runtime_error("UNREACHABLE");
    }
}

static inline pybind11::str tetromino_type_to_python_string(helper::TetrominoType type) {
    return pybind11::str(tetromino_type_to_string(type));
}


static pybind11::dict mino_to_python(const Mino& mino) {

    pybind11::dict result{};

    auto python_position = mino_position_to_python(mino.position());

    auto python_type = tetromino_type_to_python_string(mino.type());

    std::vector<std::pair<std::string, pybind11::object>> properties_vector{
        { "position", python_position },
        {     "type",     python_type }
    };

    for (const auto& [key, value] : properties_vector) {
        result[pybind11::str(key)] = value;
    }

    return result;
}


static pybind11::list mino_stack_to_python(const std::vector<Mino>& mino_stack) {
    pybind11::list array{};

    for (auto& mino : mino_stack) {
        array.append(mino_to_python(mino));
    }

    return array;
}


static pybind11::dict snapshot_to_python(const TetrionSnapshot& snapshot) {

    pybind11::dict result{};

    auto python_level = pybind11::int_(snapshot.level());

    auto python_lines_cleared = pybind11::int_(snapshot.lines_cleared());

    auto python_mino_stack = mino_stack_to_python(snapshot.mino_stack().minos());

    auto python_score = pybind11::int_(snapshot.score());

    auto python_simulation_step_index = pybind11::int_(snapshot.simulation_step_index());

    auto python_tetrion_index = pybind11::int_(snapshot.tetrion_index());

    std::vector<std::pair<std::string, pybind11::object>> properties_vector{
        {                 "level",                 python_level },
        {         "lines_cleared",         python_lines_cleared },
        {            "mino_stack",            python_mino_stack },
        {                 "score",                 python_score },
        { "simulation_step_index", python_simulation_step_index },
        {         "tetrion_index",         python_tetrion_index },
    };

    for (const auto& [key, value] : properties_vector) {
        result[pybind11::str(key)] = value;
    }

    return result;
}


static pybind11::list snapshots_to_python(const std::vector<TetrionSnapshot>& snapshots) {
    pybind11::list array{};


    for (auto& snapshot : snapshots) {
        array.append(snapshot_to_python(snapshot));
    }

    return array;
}


pybind11::dict recording_reader_to_python(const recorder::RecordingReader& reader) {

    pybind11::dict result{};

    pybind11::int_ python_version{ recorder::Recording::current_supported_version_number };

    auto python_information = information_to_python(reader.information());

    auto python_records = records_to_python(reader.records());

    auto python_snapshots = snapshots_to_python(reader.snapshots());

    auto python_tetrion_headers = headers_to_python(reader.tetrion_headers());


    std::vector<std::pair<std::string, pybind11::object>> properties_vector{
        {     "information",     python_information },
        {         "records",         python_records },
        {       "snapshots",       python_snapshots },
        { "tetrion_headers", python_tetrion_headers },
        {         "version",         python_version }
    };

    for (const auto& [key, value] : properties_vector) {
        result[pybind11::str(key)] = value;
    }

    return result;
}
