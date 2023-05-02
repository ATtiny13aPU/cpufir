#pragma once

class WorldCS;

// тип блока
class BlockType {
public:
	BlockType() {}
	std::string name;

	frac32 grow_price; // стоимость постройки в единицах света
	frac32 life_support_price; // пассивная стоимость содержания в единицах света / шаг
	frac32 light_absorption; // коэффициент поглощения света блоком при прохождении
	frac32 weight; // вес блока
	frac32 durability; // прочность блока


	uint8_t type; // класс блока
	// типы блоков
	enum TPS { 
		STATIC_BLOCK, // земля, трава и песок
		DYNAMIC_BLOCK, // падающие семяна, вода
		GROW_BLOCK, // древесина (ветки), плоды
		EXTRA_BLOCK // листья, корни
	};

	frac32 effectivity; // особая переменная для каждого блока

	frac32 probability_of_mutation_selection; // весовая доля в пуле мутации
};

// Единица генома
class WorldCS::Mode {
public:
	BlockType *own_transform_type;
	std::vector<BlockType *> other_transform_type;
	int own_genome_index;
	std::vector<int> other_genome_index;
};

// ДНК дерева
class WorldCS::Genome {
public:
	std::vector<Mode> mods;
};

// Хранитель элементов дерева
class WorldCS::Bank {
public:
	std::vector<Particle> particles;
};

// Частица
class WorldCS::Particle {
public:
	int current_mode_id;
	int map_index;
};

// Свободная частица
class WorldCS::FreeParticle {
public:
	Genome* current_mode;
};

// Дерево
class WorldCS::Trees {
public:
	std::vector<Bank> storges;
	Genome* genome;
};