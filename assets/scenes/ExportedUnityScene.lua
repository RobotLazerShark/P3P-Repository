tempObject0 = Hidden.UnityLight (0, 2.25, 0, 50.00001, 330, 0, 1, 1, 1, 0.3, 1)--Directional Light
Hidden.setMesh (tempObject0, 'square_arrow.obj')
tempMaterial = Hidden.colorMaterial (1, 1, 1)
Hidden.setMaterial (tempObject0, tempMaterial)
tempObject0 = Hidden.UnityLight (4.656, 0.45, -3.27, 320, 0, 0, 1, 0, 1, 0.3, 60, 0.75, 0.25, 3)--Spot Light
Hidden.setMesh (tempObject0, 'square_cone.obj')
tempMaterial = Hidden.colorMaterial (1, 0, 1)
Hidden.setMaterial (tempObject0, tempMaterial)
tempBehaviour = Hidden.rotatingBehaviour (10, 0, 1, 0)
Hidden.setBehaviour (tempObject0, tempBehaviour)
tempObject0 = Hidden.UnityObject (-0.75, 0.375, 1.5, 0, 0, 0, 0.75, 0.75, 0.75)--Cube1x1x1
Hidden.setMesh (tempObject0, 'cube_smooth.obj')
tempMaterial = Hidden.colorMaterial (1, 1, 1)
Hidden.setMaterial (tempObject0, tempMaterial)
tempObject0 = Hidden.UnityObject (-3.15, 0.225, -1.95, 0, 0, 0, 0.45, 0.45, 0.45)--Stone
Hidden.setMesh (tempObject0, 'cube_smooth.obj')
tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
Hidden.setMaterial (tempObject0, tempMaterial)
tempObject0 = Hidden.UnityObject (-4.407, 1.1475, -2.958, 0, 0, 0, 3, 3, 3)--Tree
	tempObject1 = Hidden.UnityObject (0.019, -0.1825, -0.014, 0, 0, 0, 0.2, 0.4, 0.2)--TreeStem
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.6029412, 0.3984548, 0.2128028, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (0, 0.3175, 0, 0, 0, 0, 0.6, 0.6, 0.6)--TreeLeaves
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litTextureMaterial ('Default.png', 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
tempObject0 = Hidden.UnityObject (0, -0.105, 0.02694, 0, 0, 0, 3, 3, 3)--House
	tempObject1 = Hidden.UnityObject (1.266, 0.3, 1.298, 0, 0, 0, 1.2, 0.6, 0.1)--HouseWallNorth
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (0.944, 0.3, 0.498, 0, 0, 0, 0.5, 0.6, 0.1)--HouseWallSouthEast
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (0.716, 0.3, 0.898, 0, 0, 0, 0.1, 0.6, 0.9)--HouseWallEast
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (1.816, 0.3, 0.898, 0, 0, 0, 0.1, 0.6, 0.9)--HouseWallWest
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (1.644, 0.3, 0.498, 0, 0, 0, 0.3, 0.6, 0.1)--HouseWallSouthWest
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (1.344, 0.3, 0.498, 0, 0, 0, 0.3, 0.6, 0.1)--HouseDoor
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.6029412, 0.3984548, 0.2128028, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
tempObject0 = Hidden.UnityObject (0, -0.03, 0, 0, 0, 0, 12, 0.03, 9)--TownSquare
Hidden.setMesh (tempObject0, 'plane.obj')
tempMaterial = Hidden.litColorMaterial (0.555936, 0.9117647, 0.4156574, 0)
Hidden.setMaterial (tempObject0, tempMaterial)
tempObject0 = Hidden.UnityObject (-7.5, -0.105, 0.02694, 0, 0, 0, 3, 3, 3)--Tavern
	tempObject1 = Hidden.UnityObject (1.266, 0.3, 1.298, 0, 0, 0, 1.2, 0.6, 0.1)--HouseWallNorth
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (0.894, 0.3, 0.498, 0, 0, 0, 0.4, 0.6, 0.1)--HouseWallSouthEast
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (0.716, 0.3, 0.898, 0, 0, 0, 0.1, 0.6, 0.9)--HouseWallEast
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (1.816, 0.3, 0.898, 0, 0, 0, 0.1, 0.6, 0.9)--HouseWallWest
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
	tempObject1 = Hidden.UnityObject (1.594, 0.3, 0.498, 0, 0, 0, 0.4, 0.6, 0.1)--HouseWallSouthWest
	Hidden.setMesh (tempObject1, 'cube_flat.obj')
	tempMaterial = Hidden.litColorMaterial (0.7426471, 0.7426471, 0.7426471, 0)
	Hidden.setMaterial (tempObject1, tempMaterial)
	Hidden.parent (tempObject1, tempObject0)
tempObject0 = Hidden.UnityObject (1.35, 0.45, -3.9, 0, 0, 0, 0.6, 0.9, 0.9)--Lever
Hidden.setMesh (tempObject0, 'cube_flat.obj')
tempMaterial = Hidden.litColorMaterial (0.9485294, 0.8596409, 0.362673, 0)
Hidden.setMaterial (tempObject0, tempMaterial)
tempObject0 = Hidden.UnityObject (0, 1.2, -4.5, 0, 0, 0, 1.5, 2.4, 0.45)--Gate
Hidden.setMesh (tempObject0, 'cube_flat.obj')
tempMaterial = Hidden.litColorMaterial (0.6029412, 0.3984548, 0.2128028, 0)
Hidden.setMaterial (tempObject0, tempMaterial)
tempObject0 = Hidden.UnityObject (4.95, 0.225, 3.45, 0, 0, 0, 0.45, 0.45, 0.45)--Gold
Hidden.setMesh (tempObject0, 'cube_flat.obj')
tempMaterial = Hidden.litColorMaterial (0.9485294, 0.8596409, 0.362673, 150)
Hidden.setMaterial (tempObject0, tempMaterial)
tempObject0 = Hidden.UnityObject (5.55, 0.225, -4.05, 0, 0, 0, 0.45, 0.45, 0.45)--Key
Hidden.setMesh (tempObject0, 'cube_flat.obj')
tempMaterial = Hidden.litColorMaterial (0.9485294, 0.8596409, 0.362673, 150)
Hidden.setMaterial (tempObject0, tempMaterial)
tempObject0 = Hidden.UnityObject (-2.7, 0.5475, 3.3, 0, 0, 0, 0.6, 1.2, 0.6)--NPC
Hidden.setMesh (tempObject0, 'cube_flat.obj')
tempMaterial = Hidden.colorMaterial (0.6029412, 0.3984548, 0.2128028)
Hidden.setMaterial (tempObject0, tempMaterial)
