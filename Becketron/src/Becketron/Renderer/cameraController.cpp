#include "btpch.h"
#include "cameraController.h"

#include "Becketron/Core/Input.h"
#include "Becketron/Core/KeyCodes.h"

#include "GLFW/glfw3.h"

namespace Becketron {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		BT_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(BT_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(BT_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(BT_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(BT_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(BT_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(BT_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}


	void OrthographicCameraController::OnEvent(Event& e)
	{
		BT_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		BT_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		BT_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	// ----------------- Perspective -----------------------------------
	PerspectiveCameraController::PerspectiveCameraController(float FOV, float aspectRatio, float nearClip, float farClip, bool rotation)
		: m_PerspectiveFOV(FOV), m_AspectRatio(aspectRatio), m_PerspectiveNear(nearClip), m_PerspectiveFar(farClip),
		m_Camera(FOV, aspectRatio, nearClip, farClip), m_Rotation(rotation)
	{

	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		BT_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(BT_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(BT_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(BT_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(BT_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(BT_KEY_Q) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
			{
				BT_CORE_INFO("Input keys: {0} and {1}", BT_KEY_Q, BT_KEY_LEFT_CONTROL);
				m_CameraRotation.z += m_CameraTranslationSpeed * ts;
				m_Camera.SetRotAxis(BT_axis::z);
			}
			else if (Input::IsKeyPressed(BT_KEY_E) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
			{
				m_CameraRotation.z -= m_CameraTranslationSpeed * ts;
				m_Camera.SetRotAxis(BT_axis::z);
			}

			if (Input::IsKeyPressed(BT_KEY_W) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
			{
				m_CameraRotation.x += m_CameraTranslationSpeed * ts;
				m_Camera.SetRotAxis(BT_axis::x);
			}
			else if (Input::IsKeyPressed(BT_KEY_S) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
			{
				m_CameraRotation.x -= m_CameraTranslationSpeed * ts;
				m_Camera.SetRotAxis(BT_axis::x);
			}

			 if (Input::IsKeyPressed(BT_KEY_A) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
			{
				m_CameraRotation.x += m_CameraTranslationSpeed * ts;
				m_Camera.SetRotAxis(BT_axis::x);
			}

			else if (Input::IsKeyPressed(BT_KEY_D) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
			{
				m_CameraRotation.x -= m_CameraTranslationSpeed * ts;
				m_Camera.SetRotAxis(BT_axis::x);
			}

			m_CamRotAxis = m_Camera.GetRotAxis();
			float radius = 10.0f;

			//float camX = sin(glfwGetTime()) * radius;
			//float camZ = cos(glfwGetTime()) * radius;

			//m_Camera.SetFocusPoint(glm::vec3(camX, 0.0, camZ));

			switch (m_CamRotAxis)
			{
			case BT_axis::x:	m_Camera.SetFocusPoint(m_CameraPosition * radius);
				if (m_CameraRotation.x > 180.0f)
					m_CameraRotation.x -= 360.0f;
				else if (m_CameraRotation.x <= -180.0f)
					m_CameraRotation.x += 360.0f;
				break;

			case BT_axis::y:	m_Camera.SetFocusPoint({ 0.0f, m_CameraRotation.y * radius, 0.0f, });;
				if (m_CameraRotation.y > 180.0f)
					m_CameraRotation.y -= 360.0f;
				else if (m_CameraRotation.y <= -180.0f)
					m_CameraRotation.y += 360.0f;
				break;

			case BT_axis::z:	m_Camera.SetFocusPoint(m_CameraPosition * radius);
				if (m_CameraRotation.z > 180.0f)
					m_CameraRotation.z -= 360.0f;
				else if (m_CameraRotation.z <= -180.0f)
					m_CameraRotation.z += 360.0f;
				 break;
			}
			
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}


	void PerspectiveCameraController::OnEvent(Event& e)
	{
		BT_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BT_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BT_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	void PerspectiveCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		BT_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, -0.05f);
		//m_ZoomLevel = std::min(m_ZoomLevel, 2.0f);
		m_CameraPosition.z = m_ZoomLevel;
		m_Camera.SetProjection(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		m_Camera.SetFocusPoint(m_CameraPosition);
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		BT_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}

