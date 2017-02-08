#include "mge/core/Mesh.hpp"
#include <iostream>
#include <map>
#include <string>
#include <fstream>

using namespace std;


static map <string, Mesh*> _meshCache;


Mesh::Mesh (string pId) : _id (pId), _indexBufferId (0), _vertexBufferId (0), _normalBufferId (0), _uvBufferId (0), _vertices (), _normals (), _uvs (), _indices ()
{
	//ctor
}

void Mesh::copyData (Mesh* pMesh)
{
	_id = pMesh->_id;
	_indexBufferId = pMesh->_indexBufferId;
	_vertexBufferId = pMesh->_vertexBufferId;
	_normalBufferId = pMesh->_normalBufferId;
	_uvBufferId = pMesh->_uvBufferId;
	_vertices = pMesh->_vertices;
	_normals = pMesh->_normals;
	_uvs = pMesh->_uvs;
	_indices = pMesh->_indices;
}

Mesh::~Mesh ()
{
	//dtor
}

Mesh* Mesh::load (string pFileName)
{
//	cout << "Loading " << pFileName << "..." << endl;
	//Check if mesh was already loaded
	map <string, Mesh*>::iterator itr = _meshCache.find (pFileName);
	if (itr != _meshCache.end ())
	{
//		cout << "Mesh was already cached." << endl;
		return itr->second;
	}

	//If not, attempt to load it from file
	_meshCache [pFileName] = nullptr;//Cache nullptr for bad meshes. (When loading succeeds, this map-value is changed to the loaded mesh)
	Mesh* mesh = new Mesh (pFileName);
	ifstream file (pFileName, ios::in);

	if (file.is_open ())
	{
		//these three vectors will contain the data as taken from the obj file
		//in the order it is encountered in the object file
		vector <glm::vec3> vertices;
		vector <glm::vec3> normals;
		vector <glm::vec2> uvs;

		//Use a map to make sure we don't store the same triplet multiple times.
		map <FaceIndexTriplet, unsigned int> mappedTriplets;

		string line;
		while (getline (file, line))
		{
			// c-type string to store cmd read from obj file (cmd is v, vt, vn, f)
			char cmd [13];
			cmd [0] = 0;
			int data = 0;//The obj should contain vertices (+1), but doesn't neccessarily contain uvs (+2) or normals (+3). This variable represents what data is in the file.
			//get the first string in the line of max 13 chars (c-style)
			sscanf (line.c_str (), "%13s", cmd);

			//These if statements mean that the order of the different data types doesn't matter, as long as the values of a face are defined before the face itself.
			if (strcmp (cmd, "v") == 0)//Reading a vertex line
			{
				glm::vec3 vertex;
				sscanf (line.c_str (), "%13s %f %f %f ", cmd, &vertex.x, &vertex.y, &vertex.z);//Copy .obj values into vertex
				vertices.push_back (vertex);
			}
			else if (strcmp (cmd, "vn") == 0)//Reading a normal line
			{
				glm::vec3 normal;
				sscanf (line.c_str (), "%13s %f %f %f ", cmd, &normal.x, &normal.y, &normal.z);//Copy .obj values into normal
				normals.push_back (normal);
			}
			else if (strcmp (cmd, "vt") == 0)//Reading a uv line
			{
				glm::vec2 uv;
				sscanf (line.c_str (), "%13s %f %f ", cmd, &uv.x, &uv.y);//Copy .obj values into uv
				uvs.push_back (uv);
			}
			else if (strcmp (cmd, "f") == 0)//Reading a face line
			{
				//One face contains 3 (or 4) sets of vertexIndex / uvIndex / normalIndex.
				glm::ivec4 vertexIndex;
				glm::ivec4 normalIndex;
				glm::ivec4 uvIndex;
				//Store the line values in vertexIndex, normalIndex and uvIndex
				int count = 0;
				switch (data)
				{
					case 6://All data is present
						count = sscanf (line.c_str (), "%13s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", cmd, &vertexIndex [0], &uvIndex [0], &normalIndex [0], &vertexIndex [1], &uvIndex [1], &normalIndex [1], &vertexIndex [2], &uvIndex [2], &normalIndex [2], &vertexIndex [3], &uvIndex [3], &normalIndex [3]);
						break;
					case 4://No UV's
						count = sscanf (line.c_str (), "%13s %d//%d %d//%d %d//%d %d//%d", cmd, &vertexIndex [0], &normalIndex [0], &vertexIndex [1], &normalIndex [1], &vertexIndex [2], &normalIndex [2], &vertexIndex [3], &normalIndex [3]);
						break;
					case 3://No normals
						count = sscanf (line.c_str (), "%13s %d/%d %d/%d %d/%d %d/%d", cmd, &vertexIndex [0], &uvIndex [0], &vertexIndex [1], &uvIndex [1], &vertexIndex [2], &uvIndex [2], &vertexIndex [3], &uvIndex [3]);
						break;
					case 1://No UV's and no normals
						count = sscanf (line.c_str (), "%13s %d %d %d %d", cmd, &vertexIndex [0], &vertexIndex [1], &vertexIndex [2], &vertexIndex [3]);
						break;
					case 0://Data is not yet defined, identify what data is in the obj
						data = 6;
						count = sscanf (line.c_str (), "%13s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", cmd, &vertexIndex [0], &uvIndex [0], &normalIndex [0], &vertexIndex [1], &uvIndex [1], &normalIndex [1], &vertexIndex [2], &uvIndex [2], &normalIndex [2], &vertexIndex [3], &uvIndex [3], &normalIndex [3]);
						if (count < 10)
						{
							data = 4;
							count = sscanf (line.c_str (), "%13s %d//%d %d//%d %d//%d %d//%d", cmd, &vertexIndex [0], &normalIndex [0], &vertexIndex [1], &normalIndex [1], &vertexIndex [2], &normalIndex [2], &vertexIndex [3], &normalIndex [3]);
							if (count < 7)
							{
								data = 3;
								count = sscanf (line.c_str (), "%13s %d/%d %d/%d %d/%d %d/%d", cmd, &vertexIndex [0], &uvIndex [0], &vertexIndex [1], &uvIndex [1], &vertexIndex [2], &uvIndex [2], &vertexIndex [3], &uvIndex [3]);
								if (count < 7)
								{
									data = 1;
									count = sscanf (line.c_str (), "%13s %d %d %d %d", cmd, &vertexIndex [0], &vertexIndex [1], &vertexIndex [2], &vertexIndex [3]);
									if (count < 4)
									{
										data = -1;//Something is wrong with the obj file.
									}
								}
							}
						}
						break;
					default:
						break;
				}

				//Correct negative indices
				for (int i = 0; i < 4; ++i)
				{
					if (vertexIndex [i] < 0)
					{
						//When a vertexindex is positive, it is the actual index value, but when it is negative, it is relative to the current 'bottom' of the vector.
						vertexIndex [i] = vertices.size () + (vertexIndex [i] + 1);//The one is added here because it will be removed later on.
					}
					if (uvIndex [i] < 0)
					{
						//When a vertexindex is positive, it is the actual index value, but when it is negative, it is relative to the current 'bottom' of the vector.
						uvIndex [i] = uvs.size () + (uvIndex [i] + 1);//The one is added here because it will be removed later on.
					}
					if (normalIndex [i] < 0)
					{
						//When a vertexindex is positive, it is the actual index value, but when it is negative, it is relative to the current 'bottom' of the vector.
						normalIndex [i] = normals.size () + (normalIndex [i] + 1);//The one is added here because it will be removed later on.
					}
				}

				if ((data == 6 && count == 10) || ((data == 4 || data == 3) && count == 7) || (data == 1 && count == 4))//Read triangles
				{
					for (int i = 0; i < 3; ++i )//Process one triplet at a time
					{
						//Check if this triplet is already stored in the map
						FaceIndexTriplet triplet (vertexIndex [i], uvIndex [i], normalIndex [i]);
						map <FaceIndexTriplet, unsigned int>::iterator found = mappedTriplets.find (triplet);
						//If not, store it in the map, and add it to the mesh data
						if (found == mappedTriplets.end ())
						{
							//Store the triplet in the map
							unsigned int index = mappedTriplets.size ();
							mappedTriplets [triplet] = index;
							//Store the triplet in the mesh
							mesh->_indices.push_back (index);
							mesh->_vertices.push_back (vertices [vertexIndex [i] - 1]);
							//If there are no normals or uvs available, use default values
							glm::vec3 normal = (data == 6 || data == 4) ? normals [normalIndex [i] - 1] : glm::vec3 (1.0f, 0.0f, 0.0f);
							mesh->_normals.push_back (normal);
							glm::vec2 uv = (data == 6 || data == 3) ? uvs [uvIndex [i] - 1] : glm::vec2 (0.0f, 0.0f);
							mesh->_uvs.push_back (uv);
						}
						else
						{
							//If this triplet is already in the map, use that index
							unsigned int index = found->second;
							mesh->_indices.push_back (index);
						}
					}
				}
				else if ((data == 6 && count == 13) || ((data == 4 || data == 3) && count == 9) || (data == 1 && count == 5))//Read quads and convert them to triangles
				{
					//The quads need to be triangulated. The first triangle is triplets 1, 2 and 3. The second triangle is triplets 1, 3 and 4.
					unsigned int triplet1Index;
					unsigned int triplet3Index;
					unsigned int index;
					for (int i = 0; i < 4; ++i)//Process one triplet at a time
					{
						if (i == 3)//triplet 4: form second triangle with triplet 1 and triplet 3
						{
							mesh->_indices.push_back (triplet1Index);
							mesh->_indices.push_back (triplet3Index);
						}
						//Check if this triplet is already stored in the map
						FaceIndexTriplet triplet (vertexIndex [i], uvIndex [i], normalIndex [i]);
						map <FaceIndexTriplet, unsigned int>::iterator found = mappedTriplets.find (triplet);
						//If not, store it in the map, and add it to the mesh data
						if (found == mappedTriplets.end ())
						{
							//Store the triplet in the map
							index = mappedTriplets.size ();
							mappedTriplets [triplet] = index;
							//Store the triplet in the mesh
							mesh->_indices.push_back (index);
							mesh->_vertices.push_back (vertices [vertexIndex [i] - 1]);
							//If there are no normals or uvs available, use default values
							glm::vec3 normal = (data == 6 || data == 4) ? normals [normalIndex [i] - 1] : glm::vec3 (1.0f, 0.0f, 0.0f);
							mesh->_normals.push_back (normal);
							glm::vec2 uv = (data == 6 || data == 3) ? uvs [uvIndex [i] - 1] : glm::vec2 (0.0f, 0.0f);
							mesh->_uvs.push_back (uv);
						}
						else
						{
							//If this triplet is already in the map, use that index
							index = found->second;
							mesh->_indices.push_back (index);
						}
						if (i == 0)//triplet 1: store index for second triangle
						{
							triplet1Index = index;
						}
						else if (i == 2)//triplet 3: store index for second triangle
						{
							triplet3Index = index;
						}
					}
				}
				else
				{
					//If we read a different amount, something is wrong
					cout << "Error reading obj.\tData: " << data << "\nCould not read the line:\n'" << line << "'." << endl;
					delete mesh;
					return NULL;
				}
			}
		}
		file.close ();
		mesh->_buffer ();
	//	cout << "Mesh loaded and buffered:" << (mesh->_indices.size () / 3.0f) << " triangles." << endl;
		_meshCache [pFileName] = mesh;
		return mesh;
	}
	else//File could not be opened.
	{
		cout << "Could not read " << pFileName << "\nCheck if the filename is correct." << endl;
		delete mesh;
		return NULL;
	}
}

