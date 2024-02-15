project('rouge_galaxy', 'c', version: '0.0.1', license: 'GPLv3')
cc = meson.get_compiler('c')

# dependencies
m_dep = cc.find_library('m', required : false)
sdl2_dep = dependency('SDL2', required : true)
sdl2_ttf_dep = dependency('SDL2_ttf', required : true)

curses = dependency('ncurses', required: false, disabler: true)
if not curses.found()
  curses_root = get_option('curses_root')
  curses_lib = cc.find_library('curses', dirs : curses_root, required : false, disabler: true)
  curses = declare_dependency(include_directories: curses_root, dependencies: curses_lib)
endif

add_project_dependencies([m_dep,curses,sdl2_dep,sdl2_ttf_dep],language: 'c')

inc = include_directories('src')
add_project_arguments('-pedantic', language:'c')

subdir('src')
subdir('tests')
