#include <mge/core/TerrainMesh.hpp>


//////////////////////////////|	INSTANCE MANAGEMENT
TerrainMesh::TerrainMesh () : Mesh ("terrain") {}
TerrainMesh::TerrainMesh (Mesh* pMesh) : Mesh ("loaded-terrain")
{
	copyData (pMesh);
}


//////////////////////////////|	TERRAIN GENERATION
//Build a new mesh from a heightmap, heightscale and a triangledensity
TerrainMesh* TerrainMesh::BuildTerrain (std::string pHeightMapFile, float pMaxHeight, int pDensity)
{
	//Build the default plane with the specified verticedensity
	TerrainMesh* mesh = buildBase (pDensity);

	//Get the heightmap data
	sf::Image heightmap = sf::Image ();
	mesh->_heightFile = config::MGE_TEXTURE_PATH + pHeightMapFile;
	if (!heightmap.loadFromFile (mesh->_heightFile))
	{
		std::cout << "[ERROR]: The heightmap file '" << mesh->_heightFile << "' could not be loaded!" << std::endl;
		mesh->_buffer ();
		return mesh;
	}
	sf::Vector2u imageSize = heightmap.getSize ();
	mesh->_pixelOffset = glm::vec2 (1.0f/imageSize.x, 1.0f/imageSize.y);
	mesh->_maxHeight = pMaxHeight;
	mesh->_heightMapId = Texture::load (mesh->_heightFile)->getId ();

	//Alter the vertices and normals based on the heightmap
	for (int i = 0, vertexCount = pDensity * pDensity; i < vertexCount; i ++)
	{
		//Calculate vertex height from heightmap
		unsigned int pixelX = (unsigned int)round (mesh->_uvs [i].x * imageSize.x);
		unsigned int pixelY = (unsigned int)round (mesh->_uvs [i].y * imageSize.y);
		if (pixelX >= imageSize.x)
		{
			pixelX = imageSize.x - 1;
		}
		if (pixelY >= imageSize.y)
		{
			pixelY = imageSize.y - 1;
		}
		mesh->_vertices [i].y = ((heightmap.getPixel (pixelX, pixelY)).r / 255.0f) * pMaxHeight;
		//Calculate new vertex normal (the process is easier here, since we have access to the other vertices position's)
		int z = i / pDensity;
		int x = i - z * pDensity;
		if (z > 0 && z < (pDensity - 1) && x > 0 && x < (pDensity - 1))//We need all the surrounding vertices to calculate the normal
		{
			glm::vec2 offset = mesh->_pixelOffset;
			glm::vec2 uv = mesh->_uvs [i];

			glm::vec3 leftSample = glm::vec3 ((uv.x - offset.x - 0.5f) * SIZE, 0, (uv.y - 0.5f) * SIZE);
			leftSample.y = heightmap.getPixel (pixelX - 1, pixelY).r / 255.0f * pMaxHeight;
			glm::vec3 rightSample = glm::vec3 ((uv.x + offset.x - 0.5f) * SIZE, 0, (uv.y - 0.5f) * SIZE);
			rightSample.y = heightmap.getPixel (pixelX + 1, pixelY).r / 255.0f * pMaxHeight;
			glm::vec3 topSample = glm::vec3 ((uv.x - 0.5f) * SIZE, 0, (uv.y + offset.y - 0.5f) * SIZE);
			topSample.y = heightmap.getPixel (pixelX, pixelY + 1).r / 255.0f * pMaxHeight;
			glm::vec3 bottomSample = glm::vec3 ((uv.x - 0.5f) * SIZE, 0, (uv.y - offset.y - 0.5f) * SIZE);
			bottomSample.y = heightmap.getPixel (pixelX, pixelY - 1).r / 255.0f * pMaxHeight;
			mesh->_normals [i] = glm::normalize (glm::cross (rightSample - leftSample, bottomSample - topSample));
		}
	}

	//Send the mesh data to opengl
	mesh->_buffer ();

	return mesh;
}

