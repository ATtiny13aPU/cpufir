#include "include.h"

using namespace osl;

class key_trigger {
public:
	void push(bool t) {
		if (t) {
			is_click = !is_press;
			is_press = true;
		}
		else {
			is_release = is_press;
			is_click = false, is_press = false;
		}
	}
	bool is_press = 0, is_click = 0, is_release = 0;
private:
};



vec2 v_scroll = vec2(0.);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	v_scroll[0] = xoffset;
	v_scroll[1] = yoffset;
}

class Context {
public:
	int run();
	Context(GLFWwindow *w) : window(w), frameTime(60), frameTimeLowLatency(3) {};
private:
	int err = 0;
	Randomaizer RAND;
	int Xd = 800, Yd = 600;

	bool Vsync = 1, VsyncNow = Vsync;

	GLFWwindow *window;

	shad::VoidMesh voidMesh;
	shad::SimpleMesh fullScreenMesh;

	shad::Shader worldShader;

	uint32_t count_of_frames = 0;
	uint32_t count_of_updates = 0;
	std::ostringstream buff; // текст буффер
	ch_tp frame_time_point[2]; // две переменные времени
	fastLinearFilter frameTime;
	fastLinearFilter frameTimeLowLatency;

	vec2 mPos, dmPos1, dmPos2;
	float scroll = 0.;

	vec2 mPos_to_wPos(vec2 mPos, frac mst, vec2 wPos) {
		mPos = (vec2(mPos[0] - Xd / 2., Yd - mPos[1] - Yd / 2.)) / frac(Yd) * mst;
		mPos = mPos + wPos;
		return mPos;
	}

	vec2 mVec_to_wVec(vec2 mPos, frac mst) {
		mPos = (vec2(mPos[0], mPos[1])) / frac(Yd) * mst;
		return mPos;
	}
};

void glGetIntegervCout(int l) {
	int size = 0;
	glGetIntegerv(l, &size);
	std::cout << size << '\n';
}

void glGetIntegeri_vCout(int l) {
	int size = 0;
	glGetIntegeri_v(l, 0, &size);
	std::cout << size << ' ';
	glGetIntegeri_v(l, 1, &size);
	std::cout << size << ' ';
	glGetIntegeri_v(l, 2, &size);
	std::cout << size << '\n';
}

