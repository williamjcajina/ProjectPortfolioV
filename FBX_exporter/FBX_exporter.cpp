
#include "FBX_Exporter.h"
#include <fbxsdk.h>

FbxManager* mFBXManager;
FbxScene* mFBXScene;
std::unordered_map<unsigned int, Point*> points;





struct Joint
{
	std::string mName;
	int mParentIndex = -1;

	FbxNode* mNode;
	FbxAMatrix mGlobalBindposeInverse;

};

std::vector<Joint> joints;

FBX_READER_API void getUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, float outUV[2]);
FBX_READER_API void getNormal(FbxMesh* mesh, int index, int counter, float outNormal[3]);
FBX_READER_API void getNodeVertexData(FbxNode* inNode, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes);
FBX_READER_API void processData(FbxNode* node, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes);
FBX_READER_API void processSkeleton(FbxNode* root);
FBX_READER_API void traverseSkeletonHierarchy(FbxNode* node, int depth, int index, int parentIndex);
FBX_READER_API void processJoints(FbxNode* node);
FBX_READER_API FbxAMatrix getTransforms(FbxNode* inNode);
FBX_READER_API void exportJoints(std::vector<JointData> &jef);








FBX_READER_API bool loadScene(const char * filename)
{

	FbxImporter* fbxImporter = FbxImporter::Create(mFBXManager, "myImporter");

	if (!fbxImporter->Initialize(filename, -1, mFBXManager->GetIOSettings()))
		return false;
	

	if(!fbxImporter->Import(mFBXScene))
	return false;
	
	fbxImporter->Destroy();

	return true;


}

FBX_READER_API void getUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, float outUV[2])
{
	if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer)
	{
		throw std::exception("Invalid UV Layer Number");
	}
	FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);

	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outUV[1] = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			outUV[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV[1] = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			outUV[0] = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			outUV[1] = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

FBX_READER_API void getNormal(FbxMesh* mesh, int index, int counter, float outNormal[3])
{
	if (mesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}

	FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int i = vertexNormal->GetIndexArray().GetAt(index);
			outNormal[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(i).mData[0]);
			outNormal[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(i).mData[1]);
			outNormal[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(i).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(counter).mData[0]);
			outNormal[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(counter).mData[1]);
			outNormal[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(counter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int i = vertexNormal->GetIndexArray().GetAt(index);
			outNormal[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(i).mData[0]);
			outNormal[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(i).mData[1]);
			outNormal[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(i).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

FBX_READER_API void getNodeVertexData(FbxNode* inNode,std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes)
{
	FbxMesh* currMesh = inNode->GetMesh();

	unsigned int triangleCount = currMesh->GetPolygonCount();

	int vCounter =0;
	for (unsigned int i = 0; i < triangleCount; i++)
	{
		float normal[3];
		float uv[2];

		for (unsigned int j = 0; j < 3; j++)
		{
			int pointIndex = currMesh->GetPolygonVertex(i, j);
			Point* currPoint = points[pointIndex];

			getNormal(currMesh, pointIndex, vCounter, normal);
			for (int k = 0; k < 1; ++k)
				getUV(currMesh, pointIndex, currMesh->GetTextureUVIndex(i, j), k, uv);


			Vertex temp;
			temp.x = currPoint->pos[0];
			temp.y = currPoint->pos[1];
			temp.z = currPoint->pos[2];

			temp.nx = normal[0];
			temp.ny = normal[1];
			temp.nz = normal[2];

			temp.tu = uv[0];
			temp.tv = uv[1];

			vertices.push_back(temp);
			vertexIndexes.push_back(vCounter);
			++vCounter;



		}

		

	}
	for (auto itr = points.begin(); itr != points.end(); ++itr)
	{
		delete itr->second;
	}
	points.clear();
	
}
FBX_READER_API void processPoints(FbxNode* node)
{
	FbxMesh* currMesh = node->GetMesh();
	unsigned int pointCount = currMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < pointCount; ++i)
	{
		Point* currPoint = new Point();

		currPoint->pos[0] = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPoint->pos[1] = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPoint->pos[2] = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);

		points[i] = currPoint;
	}
}


FBX_READER_API void processData(FbxNode* node,std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes)
{
	
			
			
			if (node->GetNodeAttribute())
			{
				switch (node->GetNodeAttribute()->GetAttributeType())
				{
				case FbxNodeAttribute::eMesh:
					processPoints(node);
					processJoints(node);
					getNodeVertexData(node, vertices, vertexIndexes);
					
					break;
				}
			}
			
			
			
			for (int i = 0; i < node->GetChildCount(); ++i)
			{
				processData(node->GetChild(i), vertices,vertexIndexes);
			}


	
}

FBX_READER_API void processSkeleton(FbxNode * root)
{
	for (int childIndex = 0; childIndex < root->GetChildCount(); ++childIndex)
	{
		FbxNode* currNode = root->GetChild(childIndex);
		traverseSkeletonHierarchy(currNode, 0, 0, -1);
	}
}

FBX_READER_API void traverseSkeletonHierarchy(FbxNode * node, int depth, int index, int parentIndex)
{
	if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint currJoint;
		currJoint.mParentIndex = parentIndex;
		currJoint.mName = node->GetName();
		joints.push_back(currJoint);
	}
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		traverseSkeletonHierarchy(node->GetChild(i), depth + 1, joints.size(), index);
	}
}

FBX_READER_API void processJoints(FbxNode * node)
{
	FbxMesh* currMesh = node->GetMesh();
	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	FbxAMatrix geometryTransform = getTransforms(node);

	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{

		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin)
		{
			continue;
		}
		unsigned int numOfClusters = currSkin->GetClusterCount();


		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{

			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			unsigned int currJointIndex;

			for (unsigned int i = 0; i <joints.size(); ++i)
			{
				if (joints[i].mName == currJointName)
				{
					currJointIndex = i;
					break;
				}
			}



			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			joints[currJointIndex].mGlobalBindposeInverse = globalBindposeInverseMatrix;
			joints[currJointIndex].mNode = currCluster->GetLink();
		}




	}
	

}

