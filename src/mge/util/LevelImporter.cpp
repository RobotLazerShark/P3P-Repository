#include <mge/util/LevelImporter.hpp>


//Read a tiled file and return the imported map
LevelMap* LevelImporter::ReadFile (std::string pFilename)
{
	std::ifstream file ("levels/" + pFilename, std::ios::in);
	if (file.is_open ())
	{
		int importedLayers = 0;
		std::string line;
		LevelMap* map = nullptr;

		while (importedLayers < 2)
		{
			//Read the next line in the file
			getline (file, line);
			if (map == nullptr && line.find ("<map version=") != std::string::npos)//Check if we're at map
			{
				//Read map-width
				std::string temp;
				int strPos = (int)line.find ("width=");
				strPos += 7;//Start reading the value after 'width='
				while (line [strPos] != '"')
				{
					//If we haven't reached the end of the value, add the digit to the number.
					temp += line [strPos];
					strPos ++;
				}
				int mapWidth = std::stoi (temp);

				//Read map-height
				temp = "";
				strPos = (int)line.find ("height=", strPos);
				strPos += 8;//Start reading the value after 'height='
				while (line [strPos] != '"')
				{
					//If we haven't reached the end of the value, add the digit to the number.
					temp += line [strPos];
					strPos ++;
				}
				int mapHeight = std::stoi (temp);

				//Create map
				map = new LevelMap (mapWidth, mapHeight);
			}

			if (line.find ("<data") != std::string::npos)//Check if we're at a data
			{
				//By swapping row/column, we can use actual tile coördinates to index the vector.
				std::vector <std::vector <int>> layer (map->width, std::vector <int> (map->height));//I'd be preferable to use an array, but C++ doesn't allow 2D arrays with variable sizes, so we just use a vector as if it were an array.
				for (int y = 0; y < map->height; y ++)
				{
					getline (file, line);//Read the next row
					for (int x = 0; x < map->width; x ++)
					{
						layer [x] [y] = (line [x * 2] - '0');//x is multiplied by 2 because each integer is accompanied by a comma.
					}
				}
				switch (importedLayers)
				{
					case 0:
						map->baseTiles = layer;
						importedLayers ++;
						break;
					default:
						map->objectTiles = layer;
						return map;
				}
			}
		}
	}
	else
	{
		std::cout << "[ERROR]: could not import level from file 'levels/" << pFilename << "' !" << std::endl;
		return nullptr;
	}
}