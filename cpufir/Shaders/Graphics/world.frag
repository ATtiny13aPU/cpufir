#version 430 core

in vec2 WinK;
out vec3 pixel;

uniform vec4 ViewWorld;

uniform ivec2 WinSize;
uniform ivec2 Mpos;
uniform vec2 Dr;

uniform sampler2D map_texture;

/*
struct Chunk {
	int first_list_ID;
	int linked_list;
	float brightness;
};

readonly buffer ssbo_grid {
    Chunk chunks[];
};*/

//length

bool compareDistanse(vec2 v, float l) {
	return v.x * v.x + v.y * v.y < l * l;
}

float cubicInterpolate(float x, vec4 p) {
	return p[1] + (-0.5 * p[0] + 0.5 * p[2]) * x
		+ (p[0] - 2.5 * p[1] + 2.0 * p[2] - 0.5 * p[3]) * x * x
		+ (-0.5 * p[0] + 1.5 * p[1] - 1.5 * p[2] + 0.5 * p[3]) * x * x * x;
}

float bicubicInterpolate(vec2 uv, mat4x4 p) {
	return cubicInterpolate(uv.x, vec4(
		cubicInterpolate(uv.y, p[0]),
		cubicInterpolate(uv.y, p[1]),
		cubicInterpolate(uv.y, p[2]),
		cubicInterpolate(uv.y, p[3]))
	);
}

// return 1 if v inside the box, return 0 otherwise
float insideBox(vec2 v, vec2 bottomLeft, vec2 topRight) {
    vec2 s = step(bottomLeft, v) - step(topRight, v);
    return s.x * s.y;   
}

// MAIN
///============================================================///
void main() {
	// координаты пикселя
	vec2 Pos = mix(ViewWorld.xy, ViewWorld.zw, WinK);

	pixel = vec3(40, 40, 40);
	
	
	// проверка выхода за пределы карты
	if (insideBox(Pos, vec2(0.), Dr) > 0.5) {
		ivec2 ipos = ivec2(Pos);
		vec2 fPos = Pos / Dr;
		vec2 frpos = fract(Pos);
		vec4 map_data = texelFetch(map_texture, ipos, 0);

		if(map_data[0] * map_data[1] * map_data[2] > 0.999)
			pixel = mix(vec3(175, 225, 255), vec3(40, 190, 245), fPos.y) * map_data[3]; // цвет неба
		else
			pixel = map_data.rgb * map_data.a * map_data.a * 255.;

		if (ipos == Mpos)
			pixel *= 0.9;

	} else if (insideBox(Pos, vec2(0.), vec2(Dr.x, Dr.y + 120.)) > 0.5) {
		float k = (Pos.y - Dr.y) / 100.;
		pixel = mix(vec3(40, 190, 245), vec3(20, 90, 120), (k + 0.5) * k * 0.7);
	} else
		pixel = vec3(25 ,76 ,76);

	pixel /= 255;
}