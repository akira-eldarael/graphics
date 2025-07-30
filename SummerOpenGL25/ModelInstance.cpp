#include "ModelInstance.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

void draw_mesh(ModelInstance instance, GLint program, float transperancy) {

    // Create model matrix the same way as working code
    glm::mat4 model = glm::mat4(1.0f);
    GLint mModel_UL = glGetUniformLocation(program, "mModel");
    GLint mModel_InverseTranpose_UL = glGetUniformLocation(program, "mModel_InverseTranpose");
    GLint transparency_UL = glGetUniformLocation(program, "alpha_transparency");

    /*transalate : positon */
    model = glm::translate(model, glm::vec3(instance.position.x, instance.position.y, instance.position.z));
    model = glm::rotate(model, glm::radians(instance.orientation.z), glm::vec3(0, 0, 1)); // Z
    model = glm::rotate(model, glm::radians(instance.orientation.y), glm::vec3(0, 1, 0)); // Y
    model = glm::rotate(model, glm::radians(instance.orientation.x), glm::vec3(1, 0, 0)); // X
    /*scale*/
    model = glm::scale(model, glm::vec3(instance.scale.x, instance.scale.y, instance.scale.z));
    glUniformMatrix4fv(mModel_UL, 1, GL_FALSE, glm::value_ptr(model));

    // Use the same (incorrect but working) inverse transpose calculation
    glm::mat4 mModel_InverseTranspose = glm::inverse(glm::transpose(model));

    // Set uniforms
    glUniformMatrix4fv(mModel_UL, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(mModel_InverseTranpose_UL, 1, GL_FALSE, glm::value_ptr(mModel_InverseTranspose));

    /* mvp = projection * view * model;
     glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));*/

     /*transperancy calcs*/
    glUniform1f(transparency_UL, transperancy);

    sModelDrawInfo modelToDraw;
    if (g_pMeshManager->FindDrawInfoByModelName(instance.meshName, modelToDraw))
    {
        glBindVertexArray(modelToDraw.VAO_ID);
        glDrawElements(GL_TRIANGLES, modelToDraw.numberOfIndices, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }
}
