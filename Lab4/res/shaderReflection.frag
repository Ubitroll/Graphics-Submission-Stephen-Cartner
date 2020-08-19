#version 330 core

//passing out Frag colour
out vec4 FragColor;

//bringing in variables
in vec3 Normal;
in vec3 Position;

//Bringing in uniform variables
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    vec3 Incident = normalize(Position - cameraPos);
    vec3 Reflection = reflect(Incident, normalize(Normal));
    FragColor = vec4(texture(skybox, Reflection).rgb, 1.0);
}