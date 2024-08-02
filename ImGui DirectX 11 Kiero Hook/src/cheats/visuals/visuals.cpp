#include "visuals.h"
#include "../offsets.h"
#include <cstdint>
#include "../../../ext/imgui/imgui.h"
#pragma once

uintptr_t V::client = (uintptr_t)GetModuleHandle("client.dll");

void V::PlayerESP()
{

	float(*ViewMatrix)[4][4] = (float(*)[4][4])(client + offsets::dwViewMatrix);
	auto localpawn = *(uintptr_t*)(client + offsets::dwLocalPlayerPawn);
	if (!localpawn)
		return;

	auto localteam = *(int*)(client + offsets::m_iTeamNum);
	auto entitylist = *(uintptr_t*)(client + offsets::dwEntityList);
	if (!entitylist)
	return;
	for (int i = 1; i < 64; i++) {
		uintptr_t list_entry1 = *(uintptr_t*)(entitylist + (8 * (i & 0x7FFF) >> 9) + 16);
		if (!list_entry1)
			continue;
		uintptr_t playercontroller = *(uintptr_t*)(list_entry1 + 120 * (i &0x1FF));
		if (!playercontroller)
			continue;
		uint32_t playerpawn = *(uint32_t*)(playercontroller + offsets::m_hPlayerPawn);
		if (!playerpawn)
			continue;
		uintptr_t list_entry2 = *(uintptr_t*)(entitylist + 0x8 *((playerpawn & 0x7FFF) >> 9) + 16);
		if (!list_entry2)
			continue;
		uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerpawn & 0x1FF));
			if (!pCSPlayerPawnPtr)
				continue;
			int health = *(int*)(pCSPlayerPawnPtr + offsets::m_iHealth);
			if (!health || health > 100)
				continue;

			int team = *(int*)(pCSPlayerPawnPtr + offsets::m_iTeamNum);

			if (team == localteam)
				continue;
			Vec3 feetpos = *(Vec3*)(pCSPlayerPawnPtr + offsets::m_vOldOrigin);
			Vec3 headpos = { feetpos.x + 0.0f, feetpos.y + 0.0f, feetpos.z + 65.0f };

			Vec2 feet, head;

			if (feetpos.WorldToScreen(feet, ViewMatrix) && headpos.WorldToScreen(head, ViewMatrix)) {
				float height = (feet.y - head.y) * 1.5f;
				float width = height / 1.5f;	
				ImGui::GetBackgroundDrawList()->AddRect({ feet.x - width / 2, head.y }, { feet.x - width / 2 + width, head.y + height }, ImColor(255, 255, 255));
			}


	}
}
