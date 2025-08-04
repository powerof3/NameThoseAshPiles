#include "Hooks.h"

#include "Manager.h"

namespace Hooks
{
	struct AttachAshPileRef
	{
		static void thunk(RE::ExtraDataList* a_extra, const RE::ObjectRefHandle& a_handle)
		{
			func(a_extra, a_handle);

			auto ashPile = stl::adjust_pointer<RE::TESObjectREFR>(a_extra, -static_cast<std::ptrdiff_t>(offsetof(RE::TESObjectREFR, RE::TESObjectREFR::extraList)));
			auto owner = a_handle.get();

			if (ashPile && owner && owner->IsActor() && !a_extra->HasType<RE::ExtraTextDisplayData>()) {
				Manager::GetSingleton()->SetAshPileName(ashPile, owner.get());
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(11631, 11777) };
			stl::hook_function_prologue<AttachAshPileRef, 6>(target1.address());
		}
	};

	void Install()
	{
		AttachAshPileRef::Install();
	}
}
