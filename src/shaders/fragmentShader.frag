#version 460 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D outTexture;
  void main()
  {
    vec4 texColor = texture(outTexture, TexCoord);
    FragColor = texColor * vec4(ourColor, 1.0); // Multiply texture color by ourColor  
  }
