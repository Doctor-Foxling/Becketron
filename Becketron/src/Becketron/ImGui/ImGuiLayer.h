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
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents;
		float m_Time = 0.0f;
	};
}