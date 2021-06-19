#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "shader_m.h"
#include <iostream>
#define MAX 500

//#include "cube_m.h"

using namespace std;

//4860
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float vertices[] = {
	//CARA ROJA
	//LADO TIPO A
	//A: MEDIO 5
	-0.15f, -0.15f, -0.45f,  0.0f, 0.0f,    //A 0
	0.15f, -0.15f, -0.45f,  1.0f, 0.0f,    //B 1
	0.15f,  0.15f, -0.45f,  1.0f, 1.0f,    //D 2
	0.15f,  0.15f, -0.45f,  1.0f, 1.0f,    //D 3
	-0.15f,  0.15f, -0.45f,  0.0f, 1.0f,    //C 4
	-0.15f, -0.15f, -0.45f,  0.0f, 0.0f,    //A 5
		//LADO TIPO B
		//B1: ABAJO DEL MEDIO 8
		-0.15f, -0.15f, -0.45f,  0.0f, 0.0f,    //A 6 
		0.15f, -0.15f, -0.45f,  1.0f, 0.0f,    //B 7
		-0.15f, -0.45f, -0.45f,  0.0f, 1.0f,    //E 8
		-0.15f, -0.45f, -0.45f,  0.0f, 1.0f,    //E 9
		0.15f, -0.45f, -0.45f,  0.0f, 0.0f,    //F 10
		0.15f, -0.15f, -0.45f,  1.0f, 0.0f,    //B 11
		//B2: DERECHA DEL MEDIO 6
		0.15f, -0.15f, -0.45f,  0.0f, 0.0f,    //B 12    
		0.45f, -0.15f, -0.45f,  1.0f, 0.0f,    //H 13
		0.45f,  0.15f, -0.45f,  1.0f, 1.0f,    //G 14
		0.45f,  0.15f, -0.45f,  1.0f, 1.0f,    //G 15
		0.15f,  0.15f, -0.45f,  0.0f, 1.0f,    //D 16
		0.15f, -0.15f, -0.45f,  0.0f, 0.0f,    //B 17
		//B3: IZQUIERDA DEL MEDIO 4
		-0.45f, -0.15f, -0.45f,  0.0f, 0.0f,    //I 18
		-0.15f, -0.15f, -0.45f,  1.0f, 0.0f,    //A 19
		-0.15f,  0.15f, -0.45f,  1.0f, 1.0f,    //C 20
		-0.15f,  0.15f, -0.45f,  1.0f, 1.0f,    //C 21
		-0.45f,  0.15f, -0.45f,  0.0f, 1.0f,    //J 22
		-0.45f, -0.15f, -0.45f,  0.0f, 0.0f,    //I 23
		//B4: ARRIBA DEL MEDIO 2
		-0.15f,  0.15f, -0.45f,  0.0f, 0.0f,    //C 24
		0.15f,  0.15f, -0.45f,  1.0f, 0.0f,    //D 25
		0.15f,  0.45f, -0.45f,  1.0f, 1.0f,    //L 26
		0.15f,  0.45f, -0.45f,  1.0f, 1.0f,    //L 27
		-0.15f,  0.45f, -0.45f,  0.0f, 1.0f,    //K 28
		-0.15f,  0.15f, -0.45f,  0.0f, 0.0f,    //C 29
		//TIPO C
		//C1: ESQUINA SUPERIOR DERECHA 3
		0.15f,  0.15f, -0.45f,  0.0f, 0.0f,    //D 30
		0.45f,  0.15f, -0.45f,  1.0f, 0.0f,    //G 31
		0.45f,  0.45f, -0.45f,  1.0f, 1.0f,    //M 32
		0.45f,  0.45f, -0.45f,  1.0f, 1.0f,    //M 33
		0.15f,  0.45f, -0.45f,  0.0f, 1.0f,    //L 34
		0.15f,  0.15f, -0.45f,  0.0f, 0.0f,    //D 35
		//C2: ESQUINA INFERIOR DERECHA 9
		0.15f, -0.45f, -0.45f,  0.0f, 0.0f,    //F 36
		0.45f, -0.45f, -0.45f,  1.0f, 0.0f,    //N 37
		0.45f, -0.15f, -0.45f,  1.0f, 1.0f,    //H 38
		0.45f, -0.15f, -0.45f,  1.0f, 1.0f,    //H 39
		0.15f, -0.15f, -0.45f,  0.0f, 1.0f,    //B 40
		0.15f, -0.45f, -0.45f,  0.0f, 0.0f,    //F 41
		//C3: ESQUINA INFERIOR IZQUIERDA 7
		-0.45f, -0.45f, -0.45f,  0.0f, 0.0f,    //O 42
		-0.15f, -0.45f, -0.45f,  1.0f, 0.0f,    //E 43
		-0.15f, -0.15f, -0.45f,  1.0f, 1.0f,    //A 44
		-0.15f, -0.15f, -0.45f,  1.0f, 1.0f,    //A 45
		-0.45f, -0.15f, -0.45f,  0.0f, 1.0f,    //I 46
		-0.45f, -0.45f, -0.45f,  0.0f, 0.0f,    //O 47
		//C4: ESQUINA SUPERIOR IZQUIERDA 1
		-0.45f,  0.15f, -0.45f,  0.0f, 0.0f,    //J 48
		-0.15f,  0.15f, -0.45f,  1.0f, 0.0f,    //C 49
		-0.15f,  0.45f, -0.45f,  1.0f, 1.0f,    //K 50
		-0.15f,  0.45f, -0.45f,  1.0f, 1.0f,    //K 51
		-0.45f,  0.45f, -0.45f,  0.0f, 1.0f,    //P 52
		-0.45f,  0.15f, -0.45f,  0.0f, 0.0f,    //J 53
		//CARA NARANJA  
		//LADO TIPO A
		//A_2: MEDIO 5
		-0.15f, -0.15f, 0.45f,  0.0f, 0.0f,    //A2 0 54
		0.15f, -0.15f, 0.45f,  1.0f, 0.0f,    //B2 1 55
		0.15f,  0.15f, 0.45f,  1.0f, 1.0f,    //D2 2 56
		0.15f,  0.15f, 0.45f,  1.0f, 1.0f,    //D2 3 57
		-0.15f,  0.15f, 0.45f,  0.0f, 1.0f,    //C2 4 58
		-0.15f, -0.15f, 0.45f,  0.0f, 0.0f,    //A2 5 59
		//LADO TIPO B
		//B1_2: ABAJO DEL MEDIO 8
		-0.15f, -0.15f, 0.45f,  0.0f, 0.0f,    //A2 6 60
		0.15f, -0.15f, 0.45f,  1.0f, 0.0f,    //B2 7 61
		-0.15f, -0.45f, 0.45f,  0.0f, 1.0f,    //E2 8 62
		-0.15f, -0.45f, 0.45f,  0.0f, 1.0f,    //E2 9 63
		0.15f, -0.45f, 0.45f,  0.0f, 0.0f,    //F2 10 64
		0.15f, -0.15f, 0.45f,  1.0f, 0.0f,    //B2 11 65
		//B2_2: DERECHA DEL MEDIO 6
		0.15f, -0.15f, 0.45f,  0.0f, 0.0f,    //B2 12 66
		0.45f, -0.15f, 0.45f,  1.0f, 0.0f,    //H2 13 67
		0.45f,  0.15f, 0.45f,  1.0f, 1.0f,    //G2 14 68
		0.45f,  0.15f, 0.45f,  1.0f, 1.0f,    //G2 15 69
		0.15f,  0.15f, 0.45f,  0.0f, 1.0f,    //D2 16 70
		0.15f, -0.15f, 0.45f,  0.0f, 0.0f,    //B2 17 71
		//B3_2: IZQUIERDA DEL MEDIO 4
		-0.45f, -0.15f, 0.45f,  0.0f, 0.0f,    //I2 18 72
		-0.15f, -0.15f, 0.45f,  1.0f, 0.0f,    //A2 19 73
		-0.15f,  0.15f, 0.45f,  1.0f, 1.0f,    //C2 20 74
		-0.15f,  0.15f, 0.45f,  1.0f, 1.0f,    //C2 21 75
		-0.45f,  0.15f, 0.45f,  0.0f, 1.0f,    //J2 22 76
		-0.45f, -0.15f, 0.45f,  0.0f, 0.0f,    //I2 23 77
		//B4_2: ARRIBA DEL MEDIO 2
		-0.15f,  0.15f, 0.45f,  0.0f, 0.0f,    //C2 24 78
		0.15f,  0.15f, 0.45f,  1.0f, 0.0f,    //D2 25 79
		0.15f,  0.45f, 0.45f,  1.0f, 1.0f,    //L2 26 80
		0.15f,  0.45f, 0.45f,  1.0f, 1.0f,    //L2 27 81
		-0.15f,  0.45f, 0.45f,  0.0f, 1.0f,    //K2 28 82
		-0.15f,  0.15f, 0.45f,  0.0f, 0.0f,    //C2 29 83
		//TIPO C
		//C1_2: ESQUINA SUPERIOR DERECHA 3
		0.15f,  0.15f, 0.45f,  0.0f, 0.0f,    //D2 30 84
		0.45f,  0.15f, 0.45f,  1.0f, 0.0f,    //G2 31 85
		0.45f,  0.45f, 0.45f,  1.0f, 1.0f,    //M2 32 86
		0.45f,  0.45f, 0.45f,  1.0f, 1.0f,    //M2 33 87
		0.15f,  0.45f, 0.45f,  0.0f, 1.0f,    //L2 34 88
		0.15f,  0.15f, 0.45f,  0.0f, 0.0f,    //D2 35 89

		//C2_2: ESQUINA INFERIOR DERECHA 9
		0.15f, -0.45f, 0.45f,  0.0f, 0.0f,    //F2 36 90
		0.45f, -0.45f, 0.45f,  1.0f, 0.0f,    //N2 37 91
		0.45f, -0.15f, 0.45f,  1.0f, 1.0f,    //H2 38 92
		0.45f, -0.15f, 0.45f,  1.0f, 1.0f,    //H2 39 93
		0.15f, -0.15f, 0.45f,  0.0f, 1.0f,    //B2 40 94
		0.15f, -0.45f, 0.45f,  0.0f, 0.0f,    //F2 41 95
		//C3_2: ESQUINA INFERIOR IZQUIERDA 7
		-0.45f, -0.45f, 0.45f,  0.0f, 0.0f,    //O2 42 96
		-0.15f, -0.45f, 0.45f,  1.0f, 0.0f,    //E2 43
		-0.15f, -0.15f, 0.45f,  1.0f, 1.0f,    //A2 44
		-0.15f, -0.15f, 0.45f,  1.0f, 1.0f,    //A2 45
		-0.45f, -0.15f, 0.45f,  0.0f, 1.0f,    //I2 46
		-0.45f, -0.45f, 0.45f,  0.0f, 0.0f,    //O2 47
		//C4_2: ESQUINA SUPERIOR IZQUIERDA 1
		-0.45f,  0.15f, 0.45f,  0.0f, 0.0f,    //J2 48 102
		-0.15f,  0.15f, 0.45f,  1.0f, 0.0f,    //C2 49
		-0.15f,  0.45f, 0.45f,  1.0f, 1.0f,    //K2 50
		-0.15f,  0.45f, 0.45f,  1.0f, 1.0f,    //K2 51
		-0.45f,  0.45f, 0.45f,  0.0f, 1.0f,    //P2 52
		-0.45f,  0.15f, 0.45f,  0.0f, 0.0f,    //J2 53
		//CARA VERDE
		//TIPO A
		//A_3: MEDIO 5
		-0.45f,  0.15f,  0.15f, 1.0f, 0.0f,    //A3 108
		-0.45f,  0.15f, -0.15f, 1.0f, 1.0f,    //B3 109
		-0.45f, -0.15f, -0.15f, 0.0f, 1.0f,    //C3 110
		-0.45f, -0.15f, -0.15f, 0.0f, 1.0f,    //C3 111
		-0.45f, -0.15f,  0.15f, 0.0f, 0.0f,    //D3 112
		-0.45f,  0.15f,  0.15f, 1.0f, 0.0f,    //A3 113
		//TIPO B
		//B1_3: ABAJO DEL MEDIO 8
		-0.45f, -0.15f,  0.15f, 1.0f, 0.0f,    //D3 114
		-0.45f, -0.15f, -0.15f, 1.0f, 1.0f,    //C3 115
		-0.45f, -0.45f, -0.15f, 0.0f, 1.0f,    //E3 116
		-0.45f, -0.45f, -0.15f, 0.0f, 1.0f,    //E3 117
		-0.45f, -0.45f,  0.15f, 0.0f, 0.0f,    //F3 118
		-0.45f, -0.15f,  0.15f, 1.0f, 0.0f,    //D3 119
		//B2_3: ARRIBA DEL MEDIO 2
		-0.45f,  0.45f,  0.15f, 1.0f, 0.0f,    //G3 120
		-0.45f,  0.45f, -0.15f, 1.0f, 1.0f,    //H3 121
		-0.45f,  0.15f, -0.15f, 0.0f, 1.0f,    //B3 122
		-0.45f,  0.15f, -0.15f, 0.0f, 1.0f,    //B3 123
		-0.45f,  0.15f,  0.15f, 0.0f, 0.0f,    //A3 124
		-0.45f,  0.45f,  0.15f, 1.0f, 0.0f,    //G3 125
		//B3_3: DERECHA DEL MEDIO 6
		-0.45f,  0.15f, -0.15f, 1.0f, 0.0f,    //B3 126
		-0.45f,  0.15f, -0.45f, 1.0f, 1.0f,    //I3 127
		-0.45f, -0.15f, -0.45f, 0.0f, 1.0f,    //J3 128
		-0.45f, -0.15f, -0.45f, 0.0f, 1.0f,    //J3 129
		-0.45f, -0.15f, -0.15f, 0.0f, 0.0f,    //C3 130
		-0.45f, 0.15f, -0.15f, 1.0f, 0.0f,    //B3 131
		//B4_3: IZQUIERDA DEL MEDIO 4
		-0.45f, 0.15f, 0.45f, 1.0f, 0.0f,   //K3 132
		-0.45f, 0.15f, 0.15f, 1.0f, 1.0f,   //A3 133
		-0.45f, -0.15f, 0.15f, 0.0f, 1.0f,  //D3 134
		-0.45f, -0.15f, 0.15f, 0.0f, 1.0f,  //D3 135
		-0.45f, -0.15f, 0.45f, 0.0f, 0.0f,  //L3 136
		-0.45f, 0.15f, 0.45f, 1.0f, 0.0f,   //K3 137
		//TIPO C
		//C1_3: ESQUINA SUPERIOR IZQUIERDA
		-0.45f, 0.45f, 0.45f, 1.0f, 0.0f,   //M3 138
		-0.45f, 0.45f, 0.15f, 1.0f, 1.0f,   //G3 139
		-0.45f, 0.15f, 0.15f, 0.0f, 1.0f,   //A3 140
		-0.45f, 0.15f, 0.15f, 0.0f, 1.0f,   //A3 141
		-0.45f, 0.15f, 0.45f, 0.0f, 0.0f,   //K3 142
		-0.45f, 0.45f, 0.45f, 1.0f, 0.0f,   //M3 143
		//C2_3: ESQUINA SUPERIOR DERECHA
		-0.45f, 0.45f, -0.15f, 1.0f, 0.0f,  //H3 144
		-0.45f, 0.45f, -0.45f, 1.0f, 1.0f,  //N3 145
		-0.45f, 0.15f, -0.45f, 0.0f, 1.0f,  //I3 146
		-0.45f, 0.15f, -0.45f, 0.0f, 1.0f,  //I3 147
		-0.45f, 0.15f, -0.15f, 0.0f, 0.0f,  //B3 148
		-0.45f, 0.45f, -0.15f, 1.0f, 0.0f,  //H3 149
		//C3_3: ESQUINA INFERIOR DERECHA
		-0.45f, -0.15f, -0.15f, 1.0f, 0.0f, //C3 150
		-0.45f, -0.15f, -0.45f, 1.0f, 1.0f, //J3 151
		-0.45f, -0.45f, -0.45f, 0.0f, 1.0f, //O3 152
		-0.45f, -0.45f, -0.45f, 0.0f, 1.0f, //O3 153
		-0.45f, -0.45f, -0.15f, 0.0f, 0.0f, //E3 154
		-0.45f, -0.15f, -0.15f, 1.0f, 0.0f, //C3 155
		//C4_3: ESQUINA INFERIOR IZQUIERDA
		-0.45f, -0.15f, 0.45f, 1.0f, 0.0f,  //L3 156
		-0.45f, -0.15f, 0.15f, 1.0f, 1.0f,  //D3 157
		-0.45f, -0.45f, 0.15f, 0.0f, 1.0f,  //F3 158
		-0.45f, -0.45f, 0.15f, 0.0f, 1.0f,  //F3 159
		-0.45f, -0.45f, 0.45f, 0.0f, 0.0f,  //P3 160
		-0.45f, -0.15f, 0.45f, 1.0f, 0.0f,  //L3 161
		//CARA AZUL
		//TIPO A
		//A_4: MEDIO 5
		0.45f, 0.15f, 0.15f, 1.0f, 0.0f,    //A3 108 162
		0.45f, 0.15f, -0.15f, 1.0f, 1.0f,    //B3 109 163
		0.45f, -0.15f, -0.15f, 0.0f, 1.0f,    //C3 110 164
		0.45f, -0.15f, -0.15f, 0.0f, 1.0f,    //C3 111 165
		0.45f, -0.15f, 0.15f, 0.0f, 0.0f,    //D3 112 166
		0.45f, 0.15f, 0.15f, 1.0f, 0.0f,    //A3 113 167
		//TIPO B
		//B1_4: ABAJO DEL MEDIO 8
		0.45f, -0.15f, 0.15f, 1.0f, 0.0f,    //D3 114 168
		0.45f, -0.15f, -0.15f, 1.0f, 1.0f,    //C3 115 169
		0.45f, -0.45f, -0.15f, 0.0f, 1.0f,    //E3 116 170
		0.45f, -0.45f, -0.15f, 0.0f, 1.0f,    //E3 117 171
		0.45f, -0.45f, 0.15f, 0.0f, 0.0f,    //F3 118 172
		0.45f, -0.15f, 0.15f, 1.0f, 0.0f,    //D3 119 173
		//B2_4: ARRIBA DEL MEDIO 2
		0.45f, 0.45f, 0.15f, 1.0f, 0.0f,    //G3 120 174
		0.45f, 0.45f, -0.15f, 1.0f, 1.0f,    //H3 121
		0.45f, 0.15f, -0.15f, 0.0f, 1.0f,    //B3 122
		0.45f, 0.15f, -0.15f, 0.0f, 1.0f,    //B3 123
		0.45f, 0.15f, 0.15f, 0.0f, 0.0f,    //A3 124
		0.45f, 0.45f, 0.15f, 1.0f, 0.0f,    //G3 125
		//B3_4: DERECHA DEL MEDIO 6
		0.45f, 0.15f, -0.15f, 1.0f, 0.0f,    //B3 126 180
		0.45f, 0.15f, -0.45f, 1.0f, 1.0f,    //I3 127
		0.45f, -0.15f, -0.45f, 0.0f, 1.0f,    //J3 128
		0.45f, -0.15f, -0.45f, 0.0f, 1.0f,    //J3 129
		0.45f, -0.15f, -0.15f, 0.0f, 0.0f,    //C3 130
		0.45f, 0.15f, -0.15f, 1.0f, 0.0f,    //B3 131
		//B4_4: IZQUIERDA DEL MEDIO 4
		0.45f, 0.15f, 0.45f, 1.0f, 0.0f,   //K3 132 186
		0.45f, 0.15f, 0.15f, 1.0f, 1.0f,   //A3 133
		0.45f, -0.15f, 0.15f, 0.0f, 1.0f,  //D3 134
		0.45f, -0.15f, 0.15f, 0.0f, 1.0f,  //D3 135
		0.45f, -0.15f, 0.45f, 0.0f, 0.0f,  //L3 136
		0.45f, 0.15f, 0.45f, 1.0f, 0.0f,   //K3 137
		//TIPO C
		//C1_4: ESQUINA SUPERIOR IZQUIERDA
		0.45f, 0.45f, 0.45f, 1.0f, 0.0f,   //M3 138 192
		0.45f, 0.45f, 0.15f, 1.0f, 1.0f,   //G3 139
		0.45f, 0.15f, 0.15f, 0.0f, 1.0f,   //A3 140
		0.45f, 0.15f, 0.15f, 0.0f, 1.0f,   //A3 141
		0.45f, 0.15f, 0.45f, 0.0f, 0.0f,   //K3 142
		0.45f, 0.45f, 0.45f, 1.0f, 0.0f,   //M3 143
		//C2_4: ESQUINA SUPERIOR DERECHA
		0.45f, 0.45f, -0.15f, 1.0f, 0.0f,  //H3 144 198
		0.45f, 0.45f, -0.45f, 1.0f, 1.0f,  //N3 145 199
		0.45f, 0.15f, -0.45f, 0.0f, 1.0f,  //I3 146 200
		0.45f, 0.15f, -0.45f, 0.0f, 1.0f,  //I3 147 201
		0.45f, 0.15f, -0.15f, 0.0f, 0.0f,  //B3 148 202
		0.45f, 0.45f, -0.15f, 1.0f, 0.0f,  //H3 149 203
		//C3_4: ESQUINA INFERIOR DERECHA
		0.45f, -0.15f, -0.15f, 1.0f, 0.0f, //C3 150 204
		0.45f, -0.15f, -0.45f, 1.0f, 1.0f, //J3 151
		0.45f, -0.45f, -0.45f, 0.0f, 1.0f, //O3 152
		0.45f, -0.45f, -0.45f, 0.0f, 1.0f, //O3 153
		0.45f, -0.45f, -0.15f, 0.0f, 0.0f, //E3 154
		0.45f, -0.15f, -0.15f, 1.0f, 0.0f, //C3 155
		//C4_4: ESQUINA INFERIOR IZQUIERDA
		0.45f, -0.15f, 0.45f, 1.0f, 0.0f,  //L3 156 210
		0.45f, -0.15f, 0.15f, 1.0f, 1.0f,  //D3 157
		0.45f, -0.45f, 0.15f, 0.0f, 1.0f,  //F3 158
		0.45f, -0.45f, 0.15f, 0.0f, 1.0f,  //F3 159
		0.45f, -0.45f, 0.45f, 0.0f, 0.0f,  //P3 160
		0.45f, -0.15f, 0.45f, 1.0f, 0.0f,  //L3 161
		//CARA AMARILLA
		//TIPO A
		//A_5: MEDIO 5
		-0.15f, -0.45f, -0.15f,  0.0f, 1.0f,    //A4 216
		0.15f, -0.45f, -0.15f,  1.0f, 1.0f,    //B4 217
		0.15f, -0.45f,  0.15f,  1.0f, 0.0f,    //C4 218
		0.15f, -0.45f,  0.15f,  1.0f, 0.0f,    //C4 219
		-0.15f, -0.45f,  0.15f,  0.0f, 0.0f,    //D4 220
		-0.15f, -0.45f, -0.15f,  0.0f, 1.0f,    //A4 221
		//TIPO B
		//B1_5: IZQUIERDA DEL MEDIO 4
		-0.45f, -0.45f, -0.15f, 0.0f, 1.0f, //E4 222
		-0.15f, -0.45f, -0.15f, 1.0f, 1.0f, //A4 223
		-0.15f, -0.45f, 0.15f, 1.0f, 0.0f,  //D4 224
		-0.15f, -0.45f, 0.15f, 1.0f, 0.0f,  //D4 225
		-0.45f, -0.45f, 0.15f, 0.0f, 0.0f,  //F4 226
		-0.45f, -0.45f, -0.15f, 0.0f, 1.0f, //E4 227
		//B2_5: DERECHA DEL MEDIO 6
		0.15f, -0.45f, -0.15f, 0.0f, 1.0f,    //B4 228
		0.45f, -0.45f, -0.15f, 1.0f, 1.0f,    //G4 229
		0.45f, -0.45f,  0.15f, 1.0f, 0.0f,    //H4 230
		0.45f, -0.45f, 0.15f, 1.0f, 0.0f,    //H4 231
		0.15f, -0.45f, 0.15f, 0.0f, 0.0f,    //C4 232
		0.15f, -0.45f, -0.15f, 0.0f, 1.0f,    //B4 233  
		//B3_5: ABAJO DEL MEDIO 8
		-0.15f, -0.45f, 0.15f, 0.0f, 1.0f,  //D4 234
		0.15f, -0.45f, 0.15f, 1.0f, 1.0f,    //C4 235
		0.15f, -0.45f, 0.45f, 1.0f, 0.0f,    //J4 236
		0.15f, -0.45f, 0.45f, 1.0f, 0.0f,    //J4 237
		-0.15f, -0.45f, 0.45f, 0.0f, 0.0f,  //I4 238
		-0.15f, -0.45f, 0.15f, 0.0f, 1.0f,  //D4 239
		//B4_5: ARRIBA DEL MEDIO 2
		-0.15f, -0.45f, -0.45f, 0.0f, 1.0f,    //K4 240
		0.15f, -0.45f, -0.45f, 1.0f, 1.0f,    //L4 241
		0.15f, -0.45f, -0.15f, 1.0f, 0.0f,    //B4 242
		0.15f, -0.45f, -0.15f, 1.0f, 0.0f,    //B4 243
		-0.15f, -0.45f, -0.15f, 0.0f, 0.0f,    //A4 244
		-0.15f, -0.45f, -0.45f, 0.0f, 1.0f,    //K4 245
		//TIPO C
		//C1_5: ESQUINA SUPERIOR DERECHA 3
		0.15f, -0.45f, -0.45f, 0.0f, 1.0f,    //L4 246 
		0.45f, -0.45f, -0.45f, 1.0f, 1.0f,    //M4 247
		0.45f, -0.45f, -0.15f, 1.0f, 0.0f,    //G4 248
		0.45f, -0.45f, -0.15f, 1.0f, 0.0f,    //G4 249
		0.15f, -0.45f, -0.15f, 0.0f, 0.0f,    //B4 250
		0.15f, -0.45f, -0.45f, 0.0f, 1.0f,    //L4 251  
		//C2_5: ESQUINA INFERIOR DERECHA 9
		0.15f, -0.45f, 0.15f, 0.0f, 1.0f,    //C4 252
		0.45f, -0.45f, 0.15f, 1.0f, 1.0f,    //H4 253
		0.45f, -0.45f, 0.45f, 1.0f, 0.0f,    //N4 254
		0.45f, -0.45f, 0.45f, 1.0f, 0.0f,    //N4 255
		0.15f, -0.45f, 0.45f, 0.0f, 0.0f,    //J4 256
		0.15f, -0.45f, 0.15f, 0.0f, 1.0f,    //C4 257
		//C3_5: ESQUINA INFERIOR IZQUIERDA 7
		-0.45f, -0.45f, 0.15f, 0.0f, 1.0f,  //F4 258
		-0.15f, -0.45f, 0.15f, 1.0f, 1.0f,  //D4 259
		-0.15f, -0.45f, 0.45f, 1.0f, 0.0f,  //I4 260
		-0.15f, -0.45f, 0.45f, 1.0f, 0.0f,  //I4 261
		-0.45f, -0.45f, 0.45f, 0.0f, 0.0f,  //P4 262
		-0.45f, -0.45f, 0.15f, 0.0f, 1.0f,  //F4 263
		//C4_5: ESQUINA SUPERIOR IZQUIERDA 1
		-0.45f, -0.45f, -0.45f, 0.0f, 1.0f,    //O4 264
		-0.15f, -0.45f, -0.45f, 1.0f, 1.0f,    //K4 265
		-0.15f, -0.45f, -0.15f, 1.0f, 0.0f,    //A4 266
		-0.15f, -0.45f, -0.15f, 1.0f, 0.0f,    //A4 267
		-0.45f, -0.45f, -0.15f, 0.0f, 0.0f,    //E4 268
		-0.45f, -0.45f, -0.45f, 0.0f, 1.0f,    //O4 269
		//CARA BLANCA
		//TIPO A
		//A_6: MEDIO 5
		-0.15f,  0.45f, -0.15f,  0.0f, 1.0f, // 270
		0.15f,  0.45f, -0.15f,  1.0f, 1.0f, // 271
		0.15f,  0.45f,  0.15f,  1.0f, 0.0f, // 272
		0.15f,  0.45f,  0.15f,  1.0f, 0.0f, // 273
		-0.15f,  0.45f,  0.15f,  0.0f, 0.0f, // 274
		-0.15f,  0.45f, -0.15f,  0.0f, 1.0f, // 275
		//TIPO B
		//B1_6: IZQUIERDA DEL MEDIO 4
		-0.45f, 0.45f, -0.15f, 0.0f, 1.0f, //E4 222 276 
		-0.15f, 0.45f, -0.15f, 1.0f, 1.0f, //A4 223 277
		-0.15f, 0.45f, 0.15f, 1.0f, 0.0f,  //D4 224 278
		-0.15f, 0.45f, 0.15f, 1.0f, 0.0f,  //D4 225 279
		-0.45f, 0.45f, 0.15f, 0.0f, 0.0f,  //F4 226 280
		-0.45f, 0.45f, -0.15f, 0.0f, 1.0f, //E4 227 281
		//B2_6: DERECHA DEL MEDIO 6
		0.15f, 0.45f, -0.15f, 0.0f, 1.0f,    //B4 228 282
		0.45f, 0.45f, -0.15f, 1.0f, 1.0f,    //G4 229 283
		0.45f, 0.45f, 0.15f, 1.0f, 0.0f,    //H4 230 284
		0.45f, 0.45f, 0.15f, 1.0f, 0.0f,    //H4 231 285
		0.15f, 0.45f, 0.15f, 0.0f, 0.0f,    //C4 232 286
		0.15f, 0.45f, -0.15f, 0.0f, 1.0f,    //B4 233 287
		//B3_6: ABAJO DEL MEDIO 8
		-0.15f, 0.45f, 0.15f, 0.0f, 1.0f,  //D4 234 288
		0.15f, 0.45f, 0.15f, 1.0f, 1.0f,    //C4 235 289
		0.15f, 0.45f, 0.45f, 1.0f, 0.0f,    //J4 236 290
		0.15f, 0.45f, 0.45f, 1.0f, 0.0f,    //J4 237 291
		-0.15f, 0.45f, 0.45f, 0.0f, 0.0f,  //I4 238 292
		-0.15f, 0.45f, 0.15f, 0.0f, 1.0f,  //D4 239 293
		//B4_6: ARRIBA DEL MEDIO 2
		-0.15f, 0.45f, -0.45f, 0.0f, 1.0f,    //K4 240 294
		0.15f, 0.45f, -0.45f, 1.0f, 1.0f,    //L4 241 295
		0.15f, 0.45f, -0.15f, 1.0f, 0.0f,    //B4 242 296
		0.15f, 0.45f, -0.15f, 1.0f, 0.0f,    //B4 243 297
		-0.15f, 0.45f, -0.15f, 0.0f, 0.0f,    //A4 244 298
		-0.15f, 0.45f, -0.45f, 0.0f, 1.0f,    //K4 245 299
		//TIPO C
		//C1_6: ESQUINA SUPERIOR DERECHA 3
		0.15f, 0.45f, -0.45f, 0.0f, 1.0f,    //L4 246 300
		0.45f, 0.45f, -0.45f, 1.0f, 1.0f,    //M4 247 301
		0.45f, 0.45f, -0.15f, 1.0f, 0.0f,    //G4 248 302
		0.45f, 0.45f, -0.15f, 1.0f, 0.0f,    //G4 249 303
		0.15f, 0.45f, -0.15f, 0.0f, 0.0f,    //B4 250 304
		0.15f, 0.45f, -0.45f, 0.0f, 1.0f,    //L4 251 305
		//C2_6: ESQUINA INFERIOR DERECHA 9
		0.15f, 0.45f, 0.15f, 0.0f, 1.0f,    //C4 252 306
		0.45f, 0.45f, 0.15f, 1.0f, 1.0f,    //H4 253 307
		0.45f, 0.45f, 0.45f, 1.0f, 0.0f,    //N4 254 308
		0.45f, 0.45f, 0.45f, 1.0f, 0.0f,    //N4 255 309
		0.15f, 0.45f, 0.45f, 0.0f, 0.0f,    //J4 256 310
		0.15f, 0.45f, 0.15f, 0.0f, 1.0f,    //C4 257 311
		//C3_6: ESQUINA INFERIOR IZQUIERDA 7 
		-0.45f, 0.45f, 0.15f, 0.0f, 1.0f,  //F4 258 312
		-0.15f, 0.45f, 0.15f, 1.0f, 1.0f,  //D4 259 313
		-0.15f, 0.45f, 0.45f, 1.0f, 0.0f,  //I4 260 314
		-0.15f, 0.45f, 0.45f, 1.0f, 0.0f,  //I4 261 315
		-0.45f, 0.45f, 0.45f, 0.0f, 0.0f,  //P4 262 316
		-0.45f, 0.45f, 0.15f, 0.0f, 1.0f,  //F4 263 317
		//C4_6  : ESQUINA SUPERIOR IZQUIERDA 1
		-0.45f, 0.45f, -0.45f, 0.0f, 1.0f,    //O4 264 318
		-0.15f, 0.45f, -0.45f, 1.0f, 1.0f,    //K4 265 319
		-0.15f, 0.45f, -0.15f, 1.0f, 0.0f,    //A4 266 320
		-0.15f, 0.45f, -0.15f, 1.0f, 0.0f,    //A4 267 321
		-0.45f, 0.45f, -0.15f, 0.0f, 0.0f,    //E4 268 322
		-0.45f, 0.45f, -0.45f, 0.0f, 1.0f    //O4 269 323
};

