#include "Manager.h"

void NameManager::LoadNames()
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
		names.reserve(values.size());
		for (auto& value : values) {
			names.emplace_back(value.pItem);
		}
	} else {
		ini.SetValue("Ash Pile Names", "Name", defaultName.c_str(), "", false);
		names.emplace_back(defaultName);
	}

	ini.SaveFile(path);
}

std::string NameManager::GetName(const std::string& a_ownerName, const std::string& a_ashpileName) const
{
	std::string templateName;
	if (names.size() > 1) {
		const auto size = static_cast<std::uint32_t>(names.size() - 1);

		templateName = names[RNG::GetSingleton()->Generate<std::uint32_t>(0, size)];
	} else {
		templateName = names[0];
	}

	stl::string::replace_all(templateName, "[npc]", a_ownerName);
	stl::string::replace_all(templateName, "[ashpile]", a_ashpileName);

	return templateName;
}

CrosshairRefManager::EventResult CrosshairRefManager::ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*)
{
    const auto crosshairRef =
		a_event && a_event->crosshairRef ?
			a_event->crosshairRef->CreateRefHandle() :
			RE::ObjectRefHandle();
	if (_cachedRef == crosshairRef) {
		return EventResult::kContinue;
	}

	_cachedRef = crosshairRef;
	_cachedAshPile.reset();

	Process(a_event->crosshairRef);

	return EventResult::kContinue;
}

void CrosshairRefManager::Process(const RE::TESObjectREFRPtr& a_ref)
{
	if (IsAshPile(a_ref)) {
		auto owner = _cachedAshPile.get();
		auto ownerName = owner ? owner->GetDisplayFullName() : std::string();
		if (!ownerName.empty()) {
			auto finalName = NameManager::GetSingleton()->GetName(ownerName, a_ref->GetName());
			a_ref->SetDisplayName(finalName, true);
		}
	}
}

bool CrosshairRefManager::IsAshPile(const RE::TESObjectREFRPtr& a_ref)
{
	auto obj = a_ref ? a_ref->GetObjectReference() : nullptr;
	if (!a_ref || !obj) {
		return false;
	}

	if (obj->Is(RE::FormType::Activator)) {
		auto ashPile = a_ref->extraList.GetAshPileRef();
		if (!ashPile || _cachedAshPile == ashPile) {
			return false;
		}
		_cachedAshPile = ashPile;

		return !a_ref->extraList.HasType<RE::ExtraTextDisplayData>();
	}

	return false;
}
