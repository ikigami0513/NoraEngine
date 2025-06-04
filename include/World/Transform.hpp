#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Texture.hpp"

class Transform {
    protected:
        // Local space information
        glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_eulerRot = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

        // Global space information concatenate in matrix
        glm::mat4 m_modelMatrix = glm::mat4(1.0f);

        // Dirty flag
        bool m_isDirty = true;

        glm::mat4 GetLocalModelMatrix() {
            const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
            const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
            const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.z), glm::vec3(0.0f, 0.0f, 1.0f));

            // Y * X * Z
            const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

            // translation * rotation * scale (also know as TRS matrix)
            return glm::translate(glm::mat4(1.0f), m_pos) * rotationMatrix * glm::scale(glm::mat4(1.0f), m_scale);
        }

    public:
        void ComputeModelMatrix() {
            m_modelMatrix = GetLocalModelMatrix();
            m_isDirty = false;
        }

        void ComputeModelMatrix(const glm::mat4& parentGlobalModelMatrix) {
            m_modelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
            m_isDirty = false;
        }

        void SetLocalPosition(const glm::vec3& newPosition) {
            m_pos = newPosition;
            m_isDirty = true;
        }

        void SetLocalRotation(const glm::vec3& newRotation) {
            m_eulerRot = newRotation;
            m_isDirty = true;
        }

        void SetLocalScale(const glm::vec3& newScale) {
            m_scale = newScale;
            m_isDirty = true;
        }

        const glm::vec3 GetGlobalPosition() const {
            return m_modelMatrix[3];
        }

        const glm::vec3& GetLocalPosition() const {
            return m_pos;
        }

        const glm::vec3& GetLocalRotation() const {
            return m_eulerRot;
        }

        const glm::vec3& GetLocalScale() const {
            return m_scale;
        }

        const glm::mat4& GetModelMatrix() const {
            return m_modelMatrix;
        }

        glm::vec3 GetRight() const {
            return m_modelMatrix[0];
        }

        glm::vec3 GetUp() const {
            return m_modelMatrix[1];
        }

        glm::vec3 GetBackward() const {
            return m_modelMatrix[2];
        }

        glm::vec3 GetForward() const {
            return -m_modelMatrix[2];
        }

        glm::vec3 GetGlobalScale() const {
            return { glm::length(GetRight()), glm::length(GetUp()), glm::length(GetBackward()) };
        }

        bool IsDirty() const {
            return m_isDirty;
        }

        glm::mat4 GetLocalModelMatrix2D(std::shared_ptr<Texture> texture, const glm::vec4& textureRectNormalized) const {
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), m_pos);
            
            // Rotation for 2D is typically just around the Z-axis
            glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.z), glm::vec3(0.0f, 0.0f, 1.0f));

            float displayWidth = 1.0f;  // Default width if texture is null or has no dimensions
            float displayHeight = 1.0f; // Default height

            if (texture && texture->Width() > 0 && texture->Height() > 0) {
                float u_min = textureRectNormalized.x;
                float v_min = textureRectNormalized.y;
                float u_max = textureRectNormalized.z;
                float v_max = textureRectNormalized.w;

                float rect_u_width_normalized = u_max - u_min;
                float rect_v_height_normalized = v_max - v_min;

                // Calculate the display size based on the sub-rectangle's dimensions
                displayWidth = rect_u_width_normalized * texture->Width();
                displayHeight = rect_v_height_normalized * texture->Height();
            } else if (texture) {
                // Fallback or warning if texture dimensions are invalid
                // std::cerr << "Warning: Texture has zero dimensions." << std::endl;
                // Keep displayWidth/Height as 1.0f or use full m_scale if desired.
                // For now, we'll use 1.0 which means the m_scale will be the direct pixel size.
            }


            // Apply the entity's local scale to the calculated display size
            // The sprite quad is 1x1 unit, so scaling by displayWidth/Height makes it pixel-correct (before m_scale)
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), 
                                            glm::vec3(displayWidth * m_scale.x, 
                                                        displayHeight * m_scale.y, 
                                                        1.0f)); // Z-scale is 1 for 2D

            model = translateMatrix * rotateMatrix * scaleMatrix;
            return model;
        }
};

#endif