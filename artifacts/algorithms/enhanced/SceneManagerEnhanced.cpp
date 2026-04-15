///////////////////////////////////////////////////////////////////////////////
// scenemanager.cpp
// ============
// Author: Laurel Horwath
// Course: CS 330 / CS 499 Enhancement
// Date: 03/29/2026
// 
// Description: This source file contains the implementation of the SceneManager class, which is responsible for managing the 3D scene, including loading textures, defining materials, setting up lights, and rendering the scene.
// The SceneManager interacts with the ShaderManager to set shader values and uses ShapeMeshes to draw basic 3D shapes. The code includes methods for creating OpenGL textures from image files, binding textures to memory slots, and setting transformations and materials for rendering. 
// The PrepareScene() method is used to set up the scene before rendering, while RenderScene() contains the code for drawing the objects in the scene with specific transformations and materials.
//	
///////////////////////////////////////////////////////////////////////////////
   
#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>
#include <unordered_map>

//Hash maps for fast lookup
std::unordered_map<std::string, int> textureMap;
std::unordered_map<std::string, int> textureSlotMap;

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		
		// Store in hash maps
		textureMap[tag] = textureID;
		textureSlotMap[tag] = m_loadedTextures;
		
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glDeleteTextures(1, &m_textureIDs[i].ID);
	}
}


