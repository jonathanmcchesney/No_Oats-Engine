#include "AssetManager.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "ManualCube.h"
#include "ManualQuad.h"
#include "Logging.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~5

class Assets
{
// These methods require public access as they are used within the main for setting up and loading assets.
public:
	// Externally Accessible
	Assets::Assets();
	void Assets::SetupAssets();
};