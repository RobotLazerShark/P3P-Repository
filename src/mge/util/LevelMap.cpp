#include <mge/util/LevelMap.hpp>


//Constructor
LevelMap::LevelMap (int pWidth, int pHeight) : width (pWidth), height (pHeight)
{
}


/*
void LevelMap::loadLevel()
{
    Mesh * cubeMeshS = Mesh::load (config::MGE_MODEL_PATH+"cube_smooth.obj");

    AbstractMaterial* redMat = new ColorMaterial (glm::vec3(1,0,0));
    AbstractMaterial* greenMat = new ColorMaterial (glm::vec3(0,1,0));
    AbstractMaterial* blueMat = new ColorMaterial (glm::vec3(0,0,1));
    AbstractMaterial* whiteMat = new ColorMaterial (glm::vec3(1,1,1));
    AbstractMaterial* dirtyMat = new ColorMaterial (glm::vec3(.5f,.5f,0));

    int sizeX = objectTiles.size();
    int sizeY = objectTiles[0].size();
    platforms = std::vector <std::vector <GameObject*>>  (sizeX, std::vector <GameObject*> (sizeY));
    spawnedObjects = std::vector <std::vector <GameObject*>>  (sizeX, std::vector <GameObject*> (sizeY));
    Player * player;
    ProgressTracker * progressTracker = new ProgressTracker();

    //platform layer
    for(int x = 0; x< baseTiles.size(); x++)
    {
        for(int y = 0; y< baseTiles[0].size(); y++)
        {
            if(baseTiles[x][y] == 1)
            {
                //red platform
                GameObject * plat = new GameObject("plat", glm::vec3(x*TILESIZE,0,y*TILESIZE));
                plat->setMesh(cubeMeshS);
                plat->setMaterial(redMat);
                World::singletonInstance->add(plat);

                platforms[x][y] = plat;
            }
            else if(baseTiles[x][y] == 2)
            {
                //green platform
                GameObject * plat = new GameObject("plat", glm::vec3(x*TILESIZE,0,y*TILESIZE));
                plat->setMesh(cubeMeshS);
                plat->setMaterial(greenMat);
	      World::singletonInstance->add(plat);

                platforms[x][y] = plat;
            }
            else if(baseTiles[x][y] == 3)
            {
                //box spot
                BoxSpot * boxSpot = new BoxSpot("boxSpot", glm::vec3(x*TILESIZE,0,y*TILESIZE));
                boxSpot->setMesh(cubeMeshS);
                boxSpot->setMaterial(dirtyMat);
	      World::singletonInstance->add(boxSpot);

                platforms[x][y] = boxSpot;

                //let progress tracker now which boxSpots to check
                progressTracker->boxSpots.push_back(boxSpot);
            }
        }
    }

    //object layer
    for(int x = 0; x< objectTiles.size(); x++)
    {
        for(int y = 0; y< objectTiles[0].size(); y++)
        {
            if(objectTiles[x][y] == 4)
            {
                //blue box
                Box * box = new Box("box", glm::vec3(x*TILESIZE,TILESIZE,y*TILESIZE));
                box->setMesh(cubeMeshS);
                box->setMaterial(blueMat);
	      World::singletonInstance->add(box);

                spawnedObjects[x][y] = box;
            }
            else if(objectTiles[x][y] == 5)
            {
                //blue box
                Box * box = new Box("box", glm::vec3(x*TILESIZE,TILESIZE,y*TILESIZE));
                box->setMesh(cubeMeshS);
                box->setMaterial(blueMat);
	      World::singletonInstance->add(box);

                spawnedObjects[x][y] = box;
            }
            else if(objectTiles[x][y] == 6)
            {
                //player
                player = new Player(glm::vec3(x*TILESIZE, TILESIZE, y*TILESIZE), progressTracker);
                player->setMesh(cubeMeshS);
                player->setMaterial(whiteMat);
                player->setBehaviour(new PlayerBehaviour(this, World::singletonInstance));
	      World::singletonInstance->add(player);

                spawnedObjects[x][y] = player;

                player->currentTile[0] = x;
                player->currentTile[1] = y;
            }
            else if(objectTiles[x][y] == 7)
            {
                //door
                Door * door = new Door("door", glm::vec3(x*TILESIZE,TILESIZE,y*TILESIZE));
                door->setMesh(cubeMeshS);
                door->setMaterial(dirtyMat);
	      World::singletonInstance->add(door);

                spawnedObjects[x][y] = door;
            }
        }
    }

}
void LevelMap::clearLevel()
{
    //remove platforms
     for(int x = 0; x< platforms.size(); x++)
     {
        for(int y = 0; y< platforms[0].size(); y++)
        {
            if(platforms[x][y] != nullptr )
		  World::singletonInstance->remove(platforms[x][y]);
        }
     }

     //remove objects
     for(int x = 0; x< spawnedObjects.size(); x++)
     {
        for(int y = 0; y< spawnedObjects[0].size(); y++)
        {
            if(spawnedObjects[x][y] != nullptr )
		  World::singletonInstance->remove(spawnedObjects[x][y]);
        }
     }
}
*/