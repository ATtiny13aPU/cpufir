#ifndef CUSTOMIMGUI
#define CUSTOMIMGUI



float btff(uint8_t b) { return b / 255.f; }

double btf(uint8_t b) { return b / 255.; }


void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void WriteColorText(const char* text, const char* color, const char* v_t_c) {
	const ImVec4 castom_palette[16] = {
		{1.f,	1.f,	1.f,	1.f}, // 0
		{0.67f,	0.67f,	0.67f,	1.f}, // 1
		{0.33f,	0.33f, 	0.33f,	1.f}, // 2
		{0.f, 	0.f,  	0.f,  	1.f}, // 3
		{1.f,	1.f,	0.33f,	1.f}, // 4
		{0.f, 	0.67f,	0.f,   	1.f}, // 5
		{0.33f,	1.f,	0.33f,	1.f}, // 6
		{1.f,	0.33f, 	0.33f,	1.f}, // 7
		{0.67f,	0.0f, 	0.f,   	1.f}, // 8
		{0.67f,	0.33f, 	0.f,   	1.f}, // 9
		{0.67f,	0.f,  	0.67f,	1.f}, // 10
		{1.f,	0.33f, 	1.f,	1.f}, // 11
		{0.33f,	1.f,	1.f,	1.f}, // 12
		{0.f, 	0.67f,	0.67f,	1.f}, // 13
		{0.f, 	0.f,  	0.67f,	1.f}, // 14
		{0.33f,	0.33f, 	1.f,	1.f}  // 15
	};

	if (text[0] != 0) {
		ImGui::NewLine();
		ImVec2 pos;
		ImVec2 delta = ImGui::CalcTextSize("a");

		char cc[2] = { 0, 0 };
		for (int c = 0; text[c] != 0; c++) {
			ImGui::PushStyleColor(ImGuiCol_Text, castom_palette[v_t_c[color[c] - 48]]);
			ImGui::SameLine();
			pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(pos.x - delta.x - 1.f, pos.y));
			cc[0] = text[c];
			ImGui::Text(cc);
			ImGui::PopStyleColor();
		}
	}
}

void WriteColorText(const char* text, const char* color) {
	const ImVec4 castom_palette[16] = {
		{1.f,	1.f,	1.f,	1.f}, // 0
		{0.67f,	0.67f,	0.67f,	1.f}, // 1
		{0.33f,	0.33f, 	0.33f,	1.f}, // 2
		{0.f, 	0.f,  	0.f,  	1.f}, // 3
		{1.f,	1.f,	0.33f,	1.f}, // 4
		{0.f, 	0.67f,	0.f,   	1.f}, // 5
		{0.33f,	1.f,	0.33f,	1.f}, // 6
		{1.f,	0.33f, 	0.33f,	1.f}, // 7
		{0.67f,	0.0f, 	0.f,   	1.f}, // 8
		{0.67f,	0.33f, 	0.f,   	1.f}, // 9
		{0.67f,	0.f,  	0.67f,	1.f}, // 10
		{1.f,	0.33f, 	1.f,	1.f}, // 11
		{0.33f,	1.f,	1.f,	1.f}, // 12
		{0.f, 	0.67f,	0.67f,	1.f}, // 13
		{0.f, 	0.f,  	0.67f,	1.f}, // 14
		{0.33f,	0.33f, 	1.f,	1.f}  // 15
	};

	if (text[0] != 0) {
		ImGui::NewLine();
		ImVec2 pos;
		ImVec2 delta = ImGui::CalcTextSize("a");

		char cc[2] = { 0, 0 };
		for (int c = 0; text[c] != 0; c++) {
			ImGui::PushStyleColor(ImGuiCol_Text, castom_palette[color[c] - 48]);
			ImGui::SameLine();
			pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(pos.x - delta.x - 1.f, pos.y));
			cc[0] = text[c];
			ImGui::Text(cc);
			ImGui::PopStyleColor();
		}
	}
}

#endif