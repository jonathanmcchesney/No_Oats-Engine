#include "Skybox.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~76

// Skybox Constructor
Skybox::Skybox() 
{
	glm::mat4 projection = projection;
}

// Faces vector for the skybox cubemap.
vector<const GLchar*> faces;

// This allows us to render the skybox scene - we enable the  glStencilMask (0x00):- Each bit ends up as 0 in the stencil buffer. We then choose to set the depth func to GL_LEQUAL
// Passes if the incoming depth value is less than or equal to the stored depth value. The shader is then used, the view matrix is set in the shaders, the cubemap texture is binded, the vertex array
// is unbinded, then the skybow cube map is rendered, finally gl_less is re enabled:- Passes if the incoming depth value is less than the stored depth value.
void Skybox::RenderSkyboxScene(AssetShader shader, Camera cam, glm::mat4 &projection)
{
	glStencilMask(0x00);
	glDepthFunc(GL_LEQUAL);
	shader.use();
	view = glm::mat4(glm::mat3(cam.GetViewMatrix()));
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glBindVertexArray(0);
	skybox.RenderSkybox();
	glDepthFunc(GL_LESS);
}

// This method allows us to choose a random skybox when the program begins based on a random integer passed through.
// We then push back all the faces (right -> left -> top -> bottom -> back -> front) - finally adding the these faces to the cubemapTexture asset.
GLuint Skybox::RandomSkybox() 
{
	switch (random_integer_2) 
	{
	case 0:
		faces.push_back("res/images/skybox/right.tga");
		faces.push_back("res/images/skybox/left.tga");
		faces.push_back("res/images/skybox/top.tga");
		faces.push_back("res/images/skybox/bottom.tga");
		faces.push_back("res/images/skybox/back.tga");
		faces.push_back("res/images/skybox/front.tga");
		return 	AssetManager::addCubemap(faces, "cubemapTexture");
		break;
	case 1:
		faces.push_back("res/images/skybox/blood_rt.tga");
		faces.push_back("res/images/skybox/blood_lf.tga");
		faces.push_back("res/images/skybox/blood_up.tga");
		faces.push_back("res/images/skybox/blood_dn.tga");
		faces.push_back("res/images/skybox/blood_bk.tga");
		faces.push_back("res/images/skybox/blood_ft.tga");
		return 	AssetManager::addCubemap(faces, "cubemapTexture");
		break;
	case 2:
		faces.push_back("res/images/skybox/mp_arctic/arctic-ice_rt.tga");
		faces.push_back("res/images/skybox/mp_arctic/arctic-ice_lf.tga");
		faces.push_back("res/images/skybox/mp_arctic/arctic-ice_up.tga");
		faces.push_back("res/images/skybox/mp_arctic/arctic-ice_dn.tga");
		faces.push_back("res/images/skybox/mp_arctic/arctic-ice_bk.tga");
		faces.push_back("res/images/skybox/mp_arctic/arctic-ice_ft.tga");
		return 	AssetManager::addCubemap(faces, "cubemapTexture");
		break;
	case 3:
		faces.push_back("res/images/skybox/hw_crater/craterlake_rt.tga");
		faces.push_back("res/images/skybox/hw_crater/craterlake_lf.tga");
		faces.push_back("res/images/skybox/hw_crater/craterlake_up.tga");
		faces.push_back("res/images/skybox/hw_crater/craterlake_dn.tga");
		faces.push_back("res/images/skybox/hw_crater/craterlake_bk.tga");
		faces.push_back("res/images/skybox/hw_crater/craterlake_ft.tga");
		return 	AssetManager::addCubemap(faces, "cubemapTexture");
		break;
	case 4:
		faces.push_back("res/images/skybox/mp_crimimpact/criminal-impact_rt.tga");
		faces.push_back("res/images/skybox/mp_crimimpact/criminal-impact_lf.tga");
		faces.push_back("res/images/skybox/mp_crimimpact/criminal-impact_up.tga");
		faces.push_back("res/images/skybox/mp_crimimpact/criminal-impact_dn.tga");
		faces.push_back("res/images/skybox/mp_crimimpact/criminal-impact_bk.tga");
		faces.push_back("res/images/skybox/mp_crimimpact/criminal-impact_ft.tga");
		return 	AssetManager::addCubemap(faces, "cubemapTexture");
		break;
	case 5:
		faces.push_back("res/images/skybox/mp_mistylake/mistylake_rt.tga");
		faces.push_back("res/images/skybox/mp_mistylake/mistylake_lf.tga");
		faces.push_back("res/images/skybox/mp_mistylake/mistylake_up.tga");
		faces.push_back("res/images/skybox/mp_mistylake/mistylake_dn.tga");
		faces.push_back("res/images/skybox/mp_mistylake/mistylake_bk.tga");
		faces.push_back("res/images/skybox/mp_mistylake/mistylake_ft.tga");
		return 	AssetManager::addCubemap(faces, "cubemapTexture");
		break;
	}
}
