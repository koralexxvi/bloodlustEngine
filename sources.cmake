set(
	SOURCES
	src/app.cpp src/assets.cpp src/audio.cpp src/camera.cpp src/l10n.cpp src/logging.cpp
	src/memory.cpp src/scene.cpp src/world.cpp

	src/datatype/collisionGrid.cpp src/datatype/font.cpp src/datatype/inputEvent.cpp
	src/datatype/soundChannel.cpp src/datatype/soundClip.cpp src/datatype/texture.cpp

	src/object/collider.cpp src/object/entity.cpp src/object/uiElement.cpp src/object/uiLabel.cpp
	src/object/uiRect.cpp src/object/uiSprite.cpp

	src/render/graphics.cpp src/render/lineShader.cpp src/render/rectFillShader.cpp
	src/render/shaderProgram.cpp src/render/shaderUtil.cpp src/render/textShader.cpp
	src/render/textureShader.cpp src/render/uiManager.cpp src/render/window.cpp
)

set(
	HEADERS_MAIN
	include/app.hpp include/assets.hpp include/audio.hpp include/camera.hpp include/geometry.hpp
	include/l10n.hpp include/logging.hpp include/math.hpp include/memory.hpp include/scene.hpp
	include/util.hpp include/world.hpp
)

set(
	HEADERS_DATATYPE
	include/datatype/collisionGrid.hpp include/datatype/colour.hpp include/datatype/font.hpp
	include/datatype/inputEvent.hpp include/datatype/raycastHit.hpp
	include/datatype/soundChannel.hpp include/datatype/soundClip.hpp include/datatype/texture.hpp
	include/datatype/uiPoint.hpp include/datatype/vector2.hpp
)

set(
	HEADERS_OBJECT
	include/object/collider.hpp include/object/entity.hpp include/object/gameObject.hpp
	include/object/uiElement.hpp include/object/uiLabel.hpp include/object/uiRect.hpp
	include/object/uiSprite.hpp
)

set(
	HEADERS_RENDER
	include/render/graphics.hpp include/render/lineShader.hpp include/render/rectFillShader.hpp
	include/render/shaderProgram.hpp include/render/shaderUtil.hpp include/render/textShader.hpp
	include/render/textureShader.hpp include/render/uiManager.hpp include/render/window.hpp
)
