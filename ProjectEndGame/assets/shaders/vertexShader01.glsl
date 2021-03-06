#version 420

//uniform mat4 MVP;
uniform mat4 matModel;		// Model or World 
uniform mat4 matModelInverseTranspose;		// For normal calculation
uniform mat4 matView; 		// View or camera
uniform mat4 theAIView;		// View for AI
uniform mat4 thePlatformView;	//View for Platform game
uniform mat4 theFullScreenView; //View for fullscreen fbo
uniform mat4 matProj;		// Projection transform

uniform bool useAI;
uniform bool usePlatform;
uniform bool useFullScreen;

in vec4 vColour;				// Was vec3
in vec4 vPosition;				// Was vec3
in vec4 vNormal;				// Vertex normal
in vec4 vUVx2;					// 2 x Texture coords
in vec4 vBI;					// bitangent
in vec4 vTAN;					// vertex tangent
in vec4 vBoneID;				// bone ID
in vec4 vBoneWeight;			// bone weights

//out vec3 color;
//out vec4 vertWorld;			// Location of the vertex in the world
out vec4 fColour;	
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec4 fUVx2;
//out vec4 fBI;					// idk what these are, something to do with the bones
//out vec4 fTAN;					// vertex tan
//out vec4 fBoneID;				// bone ID
//out vec4 fBoneWeights;			// bone weights

uniform sampler2D heightMap;
uniform bool useHeightMap;	// If true, use heightmap
uniform vec2 textOffset;

const int MAXNUMBEROFBONES = 100;
uniform mat4 matBonesArray[MAXNUMBEROFBONES];
uniform bool isSkinnedMesh;
uniform float randomLightContrib;

uniform bool shipLevelCompleted;
uniform bool platformerCompleted;
uniform float randomScaling;
uniform bool isMenuSelector;

uniform bool isMenu;
uniform bool isMenuNotSelected;

void main()
{
    vec4 vertPosition = vPosition;

//	if (platformerCompleted)
//	{
//		vertPosition.y += randomScaling;
//	}

	if ( useHeightMap )
	{
		// Move the y value by some amount from texture	
		// Since it's black and white, I only sample 1 colour.
		
		vec2 texUV1 = vUVx2.st + textOffset.xy;
		float texValue1 = texture( heightMap, texUV1.st ).r;
		float ratio1 = 1.0;
		
		
		// This will pick a completely different location
		// (note the reversal of the xy to yx, called a "swizzle")
		vec2 texUV2 = vUVx2.st + textOffset.yx * vec2(-0.5f, 0.75f);	
		float texValue2 = texture( heightMap, texUV2.st ).r;
		float ratio2 = 1.5f;
		
		
		vertPosition.y += (texValue1*ratio1) + (texValue2 * ratio2);
	}

	if (shipLevelCompleted)
	{
		vertPosition.x += (randomScaling*1.0f) + (randomScaling * 1.5f);
		vertPosition.y += (randomScaling*1.0f) + (randomScaling * 1.5f);
		vertPosition.z += (randomScaling*1.0f) + (randomScaling * 1.5f);
	}
	
	if (isSkinnedMesh)
	{
		// *************************************************
		mat4 BoneTransform = matBonesArray[ int(vBoneID[0]) ] * vBoneWeight[0];
		     BoneTransform += matBonesArray[ int(vBoneID[1]) ] * vBoneWeight[1];
		     BoneTransform += matBonesArray[ int(vBoneID[2]) ] * vBoneWeight[2];
		     BoneTransform += matBonesArray[ int(vBoneID[3]) ] * vBoneWeight[3];

		// Apply the bone transform to the vertex:
		vec4 vertOriginal = vec4(vertPosition.xyz, 1.0f);

		vec4 vertAfterBoneTransform = BoneTransform * vertOriginal;

		mat4 matMVP;
		
		if (useAI)
		{
			matMVP = matProj * theAIView * matModel;	
		}
		else if (usePlatform)
		{
			matMVP = matProj * thePlatformView * matModel;	
		}
		else if (useFullScreen)
		{
			matMVP = matProj * theFullScreenView * matModel;
		}
		else
		{
			matMVP = matProj * matView * matModel;	
		}
			
		// Transform the updated vertex location (from the bone)
		//  and transform with model view projection matrix (as usual)
		gl_Position = matMVP * vertAfterBoneTransform;		
		
		// Then we do the normals, etc.
		fVertWorldLocation = matModel * vertAfterBoneTransform;	
		
		// Updated "world" or "model" transform 
		mat4 matModelAndBone = matModel * BoneTransform;
		
		vec3 theNormal = normalize(vNormal.xyz);
		fNormal = inverse(transpose(matModelAndBone)) * vec4(theNormal, 1.0f);
		fNormal.xyz = normalize(fNormal.xyz); 
		
		fColour = vColour;	
		fUVx2 = vUVx2;
		
		// *************************************************
	}
	else
	{
		vec4 vertOriginal = vec4(vertPosition.xyz, 1.0f);

		mat4 matMVP;
		
		if (useAI)
		{
			matMVP = matProj * theAIView * matModel;	
		}
		else if (usePlatform)
		{
			matMVP = matProj * thePlatformView * matModel;	
		}
		else if (useFullScreen)
		{
			matMVP = matProj * theFullScreenView * matModel;
		}
		else
		{
			matMVP = matProj * matView * matModel;	
		}
	
		gl_Position = matMVP * vertOriginal;
	
		// Vertex location in "world space"
		fVertWorldLocation = matModel * vec4(vertPosition.xyz, 1.0);
	
		mat4 matModelInverseTranspose = inverse(transpose(matModel));
	
 		//fNormal = vNormal;

		vec3 theNormal = normalize(vNormal.xyz);
 		fNormal = matModelInverseTranspose * vec4(theNormal, 1.0f);

		if (shipLevelCompleted)
		{
			fNormal *= randomLightContrib;
		}

		if (platformerCompleted)
		{
			fNormal *= randomLightContrib * 2.0f;
		}

		if (isMenu)
		{
			fNormal *= 14.5f;
		}

		if (isMenuNotSelected)
		{
			fNormal *= 5.0f;
		}

		if (isMenuSelector)
		{
			fNormal *= 0.01f;
		}
	
		// Pass the colour and UV unchanged. And the other stuff??? maybe??
		fColour = vColour;	
		fUVx2 = vUVx2;
	
	}//if (isSkinnedMesh)
}
