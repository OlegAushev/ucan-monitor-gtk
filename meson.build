project('ucan-monitor', ['c', 'vala', 'cpp'],
	version: '0.1.0',
	meson_version: '>= 0.59.0',
	default_options: [
		'c_std=c11',
		'cpp_std=c++20',
		'warning_level=3',
		'buildtype=release'
	],
)

# Git Info
git_describe_res = run_command(['git', 'describe', '--tags', '--long', '--always'], capture : true, check : true)
git_describe_str = git_describe_res.stdout()
git_describe_str = git_describe_str.replace('\n', '')

git_hash_res = run_command(['git', 'log', '-1', '--format=%h'], capture : true, check : true)
git_hash_str = git_hash_res.stdout()
git_hash_str = git_hash_str.replace('\n', '')

git_branch_res = run_command(['git', 'rev-parse', '--abbrev-ref', 'HEAD'], capture : true, check : true)
git_branch_str = git_branch_res.stdout()
git_branch_str = git_branch_str.replace('\n', '')

git_diff_res = run_command(['bash', '-c', 'git diff --quiet --exit-code || echo +'], capture : true, check : true)
git_diff_str = git_diff_res.stdout()
git_diff_str = git_diff_str.replace('\n', '')

git_version_cpp = configure_file(
	input : 'cmake/check-git/git_version.cpp.in',
	output : 'git_version.cpp',
	configuration : {
		'GIT_DESCRIBE' : git_describe_str,
		'GIT_HASH' : git_hash_str,
		'GIT_BRANCH' : git_branch_str,
		'GIT_DIFF' : git_diff_str
	}
)

#
gnome = import('gnome')

# Sources
app_sources = files(
	'builddir/git_version.cpp',

	'gui/components/logtextview.vala',
	'gui/components/spinbuttonscale.vala',
	'gui/components/tableboolentry.vala',
	'gui/components/tableentry.vala',

	'gui/template/template_widget.vala',
	
	'gui/application.vala',
	'gui/main.vala',
	'gui/srmdrive_controlpanel.vala',
	'gui/srmdrive_datatables.vala',
	'gui/window_canbuspreferences.vala',
	'gui/window.vala',


	'src/main.cpp',

	'src/cansocket/cansocket.cpp',

	'src/logger/logger.cpp',

	'src/ucanopen/client/ucanopen_client.cpp',
	'src/ucanopen/server/ucanopen_server.cpp',

	'src/srmdrive/server/srmdrive_server.cpp',
	'src/srmdrive/server/srmdrive_objectdictionary.cpp',
	'src/srmdrive/controller/srmdrive_controller.cpp',
	'src/srmdrive/observer/srmdrive_observer.cpp',

	'src/api/cansocket.cpp',
	'src/api/ucanopen_client.cpp',
	'src/api/srmdrive.cpp',
)

subdir('gui/advvchart')

app_sources += gnome.compile_resources('app_resources',
	'gresource.xml',
	c_name: 'ucan_monitor'
)

# Includes
app_incdirs = include_directories('src')

# Dependencies
app_deps = [
	dependency('gtk4',		version: '>= 4.5.0'),
	dependency('libadwaita-1',	version: '>= 1.0'),
]

# Executable
executable('ucan-monitor', app_sources,
	include_directories: app_incdirs,
	dependencies: app_deps, export_dynamic: true,
	#cpp_args : '-DSTD_COUT_ENABLED',
)