int Context::run() {
	if (err != 0)
		return err;
	frame_time_point[0] = chGetTime();
	frame_time_point[1] = frame_time_point[0];
	RAND.ini();

	ImGui::GetIO().Fonts->AddFontFromFileTTF("SFProText-Regular.ttf", 18.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	// Меню и свойства окон Imgui
	///==============================
	ImGui::GetIO().IniFilename = NULL;
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	glfwSetScrollCallback(window, scroll_callback);

	{
		std::vector<float> m = { -1., -1., -1., 1., 1., -1., 1., 1. };
		fullScreenMesh.loadFrom(&m[0], m.size());
	}


	// шейдеры графики

	{
		worldShader.name = std::string("worldShader");
		std::ifstream inpf;
		inpf.open("Shaders/Graphics/world.vert");
		std::string sourseV{ std::istreambuf_iterator<char>(inpf), std::istreambuf_iterator<char>() };
		inpf.close();
		inpf.open("Shaders/Graphics/world.frag", std::ios_base::in);
		std::string sourseF{ std::istreambuf_iterator<char>(inpf), std::istreambuf_iterator<char>() };
		inpf.close();
		worldShader.compile(sourseV.c_str(), sourseF.c_str());
	}

	// создание мира
	WorldCS world;

	world.iniWorld();

	std::thread thr(&WorldCS::renderWorld, &world);

	unsigned int map_texture = 1;
	glGenTextures(1, &map_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, map_texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(worldShader.glID, "MAP"), 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, map_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, world.size[0], world.size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, &world.color_map[0]);

	// Цикл графики
	///\/\/\/\/\/\/\/\///
	glfwSwapInterval(Vsync);
	while (!glfwWindowShouldClose(window)) {
		// Работа с glfw3
		{	///============================================================///
			if (VsyncNow != Vsync) {
				glfwSwapInterval(Vsync);
				VsyncNow = Vsync;
			}

			// Получаем эвенты
			glfwPollEvents();

			// Настраиваем камеру под разрешение окна
			glfwGetFramebufferSize(window, &Xd, &Yd);
			glViewport(0, 0, Xd, Yd);
		}	///============================================================///

	//	glClearColor(0.8f, 0.8f, 0.8f, 1.f);
	//	glClear(GL_COLOR_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Управление камерой мышью и клавой
		static key_trigger boost, test, test2, gmesh, msaa;
		vec2 mnPos, mxPos;
		{
			boost.push(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS);
			test.push(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS);
			test2.push(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS);
			// управление камерой
			{	///==============================
				// движение клавиатурой
				{	///==============================
					if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
						world.view.pos[0] -= (frameTimeLowLatency.get() / 1000.) * world.view.mst;
					if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
						world.view.pos[0] += (frameTimeLowLatency.get() / 1000.) * world.view.mst;
					if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
						world.view.pos[1] -= (frameTimeLowLatency.get() / 1000.) * world.view.mst;
					if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
						world.view.pos[1] += (frameTimeLowLatency.get() / 1000.) * world.view.mst;
				}	///==============================


				// масштабирование в указатель и движение
				{	///==============================
					// получение эвента колёсика мыши
					//float scroll = ImGui::GetIO().MouseWheel;
					scroll += v_scroll[1];
					v_scroll = vec2(0);

					if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
						scroll -= 10. * (frameTimeLowLatency.get() / 1000.);
					if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
						scroll += 10. * (frameTimeLowLatency.get() / 1000.);

					dmPos2 = dmPos1; // движение
					glfwGetCursorPos(window, &dmPos1[0], &dmPos1[1]); dmPos1[0] = -dmPos1[0];
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
						world.view.pos -= mVec_to_wVec(dmPos2 - dmPos1, world.view.mst);
					}
					mPos = mPos_to_wPos(vec2(-dmPos1[0], dmPos1[1]), world.view.mst, world.view.pos);

					/// компенсация движения
					// dmPos3 *= vec2(std::min(vec2(dmPos2.x - dmPos1.x).length() * 0.05, 2.), std::min(vec2(dmPos2.y - dmPos1.y).length() * 0.05, 2.)) * 0.3;
					const frac pk = 0.2;

					while (scroll > 0.01) { // приближение
						scroll -= 0.1 * pk;
						if (scroll < 0.) scroll = 0.;
						vec2 mnPos = mPos_to_wPos(vec2(0, 0), world.view.mst, world.view.pos);
						vec2 mxPos = mPos_to_wPos(vec2(Xd, Yd), world.view.mst, world.view.pos);
						mnPos = mix(mnPos, mPos, +0.012 * pk);
						mxPos = mix(mxPos, mPos, +0.012 * pk);
						world.view.pos = mix(mnPos, mxPos, 0.5);
						world.view.mst = mnPos[1] - mxPos[1];

						world.view.mst = std::max(world.view.mst, 10.);
					}
					while (scroll < -0.01) { // отдаление 
						scroll += 0.1 * pk;
						vec2 mnPos = mPos_to_wPos(vec2(0, 0), world.view.mst, world.view.pos);
						vec2 mxPos = mPos_to_wPos(vec2(Xd, Yd), world.view.mst, world.view.pos);
						mnPos = mix(mnPos, mPos, -0.012 * pk);
						mxPos = mix(mxPos, mPos, -0.012 * pk);
						world.view.pos = mix(mnPos, mxPos, 0.5);
						world.view.mst = mnPos[1] - mxPos[1];

						world.view.mst = std::min(world.view.mst, world.size[0] * 2.);
					}
				}	///==============================
			}	///==============================

			world.view.pos = clamp(world.view.pos, vec2(-world.size[0] * 0.9), vec2(world.size[0] * 1.9));

			mnPos = mPos_to_wPos(vec2(0, 0), world.view.mst, world.view.pos);
			mxPos = mPos_to_wPos(vec2(Xd, Yd), world.view.mst, world.view.pos);
		}
		auto mp = mix(vec2(mnPos[0], mxPos[1]), vec2(mxPos[0], mnPos[1]), vec2(-dmPos1[0], Yd - dmPos1[1]) / vec2(Xd, Yd));

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			world.event_pool.push(WorldCommand::paintBlock(ivec2(mp[0], mp[1]), ivec4(0, 0, 0, 255)));
		}

		static bool texture_update_mode = 1;
		// Графика
		{
			// чашка петри
			if (1) {
				glUseProgram(worldShader.glID);
				static bool first_call = 1;
				if (first_call) {
					first_call = 0;
					glUniform2f(glGetUniformLocation(worldShader.glID, "Dr"), world.size[0], world.size[1]);
				}
				glUniform4f(glGetUniformLocation(worldShader.glID, "ViewWorld"), mnPos[0], mxPos[1], mxPos[0], mnPos[1]);

				glUniform2i(glGetUniformLocation(worldShader.glID, "Mpos"), mp[0], mp[1]);

				glBindTexture(GL_TEXTURE_2D, map_texture);
				glActiveTexture(GL_TEXTURE0);
				if (texture_update_mode)
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, world.size[0], world.size[1], GL_RGBA, GL_UNSIGNED_BYTE, &world.color_map[0]);
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, world.Xp, world.Yp, 0, GL_RGBA, GL_UNSIGNED_BYTE, &world.color_map[0]);
				glUniform1i(glGetUniformLocation(worldShader.glID, "map_texture"), 0);

				glBindVertexArray(fullScreenMesh.VAO);
				for (int i = -1; i < 50 * boost.is_press; i++) {
					glDrawArrays(GL_TRIANGLE_STRIP, 0, fullScreenMesh.size);
				}
			}
		}

		// Работа с ImGui
		static bool imgui_overlay = 1;
		if (imgui_overlay)
		{	///============================================================///
			// Что-то нужное для ImGui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			ImGui::Begin("Manage", &imgui_overlay, window_flags);
			ImGui::SetWindowPos(ImVec2(0, 0));

			if (ImGui::Button("Шаг")) {
				if (world.auto_run)
					world.auto_run = 0;
				world.task = std::min(10, world.task + 1);
			}
			ImGui::SameLine();
			ImGui::Checkbox("Автозапуск", &world.auto_run);

			ImGui::Checkbox("Обновление текстуры", &texture_update_mode);
			ImGui::Checkbox("Верт. синх.", &Vsync);

			// Вывод FPS и времени кадра
			ImGui::Text("fps %.1f ft %.1f", 1000. / frameTime.get(), frameTime.get());

			ImGui::Text("UPS: %.0f", 1000. / world.updateTime.get());
			ImGui::Text("-----------------------------");
			ImGui::Text("позиция мыши: (%.0f, %.0f)", mp[0], mp[1]);
			

			ImGui::End();

			// Запускаем рендер меню
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		// Подсчёт кадров
		{
			count_of_frames++;
			bool swapBit = count_of_frames % 2;
			frame_time_point[swapBit] = chGetTime();
			frameTime.push(chDurationMillis(frame_time_point[swapBit], frame_time_point[!swapBit]));
			frameTimeLowLatency.push(chDurationMillis(frame_time_point[swapBit], frame_time_point[!swapBit]));
			if (count_of_frames % 100000 == 0)
				frameTime.resum(), frameTimeLowLatency.resum();
		}

		glfwSwapBuffers(window);
	}

	world.shutdown();
	thr.join();
	
	return 1;
}




int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 600, "Ёлки", NULL, NULL);
	if (window == NULL) return -1;

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 430 core");
	}

	Context c(window);
	int r = c.run();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	//int t;
	//std::cin >> t;
	return r;
}