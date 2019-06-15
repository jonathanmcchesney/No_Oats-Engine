#include "Assets.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~72
// Erin Hughes LOC : ~18

//
Assets::Assets(){}

// This method simply contains all our setup involving adding textures and shaders to the asset manager.
void Assets::SetupAssets() 
{
#pragma region ShaderLoading
	Logging::logMessageOnly("Beginning to load in shaders...");
	AssetManager::addShader("res/shaders/depth.vertex", "res/shaders/depth.fragment", "depthShader");
	AssetManager::addShader("res/shaders/depth.vertex", "res/shaders/stencil.fragment", "stencilShader");
	AssetManager::addShader("res/shaders/blend.vertex", "res/shaders/blend.fragment", "blendShader");
	AssetManager::addShader("res/shaders/window.vertex", "res/shaders/window.fragment", "windowShader");
	AssetManager::addShader("res/shaders/fb.vertex", "res/shaders/fb.fragment", "fbShader");
	AssetManager::addShader("res/shaders/fb.vertex", "res/shaders/fbCamera.fragment", "fbCameraShader");
	AssetManager::addShader("res/shaders/fb.vertex", "res/shaders/fbCamera2.fragment", "fbCameraShader2");
	AssetManager::addShader("res/shaders/instance.vertex", "res/shaders/instance.fragment", "instanceShader");
	AssetManager::addShader("res/shaders/simpleInstance.vertex", "res/shaders/instance.fragment", "simpleInstanceShader");
	AssetManager::addShader("res/shaders/text.vertex", "res/shaders/text.fragment", "textShader");
	AssetManager::addShader("res/shaders/tangent.vertex", "res/shaders/tangent.fragment", "tangentNormalShader");
	AssetManager::addShader("res/shaders/parallax.vertex", "res/shaders/parallaxOcclusion.fragment", "parallaxShader");
	AssetManager::addShader("res/shaders/sDepth.vertex", "res/shaders/sDepth.fragment", "shadowDepthShader");
	AssetManager::addShader("res/shaders/shadow.vertex", "res/shaders/shadow.fragment", "shadowShader");
	AssetManager::addShader("res/shaders/sQuad.vertex", "res/shaders/sQuad.fragment", "debugQuad");
	AssetManager::addShader("res/shaders/pShadow.vertex", "res/shaders/pShadow.fragment", "pShadowShader");
	AssetManager::addGeometryShader("res/shaders/pDepthShadow.vertex", "res/shaders/pDepthShadow.fragment", "res/shaders/pDepthShadow.geometry", "pointShadowDepthShader");
	AssetManager::addShader("res/shaders/skybox.vertex", "res/shaders/skybox.fragment", "skyboxShader");
	AssetManager::addShader("res/shaders/lighting.combinedVertex", "res/shaders/lighting.combinedFragment", "lightingShader");
	AssetManager::addShader("res/shaders/lamp.vertex", "res/shaders/lamp.fragment", "lampShader");
	AssetManager::addShader("res/shaders/blinn.vertex", "res/shaders/blinn.fragment", "blinnShader");
	AssetManager::addShader("res/shaders/reflection.vertex", "res/shaders/reflection.fragment", "modelShader");
	AssetManager::addShader("res/shaders/model.vertex", "res/shaders/model.fragment", "simpleModelShader");
	AssetManager::addShader("res/shaders/textureAtlas.vertex", "res/shaders/textureAtlas.fragment", "textureAtlas");
	AssetManager::addShader("res/shaders/particle.vertex", "res/shaders/particle.fragment", "particle");
	AssetManager::addShader("res/shaders/cube.vertex", "res/shaders/cube.frag", "cube");
	Logging::logMessageOnly("Shader loading complete!");
#pragma endregion ShaderLoading

#pragma region TextureLoading
	Logging::logMessageOnly("Beginning to load in textures...");
	AssetManager::addTexture("res/images/c2.png", "cubeTexture");
	AssetManager::addTexture("res/images/tile.png", "floorTexture");
	AssetManager::addTexture("res/images/wood.png", "altTexture");
	AssetManager::addTexture("res/images/woodbrick.jpg", "artTexture");
	AssetManager::addTexture("res/images/sketchwood.jpg", "sketchTexture");
	AssetManager::addTexture("res/images/rust1.jpg", "rust1Texture");
	AssetManager::addTexture("res/images/rust2.jpg", "rust2Texture");
	AssetManager::addTexture("res/images/rock2.jpg", "rockTexture");
	AssetManager::addTexture("res/images/metal.jpg", "metalTexture");
	AssetManager::addTexture("res/images/jade.png", "jadeTexture");
	AssetManager::addTexture("res/images/cotton.jpg", "cottonTexture");
	AssetManager::addTexture("res/images/bluecotton.jpg", "bcottonTexture");
	AssetManager::addTexture("res/images/velvet.jpg", "velvetTexture");
	AssetManager::addTexture("res/images/gold.jpg", "goldTexture");
	AssetManager::addTexture("res/images/granite.jpg", "graniteTexture");
	AssetManager::addTexture("res/images/sfabric.jpg", "fabricTexture");
	AssetManager::addTexture("res/images/para/window3.png", "transparentTexture");
	AssetManager::addTexture("res/images/brickwall.jpg", "nDiffuseMap");
	AssetManager::addTexture("res/images/brickwall_normal.jpg", "nNormalMap");
	AssetManager::addTexture("res/images/toy_box_diffuse.png", "pDiffuseMap");
	AssetManager::addTexture("res/images/toy_box_normal.png", "pNormalMap");
	AssetManager::addTexture("res/images/toy_box_disp.png", "pHeightMap");
	AssetManager::addTexture("res/images/para/SpecularMap.png", "pDiffuseMap2");
	AssetManager::addTexture("res/images/para/NormalMap.png", "pNormalMa2");
	AssetManager::addTexture("res/images/para/DisplacementMap.png", "pHeightMa2");
	AssetManager::addTexture("res/images/para/d4.png", "pDiffuseMap4");
	AssetManager::addTexture("res/images/para/normal4.png", "pNormalMap4");
	AssetManager::addTexture("res/images/para/height4.png", "pHeightMap4");
	AssetManager::addTexture("res/images/para/d3.png", "pDiffuseMap3");
	AssetManager::addTexture("res/images/para/normal3.png", "pNormalMap3");
	AssetManager::addTexture("res/images/para/height3.png", "pHeightMap3");
	AssetManager::addTexture("res/images/para/diffuse5.png", "pDiffuseMap5");
	AssetManager::addTexture("res/images/para/normal5.png", "pNormalMap5");
	AssetManager::addTexture("res/images/para/height5.png", "pHeightMap5");
	AssetManager::addTexture("res/images/para/diffuse6.png", "pDiffuseMap6");
	AssetManager::addTexture("res/images/para/normal6.png", "pNormalMap6");
	AssetManager::addTexture("res/images/para/height6.png", "pHeightMap6");
	AssetManager::addTexture("res/images/para/d7.png", "pDiffuseMap7");
	AssetManager::addTexture("res/images/para/height7.png", "pNormalMap7");
	AssetManager::addTexture("res/images/para/normal7.png", "pHeightMap7");
	AssetManager::addTexture("res/images/para/d8height.png", "pDiffuseMap8");
	AssetManager::addTexture("res/images/para/normal8.png", "pNormalMap8");
	AssetManager::addTexture("res/images/para/height8.png", "pHeightMap8");
	AssetManager::addTexture("res/images/para/d9height.png", "pDiffuseMap9");
	AssetManager::addTexture("res/images/para/normal9.png", "pNormalMap9");
	AssetManager::addTexture("res/images/para/height9.png", "pHeightMap9");
	AssetManager::addTexture("res/images/para/d10.png", "pDiffuseMap10");
	AssetManager::addTexture("res/images/para/normal10.png", "pNormalMap10");
	AssetManager::addTexture("res/images/para/d10height.png", "pHeightMap10");
	AssetManager::addTexture("res/images/drop.png", "drop");
	AssetManager::addTexture("res/images/pSparkle2.png", "star");
	AssetManager::addTexture("res/images/explosionAtlas.png", "atlas1");
	AssetManager::addTexture("res/images/explosionAtlas2.png", "atlas2");
	AssetManager::addTexture("res/images/explosionAtlas3.png", "atlas3");
	AssetManager::addTexture("res/images/explosionAtlas5.png", "atlas4");
	AssetManager::addTexture("res/images/smokeAtlas.png", "atlas5");
	AssetManager::addTexture("res/images/sparkle4.png", "sparkle");
	AssetManager::addTexture("res/images/flare2.png", "flare");
	AssetManager::addTexture("res/images/whiteFabric.png", "whiteFabric");
	Logging::logMessageOnly("Texture loading complete!");
#pragma endregion TextureLoading
}