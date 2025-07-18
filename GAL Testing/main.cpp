#define OBR_WARNINGS
#include <gal.hpp>
#include <iostream>

static void processInput()
{
	// Can use regular glfw callback too.
	if (gal::isKeyPressed(GLFW_KEY_ESCAPE))
		gal::setWindowShouldClose(true);

	if (gal::isKeyPressed(GLFW_KEY_0))
		std::cout << "Hey, you just pressed zero!\n";
}

int main()
{
	// Initialize everything GAL needs before calling any other GAL function.
	gal::init();

	GLFWwindow* window = gal::createMainWindow(4, 5, true, 800, 600, "Hello! I'm using GAL!", false, true);
	gal::setClearColor(0.1f, 0.2f, 0.2f, 1.0f);

	gal::ShaderProgram shader = gal::ShaderProgram();
	shader.addShaderFromFile("vert.vert", gal::ShaderType::Vertex)
		.addShaderFromFile("frag.frag", gal::ShaderType::Fragment)
		.link();

	gal::VertexP3C3 vertices[] =
	{
		{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },  // bottom left
		{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },  // bottom right
		{ { -0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f } },  // top left
		{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } }   // top right
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		1, 2, 3
	};

	gal::VertexArray vao = gal::VertexArray();

	gal::Buffer vbo = gal::Buffer(gal::BufferType::Array);
	vbo.allocateAndWrite(sizeof(vertices), vertices, gal::BufferUsageHint::StaticDraw);

	vao.bindVertexBuffer(vbo, 0, 0, sizeof(gal::VertexP3C3));
	vao.newVertexAttribute(0, 0, 3, GL_FLOAT, GL_FALSE, 0);
	vao.newVertexAttribute(1, 0, 3, GL_FLOAT, GL_FALSE, offsetof(gal::VertexP3C3, color));
	
	gal::Buffer ebo = gal::Buffer(gal::BufferType::ElementArray);
	ebo.allocateAndWrite(sizeof(indices), indices, gal::BufferUsageHint::StaticDraw);

	vao.bindElementBuffer(ebo, GL_UNSIGNED_INT);

	vao.bind();
	shader.use();

	while (!gal::windowShouldClose())
	{
		gal::pollEvents();
		processInput();

		gal::clearBackground();

		vao.drawElementsUnbound(GL_TRIANGLES, 0, 6);

		gal::swapBuffers();
	}

	gal::terminate();
	return 0;
}