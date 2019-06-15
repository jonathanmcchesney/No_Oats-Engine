#include "Container.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~90

// Container Constructor
Container::Container()
{
	glm::mat4 projection = projection;
}

// Lengthy method for setting the various textures associated with the moving cube.
void Container::SetContainerImage(GLboolean default, int imageInt) 
{
	// Setting the diffuse map - when the program is fist run the defauly texture will be used and image int
	// will be instantiated to 0. Each image was created/edited within Photoshop.
	if (default == true)
	{
		imageMap = SOIL_load_image("res/images/c1.png", &width, &height, 0, SOIL_LOAD_RGB);
	}
	else {
		switch (imageInt) {
		case 0:		imageMap = SOIL_load_image("res/images/c1.png", &width, &height, 0, SOIL_LOAD_RGB);
			break;
		case 1:		imageMap = SOIL_load_image("res/images/c2.png", &width, &height, 0, SOIL_LOAD_RGB);
			break;
		case 2:		imageMap = SOIL_load_image("res/images/c3.png", &width, &height, 0, SOIL_LOAD_RGB);
			break;
		case 3:		imageMap = SOIL_load_image("res/images/c5.png", &width, &height, 0, SOIL_LOAD_RGB);
			break;
		case 4:		imageMap = SOIL_load_image("res/images/c5.png", &width, &height, 0, SOIL_LOAD_RGB);
			break;
		case 5:		imageMap = SOIL_load_image("res/images/c6.png", &width, &height, 0, SOIL_LOAD_RGB);
			break;
		default:	imageMap = SOIL_load_image("res/images/container2.png", &width, &height, 0, SOIL_LOAD_RGB);
			break;
		}
	}
	// We bind the texture (of the diffuse map) then we gen our glTexImage2D (specify a two-dimensional texture image),
	// a Mipmap level is also generated using the GL_TEXTURE_2D. SOIL2 is then used to load the image data, before we
	// set the texture parameters to repeat and handle the mipmaps inclusion.
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageMap);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imageMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind

	// Setting the specular map
	imageMap = SOIL_load_image("res/images/container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageMap);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imageMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind

	// Setting the emission map
	imageMap = SOIL_load_image("res/images/emission.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, emissionMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageMap);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imageMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind
}

// This method handles setting the paramaters that are passed on to the shaders, as well
// as the binding of the diffuse, specular and emission maps. We then bind our cubes VAO (if we had more time
// we would refactor how we access this as it should ideally be private access). After updating the
// container object we then set the attributes in the shader (Specify the value of a uniform variable for the current program object)
// finally drawing the arrays (render primitives from array data). The vertex array needs unbinded in its current state.
// We unbind the vertex array by binding to index 0 (break the existing vertex array object binding).
// The emission map is then made the active texture before having the texture bind broken.
void Container::DrawContainer(AssetShader lightingShader, Camera cam, glm::mat4 &projection)
{
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);

	glBindVertexArray(Container::cube.containerVertexArrayObject);

	UpdateContainer(); // Update positioning, rotation and scaling of the container object.

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0); // Unbind

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// This update method allows us to create a new model matrix to translate, rotate, position and scale over time.
void Container::UpdateContainer() 
{
	model = glm::mat4();
	model = glm::translate(model, cubePositions[0]);
	angle = 20.0f;
	angleOverTime = ((currentFrame / 10) * (1)) + 0.5f;

	scaleAmount = (1.0 + sin(glfwGetTime()) * (0.5 + (0.1)));
	rotateAmount = sin(sin(-glfwGetTime() / 5)) * 10;

	model = glm::rotate(model, angleOverTime, glm::vec3(1.0f, 0.3f, 0.5f));
	model = glm::scale_slow(model, glm::vec3(scaleAmount));
}

// This method allows us to render our scene, by first initialising our cube/container, setting up the 
// lighting parameters as well as the various properties to be sent to the relevant shaders.
// We then call our draw container method for the rendering of the object(s).
void Container::RenderMovingContainerScene(AssetShader lightingShader, Camera cam, glm::mat4 &projection) 
{
	Container::cube.InitContainer();
	lights.SetupLightParams(lightingShader, cam);
	modelLocation = lightingShader.getModelLocation();
	viewLocation = lightingShader.getViewLocation();
	projectionLocation = lightingShader.getProjectionLocation();

	DrawContainer(lightingShader, cam, projection);
}