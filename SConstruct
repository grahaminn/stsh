env = DefaultEnvironment(CCFLAGS = ['-Wall', '-std=c11','-g'])
env['CPPPATH'] = ['include/mpc']
env.ParseConfig("pkg-config apr-1 --cflags --libs")
env.ParseConfig("pkg-config libedit --cflags --libs")
Library(target = 'libs/mpc', source = ['include/mpc/mpc.c'])
Program('stsh',Glob('src/*.c'), LIBS = ['apr-1', 'libedit', 'mpc'], LIBPATH='libs')
