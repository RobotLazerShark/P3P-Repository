#version 330

uniform float uvScaleR;
uniform float uvScaleG;
uniform float uvScaleB;
uniform float uvScaleA;
uniform int splatTexCount;
uniform sampler2D splatMap;
uniform sampler2D splatTexR;
uniform sampler2D splatTexG;
uniform sampler2D splatTexB;
uniform sampler2D splatTexA;
uniform sampler2D heightMap;
uniform float maxHeight;
uniform bool lit;
uniform float shininess;
uniform vec3 specularColor;
uniform vec4 cameraPosition;
uniform vec3 ambientColor;
uniform int directionalCount;
uniform vec3 directionalColors [10];//Added maximum enables dynamic indexing
uniform vec4 directionalNormals [10];
uniform int pointCount;
uniform vec3 pointColors [10];
uniform vec4 pointPositions [10];
uniform vec2 pointFalloffs [10];
uniform int spotCount;
uniform vec3 spotColors [10];
uniform vec4 spotPositions [10];
uniform vec4 spotNormals [10];
uniform float spotAngledots [10];
uniform vec2 spotFalloffs [10];
in vec4 fragWorldPosition;
in vec4 fragWorldNormal;
in vec2 splatUV;
in float uvWaterScale;
in float waterAmount;
vec3 baseColor;
vec4 frag2CameraNormal;
out vec4 fragment_color;


//////////////////////////////|	LIGHT CALCULATIONS
vec3 getSpecularContribution (vec3 pColor, vec4 pLight2FragNormal)
{
	if (shininess == 0&&waterAmount<=0)
	{
		return vec3 (0, 0, 0);
	}
	vec4 reflectedLightNormal = pLight2FragNormal - (2 * dot (pLight2FragNormal, fragWorldNormal) * fragWorldNormal);
	if (shininess == 0)
	{
		return (pow (max (0, dot (reflectedLightNormal, frag2CameraNormal)), waterAmount) * pColor * specularColor) * waterAmount * 0.1f;
	}
	return pow (max (0, dot (reflectedLightNormal, frag2CameraNormal)), shininess + (shininess * 1.5f * waterAmount)) * pColor * specularColor;
}
//Calculates the diffuse contribution
vec3 getDiffuseContribution (vec4 pFrag2LightNormal, vec3 pColor)
{
	return max (0, dot (pFrag2LightNormal, fragWorldNormal)) * pColor * baseColor;
}
//Calculates the contribution of this directional light to the total fragment color
vec3 getDirectionalContribution (vec3 pColor, vec4 pNormal)
{
	return getDiffuseContribution (-pNormal, pColor) + getSpecularContribution (pColor, pNormal);
}
//Calculates the contribution of this point light to the total fragment color
vec3 getPointContribution (vec3 pColor, vec4 pPosition, vec2 pFalloff)
{
	vec4 frag2LightNormal = (pPosition - fragWorldPosition);//Both have a W of 1, so it will become 0
	float dist2Light = length (frag2LightNormal);
	frag2LightNormal = normalize (frag2LightNormal);
	float falloffFactor = 1 / (1 + pFalloff.x * dist2Light + pFalloff.y * dist2Light * dist2Light);

	return (getDiffuseContribution (frag2LightNormal, pColor) + getSpecularContribution (pColor, -frag2LightNormal)) * falloffFactor;
}
//Calculates the contribution of this spot light to the total fragment color
vec3 getSpotContribution (vec3 pColor, vec4 pPosition, vec4 pNormal, float pAngledot, vec2 pFalloff)
{
	vec4 light2FragNormal = fragWorldPosition - pPosition;//Both have a W of 1, so it will become 0
	float dist2Light = length (light2FragNormal);
	light2FragNormal = normalize (light2FragNormal);
	float falloffFactor = 1 / (1 + pFalloff.x * dist2Light + pFalloff.y * dist2Light * dist2Light);

	float frag2SpotDot = max (0, dot (pNormal, light2FragNormal));//Dot of the angle between the light-to-frag normal and the light-normal
	float spotAngleFactor = (frag2SpotDot < pAngledot) ? 0 : (frag2SpotDot - pAngledot) * (1 / pAngledot);

	return (getDiffuseContribution (-light2FragNormal, pColor) + getSpecularContribution (pColor, light2FragNormal)) * spotAngleFactor * falloffFactor;
}


void main (void)
{
	vec2 texUV = splatUV;
	if (waterAmount > 0)
	{
		vec2 center = vec2 (0.5f, 0.5f);
		texUV = ((splatUV - center) * (1 + uvWaterScale * 0.01f) + center);
	}
	vec4 contributions;
	switch (splatTexCount)
	{
		case 2:
			contributions = texture2D (splatMap, splatUV);
			baseColor = contributions.r * texture2D (splatTexR, texUV * uvScaleR).rgb + contributions.g * texture2D (splatTexG, texUV * uvScaleG).rgb;
			break;
		case 3:
			contributions = texture2D (splatMap, splatUV);
			baseColor = contributions.r * texture2D (splatTexR, texUV * uvScaleR).rgb + contributions.g * texture2D (splatTexG, texUV * uvScaleG).rgb + contributions.b * texture2D (splatTexB, texUV * uvScaleB).rgb;
			break;
		case 4:
			contributions = texture2D (splatMap, splatUV);
			baseColor = contributions.r * texture2D (splatTexR, texUV * uvScaleR).rgb + contributions.g * texture2D (splatTexG, texUV * uvScaleG).rgb + contributions.b * texture2D (splatTexB, texUV * uvScaleB).rgb + contributions.a * texture2D (splatTexA, texUV * uvScaleA).rgb;
			break;
		default:
			baseColor = vec3 (1, 0, 1);
			break;
	}

	if (!lit)//Avoid light calculations if not wanted
	{
		fragment_color = baseColor;
		return;
	}

	frag2CameraNormal = normalize (cameraPosition - fragWorldPosition);//Both have a W of 1, so it will become 0

	//Light calculations. Normal calculations occur here for the point- and spotlights, so the fragment's normal can be used.
	//Calculate the total contribution of all directional lights
	vec3 directionalFactor = vec3 (0, 0, 0);
	for (int i = 0; i < directionalCount; i ++)
	{
		directionalFactor += getDirectionalContribution (directionalColors [i], directionalNormals [i]);
	}
	//Calculate the total contribution of all point lights
	vec3 pointFactor = vec3 (0, 0, 0);
	for (int i = 0; i < pointCount; i ++)
	{
		pointFactor += getPointContribution (pointColors [i], pointPositions [i], pointFalloffs [i]);
	}
	//Calculate the total contribution of all spot lights
	vec3 spotFactor = vec3 (0, 0, 0);
	for (int i = 0; i < spotCount; i ++)
	{
		spotFactor += getSpotContribution (spotColors [i], spotPositions [i], spotNormals [i], spotAngledots [i], spotFalloffs [i]);
	}

	//Calculate the final color
	fragment_color = vec4 (ambientColor + directionalFactor + pointFactor + spotFactor, 1.0f);
}