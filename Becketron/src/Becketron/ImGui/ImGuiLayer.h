#pragma once

#include "Becketron/Core/Layer.h"

#include "Becketron/Events/MouseEvent.h"
#include "Becketron/Events/KeyEvent.h"
#include "Becketron/Events/ApplicationEvent.h"

namespace Becketron {

	class BTRON_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}