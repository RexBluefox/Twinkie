#pragma once

#include "../IModule.h"

class DashboardInputsModule : public IModule
{
public:
	bool IsDebug = false;

	ImVec4 ColorSteer = ImVec4(0.976f, 0.737f, 0.008f, 1.f);
	ImVec4 ColorSteerI = ImVec4(1.f, 1.f, 1.f, 0.5f);
	ImVec4 ColorAccel = ImVec4(0.f, 0.94f, 0.f, 1.f);
	ImVec4 ColorAccelI = ImVec4(1.f, 1.f, 1.f, 0.5f);
	ImVec4 ColorBrake = ImVec4(0.94f, 0.f, 0.f, 1.f);
	ImVec4 ColorBrakeI = ImVec4(1.f, 1.f, 1.f, 0.5f);
	ImVec4 ColorBackground = ImVec4(0.f, 0.f, 0.f, 0.f);
	bool PointySteering = false;
	float DisplayMode = 0; // 0 = Pad; 1 = Keyboard;

	DashboardInputsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "Dashboard";
		this->FancyName = "Input display";
	}

	virtual void Render() {}
	virtual void RenderAnyways()
	{
		using namespace ImGui;
		if (Twinkie->IsPlaying())
		{
			static bool IsPrevHovered = false;
			VehicleInputs InputInfo = Twinkie->GetInputInfo();

			PushStyleColor(ImGuiCol_WindowBg, ColorBackground);
			PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

			int DashboardWindowFlags = ImGuiWindowFlags_NoTitleBar;
			if (!*UiRenderEnabled) DashboardWindowFlags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;

			Begin("Dashboard##Inputs", nullptr, DashboardWindowFlags);

			PopStyleColor();
			PopStyleVar();

			auto UIDrawList = GetWindowDrawList();
			auto CursorPos = GetCursorScreenPos();

			float WindowWidth = GetWindowWidth() / 3.f;
			float WindowHeight = GetWindowHeight();

			float Width = WindowWidth * -InputInfo.Steer;
			float Height = WindowHeight * -InputInfo.Steer;

			float OffsettedWidth = abs(WindowWidth - Width);

			float OffsettedHeight = abs(WindowHeight - Height);

			auto TipSteer = ImVec2();

			if (InputInfo.Steer < 0)
				TipSteer = ImVec2(CursorPos.x + OffsettedWidth, CursorPos.y + WindowHeight / 2.f);
			else if (InputInfo.Steer > 0)
				TipSteer = ImVec2(CursorPos.x + WindowWidth * 2 - Width, CursorPos.y + WindowHeight / 2.f);


			auto UpperTipSteer = ImVec2();
			auto LowerTipSteer = ImVec2();

			if (InputInfo.Steer < 0) {
				UpperTipSteer = ImVec2(CursorPos.x + OffsettedWidth, CursorPos.y + Height / 2.f);
				LowerTipSteer = ImVec2(CursorPos.x + OffsettedWidth, CursorPos.y + WindowHeight - Height / 2.f);
			}
			else if (InputInfo.Steer > 0) {
				UpperTipSteer = ImVec2(CursorPos.x + WindowWidth * 2 - Width, CursorPos.y - Height / 2.f);
				LowerTipSteer = ImVec2(CursorPos.x + WindowWidth * 2 - Width, CursorPos.y + WindowHeight + Height / 2.f);
			}



			auto TipBgL = ImVec2(CursorPos.x, CursorPos.y + WindowHeight / 2.f);
			auto TipBgR = ImVec2(CursorPos.x + GetWindowWidth(), CursorPos.y + WindowHeight / 2.f);

			auto UpperL = ImVec2(CursorPos.x + WindowWidth, CursorPos.y);
			auto LowerL = ImVec2(CursorPos.x + WindowWidth, CursorPos.y + WindowHeight);

			auto UpperR = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y);
			auto LowerR = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y + WindowHeight);

			auto BottomCornerGas = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y + WindowHeight / 2.f);
			auto TopCornerBrake = ImVec2(CursorPos.x + WindowWidth, CursorPos.y + WindowHeight / 2.f);

			switch ((int)DisplayMode) {
			case 0: // Pad
				if (InputInfo.Steer != 1)
				{
					UIDrawList->AddTriangleFilled(UpperR, LowerR, TipBgR, ColorConvertFloat4ToU32(ColorSteerI));
				}
				if (InputInfo.Steer != -1)
				{
					if (InputInfo.Steer < 0)
					{
						UIDrawList->AddQuadFilled(UpperL, TipSteer, LowerL, TipBgL, ColorConvertFloat4ToU32(ColorSteerI)); // left one (AA)
					}
					else
					{
						UIDrawList->AddTriangleFilled(TipBgL, UpperL, LowerL, ColorConvertFloat4ToU32(ColorSteerI));
					}
				}
				if (PointySteering == true) {
					if (InputInfo.Steer < 0)
						UIDrawList->AddTriangleFilled(TipSteer, UpperL, LowerL, ColorConvertFloat4ToU32(ColorSteer));
					else if (InputInfo.Steer > 0)
						UIDrawList->AddTriangleFilled(TipSteer, UpperR, LowerR, ColorConvertFloat4ToU32(ColorSteer));
				}
				else {
					if (InputInfo.Steer < 0)
						UIDrawList->AddQuadFilled(UpperTipSteer, UpperL, LowerL, LowerTipSteer, ColorConvertFloat4ToU32(ColorSteer));
					else if (InputInfo.Steer > 0)
						UIDrawList->AddQuadFilled(UpperTipSteer, UpperR, LowerR, LowerTipSteer, ColorConvertFloat4ToU32(ColorSteer));
				}
				break;
			case 1: // Keyboard
				UIDrawList->AddQuadFilled(ImVec2(UpperL.x, TopCornerBrake.y + 3.f), LowerL, ImVec2(LowerL.x - WindowWidth, LowerL.y), ImVec2(UpperL.x - WindowWidth, TopCornerBrake.y + 3.f), ColorConvertFloat4ToU32(ColorSteerI));
				UIDrawList->AddQuadFilled(ImVec2(UpperR.x, TopCornerBrake.y + 3.f), LowerR, ImVec2(LowerR.x + WindowWidth, LowerR.y), ImVec2(UpperR.x + WindowWidth, TopCornerBrake.y + 3.f), ColorConvertFloat4ToU32(ColorSteerI));
				if (InputInfo.Steer < 0)
					UIDrawList->AddQuadFilled(ImVec2(UpperL.x, TopCornerBrake.y + 3.f), LowerL, ImVec2(LowerL.x - Width, LowerL.y), ImVec2(UpperL.x - Width, TopCornerBrake.y + 3.f), ColorConvertFloat4ToU32(ColorSteer));
				else if (InputInfo.Steer > 0)
					UIDrawList->AddQuadFilled(ImVec2(UpperR.x, TopCornerBrake.y + 3.f), LowerR, ImVec2(LowerR.x - Width, LowerR.y), ImVec2(UpperR.x - Width, TopCornerBrake.y + 3.f), ColorConvertFloat4ToU32(ColorSteer));
				break;
			}





			UIDrawList->AddRectFilled(ImVec2(UpperL.x + 6.f, UpperL.y), ImVec2(BottomCornerGas.x - 6.f, BottomCornerGas.y - 3.f), InputInfo.get_Gas() ? ColorConvertFloat4ToU32(ColorAccel) : ColorConvertFloat4ToU32(ColorAccelI));
			UIDrawList->AddRectFilled(ImVec2(TopCornerBrake.x + 6.f, TopCornerBrake.y + 3.f), ImVec2(LowerR.x - 6.f, LowerR.y), InputInfo.get_Brake() ? ColorConvertFloat4ToU32(ColorBrake) : ColorConvertFloat4ToU32(ColorBrakeI));

			End();
		}
	}

	virtual void RenderSettings()
	{
		using namespace ImGui;
		const char* DisplayModes[]{ "Pad","Keyboard" };
		if (BeginTabItem(FancyName.c_str()))
		{
			Checkbox("Pointy Steering Variant", &PointySteering);

			if (BeginCombo("Display Mode", DisplayModes[(int)DisplayMode])) {
				for (int i = 0; i < IM_ARRAYSIZE(DisplayModes);i++) {
					bool is_selected = (DisplayMode == i);
					if (ImGui::Selectable(DisplayModes[i], is_selected))
						DisplayMode = i;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ColorEdit4("Steering", &ColorSteer.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Acceleration", &ColorAccel.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Brake", &ColorBrake.x, ImGuiColorEditFlags_NoInputs);

			Separator();

			ColorEdit4("Steering (inactive)", &ColorSteerI.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Acceleration (inactive)", &ColorAccelI.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Brake (inactive)", &ColorBrakeI.x, ImGuiColorEditFlags_NoInputs);

			Separator();

			ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);

			EndTabItem();
		}
	}
	virtual void RenderMenuItem()
	{
		using namespace ImGui;
		if (MenuItem("Input display", "Dashboard", Enabled))
		{
			Enabled = !Enabled;
		}
	}

	virtual void SettingsInit(SettingMgr& Settings)
	{
		Settings["Dashboard"]["Steer color"].GetAsVec4(&ColorSteer);
		Settings["Dashboard"]["Brake color"].GetAsVec4(&ColorBrake);
		Settings["Dashboard"]["Acceleration color"].GetAsVec4(&ColorAccel);

		Settings["Dashboard"]["Steer color (inactive)"].GetAsVec4(&ColorSteerI);
		Settings["Dashboard"]["Brake color (inactive)"].GetAsVec4(&ColorBrakeI);
		Settings["Dashboard"]["Acceleration color (inactive)"].GetAsVec4(&ColorAccelI);

		Settings["Dashboard"]["Input display background color"].GetAsVec4(&ColorBackground);

		Settings["Dashboard"]["Enable input display"].GetAsBool(&Enabled);
		Settings["Dashboard"]["Pointy Steering Variant"].GetAsBool(&PointySteering);
		Settings["Dashboard"]["Display Mode"].GetAsFloat(&DisplayMode);
	}

	virtual void SettingsSave(SettingMgr& Settings)
	{
		Settings["Dashboard"]["Steer color"].Set(ColorSteer);
		Settings["Dashboard"]["Brake color"].Set(ColorBrake);
		Settings["Dashboard"]["Acceleration color"].Set(ColorAccel);

		Settings["Dashboard"]["Steer color (inactive)"].Set(ColorSteerI);
		Settings["Dashboard"]["Brake color (inactive)"].Set(ColorBrakeI);
		Settings["Dashboard"]["Acceleration color (inactive)"].Set(ColorAccelI);

		Settings["Dashboard"]["Input display background color"].Set(ColorBackground);

		Settings["Dashboard"]["Enable input display"].Set(Enabled);
		Settings["Dashboard"]["Pointy Steering Variant"].Set(PointySteering);
		Settings["Dashboard"]["Display Mode"].Set(DisplayMode);
	}
};