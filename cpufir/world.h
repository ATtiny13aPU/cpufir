#pragma once

using namespace osl;


class WorldCommand {
public:
	static WorldCommand paintBlock(ivec2 ipos, ivec4 color) {
		WorldCommand r;
		r.type = PAINT_BLOCK;
		r.data = color;
		r.pos = ipos;
		return r;
	}

	enum {
		PAINT_BLOCK
	};

	int type;
	ivec4 data;
	ivec2 pos;
};

class WorldCS {
public:
	WorldCS() : updateTime(60) {}

	struct View {
		vec2 pos = vec2(0, 0); // Позиция центра камеры (0 - центр субстрата)
		frac mst = 1.; // Масштаб зрения (чем больше, тем меньше выглядит мир)
	};
	View view;

	int task = 0;
	bool auto_run = 1;

	bool is_open;
	ivec2 size = ivec2(200, 200);

	std::string name = "Unnamed";
	void iniWorld();
	void renderWorld();
	void shutdown() {
		is_open = 0;
	};

	std::vector<uint8_t> color_map;
	std::vector<uint8_t> world_map;


	std::queue<WorldCommand> event_pool;

	inline void paintBlock(int index, ivec4 color) {
		paintBlock(index, ivec3(color[0], color[1], color[2]));
		paintBlock(index, color[3]);
	}

	inline void paintBlock(int index, ivec3 color) {
		color_map[index] = color[0];
		color_map[index + 1] = color[1];
		color_map[index + 2] = color[2];
	}

	inline void paintBlock(int index, int brightness) {
		color_map[index + 3] = brightness;
	}

	inline int getIndex(ivec2 p) {
		return (p[0] + p[1] * size[0]) * 4;
	}
	std::vector<int> Direction;
	class Mode;
	class Genome;
	class Bank;
	class Particle;
	class FreeParticle;
	class Trees;
	

	int count_of_updates = 0;
	fastLinearFilter updateTime;
	ch_tp update_time_point[2]; // две переменные времени
private:
};

#include "life.h"

void WorldCS::iniWorld() {
	view.pos = size / 2.;
	view.mst = size[1] + 2;

	color_map.resize(size[0] * size[1] * 4, 255);

	Direction = {-size[0], -1, +1, size[0] };
}

void WorldCS::renderWorld() {
	is_open = 1;
	while (is_open) {
		
		if (task > 0 || auto_run == 1) {
			if (task > 0)
				task--;

			// обработка эвентов
			while (!event_pool.empty()) {
				auto e = event_pool.front();
				event_pool.pop();
				switch (e.type) {
				case WorldCommand::PAINT_BLOCK:
					if (getIndex(e.pos) >= 0 && getIndex(e.pos) < color_map.size())
						paintBlock(getIndex(e.pos), e.data);
					break;
				}
			}



			if (count_of_updates % 100000 == 0)
				updateTime.resum();
			count_of_updates++;
			bool swapBit = count_of_updates % 2;
			update_time_point[swapBit] = chGetTime();
			updateTime.push(chDurationMillis(update_time_point[swapBit], update_time_point[!swapBit]));
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}



