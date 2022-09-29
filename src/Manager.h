#pragma once

class NameManager
{
public:
	static NameManager* GetSingleton()
	{
		static NameManager singleton;
		return &singleton;
	}

	void LoadNames();
	[[nodiscard]] std::string GetName(const std::string& a_ownerName, const std::string& a_ashpileName) const;

private:
	NameManager() = default;
	NameManager(const NameManager&) = delete;
	NameManager(NameManager&&) = delete;

	~NameManager() = default;

	NameManager& operator=(const NameManager&) = delete;
	NameManager& operator=(NameManager&&) = delete;

	std::vector<std::string> names{};
	std::string defaultName{ "[npc]'s [ashpile]" };
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
		if (auto crosshair = SKSE::GetCrosshairRefEventSource()) {
			crosshair->AddEventSink(GetSingleton());
			logger::info("Registered crosshair ref manager"sv);
		}
	}

protected:
	using EventResult = RE::BSEventNotifyControl;

	EventResult ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*) override;

private:
	CrosshairRefManager() = default;
	CrosshairRefManager(const CrosshairRefManager&) = delete;
	CrosshairRefManager(CrosshairRefManager&&) = delete;

	~CrosshairRefManager() override = default;

	CrosshairRefManager& operator=(const CrosshairRefManager&) = delete;
	CrosshairRefManager& operator=(CrosshairRefManager&&) = delete;

	void Process(const RE::TESObjectREFRPtr& a_ref);
	bool IsAshPile(const RE::TESObjectREFRPtr& a_ref);

	RE::ObjectRefHandle _cachedRef{};
	RE::ObjectRefHandle _cachedAshPile{};
};
