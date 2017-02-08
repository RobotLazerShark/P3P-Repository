#ifndef TerrainMesh_Def
#define TerrainMesh_Def


//Include files
#include <string>
#include <fstream>
#include <sstream>
#include <SFML\Graphics.hpp>
#include <mge/config.hpp>
#include <mge/core/Texture.hpp>
#include <mge/core/Mesh.hpp>


class TerrainMesh : public Mesh
{

	public:
		enum Density { LowExtreme = 35, VeryLow = 45, Low = 55, Medium = 65, High = 75, VeryHigh = 85, HighExtreme = 95 };
		TerrainMesh ();
		TerrainMesh (Mesh* pMesh);
		static TerrainMesh* BuildTerrain (std::string pHeightMapFile, float pMaxHeight = 1, int pDensity = Density::Medium);
		void SaveAs (std::string pFilename);
		static TerrainMesh* loadTerrain (std::string pFilename);
		float maxHeight ();
		glm::vec2 pixelOffset ();
		GLuint heightMapId ();
		static const int SIZE = 20;
	private:
		static TerrainMesh* buildBase (int pDensity);
		std::string _heightFile;
		float _maxHeight;
		glm::vec2 _pixelOffset;
		GLuint _heightMapId;

};

#endif