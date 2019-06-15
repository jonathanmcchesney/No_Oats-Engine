#include "Buffers.h"
#include "AssetManager.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~120

// Buffers constructor
Buffers::Buffers() 
{
}

// This method allows us to perform the first time set up for our shadow buffers.
void Buffers::SetupShadowBuffers() {
	// We first generate our framebuffer using our depth map framebuffer object, using the depth map as the binded 2D texture. We then set uts attributes accordingly
	// making use of the GL_CLAMP_TO_BORDER command to ensure that oversampling does not occur. The shadow border is set and we choose not to draw or read from the buffer at this stage.
	// Finally we free up and unbind the framebuffer. ** Directional Shadows **
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logging::logError("Framebuffer not complete!", __FILE__, __LINE__);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// We generate the framebuffer by attaching the point depth map frame buffer object, differing from directional shadow buffer we attach a cubemap texture as opposed to a 2D texture.
	// We iterate over all 6 faces of the cubemap after binding our depth cubemap when generating our images (point shadow width & height are used here). Again we set up the attributes to
	// ensure correct convolution and reduce oversampling by clamping in 3 dimesnions. Finally binding and unbinding as appropriate (also making use of our logger) ** Point Shadows **
	glGenFramebuffers(1, &pointDepthMapFBO);
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, POINT_SHADOW_WIDTH, POINT_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, pointDepthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logging::logError("Framebuffer not complete!", __FILE__, __LINE__);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// This method allows us to perform the first time set up for our regular framebuffers.
void Buffers::SetupFrameBuffers() {
	// First Framebuffer setup, we generate our framebuffer object and bind it. We also have generated a multismapled texture attachment (as opposed to a rendebuffer object that cant test)
	// this allows us to reduce the aliasing effect depending on the amount of subsamples we take. We use this attachment when we attach a texture image to a framebuffer object.
	// Then we generate and bind ou render buffer object - works in opengls native language (more efficient). Finally unbinding as necessary.
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	sampledTextureAttachment = generateMultiSampleTexture(4);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, sampledTextureAttachment, 0);
	glGenRenderbuffers(1, &renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logging::logError("Framebuffer not complete!", __FILE__, __LINE__);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Second Framebuffer setup, similar to our first framebuffer. This time we simply generate an attachment texture (no multisampling), attach as necessary and use the screen texture GLuint.
	// FInally making use of our logger and unbinding the framebuffer.
	glGenFramebuffers(1, &secondaryFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, secondaryFramebuffer);
	screenTexture = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logging::logError("FRAMEBUFFER::Intermediate framebuffer is not complete!", __FILE__, __LINE__);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// This method allows us to generate return a GLuint attachment texture, to be attached to our framebuffers. (We pass in two booleans to decide what enum we will set to attachment type equal to).
GLuint Buffers::generateAttachmentTexture(GLboolean depthEnabled, GLboolean stencilEnabled)
{
	// we set our attachment type depending on whether we have stencil or depth testing enaled as appropriate.
	GLenum attachment_type;
	if (!depthEnabled && !stencilEnabled)
		attachment_type = GL_RGB;
	else if (depthEnabled && !stencilEnabled)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depthEnabled && stencilEnabled)
		attachment_type = GL_STENCIL_INDEX;

	// We create our texuter ID, generate and bind it as a result, then depening on if depth and stencil testing both are not enabled or if one or both are enabled the
	// texture when specifying a two-dimensional texture image we set them up appropiately (using 32 bits on the attachment_type or using the GL_DEPTH24_STENCIL8 command).
	// We then set our texture parameters and unbind, returing our final texture ID.
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	if (!depthEnabled && !stencilEnabled)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, SCREEN_WIDTH, SCREEN_HEIGHT, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

// This method allows us to generate return a GLuint attachment texture, to be attached to our framebuffers.
// This time we use a number of samples Gluint which dictates the number of samples to reduce the aliasing effect - multisampling.
GLuint Buffers::generateMultiSampleTexture(GLuint numOfSamples)
{
	// Similar to generating an attachment texture but instead we use glTexImage2DMultisample to establish the data storage, format, dimensions, and number of samples of a multisample texture image.
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numOfSamples, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	return tex;
}

// This allows us to render the Framebuffer scene, where we bind our primary and secondary framebuffer and us glBlit - copy a block of pixels from the read framebuffer to the draw framebuffer,
// after we instantaite our model matrix and free our framebuffer. This allows us to transfer pixel regions from the read region of one fb to the draw of another, this allows us to do off screen rendering
// and by unbinding the framebuffer we switch back to the default framebuffer to see the visual impact. 
void Buffers::RenderFrameBufferScene(AssetShader fbShader, AssetShader fbCameraShader, AssetShader fbCameraShader2, GLuint screenTexture, int currentCamera) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, secondaryFramebuffer);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	model = glm::mat4();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// The colours and buffers are reset, depending on the active camera a different shader for post processing effects using a convolution matrix/kernel will be used.
	// We then draw to the screen quad, unvind and then enable depth testing again (needs to be deactivated here).
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	if (currentCamera == 0) fbShader.use();
	else if (currentCamera == 1) fbCameraShader.use();
	else  fbCameraShader2.use();

	quad.InitScreen();
	glBindVertexArray(quad.screenQuadVAO);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}
