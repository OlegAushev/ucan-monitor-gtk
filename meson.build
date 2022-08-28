project('ucan-monitor', 'c',
	version: '0.1.0',
	meson_version: '>= 0.59.0',
	default_options: [
	],
)


gnome = import('gnome')

app_sources = [
	'src/main.c',
	'gui/ucan_monitor_application.c',
	'gui/ucan_monitor_window.c'
]

app_deps = [
	dependency('gtk4',		version: '>= 4.5.0'),
	dependency('libadwaita-1',	version: '>= 1.0'),
]

app_sources += gnome.compile_resources('app_resources',
	'gresource.xml',
	c_name: 'ucan_monitor'
)

executable('ucan-monitor', app_sources,
	dependencies: app_deps,
)
