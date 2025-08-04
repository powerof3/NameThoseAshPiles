#pragma once

#include "NND_API.h"

class Manager : public REX::Singleton<Manager>
{
public:
	void RequestAPI();
	void LoadNames();
	void SetAshPileName(RE::TESObjectREFR* a_ashpile, RE::TESObjectREFR* a_owner) const;

private:
	std::string GetOwnerName(RE::TESObjectREFR* a_owner) const;

	// members
	std::vector<std::string> _names{};
	NND_API::IVNND2*         _nnd{};
};
