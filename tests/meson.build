c = run_command('../grabber.sh', check: true)
sources = c.stdout().strip().split('\n')

testexe = executable('testexe',sources, include_directories: inc, build_by_default: false)
test('Rouge Galaxy Engine Test', testexe)
