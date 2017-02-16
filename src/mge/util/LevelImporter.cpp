#include <mge/util/LevelImporter.hpp>
#include <mge/core/AbstractGame.hpp>


//Static variables
const int LevelImporter::TILED_TILESIZE = 64;


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
			//Read map size
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
			//Read map data
			else if (line.find ("<data") != std::string::npos)//Check if we're at a data
			{
				//By swapping row/column, we can use actual tile coördinates to index the vector.
				std::vector <std::vector <int>> layer (map->width, std::vector <int> (map->height));//I'd be preferable to use an array, but C++ doesn't allow 2D arrays with variable sizes, so we just use a vector as if it were an array.
				int strPtr = 0;
				int strSize;
				std::string readString = "";
				for (int y = 0; y < map->height; y ++)
				{
					getline (file, line);//Read the next row
					strSize = line.length ();
					strPtr = 0;//Start at beginning of line
					for (int x = 0; x < map->width; x ++)
					{
						while (line [strPtr] != ',' && strPtr < strSize)
						{
							readString += line [strPtr];
							strPtr ++;
						}
						layer [x] [y] = std::stoi (readString);
						readString = "";
						strPtr ++;//Skip the comma
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
						importedLayers ++;
						break;
				}
			}
		}

		bool readingObjects = true;
		while (readingObjects)
		{
			//Read the next line in the file
			getline (file, line);
			if (line.find ("</objectgroup") != std::string::npos)//Check if we reached the end of the object layer
			{
				readingObjects = false;
				break;
			}
			else if (line.find ("<object id") != std::string::npos)//Check if we're at an object
			{
				XmlObject* object = new XmlObject ();
				std::string temp;
				//Get object's gid/type
				int strPos = (int)line.find ("gid=");
				strPos += 5;
				while (line [strPos] != '"')
				{
					//If we haven't reached the end of the value, add the digit to the number.
					temp += line [strPos];
					strPos ++;
				}
				object->type = std::stoi (temp);
				//Get object's x position
				temp = "";
				strPos = (int)line.find ("x=");
				strPos += 3;
				while (line [strPos] != '"')
				{
					//If we haven't reached the end of the value, add the digit to the number.
					temp += line [strPos];
					strPos ++;
				}
				object->x = std::stoi (temp) / TILED_TILESIZE;
				//Get object's z position
				temp = "";
				strPos = (int)line.find ("y=");
				strPos += 3;
				while (line [strPos] != '"')
				{
					//If we haven't reached the end of the value, add the digit to the number.
					temp += line [strPos];
					strPos ++;
				}
				object->z = (std::stoi (temp) / TILED_TILESIZE - 1);
				//Get object's properties
				while (line.find ("</properties") == std::string::npos )//While we are still reading properties
				{
					while (line.find ("<property") == std::string::npos)//Skip through lines until a property is found
					{
						getline (file, line);
					}
					//Add the property value to the property list
					temp = "";
					strPos = (int)line.find ("value=");
					strPos += 7;
					while (line [strPos] != '"')
					{
						//If we haven't reached the end of the value, add the character to the value.
						temp += line [strPos];
						strPos ++;
						if (strPos >= line.length ())
						{
							getline (file, line);
							strPos = 0;
							temp += "\n";
						}
					}
					object->properties.push_back (temp);
					getline (file, line);
				}
				map->xmlObjects.push_back (object);
			}
		}
		return map;
	}
	else
	{
		std::cout << "[ERROR]: could not import level from file 'levels/" << pFilename << "'!" << std::endl;
		AbstractGame::singletonInstance->Stop ();
		return nullptr;
	}
}