#define GAL_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include <gal.hpp>
#include <shapes.hpp>

#include "stb_image.h"

static gal::Camera camera = gal::Camera(glm::vec3(0.0f, 0.0f, 3.0f));
static constexpr float cameraMoveSpeed = 3.0f;
static constexpr float cameraRotateSpeed = 2.0f;

static float t;
static bool wireframe = false;

static void processInput(gal::Window& window)
{
	using namespace gal::keyboard;

	// ============ Keypresses ============

	// Can use regular glfw callback too if you wish.
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

	// ============ Keydowns ============

	const float dt = gal::getDeltaTime<float>();

	if (isKeyDown(GLFW_KEY_W))
		camera.moveLocal(glm::vec3(0.0f, 0.0f, cameraMoveSpeed * dt));
	if (isKeyDown(GLFW_KEY_A))
		camera.moveLocal(glm::vec3(-cameraMoveSpeed * dt, 0.0f, 0.0f));
	if (isKeyDown(GLFW_KEY_S))
		camera.moveLocal(glm::vec3(0.0f, 0.0f, -cameraMoveSpeed * dt));
	if (isKeyDown(GLFW_KEY_D))
		camera.moveLocal(glm::vec3(cameraMoveSpeed * dt, 0.0f, 0.0f));
	if (isKeyDown(GLFW_KEY_LEFT_SHIFT))
		camera.moveLocal(glm::vec3(0.0f, cameraMoveSpeed * dt, 0.0f));
	if (isKeyDown(GLFW_KEY_LEFT_CONTROL))
		camera.moveLocal(glm::vec3(0.0f, -cameraMoveSpeed * dt, 0.0f));

	if (isKeyDown(GLFW_KEY_UP))
		camera.rotateGlobal(gal::Rotation(camera.getRight(), cameraRotateSpeed * dt));
	if (isKeyDown(GLFW_KEY_LEFT))
		camera.rotateGlobal(gal::Rotation(glm::vec3(0.0f, 1.0f, 0.0f), cameraRotateSpeed * dt));
	if (isKeyDown(GLFW_KEY_DOWN))
		camera.rotateGlobal(gal::Rotation(camera.getRight(), -cameraRotateSpeed * dt));
	if (isKeyDown(GLFW_KEY_RIGHT))
		camera.rotateGlobal(gal::Rotation(glm::vec3(0.0f, 1.0f, 0.0f), -cameraRotateSpeed * dt));
}

int main()
{
	gal::init();  // Initialize everything GAL needs before calling any other GAL function.
	gal::setOpenGLVersion(4, 5);  // OpenGL version must be set before creating a window.

	gal::Window window = gal::Window(800, 600, "Hello! I'm using GAL!", true);  // Create a window with debug context active.
	window.setClearColor(0.1f, 0.2f, 0.2f, 1.0f);

	// ============ Shader ============

	// GAL tries to make clearer the way OpenGL wants you to think of their objects. OpenGL is really an object-oriented library at heart.
	gal::ShaderProgram shader = gal::ShaderProgram();
	shader.addShaderFromFile("vert.vert", gal::ShaderType::Vertex)
		.addShaderFromFile("frag.frag", gal::ShaderType::Fragment)
		.link();

	shader.use();

	// ============ buffers ============

	gal::VertexArray vao = gal::VertexArray();

	// This funny looking syntax just unpacks the vertex data vector and index vector pair this functions gives us into two variables.
	auto [vertexData, indices] = gal::shapes::generateCubeVertices(6, false, true);  // Generate cube subdivided 6 times, with tex coords.
	
	gal::Buffer vbo = gal::Buffer(gal::BufferType::Array);
	vbo.allocateAndWrite(vertexData, gal::BufferUsageHint::StaticDraw);

	// You have to rely on knowing what attributes you picked to be generated and how they're ordered to do this.
	// See the generateCubeVertices docstring for that information.
	vao.bindVertexBuffer(vbo, 0, 0, sizeof(gal::VertexP3T2));
	vao.newVertexAttribute(0, 0, 3, GL_FLOAT, GL_FALSE, 0);
	vao.newVertexAttribute(1, 0, 2, GL_FLOAT, GL_FALSE, offsetof(gal::VertexP3T2, texCoords));

	gal::Buffer ebo = gal::Buffer(gal::BufferType::ElementArray);
	ebo.allocateAndWrite(indices, gal::BufferUsageHint::StaticDraw);

	vao.bindElementBuffer(ebo, GL_UNSIGNED_INT);

	// setDrawSettings sets the parameters to be passed to glDrawArrays/Elements() so they don't have to be repeated.
	vao.setDrawSettings(GL_TRIANGLES, 0, static_cast<GLsizei>(indices.size()));
	vao.bind();

	// ============ Mesh Instances ============
	
	gal::MeshInstance instances[] =
	{
		{ vao, gal::Transform({ 0.0f, 1.8f, -1.0f }, gal::Rotation(glm::vec3(1.0f, 0.3f, 0.0f)), { 0.5f, 0.3f, 0.5f })},
		{ vao, gal::Transform({ -1.0f, 1.8f, -2.0f }, gal::Rotation(glm::vec3(1.0f, 0.3f, 2.0f)), { 0.3f, 0.5f, 0.3f }) },
		{ vao, gal::Transform({ 1.0f, 1.0f, -4.0f }, gal::Rotation(glm::vec3(0.2f, 1.2f, 0.4f)), { 1.2f, 1.2f, 1.2f }) },
		{ vao, gal::Transform({ 0.0f, 0.0f, -1.0f }) }
	};

	// ============ Texture ============

	gal::Texture tex = gal::Texture(gal::TextureType::TwoD);

	// TODO: Func for setting texture wrap and similar settings for all dimensions at the same time.
	tex.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tex.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tex.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;

	unsigned char* data = stbi_load("resources/noelle.png", &width, &height, &channels, 0);

	tex.imageAB(0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, data, width, height);
	stbi_image_free(data);
	
	tex.generateMipmapAB();
	tex.bindTextureUnit(0);

	// ============ Uniforms ============

	shader.setUniform("texture1", 0);
	shader.setUniform("hue", glm::vec4(0.4f, 0.7f, 0.7f, 1.0f));
	shader.setUniform("hueStrength", 0.2f);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	shader.setUniform("projection", projection);

	while (!window.shouldClose())
	{
		window.pollEvents();   // Poll for window events (keypresses, window resizing, etc.).
		window.updateState();  // Update internal frame-by-frame state.
							   // Your update loop should start with these two function calls.
		processInput(window);

		window.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader.setUniform("view", camera.getViewMatrix());

		for (auto& instance : instances)
			instance.drawNB(shader, "model");

		vao.drawNB();

		window.swapBuffers();  // Swap buffers. your update loop should end with this.
	}

	gal::terminate();
	return 0;
}