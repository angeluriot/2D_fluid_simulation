#include "menu/Menu.hpp"
#include "Simulator.hpp"

bool	Menu::visible	= true;
bool	Menu::active	= false;

void Menu::check_events(const sf::Event& sf_event)
{
	if (sf_event.type == sf::Event::KeyReleased && sf_event.key.code == sf::Keyboard::Key::F1)
		visible = !visible;
}

void Menu::title(const std::string& text)
{
	float window_width = ImGui::GetWindowSize().x;
	float text_width = ImGui::CalcTextSize(text.data()).x;

	ImGui::NewLine();
	ImGui::SetCursorPosX((window_width - text_width) * 0.5f);
	ImGui::Text(text.data());
	ImGui::Separator();
}

std::vector<bool> Menu::centered_buttons(const std::vector<std::string> texts, float buttons_height, float spaces_size)
{
	float window_width = ImGui::GetWindowSize().x;
	float buttons_widths = (window_width - ((texts.size() + 2) * spaces_size)) / texts.size();
	std::vector<bool> result(texts.size());

	ImGui::NewLine();
	ImGui::NewLine();

	for (int i = 0; i < texts.size(); i++)
	{
		ImGui::SameLine();
		ImGui::SetCursorPosX(spaces_size + buttons_widths * i + spaces_size * i);
		ImGui::Text("");
		ImGui::SameLine();
		result[i] = ImGui::Button(texts[i].data(), ImVec2(buttons_widths, buttons_height));
	}

	return result;
}

void Menu::display()
{
	if (visible)
	{
		ImGui::Begin("Simulation settings (F1 to hide)");
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.9f);

		ImGui::NewLine();
		ImGui::Text("The layer to show:");
		ImGui::Combo("##layer", reinterpret_cast<int*>(&Renderer::layer), "Density\0Velocity\0Pressure\0Divergence");

		ImGui::NewLine();
		ImGui::Text("The quality of the simulation:");
		if (ImGui::SliderFloat("##quality", &Simulator::quality, 0.05f, 1.f))
			Simulator::reset();

		ImGui::NewLine();
		ImGui::Text("The time step of the simulation:");
		ImGui::SliderFloat("##time_step", &Simulator::time_step, 0.01f, 1.f, NULL, ImGuiSliderFlags_Logarithmic);

		ImGui::NewLine();
		ImGui::Text("The dissipation of the dye:");
		ImGui::SliderFloat("##dissipation", &Simulator::dissipation, 0.001f, 0.1f, NULL, ImGuiSliderFlags_Logarithmic);

		ImGui::NewLine();
		ImGui::Text("The fluid viscosity:");
		ImGui::SliderFloat("##viscosity", &Simulator::viscosity, 0.f, 1.f, NULL, ImGuiSliderFlags_Logarithmic);

		ImGui::NewLine();
		ImGui::Text("The fluid vorticity:");
		ImGui::SliderFloat("##vorticity", &Simulator::vorticity, 0.f, 1.f);

		ImGui::NewLine();
		ImGui::Text("The precision of the computations:");
		ImGui::SliderInt("##viscous_precision", &Simulator::precision, 10, 100);

		ImGui::NewLine();
		ImGui::Text("The strength of the mouse:");
		ImGui::SliderFloat("##mouse_strength", &Simulator::mouse_strength, 1.f, 100.f);

		active = ImGui::IsWindowFocused();

		ImGui::End();
	}
}
