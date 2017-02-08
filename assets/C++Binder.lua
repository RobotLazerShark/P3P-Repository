--Define colors
Color =
{
	red = { r = 1, g = 0, b = 0 },
	yellow = { r = 1, g = 1, b = 0 },
	green = { r = 0, g = 1, b = 0 },
	cyan = { r = 0, g = 1, b = 1 },
	blue = { r = 0, g = 0, b = 1 },
	magenta = { r = 1, g = 0, b = 1 },
	white = { r = 1, g = 1, b = 1 },
	gray = { r = 0.5, g = 0.5, b = 0.5 },
	black = { r = 0, g = 0, b = 0 }
}
--Define terrain densities
Density =
{
	veryLow = 35,
	low = 50,
	medium = 65,
	high = 80,
	veryHigh = 95,
}

------------------------------	DRAWABLES
--Define Image
Image = {}
ImageMetatable =
{
	__call =
	function (pTable, pFilename)
		local newTable =
		{
			pointer = nil,
			--Functions
			draw =
			function (pSelf)
				Hidden.draw (pSelf.pointer)
			end,
			ifClicked =
			function (pSelf)
				return Lua.ifClicked (pSelf.pointer)
			end,
			--||||Getters
				position =
				function (pSelf, pX, pY)
					if pX and pY
					then
						Hidden.setDrawablePosition (pSelf.pointer, pX, pY)
					else
						return Hidden.getDrawablePosition (pSelf.pointer)
					end
				end,
			--||||Transformations
				move =
				function (pSelf, pX, pY)
					Hidden.moveDrawable (pSelf.pointer, pX, pY)
				end,
				rotate =
				function (pSelf, pAngle)
					Hidden.rotateDrawable (pSelf.pointer, pAngle)
				end,
				scale =
				function (pSelf, pXScale, pYScale)
					if pXScale and pYScale
					then
						Hidden.scaleDrawable (pSelf.pointer, pXScale, pYScale)
					else
						Hidden.scaleDrawable (pSelf.pointer, pXScale)
					end
				end,
			delete =
			function (pSelf)
				Hidden.deleteDrawable (pSelf.pointer)
			end
		}
		newTable.pointer = Lua.image (pFilename, 0, 0)
		return newTable
	end
}
setmetatable (Image, ImageMetatable)
--Define Text
Text = {}
TextMetatable =
{
	__call =
	function (pTable, pText, pFont, pCharSize, pColor)
		local newTable =
		{
			pointer = nil,
			--Functions
			draw =
			function (pSelf)
				Hidden.draw (pSelf.pointer)
			end,
			--||||Getters
				position =
				function (pSelf, pX, pY)
					if pX and pY
					then
						Hidden.setDrawablePosition (pSelf.pointer, pX, pY)
					else
						return Hidden.getDrawablePosition (pSelf.pointer)
					end
				end,
			--||||Transformations
				move =
				function (pSelf, pX, pY)
					Hidden.moveDrawable (pSelf.pointer, pX, pY)
				end,
				rotate =
				function (pSelf, pAngle)
					Hidden.rotateDrawable (pSelf.pointer, pAngle)
				end,
				scale =
				function (pSelf, pXScale, pYScale)
					if pXScale and pYScale
					then
						Hidden.scaleDrawable (pSelf.pointer, pXScale, pYScale)
					else
						Hidden.scaleDrawable (pSelf.pointer, pXScale)
					end
				end,
			delete =
			function (pSelf)
				Hidden.deleteDrawable (pSelf.pointer)
			end
		}
		if pColor
		then
			newTable.pointer = Lua.text (pText, pFont, pCharSize, 0, 0, pColor.r, pColor.g, pColor.b)
		else
			newTable.pointer = Lua.text (pText, pFont, pCharSize, 0, 0)
		end
		return newTable
	end
}
setmetatable (Text, TextMetatable)
--Define Sound
Sound = {}
SoundMetatable =
{
	__call =
	function (pTable, pFilename)
		Lua.sound (pFilename)
	end
}
setmetatable (Sound, SoundMetatable)
--Define SoundLoop
SoundLoop = {}
SoundLoopMetatable =
{
	__call =
	function (pTable, pFilename)
		local newTable =
		{
			index = nil,
			--Functions
			stop =
			function (pSelf)
				Lua.stopLoop (pSelf.index)
			end
		}
		newTable.index = Lua.soundLoop (pFilename)
		return newTable
	end
}
setmetatable (SoundLoop, SoundLoopMetatable)
------------------------------	OBJECTS
--Define Object. When 'instantiated' only the pointer has to be set.
--This setup allows designers to call functions with less arguments, since the pointer of the 'object' it is called on is omitted.
--This does mean however that all functions should be called using the semicolon. [ object:function (args) instead of object.function (args) ]
Object = {}
--Object çlass'/'struct' metatable
ObjectMetatable =
{
	--Create a new 'Object' with the syntax 'Object (pFilename)'
	__call =
	function (pTable, pFilename)
		local newTable =
		{
			pointer = nil,
			--Functions
			--||||Getters
				localPosition =
				function (pSelf, pX, pY, pZ)
					if pX and pY and pZ
					then
						Hidden.setLocalPosition (pSelf.pointer)
					else
						return Hidden.getLocalPosition (pSelf.pointer)
					end
				end,
				worldPosition =
				function (pSelf, pX, pY, pZ)
					if pX and pY and pZ
					then
						Hidden.setWorldPosition (pSelf.pointer)
					else
						return Hidden.getWorldPosition (pSelf.pointer)
					end
				end,
			--||||Setters
				setMesh =
				function (pSelf, pMesh)
					Hidden.setMesh (pSelf.pointer, pMesh.pointer)
				end,
				setMaterial =
				function (pSelf, pMaterial)
					Hidden.setMaterial (pSelf.pointer, pMaterial.pointer)
				end,
				setBehaviour =
				function (pSelf, pBehaviour)
					Hidden.setBehaviour (pSelf.pointer, pBehaviour.pointer)
				end,
				parent =
				function (pSelf, pParent)
					Hidden.parent (pSelf.pointer, pParent.pointer)
				end,
				unparent =
				function (pSelf)
					Hidden.unparent (pSelf.pointer)
				end,
			--||||Transformations
				move =
				function (pSelf, pX, pY, pZ)
					Hidden.move (pSelf.pointer, pX, pY, pZ)
				end,
				rotate =
				function (pSelf, pXAngle, pYAngle, pZAngle)
					Hidden.rotate (pSelf.pointer, pXAngle, pYAngle, pZAngle)
				end,
				scale =
				function (pSelf, pScale)
					Hidden.scale (pSelf.pointer, pScale)
				end,
				badScale =
				function (pSelf, pXScale, pYScale, pZScale)
					Hidden.badScale (pSelf.pointer, pXScale, pYScale, pZScale)
				end,
			--||||Collisions
				addSphereCollider =
				function (pSelf, pRadius, pOffset, pMovedByCollisions)
					Hidden.addCollider (pSelf.pointer, pRadius, pOffset [1], pOffset [2], pOffset [3], pMovedByCollisions)
				end,
				addBoxCollider =
				function (pSelf, pSize, pOffset, pUseOuterBox, pMovedByCollisions)
					Hidden.addCollider (pSelf.pointer, pSize [1], pSize [2], pSize [3], pOffset [1], pOffset [2], pOffset [3], pUseOuterBox, pMovedByCollisions)
				end,
				removeCollider =
				function (pSelf)
					Hidden.removeCollider (pSelf.pointer)
				end,
				ifCollide =
				function (pSelf, pObjectToCheck, pResolveCollision)
					return Hidden.ifCollide (pSelf.pointer, pObjectToCheck.pointer, pResolveCollision)
				end,
			delete =
			function (pSelf)
				Hidden.delete (pSelf.pointer)
			end
		}
		newTable.pointer = Hidden.object (pFilename)
		return newTable
	end
}
setmetatable (Object, ObjectMetatable)
--Define Light
Light =
{
	setAmbientLight =
	function (pSelf, pColor, pIntensity)
		Game.setAmbientLight (pColor.r, pColor.g, pColor.b, pIntensity)
	end
}
LightMetatable =
{
	__call =
	function (pTable, pColor, pIntensity, pFalloff, pAngle)
		local newTable =
		{
			pointer = nil,
			lightType = 0,
			--Functions
			--||||Getters
				localPosition =
				function (pSelf, pX, pY, pZ)
					if pX and pY and pZ
					then
						Hidden.setLocalPosition (pSelf.pointer)
					else
						return Hidden.getLocalPosition (pSelf.pointer)
					end
				end,
				worldPosition =
				function (pSelf, pX, pY, pZ)
					if pX and pY and pZ
					then
						Hidden.setWorldPosition (pSelf.pointer)
					else
						return Hidden.getWorldPosition (pSelf.pointer)
					end
				end,
			--||||Setters
				setColor =
				function (pSelf, pColor)
					Hidden.setLightColor (pSelf.pointer, pColor.r, pColor.g, pColor.b)
				end,
				setIntensity =
				function (pSelf, pIntensity)
					Hidden.setLightIntensity (pSelf.pointer, pIntensity)
				end,
				setFalloff =
				function (pSelf, pFalloff)
					if pSelf.lightType > 1
					then
						Hidden.setLightFalloff (pSelf.pointer, pFalloff)
					end
				end,
				setAngle =
				function (pSelf, pAngle)
					if pSelf.lightType == 3
					then
						Hidden.setLightAngle (pSelf.pointer, pAngle)
					end
				end,
				setBehaviour =
				function (pSelf, pBehaviour)
					Hidden.setBehaviour (pSelf.pointer, pBehaviour.pointer)
				end,
				parent =
				function (pSelf, pParent)
					Hidden.parent (pSelf.pointer, pParent.pointer)
				end,
				unparent =
				function (pSelf)
					Hidden.unparent (pSelf.pointer)
				end,
			--||||Transformations
				move =
				function (pSelf, pX, pY, pZ)
					Hidden.move (pSelf.pointer, pX, pY, pZ)
				end,
				rotate =
				function (pSelf, pXAngle, pYAngle, pZAngle)
					Hidden.rotate (pSelf.pointer, pXAngle, pYAngle, pZAngle)
				end,
			delete =
			function (pSelf)
				Hidden.delete (pSelf.pointer)
			end
		}
		--Using 'overloads' for this function
		if pColor and pIntensity and pFalloff and pAngle
		then
			--Create a spotlight
			newTable.pointer = Hidden.spotLight (pColor.r, pColor.g, pColor.b, pIntensity, pAngle, pFalloff)
			newTable.lightType = 3
		elseif pColor and pIntensity and pFalloff
		then
			--Create a pointlight
			newTable.pointer = Hidden.pointLight (pColor.r, pColor.g, pColor.b, pIntensity, pFalloff)
			newTable.lightType = 2
		else
			--Create a directionallight
			newTable.pointer = Hidden.directionalLight (pColor.r, pColor.g, pColor.b, pIntensity)
			newTable.lightType = 1
		end
		return newTable
	end
}
setmetatable (Light, LightMetatable)
--Define DoomObject
DoomObject = {}
DoomObjectMetatable =
{
	__call =
	function (pTable, pArg1, pArg2, pArg3, pArg4)
		local newTable =
		{
			pointer = nil,
			--Functions
			ifClicked =
			function (pSelf)
				return Hidden.ifDoomClicked (pSelf.pointer)
			end,
			--||||Getters
				position =
				function (pSelf, pX, pY, pZ)
					if pX and pY and pZ
					then
						Hidden.setLocalPosition (pSelf.pointer)--Doom object's cannot have a parent (other than world)
					else
						return Hidden.getLocalPosition (pSelf.pointer)
					end
				end,
			--||||Setters
				setClickbox =
				function (pSelf, pOffset, pSize)
					Hidden.setClickHitbox (pSelf.pointer, pOffset [1], pOffset [2], pSize [1] * 0.5, pSize [2] * 0.5)
				end,
			--||||Transformations
				move =
				function (pSelf, pX, pY, pZ)
					Hidden.move (pSelf.pointer, pX, pY, pZ)
				end,
			delete =
			function (pSelf)
				Hidden.delete (pSelf.pointer)
			end
		}
		--Using 'overloads' for this function
		if pArg2 and pArg3 and pArg4
		then
			if type (pArg4) ~= "table"
			then
				--Create DoomAnimation
				newTable.pointer = Hidden.doomObject (pArg1, pArg2, pArg3, pArg4)
			else
				--Create DoomText (Colored)
				newTable.pointer = Hidden.doomObject (pArg1, pArg2, pArg3, pArg4.r, pArg4.g, pArg4.b)
			end
		elseif pArg2 and pArg3
		then
			--Create DoomText
			newTable.pointer = Hidden.doomObject (pArg1, pArg2, pArg3)
		else
			--Create DoomSprite
			newTable.pointer = Hidden.doomObject (pArg1)
		end
		return newTable
	end
}
setmetatable (DoomObject, DoomObjectMetatable)
--Define Scene
Scene = {}
SceneMetatable =
{
	__call =
	function (pTable, pExportFile)
		local newTable =
		{
			pointer = nil,
			--Functions
			--||||Getters
				position =
				function (pSelf, pX, pY, pZ)
					if pX and pY and pZ
					then
						Hidden.setLocalPosition (pSelf.pointer)--Scene's parent is always World
					else
						return Hidden.getLocalPosition (pSelf.pointer)
					end
				end,
			--||||Transformations
				move =
				function (pSelf, pX, pY, pZ)
					Hidden.move (pSelf.pointer, pX, pY, pZ)
				end,
				rotate =
				function (pSelf, pXAngle, pYAngle, pZAngle)
					Hidden.rotate (pSelf.pointer, pXAngle, pYAngle, pZAngle)
				end,
			delete =
			function (pSelf)
				Hidden.delete (pSelf.pointer)
			end
		}
		newTable.pointer = Hidden.loadScene (pExportFile)
		return newTable
	end
}
setmetatable (Scene, SceneMetatable)
------------------------------	MESHES
--Define Mesh
Mesh = {}
MeshMetatable =
{
	__call =
	function (pTable, pMeshFile)
		localnewTable =
		{
			pointer = nil
		}
		newTable.pointer = Hidden.mesh (pMeshFile)
		return newTable
	end
}
setmetatable (Mesh, MeshMetatable)
--Define TerrainMesh
TerrainMesh = {}
TerrainMeshMetatable =
{
	__call =
	function (pTable, pFile, pMaxHeight, pTerrainDensity)
		local newTable =
		{
			pointer = nil
		}
		--Using 'overloads' for this function
		if pMaxHeight and pTerrainDensity
		then
			--Build terrain from scratch
			newTable.pointer = Hidden.terrainMesh (pFile, pMaxHeight, pTerrainDensity)
		else
			--Load terrain from file
			newTable.pointer = Hidden.terrainMesh (pFile)
		end
		return newTable
	end
}
setmetatable (TerrainMesh, TerrainMeshMetatable)
------------------------------	MATERIALS
--Define ColorMaterial
ColorMaterial = {}
ColorMaterialMetatable =
{
	__call =
	function (pTable, pColor)
		local newTable =
		{
			pointer = nil,
			--Functions
			--||||Setters
				setColor =
				function (pSelf, pNewColor)
					Hidden.setColorMaterialColor (pSelf.pointer, pNewColor.r, pNewColor.g, pNewColor.b)
				end
		}
		newTable.pointer = Hidden.colorMaterial (pColor.r, pColor.g, pColor.b);
		return newTable;
	end
}
setmetatable (ColorMaterial, ColorMaterialMetatable)
--Define LitColorMaterial
LitColorMaterial = {}
LitColorMaterialMetatable =
{
	__call =
	function (pTable, pColor, pShininess)
		local newTable =
		{
			pointer = nil,
			--Functions
			--||||Setters
				setColor =
				function (pSelf, pNewColor)
					Hidden.setLitMaterialColor (pSelf.pointer, pNewColor.r, pNewColor.g, pNewColor.b)
				end,
				setShininess =
				function (pSelf, pShininess)
					Hidden.setLitMaterialShininess (pSelf.pointer, pShininess)
				end,
				setSpecularColor =
				function (pSelf, pColor)
					Hidden.setLitMaterialSpecularColor (pSelf.pointer, pColor.r, pColor.g, pColor.b)
				end
		}
		newTable.pointer = Hidden.litColorMaterial (pColor.r, pColor.g, pColor.b, pShininess)
		return newTable
	end
}
setmetatable (LitColorMaterial, LitColorMaterialMetatable)
--Define TextureMaterial
TextureMaterial = {}
TextureMaterialMetatable =
{
	__call =
	function (pSelf, pTextureFile)
		local newTable =
		{
			pointer = nil,
			--Functions
			--||||Setters
				setTexture =
				function (pSelf, pTextureFile)
					Hidden.setTextureMaterialTexture (pSelf.pointer, pTextureFile)
				end,
				setScale =
				function (pSelf, pTextureScale)
					Hidden.setTextureMaterialScale (pSelf.pointer, pTextureScale)
				end
		}
		newTable.pointer = Hidden.textureMaterial (pTextureFile)
		return newTable
	end
}
setmetatable (TextureMaterial, TextureMaterialMetatable)
--Define LitTextureMaterial
LitTextureMaterial = {}
LitTextureMaterialMetatable =
{
	__call =
	function (pSelf, pTextureFile, pShininess)
		local newTable =
		{
			pointer = nil,
			--Functions
			--||||Setters
				setTexture =
				function (pSelf, pTextureFile)
					Hidden.setLitMaterialTexture (pSelf.pointer, pTextureFile)
				end,
				setScale =
				function (pSelf, pTextureScale)
					Hidden.setLitMaterialScale (pSelf.pointer, pTextureScale)
				end,
				setShininess =
				function (pSelf, pShininess)
					Hidden.setLitMaterialShininess (pSelf.pointer, pShininess)
				end,
				setSpecularColor =
				function (pSelf, pColor)
					Hidden.setLitMaterialSpecularColor (pSelf.pointer, pColor.r, pColor.g, pColor.b)
				end
		}
		newTable.pointer = Hidden.litTextureMaterial (pTextureFile, pShininess)
		return newTable
	end
}
setmetatable (LitTextureMaterial, LitTextureMaterialMetatable)
--Define TerrainMaterial
TerrainMaterial = {}
TerrainMaterialMetatable =
{
	__call =
	function (pTable, pSplatTexFile, pTextureFiles, pLit)
		local newTable =
		{
			pointer = nil,
			--Functions
			--||||Textures
				setTexR =
				function (pSelf, pTexRFile)
					Hidden.setTerrainMaterialTexR (pSelf.pointer, pTexRFile)
				end,
				setTexRScale =
				function (pSelf, pRScale)
					Hidden.setTerrainMaterialTexRScale (pSelf.pointer, pRScale)
				end,
				setTexG =
				function (pSelf, pTexGFile)
					Hidden.setTerrainMaterialTexG (pSelf.pointer, pTexGFile)
				end,
				setTexGScale =
				function (pSelf, pGScale)
					Hidden.setTerrainMaterialTexGScale (pSelf.pointer, pGScale)
				end,
				setTexB =
				function (pSelf, pTexBFile)
					Hidden.setTerrainMaterialTexB (pSelf.pointer, pTexBFile)
				end,
				setTexBScale =
				function (pSelf, pBScale)
					Hidden.setTerrainMaterialTexBScale (pSelf.pointer, pBScale)
				end,
				setTexA =
				function (pSelf, pTexAFile)
					Hidden.setTerrainMaterialTexA (pSelf.pointer, pTexAFile)
				end,
				setTexAScale =
				function (pSelf, pAScale)
					Hidden.setTerrainMaterialTexAScale (pSelf.pointer, pAScale)
				end,
				setAllTexScales =
				function (pSelf, pRScale, pGScale, pBScale, pAScale)
					Hidden.setTerrainMaterialTexScales (pSelf.pointer, pRScale, pGScale, pBScale, pAScale)
				end,
			--||||Lighting
				setLit =
				function (pSelf, pLit)
					Hidden.setTerrainMaterialLit (pSelf.pointer, pLit)
				end,
				setShininess =
				function (pSelf, pShininess)
					Hidden.setTerrainMaterialShininess (pSelf.pointer, pShininess)
				end,
				setSpecularColor =
				function (pSelf, pSpecularColor)
					Hidden.setTerrainMaterialSpecularColor (pSelf.pointer, pSpecularColor.r, pSpecularColor.g, pSpecularColor.b)
				end,
			--||||Water
				setWaterIndex =
				function (pSelf, pWaterIndex)
					Hidden.setTerrainMaterialWaterIndex (pSelf.pointer, pWaterIndex)
				end,
				setWaveHeight =
				function (pSelf, pWaveHeight)
					Hidden.setTerrainMaterialWaveHeight (pSelf.pointer, pWaveHeight)
				end,
				setWaveCount =
				function (pSelf, pWaveCount)
					Hidden.setTerrainMaterialWaveCount (pSelf.pointer, pWaveCount)
				end
		}
		newTable.pointer = Hidden.terrainMaterial (pSplatTexFile, pTextureFiles, pLit)
		return newTable
	end
}
setmetatable (TerrainMaterial, TerrainMaterialMetatable)
------------------------------	BEHAVIOURS
--Define RotatingBehaviour
RotatingBehaviour = {}
RotatingBehaviourMetatable =
{
	__call =
	function (pTable, pXAngle, pYAngle, pZAngle)
		local newTable =
		{
			pointer = nil,
			delete =
			function (pSelf)
				Hidden.delete (pSelf.pointer)
			end
		}
		newTable.pointer = Hidden.rotatingBehaviour (pXAngle, pYAngle, pZAngle)
		return newTable
	end
}
setmetatable (RotatingBehaviour, RotatingBehaviourMetatable)
--Define KeysBehaviour
KeysBehaviour = {}
KeysBehaviourMetatable =
{
	__call =
	function (pTable)
		local newTable =
		{
			pointer = nil,
			delete =
			function (pSelf)
				Hidden.delete (pSelf.pointer)
			end
		}
		newTable.pointer = Hidden.keysBehaviour ()
		return newTable
	end
}
setmetatable (KeysBehaviour, KeysBehaviourMetatable)