#include "Text.h"

// Created by Jonathan McChesney - Setup func was sourced from - https://www.freetype.org/
// Jonathan McChesney LOC : ~160

// Text Constructor
Text::Text() 
{
}

// This method allows us to setup freetype, using our custom asset shaders and logger.
void Text::SetupFreeType(AssetShader &textShader, glm::mat4 *projection, int WIDTH, int HEIGHT) {
	*projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
	textShader.use();
	textShader.setMat4("projection", *projection);


	FT_Library freeType;
	if (FT_Init_FreeType(&freeType))
		Logging::logFreetypeError("Could not initialize the FreeType Library", __FILE__, __LINE__);

	FT_Face glyphFace;
	if (FT_New_Face(freeType, "res/images/arial.ttf", 0, &glyphFace))
		Logging::logFreetypeError("Failed to load font", __FILE__, __LINE__);

	FT_Set_Pixel_Sizes(glyphFace, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte omega = 0; omega < 128; omega++)
	{
		if (FT_Load_Char(glyphFace, omega, FT_LOAD_RENDER))
		{
			Logging::logFreetypeError("Failed to load glyph", __FILE__, __LINE__);
			continue;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			glyphFace->glyph->bitmap.width,
			glyphFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			glyphFace->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Character character = {
			texture, glm::ivec2(glyphFace->glyph->bitmap.width, glyphFace->glyph->bitmap.rows),
			glm::ivec2(glyphFace->glyph->bitmap_left, glyphFace->glyph->bitmap_top),
			glyphFace->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(omega, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(glyphFace);
	FT_Done_FreeType(freeType);

	glGenVertexArrays(1, &textVertexArrayObject);
	glGenBuffers(1, &textVertexBufferObject);
	glBindVertexArray(textVertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, textVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// This method allows us to render text and set its depth as well as its colour. https://www.freetype.org/
void Text::RenderText(AssetShader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 colour)
{
	shader.use();
	shader.setVec3f("textColor", colour.x, colour.y, colour.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVertexArrayObject);

	std::string::const_iterator bravo;
	for (bravo = text.begin(); bravo != text.end(); bravo++)
	{
		Character character = Characters[*bravo];

		GLfloat xpos = x + character.baselineOffset.x * scale;
		GLfloat ypos = y - (character.glyphSize.y - character.baselineOffset.y) * scale;
		GLfloat h = character.glyphSize.y * scale;
		GLfloat w = character.glyphSize.x * scale;

		GLfloat textVertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		glBindTexture(GL_TEXTURE_2D, character.GlyphTextureID);
		glBindBuffer(GL_ARRAY_BUFFER, textVertexBufferObject);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textVertices), textVertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (character.horizontalOffset >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// This is our text scene for tooltips located on the upper left portion of the screen.
void Text::RenderTextScene(AssetShader &textShader, int currentCamera, std::string fps) {
	if (currentCamera == 0) {
		RenderText(textShader, "Dynamic Camera", 880.0f, 25.0f, 1.0f, glm::vec3(1.0, 0.0f, 0.0f));
	}
	else {
		RenderText(textShader, "Static Camera", 950.0f, 25.0f, 1.0f, glm::vec3(0.0, 0.0f, 1.0f));
	}
	RenderText(textShader, fps.c_str(), 25.0f, 670.0f, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));

	RenderText(textShader, "G : Enter Edit Mode (Must have spawned at least 1 GameObject)", 25.0f, 655.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "W / A / S / D : - Movement [ +Shift : - Walk Mode ]",			25.0f, 640.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "Q / E / R :- Toggle Render Mode",								25.0f, 625.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "Left / Middle / Right Click : - Change Camera",					25.0f, 610.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "C : - Change Cube Texture",										25.0f, 595.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "B : - Change Light Model",										25.0f, 580.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "SPACE : - Toggle Shadows",										25.0f, 565.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "T : - Change Shadow Model",										25.0f, 550.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "Y : - Enable Shadow Debug Quad",								25.0f, 535.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "V : - Toggle Room",												25.0f, 520.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "X : - Toggle Stencil",											25.0f, 505.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "O / P: - Change Parallax Height",								25.0f, 490.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "Arrow Keys: - Change Texture Atlas",							25.0f, 475.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "Z : - Toggle Particle Trail (disables movement when enabled)",  25.0f, 460.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "ENTER : - Toggle No_Clip",										25.0f, 440.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "1 : Spawn Game Object",											25.0f, 420.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	}

void Text::RenderEditTextScene(AssetShader &textShader, GameObject* gameObject, int editModeIndex, int gameObjectIndex, glm::vec3 customScriptVector)
{
	//Help Text
	RenderText(textShader, "G : Exit Edit Mode", 25.0f, 665.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "[ ]  : Previous/Next GameObject", 25.0f, 645.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "Up/Down Arrows : Navigate Options", 25.0f, 625.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "Left/Right Arrows : Adjust Values", 25.0f, 605.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "1/2/3 : Add Translation/Scale/Rotation Script using the below values", 25.0f, 585.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));
	RenderText(textShader, "4/5/6 : Remove Translation/Scale/Rotation Script", 25.0f, 565.0f, 0.23f, glm::vec3(1.0, 1.0f, 1.0f));

	ostringstream oss;
	oss << "Current GameObject : " << gameObjectIndex << "  Tag : " << gameObject->tag;
	RenderText(textShader, oss.str(), 25.0f, 545.0f, 0.23f, glm::vec3(1.0f, 1.0f, 1.0f));
	oss.str("");

	oss << "Script Value X : " << customScriptVector.x;
	EditModeText(oss.str(), editModeIndex, 0, textShader);
	oss.str("");

	oss << "Script Value Y : " << customScriptVector.y;
	EditModeText(oss.str(), editModeIndex, 1, textShader);
	oss.str("");

	oss << "Script Value Z : " << customScriptVector.z;
	EditModeText(oss.str(), editModeIndex, 2, textShader);
	oss.str("");

	//Translation
	oss << "Translation X : " << gameObject->transform.Translation.x;
	EditModeText(oss.str(), editModeIndex, 3, textShader);
	oss.str("");

	oss << "Translation Y : " << gameObject->transform.Translation.y;
	EditModeText(oss.str(), editModeIndex, 4, textShader);
	oss.str("");

	oss << "Translation Z : " << gameObject->transform.Translation.z;
	EditModeText(oss.str(), editModeIndex, 5, textShader);
	oss.str("");

	//Scale
	oss << "Scale X : " << gameObject->transform.Scale.x;
	EditModeText(oss.str(), editModeIndex, 6, textShader);
	oss.str("");

	oss << "Scale Y : " << gameObject->transform.Scale.y;
	EditModeText(oss.str(), editModeIndex, 7, textShader);
	oss.str("");

	oss << "Scale Z : " << gameObject->transform.Scale.z;
	EditModeText(oss.str(), editModeIndex, 8, textShader);
	oss.str("");


	//Rotation
	oss << "Rotation X : " << gameObject->transform.Rotation.x;
	EditModeText(oss.str(), editModeIndex, 9, textShader);
	oss.str("");

	oss << "Rotation Y : " << gameObject->transform.Rotation.y;
	EditModeText(oss.str(), editModeIndex, 10, textShader);
	oss.str("");

	oss << "Rotation Z : " << gameObject->transform.Rotation.z;
	EditModeText(oss.str(), editModeIndex, 11, textShader);
	oss.str("");

}

void Text::EditModeText(string str, int editModeIndex, int i, AssetShader &textShader)
{
	glm::vec3 chosenColour = glm::vec3(1.0f, 0.0f, 0.0f), defaultColour = glm::vec3(1.0f, 1.0f, 1.0f);

	if (editModeIndex == i)
	{
		RenderText(textShader, str, 25.0f, 505.0f - (i * 20), 0.23f, chosenColour);
	}
	else
		RenderText(textShader, str, 25.0f, 505.0f - (i * 20), 0.23f, defaultColour);
}