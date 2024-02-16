c = run_command('../grabber.sh', check: true)
sources = c.stdout().strip().split('\n')

testexe = executable('testexe',sources, build_by_default: false, dependencies: this_dep)
test('Rouge Galaxy Engine Test', testexe)
