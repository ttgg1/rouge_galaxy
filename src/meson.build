c = run_command('../grabber.sh', check: true)
sources = c.stdout().strip().split('\n')

e = executable('rouge_galaxy',sources,
      win_subsystem: 'windows',
      dependencies: [m_dep,sdl2_dep,sdl2_ttf_dep,sdl2_main_dep]
)

#remove main so that test can be an executable too
sources_objs = []
foreach item : sources
  if item not in ['main.c']
    sources_objs += item
  endif
endforeach

this_dep = declare_dependency(dependencies: [m_dep,sdl2_dep,sdl2_ttf_dep], include_directories: inc,objects: e.extract_objects(sources_objs))
