project('rouge_galaxy', 'c', version: '0.0.1', license: 'GPLv3',
        default_options: ['default_library=static','buildtype=debug','warning_level=everything'])
cc = meson.get_compiler('c')

# dependencies
m_dep = cc.find_library('m', required : false)
#sdl2_dep = dependency('SDL2', required : true)
#sdl2_ttf_dep = dependency('SDL2_ttf', required : true)
#sdl2_main_dep = dependency('sdl2main', required: false) # only required on Windows

raydep = dependency('raylib',required : true)
#add_project_dependencies([m_dep,sdl2_dep,sdl2_ttf_dep,sdl2_main_dep],language: 'c')

inc = include_directories('src')

subdir('src')
subdir('tests')
