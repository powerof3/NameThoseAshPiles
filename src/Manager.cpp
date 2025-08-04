#include "Manager.h"

void Manager::RequestAPI()
{
	_nnd = static_cast<NND_API::IVNND2*>(NND_API::RequestPluginAPI());
	if (_nnd) {
		logger::info("NND API requested successfully");
	} else {
		logger::error("Failed to request NND API");
	}
}

void Manager::LoadNames()
{
	constexpr auto path = L"Data/SKSE/Plugins/po3_NameThoseAshpiles.ini";

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.SetMultiKey();

	ini.LoadFile(path);

	CSimpleIniA::TNamesDepend values;
	ini.GetAllValues("Ash Pile Names", "Name", values);
	values.sort(CSimpleIniA::Entry::LoadOrder());

	if (const auto size = values.size(); size > 0) {
		logger::info("{} names found", size);
		_names.reserve(values.size());
		for (auto& value : values) {
			_names.emplace_back(value.pItem);
		}
	} else {
		std::string defaultName = "[npc]'s [ashpile]";
		ini.SetValue("Ash Pile Names", "Name", defaultName.c_str(), "", false);
		_names.emplace_back(defaultName);
	}

	(void)ini.SaveFile(path);
}

void Manager::SetAshPileName(RE::TESObjectREFR* a_ashpile, RE::TESObjectREFR* a_owner) const
{
	std::string templateName;
	if (_names.size() > 1) {
		const auto size = static_cast<std::uint32_t>(_names.size() - 1);
		templateName = _names[clib_util::RNG().generate<std::uint32_t>(0, size)];
	} else {
		templateName = _names[0];
	}

	string::replace_all(templateName, "[npc]", GetOwnerName(a_owner));
	string::replace_all(templateName, "[ashpile]", a_ashpile->GetDisplayFullName());

	a_ashpile->SetDisplayName(templateName, true);
}

std::string Manager::GetOwnerName(RE::TESObjectREFR* a_owner) const
{
	if (_nnd) {
		if (auto actor = a_owner->As<RE::Actor>()) {
			if (auto name = _nnd->GetName(actor, NND_API::NameContext::kCrosshair); !name.empty()) {
				return { name.data(), name.size() };
			}
		}
	}

	return a_owner->GetDisplayFullName();
}
