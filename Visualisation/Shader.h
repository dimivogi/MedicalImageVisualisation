#include	<iostream>
#include	<glut.h>
using namespace std;



#ifndef	SHADER
#define	SHADER


// A class containing a Shader object
class	Shader
{
	private:

		// Shader Program ID
		GLuint		program;
		// Fragment Shader ID
		GLuint		fragmentShader;
		// Fragment Shader source code filename
		wstring		fragmentShaderFile;
		// Fragment Shader source code
		string		fragmentShaderSource;
		// Vertex Shader ID
		GLuint		vertexShader;
		// Vertex Shader source code filename
		wstring		vertexShaderFile;
		// Vertex Shader source code
		string		vertexShaderSource;



	public:

		Shader();
		Shader( wstring fragmentShaderFilename , wstring vertexShaderFilename );
		~Shader();
		

		// Manually setting the program on which the shader(s) are attached to.
		void		setProgram( GLuint id );
		// Load a fragment shader file into memory
		bool		LoadFragmentShader( wstring filename );
		// Load a vertex shader file into memory
		bool		LoadVertexShader( wstring filename );
		// Create and compile the loaded shaders
		void		CreateAndCompileShaders();
		// Attach the shaders to the program specified
		void		AttachShaders();
		// Detach the shaders from the program
		void		DetachShaders();
		// Delete the shaders from the program
		void		DeleteShaders();
		// Create and link the program
		void		CreateAndLinkProgram();
		// Set the program as the active program
		void		ActivateProgram();
		// Return to default functionality
		void		DeactivateProgram();
		// Delete the program
		void		DeleteProgram();
		

		// Get the program ID
		GLuint		getProgram();
		// Get the fragment shader ID
		GLuint		getFragmentShader();
		// Get the fragment shader filename
		wstring		getFragmentShaderFile();
		// Get the fragment shader source code
		string		getFragmentShaderSource();
		// Get the vertex shader ID
		GLuint		getVertexShader();
		// Get the vertex shader filename
		wstring		getVertexShaderFile();
		// Get the vertex shader source code
		string		getVertexShaderSource();
		

		// Print functions
		void		printFragmentShaderLog();
		void		printVertexShaderLog();
		void		printShaderLogs();
		void		printProgramLog();
};


#endif	/* SHADER */