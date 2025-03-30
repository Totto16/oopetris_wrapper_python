src_files = files('convert.cpp', 'convert.hpp', 'recordings.cpp')

python_sources = files('__init__.py', 'recordings.pyi')

py.install_sources(
    python_sources,
    subdir: 'oopetris',
)

oopetris_python_wrapper_module = py.extension_module(
    'recordings',
    src_files,
    install: true,
    dependencies: deps,
    subdir: 'oopetris',
)
