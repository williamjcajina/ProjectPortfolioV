
#include "Model.h"


void Model::loadModel(const char* file_name)
{
	
	isFBX = false;

	string line;
	ifstream file(file_name);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line[0] == 'v' && line[1] == ' ')
			{


				
				string value;
				stringstream stream(line);
				std::vector<string> values;
				getline(stream, value, ' ');
				while (getline(stream, value, ' '))
				{
					if(value!= "")
					values.push_back(value);
					
				}
				DirectX::XMFLOAT3 vertex;
				
				vertex.x = stof(values[0]);
				vertex.y = stof(values[1]);
				vertex.z = stof(values[2]);
				
				
				vertices.push_back(vertex);
				
				/*cout << vertex.x << " , " << vertex.y << " , " << vertex.z << "\n";*/


				vertsNumber++;
			}

			if (line[0] == 'v' && line[1] == 'n')
			{
				string value;
				stringstream stream(line);
				std::vector<string> values;

				getline(stream, value, ' ');
				while (getline(stream, value, ' '))
				{
					if (value != "")
					values.push_back(value);

				}
				DirectX::XMFLOAT3 normal;
				normal.x = stof(values[0]);
				normal.y = stof(values[1]);
				normal.z = stof(values[2]);

				normals.push_back(normal);
			/*	cout << normal.x << " , " << normal.y << " , " << normal.z << "\n";*/
				
			}

			if (line[0] == 'v' && line[1] == 't')
			{
				string value;
				stringstream stream(line);
				std::vector<string> values;
				getline(stream, value, ' ');
				while (getline(stream, value, ' '))
				{
					if (value != "")
					values.push_back(value);

				}
				DirectX::XMFLOAT3 uv;
				
				uv.x = stof(values[0]);
				uv.y = stof(values[1]);
				if (values.size() ==3)
					uv.z = stof(values[2]);
				else
					uv.z = NULL;
				
				uv.y = 1 - uv.y;
				uvs.push_back(uv);
			/*	cout << uv.x << " , " << uv.y << " , " << uv.z << "\n";*/
			
			}

			if (line[0] == 'f')
			{
				string value;
				stringstream stream(line);
				getline(stream, value, ' ');
				while (getline(stream, value, ' '))
				{
					if (value != "")
					{
						DirectX::XMFLOAT3 index;
						std::vector<string> values;
						string str = value;
						string w;
						stringstream stream2(str);
						int i = 0;
						while (getline(stream2, w, '/'))
						{
							values.push_back(w);

						}
						index.x = stof(values[0]);
						index.y = stof(values[1]);
						index.z = stof(values[2]);
						vertexIndexes.push_back((unsigned int)index.x);
						uvIndexes.push_back((unsigned int)index.y);
						normalIndexes.push_back((unsigned int)index.z);

						/*cout << index.x << " , " << index.y << " , " << index.z << "\n";*/
					}
					
	
					
				}

				
				
			}



		}
	

		file.close();
		CreateVertexList();


	}
	


	
	
}

bool Model::loadModelFBX(const char * file_name)
{
	std::vector<Vertex> _vertices;
	
	loadFBX(file_name, _vertices, vertexIndexes , animation );

	isFBX = true;
	for (unsigned int i = 0; i < vertexIndexes.size(); i++)
	{
		VertexPositionUVNormal vertex;
		Vertex t = _vertices[i];

		vertex.pos = t.position;
		

		vertex.normal = t.normal;
		
		vertex.uv.x = t.UV.x;
		vertex.uv.y = t.UV.y;
		vertex.uv.z = 0;
		
		t.blends.resize(4);
		vertex.jointIndex.x = t.blends[0].index;
		vertex.jointIndex.y = t.blends[1].index;
		vertex.jointIndex.z = t.blends[2].index;
		vertex.jointIndex.w = t.blends[3].index;
		vertex.weights.x = t.blends[0].weight;
		vertex.weights.y = t.blends[1].weight;
		vertex.weights.z = t.blends[2].weight;
		vertex.weights.w = t.blends[3].weight;
		float sum = (vertex.weights.x + vertex.weights.y + vertex.weights.z + vertex.weights.w);
		if (sum > 1)
			int kl = 90;
		vertexList.push_back(vertex);
	}

	
	


	return true;
}

