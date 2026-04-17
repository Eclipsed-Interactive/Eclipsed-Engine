#pragma once    

class RenderCommandBase
{
public:
    RenderCommandBase() = default;
    virtual ~RenderCommandBase() = default;

    virtual void Execute() = 0;

    RenderCommandBase* next = nullptr;
};