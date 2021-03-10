#pragma once

#include "Becketron/Core/Core.h"
#include "Becketron/Core/Log.h"
#include "Becketron/Scene/Scene.h"
#include "Becketron/Scene/Entity.h"

namespace Becketron {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}