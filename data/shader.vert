#version 150

// geometry shader based on code by Paul Houx:
// https://forum.libcinder.org/topic/smooth-thick-lines-using-geometry-shader
// http://www.cowlumbus.nl/forum/GeometryShader.zip


uniform float thickness;

layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = 7) out;

vec3 screen_space(vec4 vertex)
{
    return vec3( vertex.xy / vertex.w, 0 ) * vec3(1024, 768, 1.0);
}

void main() {
  // this could also be a uniform
  vec3 WIN_SCALE = vec3(1024, 768, 1.0);
  float MITER_LIMIT = 1.0f;

  vec3 p0 = screen_space(gl_in[0].gl_Position);
  vec3 p1 = screen_space(gl_in[1].gl_Position);
  vec3 p2 = screen_space(gl_in[2].gl_Position);
  vec3 p3 = screen_space(gl_in[3].gl_Position);

  // perform naive culling
  vec3 area = WIN_SCALE * 1.2;
  if( p1.x < -area.x || p1.x > area.x ) return;
  if( p1.y < -area.y || p1.y > area.y ) return;
  if( p2.x < -area.x || p2.x > area.x ) return;
  if( p2.y < -area.y || p2.y > area.y ) return;

  // determine the direction of each of the 3 segments (previous, current, next)
  vec3 v0 = normalize(p1-p0);
  vec3 v1 = normalize(p2-p1);
  vec3 v2 = normalize(p3-p2);

  // determine the normal of each of the 3 segments (previous, current, next)
  vec3 n0 = vec3(-v0.y, v0.x, 0);
  vec3 n1 = vec3(-v1.y, v1.x, 0);
  vec3 n2 = vec3(-v2.y, v2.x, 0);

    // determine miter lines by averaging the normals of the 2 segments
  vec3 miter_a = normalize(n0 + n1);    // miter at start of current segment
  vec3 miter_b = normalize(n1 + n2);    // miter at end of current segment

  // determine the length of the miter by projecting it onto normal and then inverse it
  float length_a = thickness / dot(miter_a, n1);
  float length_b = thickness / dot(miter_b, n1);

  vec3 up = vec3(0, 0, 1);  // arbitrary up vector

  vec3 dir = normalize(p1 - p0);      // normalized direction vector from p0 to p1
  vec3 right = normalize(cross(dir, up)); // right vector
  vec3 norm = cross(right, dir);

  right *= thickness;

  // prevent excessively long miters at sharp corners
  if( dot(v0,v1) < -MITER_LIMIT ) {
    miter_a = n1;
    length_a = thickness;

    // close the gap
    if( dot(v0,n1) > 0 ) {
        gl_Position = vec4( (p1 + thickness * n0) / WIN_SCALE, 1.0 );
        EmitVertex();
        gl_Position = vec4( (p1 + thickness * n1) / WIN_SCALE, 1.0 );
        EmitVertex();
        gl_Position = vec4( p1 / WIN_SCALE, 1.0 );
        EmitVertex();
        EndPrimitive();
    }
    else {
        gl_Position = vec4( (p1 - thickness * n1) / WIN_SCALE, 1.0 );
        EmitVertex();
        gl_Position = vec4( (p1 - thickness * n0) / WIN_SCALE, 1.0 );
        EmitVertex();
        gl_Position = vec4( p1 / WIN_SCALE, 1.0 );
        EmitVertex();
        EndPrimitive();
    }
  }

  if( dot(v1,v2) < -MITER_LIMIT ) {
    miter_b = n1;
    length_b = thickness;
  }

  // generate the triangle strip
  gl_Position = vec4( (p1 + length_a * miter_a) / WIN_SCALE, 1.0 );
  EmitVertex();
  gl_Position = vec4( (p1 - length_a * miter_a) / WIN_SCALE, 1.0 );
  EmitVertex();
  gl_Position = vec4( (p2 + length_b * miter_b) / WIN_SCALE, 1.0 );
  EmitVertex();
  gl_Position = vec4( (p2 - length_b * miter_b) / WIN_SCALE, 1.0 );
  EmitVertex();

  EndPrimitive();
}