#define GAL_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include <gal.hpp>
#include <iostream>

#include "stb_image.h"

static float t;
static bool wireframe = false;

static void processInput(gal::Window& window)
{
	using namespace gal::keyboard;

	// Can use regular glfw callback too.
	if (isKeyPressed(GLFW_KEY_ESCAPE))
		window.setShouldClose(true);

	if (isKeyPressed(GLFW_KEY_0))
		std::cout << "Hey, you just pressed zero!\n";

	if (isKeyPressed(GLFW_KEY_M))
	{
		wireframe = !wireframe;

		if (wireframe)
			window.setPolygonMode(gal::PolygonMode::Line);
		else
			window.setPolygonMode(gal::PolygonMode::Fill);
	}
}

int main()
{
	gal::init();  // Initialize everything GAL needs before calling any other GAL function.
	gal::setOpenGLVersion(4, 5);  // OpenGL version must be set before creating a window.

	gal::Window window = gal::Window(800, 600, "Hello! I'm using GAL!", true);  // Create a window with debug context active.
	window.setClearColor(0.1f, 0.2f, 0.2f, 1.0f);

	// ============ Shader ============

	gal::ShaderProgram shader = gal::ShaderProgram();
	shader.addShaderFromFile("vert.vert", gal::ShaderType::Vertex)
		.addShaderFromFile("frag.frag", gal::ShaderType::Fragment)
		.link();

	shader.use();

	// ============ buffers ============

	std::vector<gal::VertexP3> positions;
	std::vector<gal::VertexT2> texCoords;

	gal::shapes::generateCubeVertices(6, &positions, nullptr, &texCoords);

	gal::VertexArray vao = gal::VertexArray();

	gal::Buffer vbo1 = gal::Buffer(gal::BufferType::Array);
	vbo1.allocateAndWrite(positions.size() * sizeof(gal::VertexP3), positions.data(), gal::BufferUsageHint::StaticDraw);

	gal::Buffer vbo2 = gal::Buffer(gal::BufferType::Array);
	vbo2.allocateAndWrite(positions.size() * sizeof(gal::VertexT2), texCoords.data(), gal::BufferUsageHint::StaticDraw);

	vao.bindVertexBuffer(vbo1, 0, 0, sizeof(gal::VertexP3));
	vao.bindVertexBuffer(vbo2, 1, 0, sizeof(gal::VertexT2));

	vao.newVertexAttribute(0, 0, 3, GL_FLOAT, GL_FALSE, 0);
	vao.newVertexAttribute(1, 1, 2, GL_FLOAT, GL_FALSE, 0);

	vao.bind();

	// ============ Texture ============

	gal::Texture tex = gal::Texture(gal::TextureType::TwoD);

	// TODO: Func for setting texture wrap and similar settings for all dimensions at the same time.
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

	// ============ Uniforms ============

	shader.setUniform("texture1", 0);
	shader.setUniform("transparentColor", glm::vec4(0.0f, 1.0f, 1.0f, 0.2f));

	// TODO: wrap this kind of stuff in an object class or transform struct?
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	shader.setUniform("view", view);
	shader.setUniform("projection", projection);

	while (!window.shouldClose())
	{
		window.pollEvents();  // Poll for window events (keypresses, window resizing, etc.).
							  // Your update loop should start with this.
		processInput(window);

		window.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		t = static_cast<float>(glfwGetTime());

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));

		shader.setUniform("model", model);
		shader.setUniform("t", t);

		vao.drawNB(GL_TRIANGLES, 0, static_cast<GLsizei>(positions.size()));

		window.swapBuffers();  // Swap buffers. your update loop should end with this.
	}

	gal::terminate();
	return 0;
}