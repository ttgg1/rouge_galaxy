project('rouge_galaxy', 'c', version: '0.0.1', license: 'GPLv3')
cc = meson.get_compiler('c')

# dependencies
m_dep = cc.find_library('m', required : false)

add_project_dependencies(m_dep,language: 'c')

inc = include_directories('src')
add_project_arguments('-pedantic', language:'c')

subdir('src')
subdir('tests')
