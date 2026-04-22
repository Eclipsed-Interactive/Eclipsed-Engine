#pragma once

namespace Eclipse
{
    class Sprite
    {
    public:
        Sprite();
        ~Sprite() = default;

        void Render();

        static Sprite& Get()
        {
            static Sprite instance;
            return instance;
        }

    private:
        
        unsigned myVTXbuffer;
        unsigned myIDXbuffer;
        unsigned mySpriteBuffer;


    private:
        struct Vertex
        {
            float posX;
            float posY;

            float texCoordX;
            float texCoordY;
        };

#pragma region hard_coded_verts_indecies
        // 4 vertices
        static inline Vertex vertices[]{
            {-0.5f, -0.5f, 0, 0},
            {0.5f, -0.5f, 1, 0},
            {-0.5f, 0.5f, 0, 1},
            {0.5f, 0.5f, 1, 1}};

        // 6 indices
        static inline unsigned indices[]{
            0, 1, 2,
            2, 1, 3};

#pragma endregion
    };
}