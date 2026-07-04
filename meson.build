project(
	'bloodlustEngine',
	'cpp',
	default_options: {
		'buildtype': 'debug',
		'b_ndebug': 'if-release',
		'cpp_std': 'c++20',
		'warning_level': '3',
	},
	meson_version: '>=1.2.0',
	version: '0.0.0',
)

bl_deps = [dependency('sdl3'), dependency('sdl3-ttf')]

bl_headers_main = [
	'include/app.hpp',
	'include/assets.hpp',
	'include/audio.hpp',
	'include/camera.hpp',
	'include/geometry.hpp',
	'include/l10n.hpp',
	'include/logging.hpp',
	'include/math.hpp',
	'include/scene.hpp',
	'include/util.hpp',
	'include/world.hpp',
]

bl_headers_datatype = [
	'include/datatype/collisionGrid.hpp',
	'include/datatype/colour.hpp',
	'include/datatype/font.hpp',
	'include/datatype/inputEvent.hpp',
	'include/datatype/point.hpp',
	'include/datatype/raycastHit.hpp',
	'include/datatype/soundChannel.hpp',
	'include/datatype/soundClip.hpp',
	'include/datatype/texture.hpp',
	'include/datatype/uiPoint.hpp',
	'include/datatype/vector2.hpp',
]

bl_headers_object = [
	'include/object/collider.hpp',
	'include/object/entity.hpp',
	'include/object/gameObject.hpp',
	'include/object/uiElement.hpp',
	'include/object/uiLabel.hpp',
	'include/object/uiRect.hpp',
	'include/object/uiSprite.hpp',
]

bl_headers_render = [
	'include/render/graphics.hpp',
	'include/render/lineShader.hpp',
	'include/render/rectFillShader.hpp',
	'include/render/shaderProgram.hpp',
	'include/render/shaderUtil.hpp',
	'include/render/textShader.hpp',
	'include/render/textureShader.hpp',
	'include/render/uiManager.hpp',
	'include/render/window.hpp',
]

bl_sources = [
	'src/datatype/collisionGrid.cpp',
	'src/datatype/font.cpp',
	'src/datatype/inputEvent.cpp',
	'src/datatype/soundChannel.cpp',
	'src/datatype/soundClip.cpp',
	'src/datatype/texture.cpp',

	'src/object/collider.cpp',
	'src/object/uiElement.cpp',
	'src/object/uiLabel.cpp',
	'src/object/uiRect.cpp',
	'src/object/uiSprite.cpp',

	'src/render/graphics.cpp',
	'src/render/lineShader.cpp',
	'src/render/rectFillShader.cpp',
	'src/render/shaderProgram.cpp',
	'src/render/shaderUtil.cpp',
	'src/render/textShader.cpp',
	'src/render/textureShader.cpp',
	'src/render/uiManager.cpp',
	'src/render/window.cpp',

	'src/app.cpp',
	'src/assets.cpp',
	'src/audio.cpp',
	'src/camera.cpp',
	'src/l10n.cpp',
	'src/logging.cpp',
	'src/scene.cpp',
	'src/world.cpp',
]

build_args = []
if meson.get_compiler('cpp').get_id() == 'gcc' or meson.get_compiler('cpp').get_id() == 'clang'
	build_args += [
		'-Wdeprecated',
		'-Wconversion',
		'-Wimplicit-fallthrough',
		'-Wmissing-prototypes',
		'-Wmissing-variable-declarations',
		'-Wredundant-parens',
		'-Wswitch-default',
		'-Wunaligned-access',
		'-Wunreachable-code-aggressive',
	]
endif

buildtype = get_option('buildtype')
if buildtype == 'release'
	overrides = {'werror': true}
else
	overrides = {}
endif
public_headers = include_directories('include')

run_command('python', 'shaderComp.py', check: true)

bl_lib = library(
	'bloodlustEngine',
	sources: bl_sources,
	include_directories: public_headers,
	dependencies: bl_deps,
	override_options: overrides,
	c_args: build_args,
	install: true,
)

bl_dep = declare_dependency(
	include_directories: public_headers,
	link_with: bl_lib,
)
set_variable(meson.project_name() + '_dep', bl_dep)
meson.override_dependency('bloodlustEngine', bl_dep)
install_headers(bl_headers_main, subdir: meson.project_name())
install_headers(bl_headers_datatype, subdir: meson.project_name() / 'datatype')
install_headers(bl_headers_object, subdir: meson.project_name() / 'object')
install_headers(bl_headers_render, subdir: meson.project_name() / 'render')

pkg = import('pkgconfig')
pkg.generate(
	name: meson.project_name(),
	filebase: meson.project_name(),
	description: 'bloodlustEngine',
	subdirs: meson.project_name(),
	libraries: bl_lib,
)
