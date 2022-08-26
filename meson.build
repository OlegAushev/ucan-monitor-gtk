project('ucan-monitor', 'cpp',
	version: '0.1.0',
	meson_version: '>= 0.59.0',
	default_options: [
	],
)

gnome = import('gnome')

app_sources = [
	'src/main.cpp',
	#'gui/app_window.cpp'
]

app_deps = [
	dependency('libadwaita-1', version: '>= 1.0'),
]

#app_sources += gnome.compile_resources('app_resources',
#	'gresource.xml',
#	c_name: 'ucan_monitor'
#)

executable('ucan-monitor', app_sources,
	dependencies: app_deps,
)
