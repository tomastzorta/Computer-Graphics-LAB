
#include "Cube.h"

#include <GLM/detail/type_mat4x4.hpp>
#include <GLM/detail/type_vec4.hpp>

Cube::Cube()
{
		// Variable for storing our VAO
	// OpenGL has its own defined datatypes - a 'GLuint' is basically an unsigned int
	_VAO = 0;
	// Creates one VAO
	glGenVertexArrays( 1, &_VAO );
	// 'Binding' something makes it the current one we are using
	// This is like activating it, so that subsequent function calls will work on this item
	glBindVertexArray( _VAO );

	// Simple vertex data for a cube
	// (actually this is only four sides of a cube, you will have to expand this code if you want a complete cube :P )
	float vertices[] =
		{
		-0.5f, 0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,

		-0.5f,-0.5f, 0.5f,
		 0.5f,-0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,


		 0.5f, 0.5f, 0.5f,
		 0.5f,-0.5f, 0.5f,
		 0.5f, 0.5f,-0.5f,

		 0.5f,-0.5f, 0.5f,
		 0.5f,-0.5f,-0.5f,
		 0.5f, 0.5f,-0.5f,


		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f,-0.5f,
		-0.5f,-0.5f, 0.5f,

		-0.5f,-0.5f, 0.5f,
		-0.5f, 0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,


		 0.5f, 0.5f,-0.5f,
		 0.5f,-0.5f,-0.5f,
		-0.5f, 0.5f,-0.5f,

		-0.5f, 0.5f,-0.5f,
		 0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,


		-0.5f, 0.5f,-0.5f,
		-0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f,-0.5f,

		 0.5f, 0.5f,-0.5f,
		-0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f

	};
	// Number of vertices in above data
	_numVertices = 30;
	

	// Variable for storing a VBO
	GLuint buffer = 0;
	// Create a generic 'buffer'
	glGenBuffers(1, &buffer);
	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// With this buffer active, we can now send our data to OpenGL
	// We need to tell it how much data to send
	// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _numVertices * 3, vertices, GL_STATIC_DRAW);

	// This tells OpenGL how we link the vertex data to the shader
	// (We will look at this properly in the lectures)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(0);
	


	
	// Normal data for our incomplete cube
	// Each entry is the normal for the corresponding vertex in the position data above
	float normals[] =
		{
		 0.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f,
		 
		 0.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f,

		 1.0f, 0.0f, 0.0f,
		 1.0f, 0.0f, 0.0f,
		 1.0f, 0.0f, 0.0f,
		 
		 1.0f, 0.0f, 0.0f,
		 1.0f, 0.0f, 0.0f,
		 1.0f, 0.0f, 0.0f,
		 
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		 
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		
		 0.0f, 0.0f,-1.0f,
		 0.0f, 0.0f,-1.0f,
		 0.0f, 0.0f,-1.0f,
		 
		 0.0f, 0.0f,-1.0f,
		 0.0f, 0.0f,-1.0f,
		 0.0f, 0.0f,-1.0f,

		 0.0f, 1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,

		 0.0f, 1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f
	};

	// Variable for storing a VBO
	GLuint normalBuffer = 0;
	// Create a generic 'buffer'
	glGenBuffers(1, &normalBuffer);
	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	// With this buffer active, we can now send our data to OpenGL
	// We need to tell it how much data to send
	// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _numVertices * 3, normals, GL_STATIC_DRAW);

	// This tells OpenGL how we link the vertex data to the shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(1);
	
	// Unbind for neatness, it just makes life easier
	// As a general tip, especially as you're still learning, for each function that needs to do something specific try to return OpenGL in the state you found it in
	// This means you will need to set the states at the beginning of your function and set them back at the end
	// If you don't do this, your function could rely on states being set elsewhere and it's easy to lose track of this as your project grows
	// If you then change the code from elsewhere, your current code could mysteriously stop working properly!
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );
	
	// Technically we can do this, because the enabled / disabled state is stored in the VAO
	glDisableVertexAttribArray(0);

	//Cube Specific Properties
	m_cubeColour = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);
	m_lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_cubeShininess = 20.0f;
	m_roughness = 0.5f;
	m_metallic = 0.0f;
	m_sheen = 0.0f;
	m_sheenTint = 0.0f;
	m_subsurface = 0.0f;
	m_specular = 0.5f;
	m_specularTint = 0.0f;
	m_anisotropic = 0.0f;
	m_clearCoat = 0.0f;
	m_clearCoatGloss = 0.0f;
	
}

Cube::~Cube()
{
	glDeleteVertexArrays( 1, &_VAO );
	// TODO: delete the VBOs as well!
}

void Cube::Draw() const
{
		// Activate the VAO
		glBindVertexArray( _VAO );

			// Tell OpenGL to draw it
			// Must specify the type of geometry to draw and the number of vertices
			glDrawArrays(GL_TRIANGLES, 0, _numVertices);
			
		// Unbind VAO
		glBindVertexArray( 0 );
}
