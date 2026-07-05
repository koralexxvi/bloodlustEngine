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

run_command('python', 'shaderComp.py', check: true)

bl_headers = [
	'include/bloodlustEngine/datatype/collisionGrid.hpp',
	'include/bloodlustEngine/datatype/colour.hpp',
	'include/bloodlustEngine/datatype/font.hpp',
	'include/bloodlustEngine/datatype/inputEvent.hpp',
	'include/bloodlustEngine/datatype/point.hpp',
	'include/bloodlustEngine/datatype/raycastHit.hpp',
	'include/bloodlustEngine/datatype/soundChannel.hpp',
	'include/bloodlustEngine/datatype/soundClip.hpp',
	'include/bloodlustEngine/datatype/texture.hpp',
	'include/bloodlustEngine/datatype/uiPoint.hpp',
	'include/bloodlustEngine/datatype/vector2.hpp',

	'include/bloodlustEngine/object/collider.hpp',
	'include/bloodlustEngine/object/entity.hpp',
	'include/bloodlustEngine/object/gameObject.hpp',
	'include/bloodlustEngine/object/uiElement.hpp',
	'include/bloodlustEngine/object/uiLabel.hpp',
	'include/bloodlustEngine/object/uiRect.hpp',
	'include/bloodlustEngine/object/uiSprite.hpp',

	'include/bloodlustEngine/render/graphics.hpp',
	'include/bloodlustEngine/render/lineShader.hpp',
	'include/bloodlustEngine/render/rectFillShader.hpp',
	'include/bloodlustEngine/render/shaderProgram.hpp',
	'include/bloodlustEngine/render/shaderUtil.hpp',
	'include/bloodlustEngine/render/textShader.hpp',
	'include/bloodlustEngine/render/textureShader.hpp',
	'include/bloodlustEngine/render/uiManager.hpp',
	'include/bloodlustEngine/render/window.hpp',

	'include/bloodlustEngine/render/shaderSrc/lineFill.vert.hpp',
	'include/bloodlustEngine/render/shaderSrc/primitiveFill.vert.hpp',
	'include/bloodlustEngine/render/shaderSrc/solidColour.frag.hpp',
	'include/bloodlustEngine/render/shaderSrc/textRender.frag.hpp',
	'include/bloodlustEngine/render/shaderSrc/textRender.vert.hpp',
	'include/bloodlustEngine/render/shaderSrc/textureDraw.frag.hpp',
	'include/bloodlustEngine/render/shaderSrc/textureDraw.vert.hpp',

	'include/bloodlustEngine/app.hpp',
	'include/bloodlustEngine/assets.hpp',
	'include/bloodlustEngine/audio.hpp',
	'include/bloodlustEngine/camera.hpp',
	'include/bloodlustEngine/geometry.hpp',
	'include/bloodlustEngine/l10n.hpp',
	'include/bloodlustEngine/logging.hpp',
	'include/bloodlustEngine/math.hpp',
	'include/bloodlustEngine/scene.hpp',
	'include/bloodlustEngine/util.hpp',
	'include/bloodlustEngine/world.hpp',
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
install_headers(bl_headers, subdir: meson.project_name(), preserve_path: true)

pkg = import('pkgconfig')
pkg.generate(
	name: meson.project_name(),
	filebase: meson.project_name(),
	description: 'bloodlustEngine',
	subdirs: meson.project_name(),
	libraries: bl_lib,
)