void Mesh::_buffer()
{
    glGenBuffers(1, &_indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &_vertexBufferId);
    glBindBuffer( GL_ARRAY_BUFFER, _vertexBufferId);
    glBufferData( GL_ARRAY_BUFFER, _vertices.size()*sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW );

    glGenBuffers(1, &_normalBufferId);
    glBindBuffer( GL_ARRAY_BUFFER, _normalBufferId );
    glBufferData( GL_ARRAY_BUFFER, _normals.size()*sizeof(glm::vec3), &_normals[0], GL_STATIC_DRAW );

    glGenBuffers(1, &_uvBufferId);
    glBindBuffer( GL_ARRAY_BUFFER, _uvBufferId );
    glBufferData( GL_ARRAY_BUFFER, _uvs.size()*sizeof(glm::vec2), &_uvs[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Mesh::streamToOpenGL(GLint pVerticesAttrib, GLint pNormalsAttrib, GLint pUVsAttrib) {
    if (pVerticesAttrib != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
        glEnableVertexAttribArray(pVerticesAttrib);
        glVertexAttribPointer(pVerticesAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    }

    if (pNormalsAttrib != -1) {
        glBindBuffer( GL_ARRAY_BUFFER, _normalBufferId);
        glEnableVertexAttribArray(pNormalsAttrib);
        glVertexAttribPointer(pNormalsAttrib, 3, GL_FLOAT, GL_TRUE, 0, 0 );
    }

    if (pUVsAttrib != -1) {
        glBindBuffer( GL_ARRAY_BUFFER, _uvBufferId);
        glEnableVertexAttribArray(pUVsAttrib);
        glVertexAttribPointer(pUVsAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBufferId );

	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);

	// no current buffer, to avoid mishaps, very important for performance

	glBindBuffer(GL_ARRAY_BUFFER,0);

	//fix for serious performance issue
	if (pUVsAttrib != -1) glDisableVertexAttribArray(pUVsAttrib);
	if (pNormalsAttrib != -1) glDisableVertexAttribArray(pNormalsAttrib);
	if (pVerticesAttrib != -1) glDisableVertexAttribArray(pVerticesAttrib);
}

void Mesh::drawDebugInfo(const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    //demo of how to render some debug info using the good ol' direct rendering mode...
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(pProjectionMatrix));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(pViewMatrix * pModelMatrix));

    glBegin(GL_LINES);
    //for each index draw the normal starting at the corresponding vertex
    for (size_t i=0; i<_indices.size(); i++){
        //draw normal for vertex
        if (true) {
            //now get normal end
            glm::vec3 normal = _normals[_indices[i]];
            glColor3fv(glm::value_ptr(normal));

            glm::vec3 normalStart = _vertices[_indices[i]];
            glVertex3fv(glm::value_ptr(normalStart));
            glm::vec3 normalEnd = normalStart + normal*0.2f;
            glVertex3fv(glm::value_ptr(normalEnd));
        }

    }
    glEnd();
}