std::vector<Shader> shaders_masna;
int indicesL[21] = { 240,90,210,
					510,360,480,
					1320,1110,1290,
					1590,1380,1560,
					540,570,600,630,660,690,720,750,780 };
int indicesM[12] = { 120,0,30,
					390,270,300,
					1200,1080,1170,
					1470,1350,1440 };
int indicesR[21] = { 150,60,180, ///Coords de la Cara Roja: C1, B2, C2
					420,330,450,    ///Coords de la Cara Naranja: C1_2, B2_2, C2_2 
					1230,1140,1260, ///Coords de la Cara Amarilla: C1_5, B2_5, C2_5
					1500,1410,1530, ///Coords de la Cara Blanco: C1_6, B2_6, C2_6
					810,840,870,900,930,960,990,1020,1050 }; ///Coords de la Cara AZUL: INICIO A_3
int indicesU[21] = { 240,120,150,
					510,390,420,
					690,600,720,
					960,870,990,
					1350,1380,1410,1440,1470,1500,1530,1560,1590 };
int indicesM2[12] = { 90,0,60,
					360,270,330,
					660,540,630,
					930,810,900 };
int indicesD[21] = { 210,30,180,
					480,300,450,
					780,570,750,
					1050,840,1020,
					1080,1110,1140,1170,1200,1230,1260,1290,1320 };
