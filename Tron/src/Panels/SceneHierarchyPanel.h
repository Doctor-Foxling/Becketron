#pragma once

#include "Becketron/Core/Core.h"
#include "Becketron/Core/Log.h"
#include "Becketron/Scene/Scene.h"
#include "Becketron/Scene/Entity.h"

namespace Becketron {

	enum class ComponentType;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		void SceneOptions();
		bool EntityHasComponentType(Entity entity, ComponentType type);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}