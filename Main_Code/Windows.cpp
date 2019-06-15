#include "Windows.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~35

// Windows constructor - initialises bool isWindowsSetup to false when created - lazy initialisation
Windows::Windows()
{
	isWindowsSetup = false;
}

// Draw method for the windows .cpp class - taking an assetshader and camera object as params.
// After setting our shader to as well as the attibutes to be sent to the shaders, we disable face culling (as we want back faces drawn),
// bind the vertex array of the window, and bind the translucent window texture. We then iterate using a reverse iterator (an iterator adaptor that reverses the direction of a given iterator).
// Essentially we iterate over a map, with a float/glm::vec3 and relevant iterator, which iterates over the sorted windows (in reverse - we need to preserve order i.e. draw furthest windows first).
// We then create our model matrix, scale and translate via the update method. The arrays are then drawn, and the vertex array unbinded.
void Windows::Draw(AssetShader &windowShader, Camera cam, glm::mat4 &projection) {
	windowShader.use();
	view = cam.GetViewMatrix();
	windowShader.setMat4("view", view);
	windowShader.setMat4("projection", projection);

	glDisable(GL_CULL_FACE);
	quad.InitTransparentPlane();
	glBindVertexArray(quad.transparentVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("transparentTexture"));


	for (GLuint i = 0; i < translucentWindows.size(); i++)
	{
		Update(i);
		windowShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glEnable(GL_CULL_FACE);
	glBindVertexArray(0);
}


// Update method for updating and translating the windows.
void Windows::Update(GLuint i)
{
	model = glm::mat4();
	model = glm::scale(model, glm::vec3(2));
	model = glm::translate(model, translucentWindows[i]);
}

// Ee iterate over all our windows and update their order accordingly.
void Windows::SortWindows(Camera cam)
{
	// Iterate over all windows and sort them accordingly.
	for (GLuint i = 0; i < translucentWindows.size(); i++)
	{
		GLfloat distance = glm::length(cam.GetPosition() - translucentWindows[i]);
		sorted[distance] = translucentWindows[i];
	}

}

// This method allows us to do first time initialisation on the windows, pushing them back with a small offset so that there is no discrepency when drawing.
void Windows::SetupWindows()
{
	translucentWindows.push_back(glm::vec3(-3.5f, 0.25f, 3.5001f));
	translucentWindows.push_back(glm::vec3(-2.5f, 0.25f, 3.5002f));
	translucentWindows.push_back(glm::vec3(-1.5f, 0.25f, 3.5004f));
	translucentWindows.push_back(glm::vec3(-0.5f, 0.25f, 3.5005f));
	translucentWindows.push_back(glm::vec3(0.5f, 0.25f, 3.5006f));
	translucentWindows.push_back(glm::vec3(1.5f, 0.25f, 3.5007f));
	translucentWindows.push_back(glm::vec3(2.5f, 0.25f, 3.5008f));
}