//Build the base plane to use when generating a new terrain mesh
TerrainMesh* TerrainMesh::buildBase (int pDensity)
{
	//Create mesh to store mesh data
	TerrainMesh* mesh = new TerrainMesh ();
	
	//Make all vertices, uv's and normals
	int tripletIndex = 0;
	for (int z = 0; z < pDensity; z ++)//z is along world z-axis
	{
		for (int x = 0; x < pDensity; x ++)//x is along world x-axis
		{
			//Create new triplet: vertex position and uv can already be calculated, normal should point in +y direction.
			float relativeX = x / (float)(pDensity - 1);
			float relativeZ = (pDensity - (z + 1)) / (float)(pDensity - 1);//z direction has to be flipped.
			mesh->_vertices.push_back (glm::vec3 ((relativeX - 0.5f) * SIZE, 0, (relativeZ - 0.5f) * SIZE));
			mesh->_uvs.push_back (glm::vec2 (relativeX, relativeZ));//uv's will go from top-left to bottom-right.
			mesh->_normals.push_back (glm::vec3 (0, 1, 0));//Default plane: normal points up

			//Construct quads, split from upper-left to bottom-right
			if (x > 0)
			{
				if (z < (pDensity - 1))//Construct bottom-right triangle from triplets
				{
					mesh->_indices.push_back (tripletIndex - 1);
					mesh->_indices.push_back (tripletIndex);
					mesh->_indices.push_back (tripletIndex + pDensity);//Jumping forward is safe since the size of the collection is known.
				}
				if (z > 0)//Construct upper-left triangle from triplets
				{
					mesh->_indices.push_back (tripletIndex - pDensity - 1);
					mesh->_indices.push_back (tripletIndex);
					mesh->_indices.push_back (tripletIndex - 1);
				}
			}
			//Update index for next triplet
			tripletIndex ++;
		}
	}
	return mesh;
}


//////////////////////////////|	WRITING TO FILE
//Write the mesh data to a OBJ file
void TerrainMesh::SaveAs (std::string pFilename)
{
	std::ofstream file ("mge/models/"+pFilename);
	file << "# " << _maxHeight << " " << _heightFile << "\n# This file was created from a heightmap, and then converted to OBJ." << std::endl;
	int vertexCount = _vertices.size ();
	glm::vec3 vert;
	glm::vec2 uv;
	glm::vec3 norm;
	for (int i = 0; i < vertexCount; i ++)
	{
		vert = _vertices [i];
		file << "v " << vert.x << " " << vert.y << " " << vert.z << std::endl;
		uv = _uvs [i];
		file << "vt " << uv.x << " " << uv.y << std::endl;
		norm = _normals [i];
		file << "vn " << norm.x << " " << norm.y << " " << norm.z << std::endl;
	}
	int index;
	for (int i = 0, size = _indices.size (); i < size; i += 3)
	{
		index = _indices [i] + 1;//OBJ files indices start at 1.
		file << "f " << index << "/" << index << "/" << index;
		index = _indices [i + 1] + 1;
		file << " " << index << "/" << index << "/" << index;
		index = _indices [i + 2] + 1;
		file << " " << index << "/" << index << "/" << index << std::endl;
	}
}


//////////////////////////////|	LOADING FROM FILE
//Load a terrainmesh from an obj file
TerrainMesh* TerrainMesh::loadTerrain (std::string pFilename)
{
	TerrainMesh* mesh = new TerrainMesh (Mesh::load (config::MGE_MODEL_PATH+pFilename));
	std::ifstream file (config::MGE_MODEL_PATH+pFilename, std::ios::in);

	if (file.is_open ())
	{
		std::string line;
		getline (file, line);
		if (line [0] == '#')
		{
			line.erase (0, 2);

			//Read variables from file
			std::string tempString = "";
			std::stringstream stream (line);
			getline (stream, tempString, ' ');
			mesh->_maxHeight = std::stof (tempString);
			getline (stream, tempString, ' ');
			mesh->_heightFile = tempString;
			//Check if read filepath is still valid
			sf::Image heightmap = sf::Image ();
			if (!heightmap.loadFromFile (tempString))
			{
				std::cout << "[ERROR]: The heightmap file '" << tempString << "' could not be loaded!" << std::endl;
				return mesh;
			}
			//Set derived variables
			sf::Vector2u imageSize = heightmap.getSize ();
			mesh->_pixelOffset = glm::vec2 (1.0f/imageSize.x, 1.0f/imageSize.y);
			mesh->_heightMapId = Texture::load (tempString)->getId ();
		}
		else
		{
			std::cout << "[ERROR]: could not read terrain-specific data." << std::endl;
			return mesh;
		}
	}
	file.close ();
	return mesh;
}


//////////////////////////////|	SHADER DATA
float TerrainMesh::maxHeight ()
{
	return _maxHeight;
}
glm::vec2 TerrainMesh::pixelOffset ()
{
	return _pixelOffset;
}
GLuint TerrainMesh::heightMapId ()
{
	return _heightMapId;
}