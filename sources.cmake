set(
	SOURCES
	src/main.cpp src/app.cpp src/assets.cpp src/audio.cpp src/l10n.cpp
	src/logging.cpp src/memory.cpp src/scene.cpp

	src/datatype/collisionGrid.cpp src/datatype/font.cpp
	src/datatype/inputEvent.cpp src/datatype/musicTrack.cpp
	src/datatype/soundClip.cpp src/datatype/texture.cpp

	src/object/circleCollider.cpp src/object/rectCollider.cpp
	src/object/uiElement.cpp src/object/uiLabel.cpp src/object/uiRect.cpp
	src/object/uiSprite.cpp src/object/world.cpp

	src/render/graphics.cpp src/render/lineShader.cpp src/render/rectFillShader.cpp
	src/render/shaderProgram.cpp src/render/shaderUtil.cpp
	src/render/textShader.cpp src/render/textureShader.cpp
	src/render/uiManager.cpp src/render/video.cpp
)

set(
	HEADERS
	include/main.hpp include/app.hpp include/assets.hpp include/audio.hpp
	include/l10n.hpp include/logging.hpp include/memory.hpp include/scene.hpp

	include/datatype/collisionGrid.hpp include/datatype/font.hpp
	include/datatype/inputEvent.hpp include/datatype/musicTrack.hpp
	include/datatype/soundClip.hpp include/datatype/texture.hpp

	include/object/circleCollider.hpp include/object/rectCollider.hpp
	include/object/uiElement.hpp include/object/uiLabel.hpp
	include/object/uiRect.hpp include/object/uiSprite.hpp include/object/world.hpp

	include/render/graphics.hpp include/render/lineShader.hpp
	include/render/rectFillShader.hpp include/render/shaderProgram.hpp
	include/render/shaderUtil.hpp include/render/textShader.hpp
	include/render/textureShader.hpp include/render/uiManager.hpp
	include/render/video.hpp
)
