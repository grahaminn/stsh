env = DefaultEnvironment(CCFLAGS = ['-Wall', '-std=c11'])
env['CPPPATH'] = ['include/mpc']
env.ParseConfig("pkg-config apr-1 --cflags --libs")
env.ParseConfig("pkg-config libedit --cflags --libs")

if int(ARGUMENTS.get('debug', 0)):
	env.Append(CCFLAGS = '-g')

Library(target = 'libs/mpc', source = ['include/mpc/mpc.c'])
testEnv = env.Clone()
env.VariantDir('build', 'src', duplicate=0)
files = env.Glob('build/*.c')
env.Program('stsh', files, LIBS = ['apr-1', 'libedit', 'mpc'], LIBPATH='libs')

testEnv.ParseConfig("pkg-config check --cflags --libs")
testEnv.VariantDir('buildtests', '.', duplicate=0)
testfiles = ['buildtests/src/environment.c','buildtests/src/cell.c','buildtests/tst/test_cell.c','buildtests/tst/test_environment.c','buildtests/tst/tests.c']

suite = testEnv.Program('tests', testfiles, LIBS = ['apr-1', 'libedit', 'mpc', 'check'], LIBPATH='libs')


