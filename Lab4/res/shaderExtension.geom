//Version number
#version 330 core

//Layout qualfier
layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

//Bringing in variables passed out of vert
in VS_OUT {
    vec2 texCords;
    vec3 vertPos;
    vec3 vertNormal;
} gs_in[];

//passing out variables
out vec3 fragPos;
out vec2 fragTexCords;
out vec3 fragNormal;



vec3 GetNormal()
{
    //Getting the normal vector of each vertex
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

   //returns the cross product between the two vectors calculated
   return normalize(cross(a, b));
}

vec4 Move(vec4 position, vec3 moveDir)
{
    float moveAmount = 1.5;

    vec3 totalMove = moveDir * moveAmount; 

    return position + vec4(totalMove, 0.0);
}


void main()
{
    //Getting normal
    vec3 normal = GetNormal();

    vec3 upDir = vec3(0.0f,1f,0.0f);

    fragNormal = normal;

    //Setting current vertex position
    gl_Position = Move(gl_in[0].gl_Position, upDir);
    fragTexCords = gs_in[0].texCords;
    EmitVertex();

    gl_Position = Move(gl_in[1].gl_Position, upDir);
    fragTexCords = gs_in[1].texCords;
    EmitVertex();

    gl_Position = Move(gl_in[2].gl_Position, upDir);
    fragTexCords = gs_in[2].texCords;
    EmitVertex();

    EndPrimitive();

        //Setting current vertex position
    gl_Position = Move(gl_in[0].gl_Position, -upDir);
    fragTexCords = gs_in[0].texCords;
    EmitVertex();

    gl_Position = Move(gl_in[1].gl_Position, -upDir);
    fragTexCords = gs_in[1].texCords;
    EmitVertex();

    gl_Position = Move(gl_in[2].gl_Position, -upDir);
    fragTexCords = gs_in[2].texCords;
    EmitVertex();


    EndPrimitive();
}  