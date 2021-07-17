#pragma once

class NameManager
{
public:
	static NameManager* GetSingleton()
	{
		static NameManager singleton;
		return &singleton;
	}

	void LoadNames()
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
			ini.SetValue("Ash Pile Names", "Name", defaultName.data(), "", false);		
			
			names.emplace_back(defaultName);
		}

		ini.SaveFile(path);
	}

	std::string GetName(const std::string& a_ownerName, const std::string& a_ashpileName)
	{
		std::string templateName;
		if (names.size() > 1) {
			auto rng = RNG::GetSingleton()->Generate<std::uint32_t>(0, static_cast<std::uint32_t>(names.size() - 1));
			templateName = names[rng];
		} else {
			templateName = names[0];
		}

		const auto fill_template = [](std::string& a_subject, const std::string& a_search, const std::string& a_replace) {
			if (a_search.empty()) {
				return;
			}

			size_t pos = 0;
			while ((pos = a_subject.find(a_search, pos)) != std::string::npos) {
				a_subject.replace(pos, a_search.length(), a_replace);
				pos += a_replace.length();
			}
		};

		fill_template(templateName, "[npc]", a_ownerName);
		fill_template(templateName, "[ashpile]", a_ashpileName);

		return templateName;
	}

private:
	NameManager() = default;
	NameManager(const NameManager&) = delete;
	NameManager(NameManager&&) = delete;

	~NameManager() = default;

	NameManager& operator=(const NameManager&) = delete;
	NameManager& operator=(NameManager&&) = delete;

	std::vector<std::string> names;

	std::string_view defaultName{ "[npc]'s [ashpile]" };
};

class CrosshairRefManager : public RE::BSTEventSink<SKSE::CrosshairRefEvent>
{
public:
	static CrosshairRefManager* GetSingleton()
	{
		static CrosshairRefManager singleton;
		return &singleton;
	}

	static void Register()
	{
		auto crosshair = SKSE::GetCrosshairRefEventSource();
		if (crosshair) {
			crosshair->AddEventSink(GetSingleton());
			logger::info("Registered crosshair ref manager"sv);
		}
	}

protected:
	using EventResult = RE::BSEventNotifyControl;

	EventResult ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*) override
	{
		auto crosshairRef =
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

private:
	CrosshairRefManager() = default;
	CrosshairRefManager(const CrosshairRefManager&) = delete;
	CrosshairRefManager(CrosshairRefManager&&) = delete;

	~CrosshairRefManager() = default;

	CrosshairRefManager& operator=(const CrosshairRefManager&) = delete;
	CrosshairRefManager& operator=(CrosshairRefManager&&) = delete;

	void Process(RE::TESObjectREFRPtr a_ref)
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

	bool IsAshPile(RE::TESObjectREFRPtr a_ref)
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

	RE::ObjectRefHandle _cachedRef;
	RE::ObjectRefHandle _cachedAshPile;
};
