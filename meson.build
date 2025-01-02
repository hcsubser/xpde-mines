project(
	'xpde-minesweeper',
	'c',
	version: '0.1.0',
	license: 'MIT',
	meson_version: '>=0.47.0',
	default_options: [
		'c_std=c11',
		'warning_level=3',
		'werror=true',
	],
)


cc = meson.get_compiler('c')

add_project_arguments(cc.get_supported_arguments([
'-Wno-unused-parameter',
'-Wno-unused-variable',
'-Wno-unused-but-set-variable',
#'-Wno-implicit-function-declaration',
'-Wno-pedantic',
#'-Wno-deprecated-declarations'
]), language: 'c')


gtk3        = dependency('gtk+-3.0')
pthread_dep = cc.find_library('pthread', required: true)



subdir('src')
