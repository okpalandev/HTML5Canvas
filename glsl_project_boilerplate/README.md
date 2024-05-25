To set up GLSL (OpenGL Shading Language) development on WSL (Windows Subsystem for Linux), follow these organized steps:

### Requirements
1. **Windows Subsystem for Linux (WSL) installed**
2. **A Linux distribution installed via WSL**
3. **X server for Windows installed (e.g., VcXsrv)**
4. **Development tools (compilers, libraries) installed on your Linux distribution**

### Steps to Set Up GLSL on WSL

#### 1. Install and Configure WSL
- **Install WSL**:
  Open PowerShell as Administrator and run:
  ```powershell
  wsl --install
  ```
  This will install the default Linux distribution (Ubuntu) and the necessary WSL components.

- **Restart your computer** if prompted.

#### 2. Install a Linux Distribution
- **Open the Microsoft Store**, search for a Linux distribution (e.g., Ubuntu), and install it.
- **Launch the installed Linux distribution** and complete the setup by creating a user account.

#### 3. Install and Configure X Server for Windows
- **Download and install VcXsrv** from the official site: [VcXsrv](https://sourceforge.net/projects/vcxsrv/).
- **Configure VcXsrv**:
  - Start VcXsrv (XLaunch) and select "Multiple windows".
  - Select "Start no client".
  - Check the "Disable access control" option.
  - Finish the setup to start the X server.

#### 4. Set Up the Development Environment on WSL
- **Update and upgrade packages**:
  ```bash
  sudo apt update && sudo apt upgrade -y
  ```

- **Install necessary packages**:
  ```bash
  sudo apt install build-essential cmake git libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libgl1-mesa-dev libglew-dev libglfw3-dev libglfw3 libglfw3-dev libglew-dev libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev
  ``` 

- **preresequites**
```bash
sudo apt update
sudo apt upgrade
sudo apt install xserver-xorg-video-intel # For Intel GPUs
sudo apt install xserver-xorg-video-nouveau # For NVIDIA GPUs
sudo apt install xserver-xorg-video-amdgpu # For AMD GPUs 
sudo apt install mesa-utils
```

- **Verify Installation:**
Check that the GLEW header files and libraries are installed correctly:
```bash
ls /usr/include/GL/glew.h
ls /usr/lib/x86_64-linux-gnu/libGLEW.so
```
- **Set DISPLAY environment variable**:
  ```bash
  echo "export DISPLAY=$(grep -oP '(?<=nameserver ).+' /etc/resolv.conf):0.0" >> ~/.bashrc
  source ~/.bashrc
  ```

Verify the DISPLAY variable is set correctly:s

```bash
echo $DISPLAY
```

#### 5. Install an IDE or Text Editor
You can use a variety of text editors or IDEs. Here are a few options:

- **Visual Studio Code** (with Remote - WSL extension):
  ```bash
  sudo apt install code
  ```

- **Vim**:
  ```bash
  sudo apt install vim
  ```

- **Emacs**:
  ```bash
  sudo apt install emacs
  ```

#### 6. Write and Compile a GLSL Program
- **Create a directory for your project**:
  ```bash
  mkdir ~/glsl_project
  cd ~/glsl_project
  ```

- **Create a simple GLSL shader file** (`shader.frag`):
  ```glsl
  #version 330 core
  out vec4 FragColor;

  void main()
  {
      FragColor = vec4(1.0, 0.5, 0.2, 1.0);
  }
  ```

- **Compile and link your shaders with an OpenGL application**. Create a simple OpenGL application (e.g., using C++ and GLFW):
  ```cpp
  // main.cpp
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
  #include <iostream>

  const char* vertexShaderSource = "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos, 1.0);\n"
      "}\0";

  const char* fragmentShaderSource = "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\0";

  int main() {
      // Initialize GLFW
      if (!glfwInit()) {
          std::cerr << "Failed to initialize GLFW\n";
          return -1;
      }

      // Create a GLFW window
      GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
      if (!window) {
          std::cerr << "Failed to create GLFW window\n";
          glfwTerminate();
          return -1;
      }

      glfwMakeContextCurrent(window);

      // Initialize GLEW
      if (glewInit() != GLEW_OK) {
          std::cerr << "Failed to initialize GLEW\n";
          return -1;
      }

      // Build and compile our shader program
      // Vertex shader
      unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
      glCompileShader(vertexShader);
      // Check for shader compile errors
      int success;
      char infoLog[512];
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
      if (!success) {
          glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
          std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
      }

      // Fragment shader
      unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
      glCompileShader(fragmentShader);
      // Check for shader compile errors
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success) {
          glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
          std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
      }

      // Link shaders
      unsigned int shaderProgram = glCreateProgram();
      glAttachShader(shaderProgram, vertexShader);
      glAttachShader(shaderProgram, fragmentShader);
      glLinkProgram(shaderProgram);
      // Check for linking errors
      glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
      if (!success) {
          glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
          std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
      }
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      // Set up vertex data (and buffer(s)) and configure vertex attributes
      float vertices[] = {
          -0.5f, -0.5f, 0.0f,
           0.5f, -0.5f, 0.0f,
           0.0f,  0.5f, 0.0f
      };

      unsigned int VBO, VAO;
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glBindVertexArray(0);

      // Render loop
      while (!glfwWindowShouldClose(window)) {
          // Input
          if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
              glfwSetWindowShouldClose(window, true);

          // Render
          glClear(GL_COLOR_BUFFER_BIT);

          // Draw the triangle
          glUseProgram(shaderProgram);
          glBindVertexArray(VAO);
          glDrawArrays(GL_TRIANGLES, 0, 3);

          // Swap buffers and poll IO events
          glfwSwapBuffers(window);
          glfwPollEvents();
      }

      // Clean up
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);

      glfwTerminate();
      return 0;
  }
  ```
## Restart the terminal and run the following commands to compile and run the program:
- **Compile and run the program**:
  ```bash
  g++ -o myapp main.cpp -lglfw -lGL -lGLEW
  ```