#pragma once

#include "../../gl.h"
#include "GLType.h"
#include "../Utility/OpenGLHelpers.h"

#include <glm/glm.hpp>
#include <string>
#include <type_traits>

#define VERTEX_ATTRIB_DRAW_TYPE GL_STREAM_DRAW

namespace Rendering
{
    /**
     * Represents a shader vertex attribute.
     */
    class VertexAttribBase
    {
    public:
        virtual ~VertexAttribBase() = default;

        /**
         * Gets the name of the attrib.
         *
         * @returns The name of the attrib.
         */
        virtual const std::string &getName() const = 0;

        /**
         * Gets the value of the attrib.
         *
         * @returns A void pointer to the attrib's value.
         */
        virtual void *getValuePointer() = 0;

        /**
         * Gets the OpenGL type of the attrib.
         *
         * @returns The OpenGL type of the attrib.
         */
        virtual GLenum getGLType() const = 0;

        /**
         * Gets the OpenGL component type of the attrib.
         *
         * i.e. GL_FLOAT_VEC3 would return GL_FLOAT.
         *
         * @returns The OpenGL component type of the attrib.
         */
        virtual GLenum getComponentType() const = 0;

        /**
         * Gets the number of components in each value of the attrib.
         *
         * @returns The number of components in each value of the attrib.
         */
        virtual unsigned int getNumComponents() const = 0;

        /**
         * Gets the size of the matrix if the attrib is a matrix.
         *
         * i.e. 2 for mat2, 3 for mat3, 4 for mat4.
         *
         * Will be -1 if the attrib is not a matrix.
         *
         * @returns The size of the matrix if the attrib is a matrix.
         */
        virtual int getMatrixSize() const = 0;

        /**
         * Sets whether to normalize the attrib.
         *
         * @param normalize Whether to normalize the attrib
         */
        virtual void setNormalize(bool normalize) = 0;

        /**
         * Gets whether to normalize the attrib.
         *
         * @returns Whether to normalize the attrib.
         */
        virtual bool getNormalize() const = 0;

        /**
         * Sets the divisor of the attrib.
         *
         * This is used for instanced rendering, it is the number of instances between switching the attribute.
         *
         * @param divisor The divisor of the attrib
         */
        virtual void setDivisor(unsigned int divisor) = 0;

        /**
         * Gets the divisor of the attrib.
         *
         * @returns The divisor of the attrib.
         */
        virtual unsigned int getDivisor() const = 0;

        /**
         * Gets the offset of the data in the attrib array.
         *
         * @returns The offset of the data in the attrib array.
         */
        virtual unsigned int getOffset() const = 0;

        /**
         * Gets the size of the attrib array.
         *
         * @returns The size of the attrib array.
         */
        virtual size_t size() const = 0;

        /**
         * Gets the size of the template type in bytes.
         *
         * Uses sizeof(T).
         *
         * @returns The size of the template type.
         */
        virtual size_t getTSize() const = 0;
    };

    /**
     * Represents a shader vertex attribute.
     */
    template <typename T>
    class VertexAttrib : public VertexAttribBase
    {
    public:
        /**
         * Creates a vertex attribute.
         *
         * Vertex data should begin at the start of the array.
         *
         * For instance rendering the divisor should be set to the number of instances between switching the attribute. This is 0 by default and can be set with setDivisor.
         *
         * To change the value of the vertex attribute the underlying vector should be change as it is passed by reference.
         *
         * This is annoying but avoids copying the vector every frame.
         *
         * The value will not be changed from within the VertexAttrib class.
         *
         * @param name The name of the vertex attribute
         * @param value The value of the vertex attribute
         * @param type The OpenGL type of the vertex attribute
         * @param numComponents The number of components in each value of the vertex attribute
         * @param matrixSize The size of the matrix if the vertex attribute is a matrix
         */
        VertexAttrib(std::string name, const std::vector<T> &value, GLenum type = GLType::type<T>, unsigned int numComponents = glGetNumComponents(GLType::type<T>), int matrixSize = glGetMatrixSize(GLType::type<T>)) : name(name), value(value), type(type), numComponents(numComponents), matrixSize(matrixSize)
        {
            tSize = sizeof(T);
            componentType = glGetComponentType(type);
        }

        /**
         * Gets the name of the vertex attribute.
         *
         * @returns The name of the vertex attribute.
         */
        const std::string &getName() const
        {
            return name;
        }

        /**
         * Gets the value of the vertex attribute.
         *
         * @returns The value of the vertex attribute.
         */
        const std::vector<T> &get()
        {
            return value;
        }

        /**
         * Gets the value of the vertex attribute.
         *
         * @returns A void pointer to the vertex attribute's value.
         */
        void *getValuePointer()
        {
            return (void *)&value[0];
        }

        /**
         * Gets the OpenGL type of the vertex attribute.
         *
         * @returns The OpenGL type of the vertex attribute.
         */
        GLenum getGLType() const
        {
            return type;
        }

        /**
         * Gets the OpenGL component type of the vertex attribute.
         *
         * i.e. GL_FLOAT_VEC3 would return GL_FLOAT.
         *
         * @returns The OpenGL component type of the vertex attribute.
         */
        GLenum getComponentType() const
        {
            return componentType;
        }

        /**
         * Gets the number of components in each value of the vertex attribute.
         *
         * @returns The number of components in each value of the vertex attribute.
         */
        unsigned int getNumComponents() const
        {
            return numComponents;
        }

        /**
         * Gets the size of the matrix if the vertex attribute is a matrix.
         *
         * i.e. 2 for mat2, 3 for mat3, 4 for mat4.
         *
         * Will be -1 if the vertex attribute is not a matrix.
         *
         * @returns The size of the matrix if the vertex attribute is a matrix.
         */
        int getMatrixSize() const
        {
            return matrixSize;
        }

        /**
         * Sets whether to normalize the vertex attribute.
         *
         * @param normalize Whether to normalize the vertex attribute
         */
        void setNormalize(bool normalize)
        {
            this->normalize = normalize;
        }

        /**
         * Gets whether to normalize the vertex attribute.
         *
         * @returns Whether to normalize the vertex attribute.
         */
        bool getNormalize() const
        {
            return normalize;
        }

        /**
         * Sets the divisor of the vertex attribute.
         *
         * This is used for instanced rendering, it is the number of instances between switching the attribute.
         *
         * @param divisor The divisor of the vertex attribute
         */
        void setDivisor(unsigned int divisor)
        {
            this->divisor = divisor;
        }

        /**
         * Gets the divisor of the vertex attribute.
         *
         * @returns The divisor of the vertex attribute.
         */
        unsigned int getDivisor() const
        {
            return divisor;
        }

        /**
         * Gets the offset of the data in the vertex attribute array.
         *
         * This is always 0, if your data does not match then problems will occur.
         *
         * @returns The offset of the data in the vertex attribute array.
         */
        constexpr unsigned int getOffset() const
        {
            return 0;
        }

        /**
         * Gets the size of the vertex attribute array.
         *
         * @returns The size of the vertex attribute array.
         */
        size_t size() const
        {
            return value.size();
        }

        /**
         * Gets the size of the template type in bytes.
         *
         * Uses sizeof(T).
         *
         * @returns The size of the template type.
         */
        size_t getTSize() const
        {
            return tSize;
        }

    private:
        const std::string name;
        const std::vector<T> &value;
        GLenum type;
        GLenum componentType;

        /**
         * The size of the template type.
         */
        size_t tSize;

        /**
         * The number of components in the GL type.
         */
        unsigned int numComponents;

        int matrixSize;

        bool normalize = false;

        unsigned int divisor = 0;
    };
}