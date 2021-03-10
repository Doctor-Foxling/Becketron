#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Becketron {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		// copy constructor -- helps the reader that this is an intended usage for the given object
		// even tho, a copy constructorn is provided by default
		Entity(const Entity& other) = default;

		// Does perfect forwarding
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			BT_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			BT_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			BT_CORE_ASSERT(HasComponent<T>(), "Entity deos not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};
}