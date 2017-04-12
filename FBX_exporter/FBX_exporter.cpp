
#include "FBX_Exporter.h"
#include <fbxsdk.h>

FbxManager* mFBXManager;
FbxScene* mFBXScene;
std::unordered_map<unsigned int, Point> points;





struct Joint
{
	std::string mName;
	int mParentIndex = -1;

	FbxNode* mNode;
	FbxAMatrix mGlobalBindposeInverse;

};

struct KeyFrame
{
	std::vector<Joint> joints;
	std::string name;
	double time;
};

struct AnimClip
{
	std::vector<KeyFrame> frames;
	double duration;
};
AnimClip animationClip;
std::vector<Joint> joints;

FBX_READER_API void getUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, float outUV[2]);
FBX_READER_API void getNormal(FbxMesh* mesh, int index, int counter, float outNormal[3]);
FBX_READER_API void getNodeVertexData(FbxNode* inNode, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes);
FBX_READER_API void processData(FbxNode* node, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes);
FBX_READER_API void exportData(AnimationData &anim);
FBX_READER_API FbxNode* getSkeletonRoot();
FBX_READER_API void fillJointVector(FbxNode * node, int depth, int index, int parentIndex);
FBX_READER_API void GetAnimationData();



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

FBX_READER_API unsigned int getJointIndex(std::string name)
{
	for (unsigned int i = 0; i < joints.size(); ++i)
	{
		if (joints[i].mName == name)
		{
			return i;
		}
	}
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
		
			DirectX::XMFLOAT3 currPoint = points[pointIndex].point;
			
			getNormal(currMesh, pointIndex, vCounter, normal);
			for (int k = 0; k < 1; ++k)
			getUV(currMesh, pointIndex, currMesh->GetTextureUVIndex(i, j), k, uv);


			Vertex temp;
			temp.position = currPoint;
			 temp.normal.x= normal[0];
			 temp.normal.y = normal[1];
			 temp.normal.z = normal[2];

			temp.UV.x = uv[0];
			temp.UV.y = uv[1];

			
			

			
			temp.blends = points[pointIndex].blends;
			vertices.push_back(temp);
			vertexIndexes.push_back(vCounter);
			++vCounter;

		}

		

	}
	/*for (auto itr = points.begin(); itr != points.end(); ++itr)
	{
		delete itr->second;
	}*/
	points.clear();
	
}
FBX_READER_API void processPoints(FbxNode* node)
{
	FbxMesh* currMesh = node->GetMesh();
	unsigned int pointCount = currMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < pointCount; ++i)
	{
		
		DirectX::XMFLOAT3 currPoint;

		currPoint.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPoint.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPoint.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);

		points[i].point = currPoint;
	}
	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	for (int i1 = 0; i1 < numOfDeformers; i1++)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(i1, FbxDeformer::eSkin));

		if (!currSkin)
		{
			continue;
		}

		unsigned int numOfClusters = currSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			unsigned int currJointIndex = getJointIndex(currJointName);

			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				BlendInfo curr;
				curr.index = currJointIndex;
				curr.weight = currCluster->GetControlPointWeights()[i];
				points[currCluster->GetControlPointIndices()[i]].blends.push_back(curr);


			}
		}
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
					/*processJoints(node);*/
					getNodeVertexData(node, vertices, vertexIndexes);
					
					break;
				}
			}
			
			
			
			for (int i = 0; i < node->GetChildCount(); ++i)
			{
				processData(node->GetChild(i), vertices,vertexIndexes);
				
			}


	
}
FBX_READER_API void exportData(AnimationData &anim)
{
	
	anim.duration = animationClip.duration;
	for (unsigned int i = 0; i < animationClip.frames.size(); i++)
	{
		KeyFrameData out;
		
		for (unsigned int j = 0; j < animationClip.frames[i].joints.size(); j++)
		{
			JointData jd;
			jd.parentIndex = animationClip.frames[i].joints[j].mParentIndex;


			jd.name = animationClip.frames[i].joints[j].mName;
			FbxVector4 rot = animationClip.frames[i].joints[j].mGlobalBindposeInverse.GetR();
			FbxVector4 pos = animationClip.frames[i].joints[j].mGlobalBindposeInverse.GetT();

			FbxVector4 sca = animationClip.frames[i].joints[j].mGlobalBindposeInverse.GetS();
			FbxQuaternion quat = animationClip.frames[i].joints[j].mGlobalBindposeInverse.GetQ();
			 
			

			jd.rotation = DirectX::XMFLOAT4(quat.mData[0], quat.mData[1], quat.mData[2], quat.mData[3]);
			jd.translation = DirectX::XMFLOAT4((float)pos.mData[0], (float)pos.mData[1], (float)pos.mData[2], (float)pos.mData[3]);
			jd.scale = DirectX::XMFLOAT4((float)sca.mData[0], (float)sca.mData[1], (float)sca.mData[2], (float)sca.mData[3]);
			/*DirectX::XMVECTOR scale, translation, rotation, quaternion;

			scale = DirectX::XMVectorSet((float)sca.mData[0], (float)sca.mData[1], (float)sca.mData[2], (float)sca.mData[3]);
			translation = DirectX::XMVectorSet((float)pos.mData[0], (float)pos.mData[1], (float)pos.mData[2], (float)pos.mData[3]);
			rotation = DirectX::XMVectorSet(0, 0, 0, 0);
			quaternion = DirectX::XMVectorSet(quat.mData[0], quat.mData[1], quat.mData[2], quat.mData[3]);*/
			
		
			
			/*
			DirectX::XMMATRIX matrix = DirectX::XMMatrixAffineTransformation(scale, rotation, quaternion, translation);
		
			
			matrix = DirectX::XMMatrixTranspose(matrix);*/

		
			/*jd.matrix = matrix;*/

			
			

		

			out.time = animationClip.frames[i].time;
			out.name = animationClip.frames[i].name;
			out.joints.push_back(jd);

		}
		anim.frames.push_back(out);
		

	}
}
FBX_READER_API FbxNode* getSkeletonRoot()
{
	int y = mFBXScene->GetPoseCount();
	FbxPose *pose = mFBXScene->GetPose(0);
	
	
	if (pose->IsBindPose())
	{
		
		for (int i = 0; i < pose->GetCount(); i++)
		{
			FbxNode * node = pose->GetNode(i);
			FbxSkeleton* skeleton = node->GetSkeleton();
			if (skeleton == NULL)
				continue;

			if (skeleton->IsSkeletonRoot())
			{
				
				return node;
			}

		}
	}
	
	return nullptr;

}
FBX_READER_API void fillJointVector(FbxNode * node, int depth, int index, int parentIndex)
{
	

		Joint currJoint;

		currJoint.mParentIndex = parentIndex;
		currJoint.mName = node->GetName();
		FbxAMatrix matrix = node->EvaluateGlobalTransform();
		currJoint.mGlobalBindposeInverse = matrix;
		currJoint.mNode = node;
		joints.push_back(currJoint);
	


	for (int i = 0; i < node->GetChildCount(); i++)
	{
		fillJointVector(node->GetChild(i), depth+1 , joints.size(), index);
	}
}
FBX_READER_API void GetAnimationData()
{
	FbxAnimStack* animStack = mFBXScene->GetCurrentAnimationStack();
	FbxTimeSpan localTimeSpan = animStack->GetLocalTimeSpan();
	FbxTime time = localTimeSpan.GetDuration();
	unsigned int frameCount = time.GetFrameCount(FbxTime::eFrames24);

	for (unsigned int i = 0; i < frameCount; i++)
	{
		KeyFrame frame;
		FbxTime currTime;
		currTime.SetFrame(i, FbxTime::eFrames24);
		frame.time = currTime.GetMilliSeconds();
		for (unsigned int j = 0; j < joints.size(); j++)
		{
			Joint currJoint;

			currJoint.mParentIndex = joints[j].mParentIndex;
			currJoint.mName = joints[j].mName;
			FbxAMatrix matrix = joints[j].mNode->EvaluateGlobalTransform(currTime);
			currJoint.mGlobalBindposeInverse = matrix;
			currJoint.mNode = joints[j].mNode;
			frame.joints.push_back(currJoint);

		}
		animationClip.frames.push_back(frame);
		if(i>0)
		frame.time -= animationClip.frames[1].time;
		
	}
	animationClip.duration = time.GetMilliSeconds() - animationClip.frames[1].time;



}
bool loadFBX(const char * filename, std::vector<Vertex> &vertices, std::vector<unsigned int> &vertexIndexes, AnimationData &animation)
{
	if (!init())
		return false;

	if (!loadScene(filename))
		return false;
	
	fillJointVector(getSkeletonRoot(), 0, 0, -1);
	processData(mFBXScene->GetRootNode(), vertices, vertexIndexes);

	GetAnimationData();
	exportData(animation);


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
