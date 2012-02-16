#include	<glew.h>
#include	<string>
#include	<fstream>
#include	<exception>
#include	"VisualisationDefinitions.h"
#include	"Shader.h"




Shader::Shader() :
	program(0) , 
	fragmentShader(0) , fragmentShaderFile(L"") , fragmentShaderSource("") , 
	vertexShader(0) , vertexShaderFile(L"") , vertexShaderSource("") {};

Shader::Shader( wstring fragmentShaderFilename , wstring vertexShaderFilename ) : 
	program(0) , 
	fragmentShader(0) , fragmentShaderFile(fragmentShaderFilename) , 
	vertexShader(0) , vertexShaderFile(vertexShaderFilename)
{
	try
	{
		if ( fragmentShaderFilename != L"" )
			LoadFragmentShader(fragmentShaderFilename);
		else
			this->fragmentShaderSource = "";

		if ( vertexShaderFilename != L"" )
			LoadVertexShader(vertexShaderFilename);
		else
			this->vertexShaderSource = "";
	}
	catch ( exception& e )
	{
		cerr << "An exception has occurred in Shader::Shader() :\t" << e.what() << endl;
		throw exception("Shader Constructor");
	}
};

Shader::~Shader()	{};


bool	LoadFile( wstring filename , string& output )
{
	bool		returnValue = false;
	int			size;
	char*		buffer = NULL;	
	ifstream	file;



	try
	{
		if ( filename != L"" )
		{
			file.exceptions(fstream::failbit | fstream::badbit);
			file.open(filename.c_str(),ios::binary|ios::in);
		
			file.seekg(0,ios::end);
			size = file.tellg();
			file.seekg(0,ios::beg);

			if ( size > -1 )
			{
				buffer = new char[size+1];
				file.read(buffer,size);
				file.close();

				buffer[size] = '\0';
				output = buffer;

				delete[] buffer;
				buffer = NULL;
				returnValue = true;
			}
		}



		return returnValue;
	}
	catch ( exception& e )
	{
		if ( buffer != NULL )
			delete[] buffer;


		cerr << "An exception has occured while loading a shader:\t" << e.what() << endl;
		throw exception("Shader Load File");
	}
};

bool	Shader::LoadFragmentShader( wstring filename )
{
	bool	returnValue = false;



	if ( LoadFile(filename,this->fragmentShaderSource) )
	{
		this->fragmentShaderFile = filename;
		returnValue = true;
	}



	return returnValue;
};

bool	Shader::LoadVertexShader( wstring filename )
{
	bool	returnValue = false;



	if ( LoadFile(filename,this->vertexShaderSource) )
	{
		this->vertexShaderFile = filename;
		returnValue = true;
	}



	return returnValue;
};

void	Shader::CreateAndCompileShaders()
{
	const char*		temp = NULL;



	if ( this->fragmentShaderFile != L"" )
	{
		this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		temp = this->fragmentShaderSource.c_str();
		glShaderSource(this->fragmentShader,1,&temp,NULL);

		glCompileShader(this->fragmentShader);
	}
	
	if ( this->vertexShaderFile != L"" )
	{
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);

		temp = this->vertexShaderSource.c_str();
		glShaderSource(this->vertexShader,1,&temp,NULL);

		glCompileShader(this->vertexShader);
	}
};

void	Shader::AttachShaders()
{
	if ( this->program > 0 )
	{
		if ( this->fragmentShader > 0 )
			glAttachShader(this->program,this->fragmentShader);

		if ( this->vertexShader > 0 )
			glAttachShader(this->program,this->vertexShader);
	}
}

void	Shader::DetachShaders()
{
	if ( this->program > 0 )
	{
		if ( this->fragmentShader > 0 )
			glDetachShader(this->program,this->fragmentShader);

		if ( this->vertexShader > 0 )
			glDetachShader(this->program,this->vertexShader);
	}
};

void	Shader::DeleteShaders()
{
	this->DetachShaders();

	if ( this->fragmentShader > 0 )
	{
		glDeleteShader(this->fragmentShader);
		this->fragmentShader = 0;
	}

	if (this->vertexShader > 0 )
	{
		glDeleteShader(this->vertexShader);
		this->vertexShader = 0;
	}
}


void	Shader::setProgram( GLuint id )		{ this->program = id; }
void	Shader::CreateAndLinkProgram()
{
	if ( this->program <= 0 )
	{
		this->program = glCreateProgram();
		this->AttachShaders();
		glLinkProgram(this->program);
	}
}

void	Shader::ActivateProgram()
{
	if ( this->program > 0 )
		glUseProgram(this->program);
};

void	Shader::DeactivateProgram()			{ glUseProgram(0); };
void	Shader::DeleteProgram()
{
	if ( this->program > 0 )
		glDeleteProgram(this->program);
}


GLuint	Shader::getProgram()				{ return this->program; };
GLuint	Shader::getFragmentShader()			{ return this->fragmentShader; };
wstring	Shader::getFragmentShaderFile()		{ return this->fragmentShaderFile; };
string	Shader::getFragmentShaderSource()	{ return this->fragmentShaderSource; };
GLuint	Shader::getVertexShader()			{ return this->vertexShader; };
wstring	Shader::getVertexShaderFile()		{ return this->vertexShaderFile; };
string	Shader::getVertexShaderSource()		{ return this->vertexShaderSource; };


string	printShaderLog( GLuint id )
{
	int		length;
	char*	log = NULL;
	string	returnValue;



	try
	{
		glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
	
		if ( length > 0 )
		{
			int		chars;


			log = new char[length];
			glGetShaderInfoLog(id,length,&chars,log);
			returnValue = log;
			delete[] log;
			log = NULL;
		}
		else
			returnValue = "";



		return returnValue;
	}
	catch ( exception& e )
	{
		if ( log != NULL )
			delete[] log;


		cerr << "An exception occurred in Shadder:printShader() :\t" << e.what() << endl;
		throw exception("Shader Print Log");
	}
};

void	Shader::printFragmentShaderLog()
{
	string	log = printShaderLog(this->fragmentShader);

	
	
	if ( log != "" )
		cout << endl << "Fragment Shader Log:" << endl << endl << log << endl;
	else
		cout << "There is no log available for the fragment shader" << endl;
};

void	Shader::printVertexShaderLog()
{
	string	log = printShaderLog(this->vertexShader);

	
	
	if ( log != "" )
		cout << endl << "Vertex Shader Log:" << endl << endl << log << endl;
	else
		cout << "There is no log available for the vertex shader!" << endl;
};

void	Shader::printShaderLogs()
{
	this->printFragmentShaderLog();
	this->printVertexShaderLog();
}

void	Shader::printProgramLog()
{
	int		length;
	char*	log = NULL;
	string	returnValue;



	try
	{
		glGetProgramiv(this->program,GL_INFO_LOG_LENGTH,&length);
	
		if ( length > 0 )
		{
			int		chars;


			log = new char[length];
			glGetProgramInfoLog(this->program,length,&chars,log);
			cout << endl << "Program Log: " << endl << log << endl;
			delete[] log;
			log = NULL;
		}
		else
			cout << "There is no log available for the program" << endl;
	}
	catch ( exception& e )
	{
		if ( log != NULL )
			delete[] log;


		cerr << "An exception occurred in Shadder:printProgramLog() :\t" << e.what() << endl;
		throw exception("Shader Print Program Log");
	}
};