/***********************************************************
 *  FindTextureID()
 *
 *  Old: o(n) linear search
 *  New: o(1) hash map lookup
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	if (textureMap.find(tag) != textureMap.end()) {
		return textureMap[tag];
	}
	return -1; // Not found
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  Old: o(n) linear search
 *  New: o(1) hash map lookup
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	if (textureSlotMap.find(tag) != textureSlotMap.end()) {
		return textureSlotMap[tag];
	}
	return -1; // Not found
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationZ * rotationY * rotationX * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/



/***********************************************************
 *  DefineObjectMaterials()
 *
 *  This method is used for configuring the various material 
 *  settings for all of the objects within the 3D scene.
 ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	OBJECT_MATERIAL woodMaterial;
	woodMaterial.diffuseColor = glm::vec3(0.3f, 0.2f, 0.1f);
	woodMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	woodMaterial.shininess = 0.3f;
	woodMaterial.tag = "wood";

	m_objectMaterials.push_back(woodMaterial);

	OBJECT_MATERIAL plasticMaterial;
}

/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene. There are up to four light sources.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	//m_pShaderManager->setBoolValue(g_useLightingName, true);

	m_pShaderManager->setVec3Value("lightSources[0].position", 5.0f, 10.0f, 5.0f);
	m_pShaderManager->setVec3Value("lightSources[0].ambientColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setVec3Value("lightSources[0].diffuseColor", 0.9f, 0.8f, 0.8f);
	m_pShaderManager->setVec3Value("lightSources[0].specularColor", 1.0f, 1.0f, 1.0f);
	m_pShaderManager->setFloatValue("lightSources[0].focalStrength", 32.0f);
	m_pShaderManager->setFloatValue("lightSources[0].specularIntensity", 0.8f);

	m_pShaderManager->setVec3Value("lightSources[1].position", -5.0f, 8.0f, -5.0f);
	m_pShaderManager->setVec3Value("lightSources[1].ambientColor", 0.3f, 0.15f, 0.15f);
	m_pShaderManager->setVec3Value("lightSources[1].diffuseColor", 1.0f, 0.6f, 0.6f);
	m_pShaderManager->setVec3Value("lightSources[1].specularColor", 1.0f, 0.6f, 0.6f);
	m_pShaderManager->setFloatValue("lightSources[1].focalStrength", 20.0f);
	m_pShaderManager->setFloatValue("lightSources[1].specularIntensity", 0.5f); 

	m_pShaderManager->setVec3Value("pointLight.position", glm::vec3(5.0f, 5.0f, 5.0f));
	m_pShaderManager->setVec3Value("pointLight.color", glm::vec3(1.0f, 1.0f, 1.0f));
	m_pShaderManager->setFloatValue("pointLight.constant", 1.0f);
	m_pShaderManager->setFloatValue("pointLight.linear", 0.09f);
	m_pShaderManager->setFloatValue("pointLight.quadratic", 0.032f);


	m_pShaderManager->setBoolValue("bUseLighting", true);

}
/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{
	// define the materials for the objects in the scene
	DefineObjectMaterials();
	// add and define the light sources for the scene
	SetupSceneLights();
	//load the textures for the 3d scene
	SceneManager::LoadSceneTextures();


	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene

	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadBoxMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadPyramid4Mesh();
	m_basicMeshes->LoadTorusMesh();
}

//LoadSceneTextures
void SceneManager::LoadSceneTextures()
{
	bool bReturn = false;

    
	bReturn = CreateGLTexture("textures/WoodTexture.jpg", "cylinder");
	bReturn = CreateGLTexture("textures/DarkWoodTexture.jpg", "cylinder2");
	bReturn = CreateGLTexture("textures/TileTexture.jpg", "cylinder3");
	bReturn = CreateGLTexture("textures/LightWoodTexture.jpg", "WoodBlocks");
	bReturn = CreateGLTexture("textures/PolkaDotTexture.jpg", "PolkaDotTexture");


	BindGLTextures();
}


/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene()
{
	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(20.0f, 1.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(1, 1, 1, 1);

	// draw the mesh with transformation values
	//Size of plane
	scaleXYZ = glm::vec3(50.0f, 50.0f, 50.0f);

	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetShaderTexture("PolkaDotTexture");
	SetTextureUVScale(5.0f, 5.0f); //Complex texturing technique

	m_basicMeshes->DrawPlaneMesh();

	//Base of rear object (box)
	scaleXYZ = glm::vec3(6.0f, 2.0f, 2.0f);

	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	positionXYZ = glm::vec3(6.0, 1.0f, -5.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	
	//SetShaderColor(1.0f, 0.0f, 0.0f, 1.0f);
	SetShaderMaterial("wood");
	SetShaderTexture("WoodBlocks");

	m_basicMeshes->DrawBoxMesh();

	//Box on rear right structure
	scaleXYZ = glm::vec3(6.0f, 2.0f, 2.0f);
    XrotationDegrees = 45.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 90.0f;
	positionXYZ = glm::vec3(7.5f, 5.0f, -5.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	//SetShaderColor(1.0f, 0.2f, 0.6f, 1.0f);
	SetShaderMaterial("wood");
	SetShaderTexture("WoodBlocks");

	m_basicMeshes->DrawBoxMesh();

	//Small box on rear right structure
	scaleXYZ = glm::vec3(2.0f, 2.0f, 2.0f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 45.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(4.5f, 3.0f, -5.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	//SetShaderColor(0.98f, 0.80f, 0.91f, 1.0f);
	SetShaderMaterial("wood");
	SetShaderTexture("WoodBlocks");

	m_basicMeshes->DrawBoxMesh();

	//Cylinder on rear structure
	scaleXYZ = glm::vec3(1.0f, 4.0f, 1.0f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(4.5f, 4.0f, -5.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.55f, 0.27f, 0.07f, 1.0f);
	//Set shader texture
	SetShaderMaterial("wood");
	SetShaderTexture("WoodBlocks");

	
	m_basicMeshes->DrawCylinderMesh();

	//Base of right triangle on rear structure (on cylinder)
	scaleXYZ = glm::vec3(1.3f, 0.8f, 1.3f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(4.5f, 8.0f, -5.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	//SetShaderColor(0.7f, 0.87f, 0.54f, 1.0f);
	SetShaderMaterial("wood");
	SetShaderTexture("WoodBlocks");
	
	m_basicMeshes->DrawPlaneMesh();

	//Side of triangle
	scaleXYZ = glm::vec3(0.8f, 1.3f, 1.3f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 45.0f;
	positionXYZ = glm::vec3(4.2f, 9.0f, -5.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	//SetShaderColor(0.7f, 0.87f, 0.54f, 1.0f);
	SetShaderMaterial("wood");
	SetShaderTexture("WoodBlocks");
	
	m_basicMeshes->DrawPlaneMesh();

	//Other side of the triangle
	scaleXYZ = glm::vec3(1.0f, 0.8f, 1.3f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = -45.0f;
	positionXYZ = glm::vec3(5.2f, 9.0f, -5.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	//SetShaderColor(0.7f, 0.87f, 0.54f, 1.0f);
	SetShaderMaterial("wood");
	SetShaderTexture("WoodBlocks");
	
	m_basicMeshes->DrawPlaneMesh();

	//Other attempt at triangle
	scaleXYZ = glm::vec3(2.0f, 2.0f, 2.0f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(7.3f, 9.0f, -5.0f); 

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	//SetShaderColor(0.68f, 0.85f, 0.9f, 1.0f);
	SetShaderMaterial("wood");
	SetShaderTexture("WoodBlocks");
	
	m_basicMeshes->DrawPyramid4Mesh();

	//Stack of rings(purple)
	scaleXYZ = glm::vec3(6.0f, 5.0f, 5.0f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-7.3f, 1.0f, 8.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(0.29f, 0.0f, 0.51f, 1.0f);
	

	m_basicMeshes->DrawTorusMesh();

	//Stack of rings(blue)
	scaleXYZ = glm::vec3(5.5f, 4.5f, 4.5f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-7.3f, 2.5f, 8.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(0.0f, 0.0f, 1.0f, 1.0f);
	

	m_basicMeshes->DrawTorusMesh();

	//Stack of rings(green)
	scaleXYZ = glm::vec3(5.0f, 4.0f, 4.0f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-7.3f, 4.0f, 8.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(0.0f, 1.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawTorusMesh();

	//Stack of rings(yellow)
	scaleXYZ = glm::vec3(4.5f, 3.5f, 3.5f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-7.3f, 5.45f, 8.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(1.0f, 1.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawTorusMesh();

	//Stack of rings(orange)
	scaleXYZ = glm::vec3(4.0f, 4.0f, 3.0f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-7.3f, 6.6f, 8.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(1.0f, 0.5f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawTorusMesh();

	//Stack of rings(red)
	scaleXYZ = glm::vec3(3.6f, 3.6f, 2.5f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-7.3f, 7.7f, 8.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(1.0f, 0.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawTorusMesh();

	//Middle of peg game
	scaleXYZ = glm::vec3(12.0f, 5.0f, 1.0f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 120.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(17.0f, 3.0f, 8.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	//SetShaderColor(0.43f, 0.95f, 2.0f, 1.0f);
	//SetShaderMaterial("wood");
	SetShaderTexture("WoodBlocks");

	m_basicMeshes->DrawBoxMesh();


	//Peg in peg game
	scaleXYZ = glm::vec3(0.5f, 3.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(13.5f, 1.0f, 5.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(1.0f, 1.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawCylinderMesh();


	//Peg in peg game (2)
	scaleXYZ = glm::vec3(0.5f, 3.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(16.0f, 2.0f, 4.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(1.0f, 1.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawCylinderMesh();

	//Peg in peg game (3)
	scaleXYZ = glm::vec3(0.5f, 3.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(15.0f, 1.5f, 8.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(0.0f, 0.0f, 1.0f, 1.0f);
	

	m_basicMeshes->DrawCylinderMesh();

	//Peg in peg game (4)
	scaleXYZ = glm::vec3(0.5f, 3.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(17.8f, 1.8f, 7.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(0.0f, 0.0f, 1.0f, 1.0f);
	

	m_basicMeshes->DrawCylinderMesh();

	//Peg in peg game (5)
	scaleXYZ = glm::vec3(0.5f, 3.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(16.5f, .9f, 10.5f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(1.0f, 1.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawCylinderMesh();

	//Peg in peg game (6)
	scaleXYZ = glm::vec3(0.5f, 3.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(19.5f, 1.5f, 9.5f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(1.0f, 1.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawCylinderMesh();

	//Peg in peg game (7)
	scaleXYZ = glm::vec3(0.5f, 3.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(20.9f, 1.0f, 11.5f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(0.0f, 1.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawCylinderMesh();

	//Peg in peg game (8)
	scaleXYZ = glm::vec3(0.5f, 3.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(17.8f, 1.5f, 13.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(0.0f, 1.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawCylinderMesh();

	//Far side peg game 
	scaleXYZ = glm::vec3(7.0f, 6.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 30.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3( 14.5f, 3.0f, 2.5f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(1.0f, 0.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawBoxMesh();

	//Near side peg game 
	scaleXYZ = glm::vec3(7.0f, 6.0f, 0.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 30.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(20.0f, 3.0f, 13.5f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	SetShaderColor(1.0f, 0.0f, 0.0f, 1.0f);
	

	m_basicMeshes->DrawBoxMesh();

	//Base of piano toy (red)
	scaleXYZ = glm::vec3(6.0f, 3.5f, 4.0f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(5.0f, 2.0f, 19.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(0.9f, 0.1f, 0.1f, 1.0f);
	m_basicMeshes->DrawBoxMesh();

	// Piano Key 1 (Red)
	scaleXYZ = glm::vec3(0.7f, 1.0f, 0.8f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(2.5f, 2.0f, 21.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_basicMeshes->DrawBoxMesh();

	// Piano Key 2 (Orange)
	scaleXYZ = glm::vec3(0.7f, 1.0f, 0.8f);
	positionXYZ = glm::vec3(3.4f, 2.0f, 21.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(1.0f, 0.5f, 0.0f, 1.0f);
	m_basicMeshes->DrawBoxMesh();

	// Piano Key 3 (Yellow)
	scaleXYZ = glm::vec3(0.7f, 1.0f, 0.8f);
	positionXYZ = glm::vec3(4.3f, 2.0f, 21.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(1.0f, 1.0f, 0.0f, 1.0f);
	m_basicMeshes->DrawBoxMesh();

	// Piano Key 4 (Green)
	scaleXYZ = glm::vec3(0.7f, 1.0f, 0.8f);
	positionXYZ = glm::vec3(5.4f, 2.0f, 21.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_basicMeshes->DrawBoxMesh();

	// Piano Key 5 (Blue)
	scaleXYZ = glm::vec3(0.7f, 1.0f, 0.8f);
	positionXYZ = glm::vec3(6.5f, 2.0f, 21.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_basicMeshes->DrawBoxMesh();

	// Piano Key 6 (Purple)
	scaleXYZ = glm::vec3(0.7f, 1.0f, 0.8f);
	positionXYZ = glm::vec3(7.5f, 2.0f, 21.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(0.5f, 0.0f, 1.0f, 1.0f);
	m_basicMeshes->DrawBoxMesh();


	// Torus 1 (Red)
	scaleXYZ = glm::vec3(1.0f, 0.0f, 0.0f);
	YrotationDegrees = 90.0f;
	positionXYZ = glm::vec3(2.5f, 3.9f, 20.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees, 
		YrotationDegrees,
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(1.0f, 1.0f, 0.0f, 1.0f);
	m_basicMeshes->DrawTorusMesh();

	// Torus 2 (Orange)
	scaleXYZ = glm::vec3(0.8f, 0.1f, 0.8f);
	YrotationDegrees = 90.0f;
	positionXYZ = glm::vec3(3.5f, 3.9f, 20.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(1.0f, 0.5f, 0.0f, 1.0f);
	m_basicMeshes->DrawTorusMesh();

	// Torus 3 (Yellow)
	scaleXYZ = glm::vec3(0.8f, 0.1f, 0.8f);
	YrotationDegrees = 90.0f;
	positionXYZ = glm::vec3(4.5f, 3.9f, 20.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(1.0f, 1.0f, 0.0f, 1.0f);
	m_basicMeshes->DrawTorusMesh();

	// Torus 4 (Green)
	scaleXYZ = glm::vec3(0.8f, 0.1f, 0.8f);
	YrotationDegrees = 90.0f;
	positionXYZ = glm::vec3(5.5f, 3.9f, 20.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_basicMeshes->DrawTorusMesh();

	// Torus 5 (Blue)
	scaleXYZ = glm::vec3(0.8f, 0.1f, 0.8f);
	YrotationDegrees = 90.0f;
	positionXYZ = glm::vec3(6.5f, 3.9f, 20.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_basicMeshes->DrawTorusMesh();

	// Torus 6 (Purple)
	scaleXYZ = glm::vec3(0.8f, 0.1f, 0.8f);
	YrotationDegrees = 90.0f;
	positionXYZ = glm::vec3(7.5f, 3.9f, 20.0f);

	SetTransformations(
		scaleXYZ, 
		XrotationDegrees, 
		YrotationDegrees, 
		ZrotationDegrees, 
		positionXYZ);
	
	SetShaderColor(0.5f, 0.0f, 1.0f, 1.0f);
	m_basicMeshes->DrawTorusMesh();

	/****************************************************************/
}
