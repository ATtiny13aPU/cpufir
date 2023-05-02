#pragma once

class WorldCS;

// ��� �����
class BlockType {
public:
	BlockType() {}

	frac32 grow_price; // ��������� ��������� � �������� �����
	frac32 life_support_price; // ��������� ��������� ���������� � �������� ����� / ���
	frac32 light_absorption; // ����������� ���������� ����� ������ ��� �����������
	frac32 weight; // ��� �����
	frac32 durability; // ��������� �����


	uint8_t type; // ����� �����
	// ���� ������
	enum TPS { 
		STATIC_BLOCK, // �����, ����� � �����
		DYNAMIC_BLOCK, // �������� ������, ����
		GROW_BLOCK, // ��������� (�����), �����
		EXTRA_BLOCK // ������, �����
	};

	frac32 effectivity; // ������ ���������� ��� ������� �����
};

// ������� ������
class WorldCS::Mode {
public:
	
};

// ��� ������
class WorldCS::Genome {
public:
	std::vector<Mode> mods;
};

// ��������� ��������� ������
class WorldCS::Bank {
public:
	std::vector<Particle> particles;
};

// �������
class WorldCS::Particle {
public:
	int current_mode_id;
	int map_index;
};

// ��������� �������
class WorldCS::FreeParticle {
public:
	Genome* current_mode;
};

// ������
class WorldCS::Trees {
public:
	std::vector<Bank> storges;
	Genome* genome;
};