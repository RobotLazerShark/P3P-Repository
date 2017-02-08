#version 330

uniform float time;
uniform int waterIndex;
uniform float waveHeight;
uniform float waveCount;
uniform sampler2D splatMap;
uniform mat4 MVPmatrix;
uniform mat4 modelMatrix;
uniform bool lit;
uniform float maxHeight;
uniform vec2 pixelOffset;
uniform int terrainSize;
uniform sampler2D heightMap;
in vec3 vertex;
in vec3 normal;
in vec2 uv;
out vec4 fragWorldPosition;
out vec4 fragWorldNormal;
out vec2 splatUV;
out float uvWaterScale;
out float waterAmount;

void main (void)
{
	uvWaterScale = 0;
	waterAmount = 0;
	if (waterIndex >= 1 && waterIndex <= 4)
	{
		vec4 splatColor = texture (splatMap, uv);
		waterAmount = splatColor [waterIndex - 1];//waterIndex is from 1 - 4, color indices from 0 - 3
		float waveSize = sin (time + length (vertex) * waveCount) * waterAmount;
		gl_Position = MVPmatrix * vec4 (vertex.x, vertex.y + (waveSize * waveHeight), vertex.z, 1);
		uvWaterScale = waveSize;
		fragWorldPosition = modelMatrix * vec4 (vertex.x, vertex.y + (waveSize * waveHeight), vertex.z, 1);
		if (lit && maxHeight != 0)//Recalculate normal if necessary
		{
			float sampleWaterAmount;
			vec3 leftSample = vec3 (uv.x - pixelOffset.x, 0, uv.y);
			sampleWaterAmount = texture (splatMap, leftSample.xz) [waterIndex - 1];
			leftSample.y = texture (heightMap, leftSample.xz).r * maxHeight;
			leftSample.x = (leftSample.x - 0.5f) * terrainSize;
			leftSample.z = (leftSample.z - 0.5f) * terrainSize;
			leftSample.y += sin (time + (length (leftSample) * waveCount)) * sampleWaterAmount * waveHeight;
			vec3 rightSample = vec3 (uv.x + pixelOffset.x, 0, uv.y);
			sampleWaterAmount = texture (splatMap, rightSample.xz) [waterIndex - 1];
			rightSample.y = texture (heightMap, rightSample.xz).r * maxHeight;
			rightSample.x = (rightSample.x - 0.5f) * terrainSize;
			rightSample.z = (rightSample.z - 0.5f) * terrainSize;
			rightSample.y += sin (time + (length (rightSample) * waveCount)) * sampleWaterAmount * waveHeight;
			vec3 topSample = vec3 (uv.x, 0, uv.y + pixelOffset.y);
			sampleWaterAmount = texture (splatMap, topSample.xz) [waterIndex - 1];
			topSample.y = texture (heightMap, topSample.xz).r * maxHeight;
			topSample.x = (topSample.x - 0.5f) * terrainSize;
			topSample.z = (topSample.z - 0.5f) * terrainSize;
			topSample.y += sin (time + (length (topSample) * waveCount)) * sampleWaterAmount * waveHeight;
			vec3 bottomSample = vec3 (uv.x, 0, uv.y - pixelOffset.y);
			sampleWaterAmount = texture (splatMap, bottomSample.xz) [waterIndex - 1];
			bottomSample.y = texture (heightMap, bottomSample.xz).r * maxHeight;
			bottomSample.x = (bottomSample.x - 0.5f) * terrainSize;
			bottomSample.z = (bottomSample.z - 0.5f) * terrainSize;
			bottomSample.y += sin (time + (length (bottomSample) * waveCount)) * sampleWaterAmount * waveHeight;
			fragWorldNormal = normalize (modelMatrix * vec4 (cross (rightSample - leftSample, bottomSample - topSample), 0));
		}
		else
		{
			fragWorldNormal = normalize (modelMatrix * vec4 (normal, 0));
		}
	}
	else
	{
		gl_Position = MVPmatrix * vec4 (vertex, 1);
		fragWorldPosition = modelMatrix * vec4 (vertex, 1);
		fragWorldNormal = normalize (modelMatrix * vec4 (normal, 0));
	}
	splatUV = uv;
}