int indicesF[21] = { 1590,1470,1500,
					720,630,750,
					1320,1200,1230,
					990,900,1020,
					0,30,60,90,120,150,180,210,240 };
int indicesM3[12] = { 600,540,570,
					870,810,840,
					1110,1080,1140,
					1380,1350,1410 };
int indicesB[21] = { 1560,1440,1530,
					1290,1170,1260,
					960,930,1050,
					690,660,780,
					270,300,330,360,390,420,450,480,510 };
//class
class rubik {
public:
	rubik() {
		std::cout << "Constructor de la clase rubik" << std::endl;
	}
	void mov_U(float sentido, float eje) {
		//std::cout << "MOVIMIENTO R" << std::endl;
		glm::mat4 res = glm::mat4(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		//glm::rotate(model, 0.002f, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(eje * 90.0f), glm::vec3(0.0, sentido, 0.0f));
		//transformation = glm::rotate(transformation, velocidad, glm::vec3(0.0f, 0.0f, sentido));
		int aumento = 0;
		for (int i = 0; i < 21; ++i) {
			for (int j = 0; j < 6; ++j) {
				//std::cout << "INDICE EN " << indicesR[i] + aumento << std::endl;
				glm::vec4 vector((float)vertices[indicesU[i] + aumento], (float)vertices[indicesU[i] + aumento + 1], (float)vertices[indicesU[i] + aumento + 2], 1.0f);
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vector = transformation * vector;
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vertices[indicesU[i] + aumento] = (float)vector.x;
				vertices[indicesU[i] + aumento + 1] = (float)vector.y;
				vertices[indicesU[i] + aumento + 2] = (float)vector.z;
				aumento += 5;
				//std::cout << "ESTO DEBERIA SER IGUAL x: " << vertices[indicesR[i] + aumento] << " y: " << vertices[indicesR[i] + aumento + 1] << " z: " << vertices[indicesR[i] + aumento + 2] << std::endl;
			}
			aumento = 0;
		}
		aumento = 0;
	}
	void mov_M2(float sentido, float eje) {
		//std::cout << "MOVIMIENTO R" << std::endl;
		glm::mat4 res = glm::mat4(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		//glm::rotate(model, 0.002f, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(eje * 90.0f), glm::vec3(0.0f, sentido, 0.0f));
		//transformation = glm::rotate(transformation, velocidad, glm::vec3(0.0f, 0.0f, sentido));
		int aumento = 0;
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j < 6; ++j) {
				//std::cout << "INDICE EN " << indicesR[i] + aumento << std::endl;
				glm::vec4 vector((float)vertices[indicesM2[i] + aumento], (float)vertices[indicesM2[i] + aumento + 1], (float)vertices[indicesM2[i] + aumento + 2], 1.0f);
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vector = transformation * vector;
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vertices[indicesM2[i] + aumento] = (float)vector.x;
				vertices[indicesM2[i] + aumento + 1] = (float)vector.y;
				vertices[indicesM2[i] + aumento + 2] = (float)vector.z;
				aumento += 5;
				//std::cout << "ESTO DEBERIA SER IGUAL x: " << vertices[indicesR[i] + aumento] << " y: " << vertices[indicesR[i] + aumento + 1] << " z: " << vertices[indicesR[i] + aumento + 2] << std::endl;
			}
			aumento = 0;
		}
		aumento = 0;
	}
	void mov_D(float sentido, float eje) {
		//std::cout << "MOVIMIENTO R" << std::endl;
		glm::mat4 res = glm::mat4(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		//glm::rotate(model, 0.002f, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(eje * 90.0f), glm::vec3(0.0, sentido, 0.0f));
		//transformation = glm::rotate(transformation, velocidad, glm::vec3(0.0f, 0.0f, sentido));
		int aumento = 0;
		for (int i = 0; i < 21; ++i) {
			for (int j = 0; j < 6; ++j) {
				//std::cout << "INDICE EN " << indicesR[i] + aumento << std::endl;
				glm::vec4 vector((float)vertices[indicesD[i] + aumento], (float)vertices[indicesD[i] + aumento + 1], (float)vertices[indicesD[i] + aumento + 2], 1.0f);
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vector = transformation * vector;
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vertices[indicesD[i] + aumento] = (float)vector.x;
				vertices[indicesD[i] + aumento + 1] = (float)vector.y;
				vertices[indicesD[i] + aumento + 2] = (float)vector.z;
				aumento += 5;
				//std::cout << "ESTO DEBERIA SER IGUAL x: " << vertices[indicesR[i] + aumento] << " y: " << vertices[indicesR[i] + aumento + 1] << " z: " << vertices[indicesR[i] + aumento + 2] << std::endl;
			}
			aumento = 0;
		}
		aumento = 0;
	}
	void mov_L(float sentido, float eje) {
		//std::cout << "MOVIMIENTO R" << std::endl;
		glm::mat4 res = glm::mat4(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		//glm::rotate(model, 0.002f, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(eje * 90.0f), glm::vec3(sentido, 0.0f, 0.0f));
		//transformation = glm::rotate(transformation, velocidad, glm::vec3(0.0f, 0.0f, sentido));
		int aumento = 0;
		for (int i = 0; i < 21; ++i) {
			for (int j = 0; j < 6; ++j) {
				//std::cout << "INDICE EN " << indicesR[i] + aumento << std::endl;
				glm::vec4 vector((float)vertices[indicesL[i] + aumento], (float)vertices[indicesL[i] + aumento + 1], (float)vertices[indicesL[i] + aumento + 2], 1.0f);
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vector = transformation * vector;
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vertices[indicesL[i] + aumento] = (float)vector.x;
				vertices[indicesL[i] + aumento + 1] = (float)vector.y;
				vertices[indicesL[i] + aumento + 2] = (float)vector.z;
				aumento += 5;
				//std::cout << "ESTO DEBERIA SER IGUAL x: " << vertices[indicesR[i] + aumento] << " y: " << vertices[indicesR[i] + aumento + 1] << " z: " << vertices[indicesR[i] + aumento + 2] << std::endl;
			}
			aumento = 0;
		}
		aumento = 0;
	}
	void mov_M(float sentido, float eje) {
		//std::cout << "MOVIMIENTO R" << std::endl;
		glm::mat4 res = glm::mat4(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		//glm::rotate(model, 0.002f, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(eje * 90.0f), glm::vec3(sentido, 0.0f, 0.0f));
		//transformation = glm::rotate(transformation, velocidad, glm::vec3(0.0f, 0.0f, sentido));
		int aumento = 0;
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j < 6; ++j) {
				//std::cout << "INDICE EN " << indicesR[i] + aumento << std::endl;
				glm::vec4 vector((float)vertices[indicesM[i] + aumento], (float)vertices[indicesM[i] + aumento + 1], (float)vertices[indicesM[i] + aumento + 2], 1.0f);
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vector = transformation * vector;
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vertices[indicesM[i] + aumento] = (float)vector.x;
				vertices[indicesM[i] + aumento + 1] = (float)vector.y;
				vertices[indicesM[i] + aumento + 2] = (float)vector.z;
				aumento += 5;
				//std::cout << "ESTO DEBERIA SER IGUAL x: " << vertices[indicesR[i] + aumento] << " y: " << vertices[indicesR[i] + aumento + 1] << " z: " << vertices[indicesR[i] + aumento + 2] << std::endl;
			}
			aumento = 0;
		}
		aumento = 0;
	}
	void mov_R(float sentido, float eje) {
		//std::cout << "MOVIMIENTO R" << std::endl;
		glm::mat4 res = glm::mat4(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		//glm::rotate(model, 0.002f, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(eje * 90.0f), glm::vec3(sentido, 0.0f, 0.0f));
		//transformation = glm::rotate(transformation, velocidad, glm::vec3(0.0f, 0.0f, sentido));
		int aumento = 0;
		for (int i = 0; i < 21; ++i) {
			for (int j = 0; j < 6; ++j) {
				//std::cout << "INDICE EN " << indicesR[i] + aumento << std::endl;
				glm::vec4 vector((float)vertices[indicesR[i] + aumento], (float)vertices[indicesR[i] + aumento + 1], (float)vertices[indicesR[i] + aumento + 2], 1.0f);
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vector = transformation * vector;
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vertices[indicesR[i] + aumento] = (float)vector.x;
				vertices[indicesR[i] + aumento + 1] = (float)vector.y;
				vertices[indicesR[i] + aumento + 2] = (float)vector.z;
				aumento += 5;
				//std::cout << "ESTO DEBERIA SER IGUAL x: " << vertices[indicesR[i] + aumento] << " y: " << vertices[indicesR[i] + aumento + 1] << " z: " << vertices[indicesR[i] + aumento + 2] << std::endl;
			}
			aumento = 0;
		}
		aumento = 0;
	}
	void mov_F(float sentido, float eje) {
		//std::cout << "MOVIMIENTO R" << std::endl;
		glm::mat4 res = glm::mat4(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		//glm::rotate(model, 0.002f, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(eje * 90.0f), glm::vec3(0.0, 0.0f, sentido));
		//transformation = glm::rotate(transformation, velocidad, glm::vec3(0.0f, 0.0f, sentido));
		int aumento = 0;
		for (int i = 0; i < 21; ++i) {
			for (int j = 0; j < 6; ++j) {
				//std::cout << "INDICE EN " << indicesR[i] + aumento << std::endl;
				glm::vec4 vector((float)vertices[indicesF[i] + aumento], (float)vertices[indicesF[i] + aumento + 1], (float)vertices[indicesF[i] + aumento + 2], 1.0f);
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vector = transformation * vector;
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vertices[indicesF[i] + aumento] = (float)vector.x;
				vertices[indicesF[i] + aumento + 1] = (float)vector.y;
				vertices[indicesF[i] + aumento + 2] = (float)vector.z;
				aumento += 5;
				//std::cout << "ESTO DEBERIA SER IGUAL x: " << vertices[indicesR[i] + aumento] << " y: " << vertices[indicesR[i] + aumento + 1] << " z: " << vertices[indicesR[i] + aumento + 2] << std::endl;
			}
			aumento = 0;
		}
		aumento = 0;
	}
	void mov_M3(float sentido, float eje) {
		//std::cout << "MOVIMIENTO R" << std::endl;
		glm::mat4 res = glm::mat4(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		//glm::rotate(model, 0.002f, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(eje * 90.0f), glm::vec3(0.0f, 0.0f, sentido));
		//transformation = glm::rotate(transformation, velocidad, glm::vec3(0.0f, 0.0f, sentido));
		int aumento = 0;
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j < 6; ++j) {
				//std::cout << "INDICE EN " << indicesR[i] + aumento << std::endl;
				glm::vec4 vector((float)vertices[indicesM3[i] + aumento], (float)vertices[indicesM3[i] + aumento + 1], (float)vertices[indicesM3[i] + aumento + 2], 1.0f);
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vector = transformation * vector;
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vertices[indicesM3[i] + aumento] = (float)vector.x;
				vertices[indicesM3[i] + aumento + 1] = (float)vector.y;
				vertices[indicesM3[i] + aumento + 2] = (float)vector.z;
				aumento += 5;
				//std::cout << "ESTO DEBERIA SER IGUAL x: " << vertices[indicesR[i] + aumento] << " y: " << vertices[indicesR[i] + aumento + 1] << " z: " << vertices[indicesR[i] + aumento + 2] << std::endl;
			}
			aumento = 0;
		}
		aumento = 0;
	}
	void mov_B(float sentido, float eje) {
		//std::cout << "MOVIMIENTO R" << std::endl;
		glm::mat4 res = glm::mat4(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		//glm::rotate(model, 0.002f, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(eje * 90.0f), glm::vec3(0.0, 0.0f, sentido));
		//transformation = glm::rotate(transformation, velocidad, glm::vec3(0.0f, 0.0f, sentido));
		int aumento = 0;
		for (int i = 0; i < 21; ++i) {
			for (int j = 0; j < 6; ++j) {
				//std::cout << "INDICE EN " << indicesR[i] + aumento << std::endl;
				glm::vec4 vector((float)vertices[indicesB[i] + aumento], (float)vertices[indicesB[i] + aumento + 1], (float)vertices[indicesB[i] + aumento + 2], 1.0f);
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vector = transformation * vector;
				//std::cout << "vector x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
				vertices[indicesB[i] + aumento] = (float)vector.x;
				vertices[indicesB[i] + aumento + 1] = (float)vector.y;
				vertices[indicesB[i] + aumento + 2] = (float)vector.z;
				aumento += 5;
				//std::cout << "ESTO DEBERIA SER IGUAL x: " << vertices[indicesR[i] + aumento] << " y: " << vertices[indicesR[i] + aumento + 1] << " z: " << vertices[indicesR[i] + aumento + 2] << std::endl;
			}
			aumento = 0;
		}
		aumento = 0;
	}
	//int indicesR[12];
	//TipoA -> solo muestra 1 lado CENTRAL
	//TipoB -> muestra 2 lados MEDIO
	//TipoC -> muestra 3 lados ESQUINA
};


// create transformations
glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// 
	// rubik CuboDeRubik();
	// CuboDeRubik.createCubes();
	// CuboDeRubik.createTexture();
	// CuboDeRubik.Solver()
	// ------------------------------------
	Shader ourShader("6.2.coordinate_systems.vs", "6.2.coordinate_systems.fs");
	rubik cubitomaincra;

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//CUADRADO CENTRAL ESTE NO ES NECESARIO DIBUJARLO

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2, texture3, texture4, texture5, texture6;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("D://texturas//rojo.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("D://texturas//naranja.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("D://texturas//verde.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("D://texturas//azul.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("D://texturas//amarillo.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("D://texturas//blanco.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	ourShader.setInt("texture3", 2);
	ourShader.setInt("texture4", 3);
	ourShader.setInt("texture5", 4);
	ourShader.setInt("texture6", 5);

	//cubitomaincra.mov_R(1.0f, 0.2f);
	float velocidad = 0.125f;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// bind textures on corresponding texture units
		//cubitomaincra.mov_R(1.0f, 0.02f);
		// activate shader
		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

		// retrieve the matrix uniform locations
		unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		ourShader.setMat4("projection", projection);

		// render box

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glDrawArrays(GL_TRIANGLES, 0, 54);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glDrawArrays(GL_TRIANGLES, 54, 54);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glDrawArrays(GL_TRIANGLES, 108, 54);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glDrawArrays(GL_TRIANGLES, 162, 54);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		glDrawArrays(GL_TRIANGLES, 216, 54);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);
		glDrawArrays(GL_TRIANGLES, 270, 54);


		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_L(1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_L(-1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_M(1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_M(-1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_R(1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_R(-1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_U(1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_U(-1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_M2(1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_M2(-1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_D(1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_D(-1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_F(1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_F(-1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_M3(1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_M3(-1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_B(1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			glBindVertexArray(VAO);
			cubitomaincra.mov_B(-1.0f, velocidad);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}