void Model::CalculateTangentBinormal(TempVertex vertex1, TempVertex vertex2, TempVertex vertex3, Vector & tangent, Vector & binormal)
{
	float vector1[3], vector2[3];
	float u[2], v[2];
	float denominator;
	float length;

	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;
	
	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	u[0] = vertex2.tu - vertex1.tu;
	v[0] = vertex2.tv - vertex1.tv;

	u[1] = vertex3.tu - vertex1.tu;
	v[1] = vertex3.tv - vertex1.tv;

	denominator = 1.0f / (u[0] * v[1] - u[1] * v[0]);

	tangent.x = (v[1] * vector1[0] - v[0] * vector2[0]) * denominator;
	tangent.y = (v[1] * vector1[1] - v[0] * vector2[1]) * denominator;
	tangent.z = (v[1] * vector1[2] - v[0] * vector2[2]) * denominator;

	binormal.x = (u[0] * vector2[0] - u[1] * vector1[0]) * denominator;
	binormal.y = (u[0] * vector2[1] - u[1] * vector1[1]) * denominator;
	binormal.z = (u[0] * vector2[2] - u[1] * vector1[2]) * denominator;

	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void Model::CalculateNormal(Vector tangent, Vector binormal, Vector & normal)
{
	float length;

	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}

void Model::CreateVertexList()
{
	int x = 0;

	for( unsigned int i = 0; i < vertexIndexes.size(); i++)
	{
		if (i == 2960)
			int y = 0;
		VertexPositionUVNormal vertex;
		

		
		vertex.pos = vertices[vertexIndexes[i]-1];
		vertex.normal = normals[normalIndexes[i]-1];
		vertex.uv = uvs[uvIndexes[i]-1];
		/*vertex.binormal = DirectX::XMFLOAT3{ 0,0,0 };
		vertex.tangent = DirectX::XMFLOAT3{ 0,0,0 };*/

		
		vertexList.push_back(vertex);

	}

}

void Model::CalculateVectors()
{
	
	//unsigned int index = 0;
	//TempVertex vertex1; 
	//TempVertex vertex2;
	//TempVertex vertex3;
	//Vector tangent;
	//Vector binormal;
	//Vector normal;
	//
	//for (unsigned int i = 0; i <vertexList.size() / 3; i++)
	//{
	//	vertex1.x = vertexList[index].pos.x;
	//	vertex1.y = vertexList[index].pos.y;
	//	vertex1.z = vertexList[index].pos.z;
	//	vertex1.tu = vertexList[index].uv.x;
	//	vertex1.tv = vertexList[index].uv.y;
	//	vertex1.nx = vertexList[index].normal.x;
	//	vertex1.ny = vertexList[index].normal.y;
	//	vertex1.nz = vertexList[index].normal.z;
	//	index++;

	//	vertex2.x = vertexList[index].pos.x;
	//	vertex2.y = vertexList[index].pos.y;
	//	vertex2.z = vertexList[index].pos.z;
	//	vertex2.tu = vertexList[index].uv.x;
	//	vertex2.tv = vertexList[index].uv.y;
	//	vertex2.nx = vertexList[index].normal.x;
	//	vertex2.ny = vertexList[index].normal.y;
	//	vertex2.nz = vertexList[index].normal.z;
	//	index++;

	//	vertex3.x = vertexList[index].pos.x;
	//	vertex3.y = vertexList[index].pos.y;
	//	vertex3.z = vertexList[index].pos.z;
	//	vertex3.tu = vertexList[index].uv.x;
	//	vertex3.tv = vertexList[index].uv.y;
	//	vertex3.nx = vertexList[index].normal.x;
	//	vertex3.ny = vertexList[index].normal.y;
	//	vertex3.nz = vertexList[index].normal.z;
	//	index++;

	//	CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);
	//	CalculateNormal(tangent, binormal, normal);
	//	vertexList[index - 1].normal.x = normal.x;
	//	vertexList[index - 1].normal.y = normal.y;
	//	vertexList[index - 1].normal.z = normal.z;
	//	vertexList[index - 1].tangent.x = tangent.x;
	//	vertexList[index - 1].tangent.y = tangent.y;
	//	vertexList[index - 1].tangent.z = tangent.z;
	//	vertexList[index - 1].binormal.x = binormal.x;
	//	vertexList[index - 1].binormal.y = binormal.y;
	//	vertexList[index - 1].binormal.z = binormal.z;

	//	vertexList[index - 2].normal.x = normal.x;
	//	vertexList[index - 2].normal.y = normal.y;
	//	vertexList[index - 2].normal.z = normal.z;
	//	vertexList[index - 2].tangent.x = tangent.x;
	//	vertexList[index - 2].tangent.y = tangent.y;
	//	vertexList[index - 2].tangent.z = tangent.z;
	//	vertexList[index - 2].binormal.x = binormal.x;
	//	vertexList[index - 2].binormal.y = binormal.y;
	//	vertexList[index - 2].binormal.z = binormal.z;

	//	vertexList[index - 3].normal.x = normal.x;
	//	vertexList[index - 3].normal.y = normal.y;
	//	vertexList[index - 3].normal.z = normal.z;
	//	vertexList[index - 3].tangent.x = tangent.x;
	//	vertexList[index - 3].tangent.y = tangent.y;
	//	vertexList[index - 3].tangent.z = tangent.z;
	//	vertexList[index - 3].binormal.x = binormal.x;
	//	vertexList[index - 3].binormal.y = binormal.y;
	//	vertexList[index - 3].binormal.z = binormal.z;



	/*}*/





}





Model::Model()
{


}