FBX_READER_API FbxAMatrix getTransforms(FbxNode * inNode)
{
	if (!inNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

FBX_READER_API void exportJoints(std::vector<JointData> &jef)
{
	for (int i = 0; i < joints.size(); i++)
	{
		JointData j;
		j.parentIndex = joints[i].mParentIndex;
		
		FbxVector4 rot = joints[i].mGlobalBindposeInverse.GetR();
		FbxVector4 pos = joints[i].mGlobalBindposeInverse.GetT();
		FbxVector4 sca = joints[i].mGlobalBindposeInverse.GetS();
	
		j.rotation.x = rot.mData[0];
		j.rotation.y = rot.mData[1];
		j.rotation.z = rot.mData[2];
		j.rotation.w = rot.mData[3];

		j.translation.x = pos.mData[0];
		j.translation.y = pos.mData[1];
		j.translation.z = pos.mData[2];
		j.translation.w = pos.mData[3];

		j.scale.x = sca.mData[0];
		j.scale.y = sca.mData[1];
		j.scale.z = sca.mData[2];
		j.scale.w = sca.mData[3];

	 
		int k = 0;
		for (int m = 0; m < 4; m++)
		{
			for (int n = 0; n < 4; n++)
			{
				j.matrix[k] = joints[i].mGlobalBindposeInverse.mData[m][n];
				k++;
			}
		}

		jef.push_back(j);
		
	}
}


bool loadFBX(const char * filename, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes, std::vector<JointData> &jef)
{
	if (!init())
		return false;

	if (!loadScene(filename))
		return false;
	processSkeleton(mFBXScene->GetRootNode());
	processData(mFBXScene->GetRootNode(), vertices, vertexIndexes);
	exportJoints(jef);


	return true;

}

bool init()
{
	mFBXManager = FbxManager::Create();
	if (!mFBXManager)
	{
		return false;
	}

	FbxIOSettings* fbxIOSettings = FbxIOSettings::Create(mFBXManager, IOSROOT);
	mFBXManager->SetIOSettings(fbxIOSettings);

	mFBXScene = FbxScene::Create(mFBXManager, "myScene");


	return true;
}
