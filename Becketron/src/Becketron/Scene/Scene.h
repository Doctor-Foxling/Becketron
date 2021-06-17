#pragma once

#include "entt.hpp"

#include "Becketron/Core/Timestep.h"

// Temporary
#include "Becketron/Physics/BT_Physics/PhysicsEngine.h"

//#include "Becketron/Physics/PhysX/PhysXScene.h"
#include "Becketron/Physics/PhysX/PhysXManager.h"


namespace Becketron {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnShutdown();
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		PhysicsEngine m_PhysEng;

	private:
		//entt::entity m_SceneEntity;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		physx::PxRigidStatic* m_Ground;

		bool m_ScenePlay = false;
		//bool m_ScenePlayLast = false;
		bool m_SceneRestart = false;
		//bool m_SceneRestartLast = false;

		bool m_SceneFirstStart = false;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}


