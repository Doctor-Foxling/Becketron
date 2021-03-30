#type vertex
#version 330 core
			
		layout(location = 0) in vec4 a_Position;
			
		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		out vec4 v_Position;

		void main()
		{
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_Transform * a_Position;
		}		

#type fragment
#version 330 core
			
		layout(location = 0) out vec4 color;
			
		in vec4 v_Position;
		
		uniform vec4 u_Color;

		void main()
		{
			color = u_Color;
		}