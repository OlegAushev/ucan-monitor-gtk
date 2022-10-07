project('ucan-monitor', ['c', 'vala', 'cpp'],
	version: '0.1.0',
	meson_version: '>= 0.59.0',
	default_options: [
		'c_std=c11',
		'cpp_std=c++20'
	],
)


gnome = import('gnome')

app_sources = files(
	'src/gui/application.vala',
	'src/gui/control_panel.vala',
	'src/gui/data_tables.vala',
	'src/gui/main.vala',
	'src/gui/window_canbusprefs.vala',
	'src/gui/window.vala',
	'src/gui/components/slider_with_text.vala',
	'src/gui/components/table_entry.vala',


	'src/main.cpp',

	'src/cansocket/cansocket.cpp',

	'src/ucanopen/ucanopen_client.cpp',
	'src/ucanopen/objectdictionary/objectdictionary.cpp',
	'src/ucanopen/ucanopen_servernode/ucanopen_servernode.cpp',

	'src/motordrive/controller/motordrive_controller.cpp',
	'src/motordrive/observer/motordrive_observer.cpp',

	'src/c_wrappers/c_cansocket.cpp',
	'src/c_wrappers/c_motordrive_controller.cpp',
	'src/c_wrappers/c_motordrive_observer.cpp',
	'src/c_wrappers/c_ucanopen.cpp',
)

subdir('src/gui/advvchart')

app_incdirs = include_directories('src')

app_deps = [
	dependency('gtk4',		version: '>= 4.5.0'),
	dependency('libadwaita-1',	version: '>= 1.0'),
]

app_sources += gnome.compile_resources('app_resources',
	'gresource.xml',
	c_name: 'ucan_monitor'
)

executable('ucan-monitor', app_sources,
	include_directories: app_incdirs,
	dependencies: app_deps, export_dynamic: true,
	cpp_args : '-DSTD_COUT_ENABLED',
)
