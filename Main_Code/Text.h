#include "AssetManager.h"
#include "GameObject.h"
#include "Camera.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~21

class Text
{
	// This class is used to setup freetype and render text to screen.
	// The Character struct allows us to group data elements together under one name.
	// Setup was influenced by and sourced from: https://www.freetype.org/
public:
	// Accessible Externally
	Text::Text();
	void Text::SetupFreeType(AssetShader &textShader, glm::mat4 *projection, int WIDTH, int HEIGHT);
	void Text::RenderTextScene(AssetShader &textShader, int currentCamera, std::string fps);
	void Text::RenderEditTextScene(AssetShader &textShader, GameObject* gameObject, int editModeIndex, int gameObjectIndex, glm::vec3 customScriptVector);
	void Text::EditModeText(string str, int editModeIndex, int i, AssetShader &textShader);

private:
	// Accessible Internally
	void Text::RenderText(AssetShader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 colour);

	Shader shader;
	struct Character {
		GLuint GlyphTextureID;
		glm::ivec2 glyphSize;
		glm::ivec2 baselineOffset;
		GLuint horizontalOffset;
	};
	std::map<GLchar, Character> Characters;
	GLuint	textVertexArrayObject = 0, textVertexBufferObject;
	glm::mat4 *view, *projection, model;
	Camera cam;
};



