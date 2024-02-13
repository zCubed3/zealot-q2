/*
Copyright (C) 2023-2024 Liam Reese (zCubed3)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

//
// unlit_model.frag.glsl - Unlit model drawing fragment shader
//

#version 400

out vec4 out_FragColor;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

void main() {
    const vec3 LIGHT_DIR = vec3(0, 0, 1);
    out_FragColor.xyz = vec3(dot(LIGHT_DIR, normalize(v_Normal)));
}