project(
    'oopetris_python_wrapper',
    'cpp',
    meson_version: '>=1.3.0',
    default_options: {
        'buildtype': 'debug',
        'optimization': '3',
        'strip': true,
        'cpp_std': ['c++23', 'c++latest', 'vc++latest', 'c++20'],
        'b_ndebug': 'if-release',

    },
    version: '0.5.6',
)

deps = []

deps += dependency('oopetris-recordings')

py = import('python').find_installation(pure: false)
deps += dependency('pybind11')

src_files = []

subdir('src')

oopetris_python_wrapper_module = py.extension_module('oopetris', src_files, install: true, dependencies: deps)
