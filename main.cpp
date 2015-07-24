// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

// Include GLEW
#include <GL/glew.h>
//#include <OpenGL/gl3.h>
//#include <OpenGL/gl3ext.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/shader.hpp"
#include "common/controls.hpp"
#include "TriMesh.h"

GLFWwindow* init_glfw_window(int width = 1024, int height = 768, std::string title = "Subdivision Deformation") {
    // Initialise GLFW
    if( !glfwInit() )
    {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            exit(-1);
    }


    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* win =  glfwCreateWindow( 1024, 768, "Tutorial 0 - Keyboard and Mouse", NULL, NULL);

    if( win == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        exit(-2);
    }

    glfwMakeContextCurrent(win);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            exit(-3);
    }

    return win;
}

static const int unit_cube_indices[] = {
  0, 1, 2,
  0, 2, 3,
  3, 2, 6,
  3, 6, 7,
  2, 1, 5,
  2, 5, 6,
  0, 3, 4,
  3, 7, 4,
  6, 5, 4,
  6, 4, 7,
  0, 4, 1,
  1, 4, 5,
};

static const GLfloat unit_cube_vertices[] = {
  -1, -1, -1,
   1, -1, -1,
   1,  1, -1,
  -1,  1, -1,
  -1, -1, -3,
   1, -1, -3,
   1,  1, -3,
  -1,  1, -3,
};

static const GLfloat triangle_vertices[] = {
  -1, -1, -1,
   1, -1, -1,
   1,  1, -1,
};

static const int triangle_indices[] = {
   0, 1, 2,
};

//#define _shape_vertices triangle_vertices
//#define _shape_indices triangle_indices
#define _shape_vertices unit_cube_vertices
#define _shape_indices unit_cube_indices

int n_vertices = sizeof(_shape_vertices)/sizeof(_shape_vertices[0]);
int n_indices = sizeof(_shape_indices)/sizeof(_shape_indices[0]);

int main( void )
{

    TriMesh mesh = verts_to_trimesh(_shape_vertices, n_vertices, _shape_indices, n_indices);
    std::vector<float> normals = mesh_normals(mesh);
    std::cout << "Normal size: " << normals.size() << std::endl;

    print_normals(normals);

    window = init_glfw_window();

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.5f, 0.5f, 0.7f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    //GLuint programID = LoadShaders( "TransformVertexShader.vert", "TextureFragmentShader.frag" );
    GLuint programID = LoadShaders( "phong.vert", "phong.frag" );

    // Get a handle for our "MVP" uniform
    GLuint MVP_ID    = glGetUniformLocation(programID, "MVP");
    GLuint MV_ID     = glGetUniformLocation(programID, "MV");
    GLuint NORMAT_ID = glGetUniformLocation(programID, "normalMatrix");
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_shape_vertices), _shape_vertices, GL_STATIC_DRAW);

    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_shape_indices), _shape_indices, GL_STATIC_DRAW);

    
    GLuint NormalVBOID;
    glGenBuffers(1, &NormalVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, NormalVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);

    do{

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Use our shader
            glUseProgram(programID);

            // Compute the MVP matrix from keyboard and mouse input
            computeMatricesFromInputs();
            glm::mat4 ProjectionMatrix = getProjectionMatrix();
            glm::mat4 ViewMatrix = getViewMatrix();
            glm::mat4 ModelMatrix = glm::mat4(1.0);
            glm::mat4 MV  = ViewMatrix * ModelMatrix;
            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
            glm::mat4 NorMat = glm::inverse(glm::transpose(MV));
            //glm::vec3 Light = glm::vec3(1.0, 0.5, -1.0);

            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(MVP_ID,    1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(MV_ID,     1, GL_FALSE, &MV[0][0]);
            glUniformMatrix4fv(NORMAT_ID, 1, GL_FALSE, &NorMat[0][0]);
            //glUniform3fv(LIGHT_ID, 1, &Light[0]);

            // 1rst attribute buffer : vertices
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(
                    0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
            );

            glEnableVertexAttribArray(0);
            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
            
            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                sizeof(_shape_indices) / sizeof(_shape_indices[0]),    // count
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
            );

            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, NormalVBOID);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

            //glDisableVertexAttribArray(1);

            // Draw the triangle !
            //glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles


            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
               glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
