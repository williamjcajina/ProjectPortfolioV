#include <fbxsdk.h>
#include "FBX_Exporter.h"

bool loadFBX(const char * filename, std::vector<vect> vertices)
{
	FbxManager* g_pFbxSdkManager = FbxManager::Create();

	FbxIOSettings *ios = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
	g_pFbxSdkManager->SetIOSettings(ios);

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");
	if (!pImporter->Initialize(filename, -1, g_pFbxSdkManager->GetIOSettings()))
	{
		return false;
	}
	
	// Create a new scene so that it can be populated by the imported file.
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");
	
	pImporter->Import(pFbxScene);
	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();


	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;
			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();




		}
	}
}
