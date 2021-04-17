#include "btpch.h"
#include "LayerStack.h"

namespace Becketron {

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		// So when we push an overlay, the layerIndex doesn't get incremented
		// So when we push another layer, it gets pushed before the overlay
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::remove(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		//auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			it = m_Layers.erase(it);
			//if (it != m_Layers.begin())
				//it = std::prev(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			it = m_Layers.erase(it);
			size_t remaining = std::distance(m_Layers.begin(), it);
			BT_TRACE("Remaining layers {0}", remaining);
			/*if (it != m_Layers.begin())
				it = std::prev(it);*/
		}
	}

}