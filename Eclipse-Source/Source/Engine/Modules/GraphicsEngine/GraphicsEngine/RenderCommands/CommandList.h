#pragma once

#define MAXCOMMANDALLOCATION 100'000

#include "RenderCommand.h"

#include <vector>
#include <functional>
#include <assert.h>

namespace Eclipse
{
    class LambdaCommand : public RenderCommandBase
    {
    public:
        LambdaCommand(std::function<void()>&& aLambda) : myLamdaCommand(aLambda)
        {
        }

        ~LambdaCommand() override = default;

        void Execute() override { myLamdaCommand(); };

        std::function<void()> myLamdaCommand;
    };

    class CommandList
    {
    public:
        void Init();

        template <class CommandClass, class... Args>
        void EnqueueZIndex(int aZIndex, Args... args);

        template <class CommandClass, class... Args>
        void Enqueue(Args... args);

        void Enqueue(const std::function<void()>& aLambda);

        void Execute();
        void Reset();

    private:
        bool hasCommands = false;

        uint8_t* myData = nullptr;
        size_t commandCursor = 0;

        RenderCommandBase* myRoot;
        RenderCommandBase** myLink;
    };

    template <class CommandClass, class... Args>
    inline void CommandList::Enqueue(Args... args)
    {
        hasCommands = true;
        const size_t commandSize = sizeof(CommandClass);

        assert((commandCursor + commandSize) <= MAXCOMMANDALLOCATION && "This encue will make the array overflow with data so you need to make smaller commands or increase MAXCOMMANDALLOCATION :)");

        RenderCommandBase* command = reinterpret_cast<RenderCommandBase*>(myData + commandCursor);
        commandCursor += commandSize;
        ::new(command) CommandClass(std::forward<Args>(args)...);
        *myLink = command;
        myLink = &command->next;
    }

    template <class CommandClass, class... Args>
    inline void CommandList::EnqueueZIndex(int aZIndex, Args... args)
    {
        hasCommands = true;
        const size_t commandSize = sizeof(CommandClass);

        assert((commandCursor + commandSize) <= MAXCOMMANDALLOCATION && "This encue will make the array overflow with data so you need to make smaller commands or increase MAXCOMMANDALLOCATION :)");

        RenderCommandBase* command = reinterpret_cast<RenderCommandBase*>(myData + commandCursor);
        commandCursor += commandSize;
        ::new(command) CommandClass(std::forward<Args>(args)...);
        *myLink = command;
        myLink = &command->next;
    }

    class CommandListManager
    {
    public:
        static CommandList& GetUICommandList() { return UICommandList; }
        static CommandList& GetSpriteCommandList() { return SpriteCommandList; }
        static CommandList& GetDebugDrawCommandList() { return DebugDrawCommandList; }
        static CommandList& GetHappenAtBeginCommandList() { return HappenAtBeginCommandList; }

        static void ExecuteAllCommandLists()
        {
            SpriteCommandList.Execute();
            UICommandList.Execute();
            DebugDrawCommandList.Execute();
        }

        static void InitAllCommandLists()
        {
            SpriteCommandList.Init();
            UICommandList.Init();
            DebugDrawCommandList.Init();
            HappenAtBeginCommandList.Init();
        }

        static void ResetAllCommandLists()
        {
            SpriteCommandList.Reset();
            UICommandList.Reset();
            DebugDrawCommandList.Reset();
        }

    private:
        static inline CommandList UICommandList;
        static inline CommandList SpriteCommandList;
        static inline CommandList DebugDrawCommandList;


        static inline CommandList HappenAtBeginCommandList;
    };
}
