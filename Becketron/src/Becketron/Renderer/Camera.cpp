#include "btpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Becketron {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		BT_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		BT_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		BT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	// ------------------ Perspective ------------------------

	PerspectiveCamera::PerspectiveCamera(float verticalFOV, float aspectRatio, float nearClip, float farClip)
		: m_ProjectionMatrix(glm::perspective(verticalFOV, aspectRatio, nearClip, farClip)), m_ViewMatrix(1.0f),
		m_RotAxis(BT_axis::z), m_ModelMatrix(1.0f)
	{
		BT_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(float verticalFOV, float aspectRatio, float nearClip, float farClip)
	{
		BT_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(verticalFOV, aspectRatio, nearClip, farClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetFocusPoint(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	{
		BT_PROFILE_FUNCTION();

		m_ViewMatrix = glm::lookAt(position, target, up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}



	void PerspectiveCamera::RecalculateViewMatrix()
	{
		BT_PROFILE_FUNCTION();

		glm::mat4 transform;

		if (!m_RotApplied)
		{
			transform = glm::translate(glm::mat4(1.0f), m_Position);
			//m_ViewMatrix = glm::inverse(transform);
		}
		/*else if (m_RotApplied)
		{
			glm::vec3 rotAxis;
			switch (m_RotAxis)
			{
			case BT_axis::x:	rotAxis = { 1.0f, 0.0f, 0.0f };
				m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), rotAxis);
				break;
			case BT_axis::y:	rotAxis = { 0.0f, 1.0f, 0.0f };
				m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), rotAxis);
				break;
			case BT_axis::z:	rotAxis = { 0.0f, 0.0f, 1.0f };
			    m_ModelMatrix = glm::rotate(glm::mat4(1.0), glm::radians(m_Rotation.z), rotAxis);
				break;
			}

			m_RotApplied = false;
		}*/
		

		//m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}