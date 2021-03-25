#pragma once

#include "Becketron/Scene/Entity.h"

namespace Becketron {

	class PhysicsEntity
	{
	public:
		virtual ~PhysicsEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}