#define GAL_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include <gal.hpp>
#include <iostream>

#include "stb_image.h"

static void processInput(const gal::Window& window)
{
	// Can use regular glfw callback too.
	if (gal::isKeyPressed(GLFW_KEY_ESCAPE))
		window.setShouldClose(true);

	if (gal::isKeyPressed(GLFW_KEY_0))
		std::cout << "Hey, you just pressed zero!\n";
}

int main()
{
	gal::init(); // Initialize everything GAL needs before calling any other GAL function.
	gal::setOpenGLVersion(4, 5);  // OpenGL version must be set before creating a window.

	gal::Window window = gal::Window(800, 600, "Hello! I'm using GAL!", true);
	window.setClearColor(0.1f, 0.2f, 0.2f, 1.0f);

	// ============ Shader ============

	gal::ShaderProgram shader = gal::ShaderProgram();
	shader.addShaderFromFile("vert.vert", gal::ShaderType::Vertex)
		.addShaderFromFile("frag.frag", gal::ShaderType::Fragment)
		.link();

	shader.use();

	// ============ buffers ============

	gal::VertexP3C3T2 vertices[] =
	{
		{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },  // bottom left
		{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },  // bottom right
		{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },  // top left
		{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }   // top right
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		1, 2, 3
	};

	gal::VertexArray vao = gal::VertexArray();

	gal::Buffer vbo = gal::Buffer(gal::BufferType::Array);
	vbo.allocateAndWrite(sizeof(vertices), vertices, gal::BufferUsageHint::StaticDraw);

	vao.bindVertexBuffer(vbo, 0, 0, sizeof(gal::VertexP3C3T2));
	vao.newVertexAttribute(0, 0, 3, GL_FLOAT, GL_FALSE, 0);
	vao.newVertexAttribute(1, 0, 3, GL_FLOAT, GL_FALSE, offsetof(gal::VertexP3C3T2, color));
	vao.newVertexAttribute(2, 0, 2, GL_FLOAT, GL_FALSE, offsetof(gal::VertexP3C3T2, texCoords));
	
	gal::Buffer ebo = gal::Buffer(gal::BufferType::ElementArray);
	ebo.allocateAndWrite(sizeof(indices), indices, gal::BufferUsageHint::StaticDraw);

	vao.bindElementBuffer(ebo, GL_UNSIGNED_INT);

	vao.bind();

	// ============ Texture ============

	gal::Texture tex = gal::Texture(gal::TextureType::TwoD);

	tex.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tex.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tex.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;

	unsigned char* data = stbi_load("resources/pone.png", &width, &height, &channels, 0);

	tex.imageAB(0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, data, width, height);
	stbi_image_free(data);
	
	tex.generateMipmapAB();
	tex.bindTextureUnit(0);

	shader.setUniform("texture1", 0);
	shader.setUniform("transparentColor", glm::vec4(0.0f, 1.0f, 1.0f, 0.2f));

	while (!window.shouldClose())
	{
		window.pollEvents();  // Poll for window events (keypresses, window resizing, etc.).
							  // Your update loop should start with this.
		processInput(window);

		window.clearBackground();

		shader.setUniform("t", static_cast<float>(glfwGetTime()));
		vao.drawElementsNB(GL_TRIANGLES, 0, 6);

		window.swapBuffers();  // Swap buffers. your update loop should end with this.
	}

	gal::terminate();
	return 0;
}