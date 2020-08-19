#version 400

in vec3 fragPos;
in vec2 fragTexCords;
in vec3 fragNormal;

uniform sampler2D texture;

uniform vec3 toonLightPosition;
uniform vec3 bpLightPosition;

uniform vec3 viewPosition;


void main()
{
	
	//Calculating the light directions and normalizing inputted Normal and frag position
	vec3 normal = fragNormal;
	vec3 position = fragPos;

	vec3 pbLightDirection = normalize(bpLightPosition - position);
	vec3 toonLightDirection = normalize(toonLightPosition - position);


	//Calculating Blinn-Phong lighting

	vec3 textureColour = texture(texture, fragTexCords).rgb;
    vec3 ambient = 0.05 * textureColour;

	vec3 viewDirection = normalize(viewPosition - position);

	vec3 reflectDir = reflect(-pbLightDirection, normal);

	float diff = max(dot(pbLightDirection, normal), 0.0);

    vec3 diffuse = diff * textureColour;

	vec3 halfwayDir = normalize(pbLightDirection + viewDirection);  

	float spec = 0.0;
    spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
	
	vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    
	vec4 pbLightingColour = vec4(ambient + diffuse + specular, 1.0);



	//Calculating the ToonShader

	float intensity;
	vec4 toonShaderColour;
	intensity = dot(toonLightDirection,normal);

	if (intensity > 0.95)
		toonShaderColour = vec4(0.7,0.7,0.7,1.0);
	else if (intensity > 0.5)
		toonShaderColour = vec4(0.5,0.5,0.5,1.0);
	else if (intensity > 0.25)
		toonShaderColour = vec4(0.2,0.2,0.2,1.0);
	else
		toonShaderColour = vec4(0.1,0.1,0.1,1.0);


	//Outputting final colour
	gl_FragColor =  toonShaderColour * pbLightingColour;

}