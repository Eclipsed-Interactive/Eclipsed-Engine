#include "Sprite.h"

#include "glad/glad.h"

namespace Eclipse
{
    Sprite::Sprite()
    {
        glGenVertexArrays(1, &mySpriteBuffer);
        glBindVertexArray(mySpriteBuffer);

        glGenBuffers(1, &myVTXbuffer);
        glGenBuffers(1, &myIDXbuffer);

        glBindBuffer(GL_ARRAY_BUFFER, myVTXbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIDXbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        {
            // glVertexAttribPointer(shader layout index, total number of floats/ints/bools etc, GL_[your type], GL_FALSE, vertex stride, offset from last layout position start)
            // Matrix4x4 would be index 0, 1, 2 and 3 set as in setting 4 float4's

            size_t offset = 0;
            // Position
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), (void*)offset);
            offset += 2 * sizeof(float);

            // TexCoord
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), (void*)offset);
            offset += 2 * sizeof(float);

            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(0);
        }

        glBindVertexArray(0);
    }

    void Sprite::Render()
    {
        glBindVertexArray(mySpriteBuffer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}