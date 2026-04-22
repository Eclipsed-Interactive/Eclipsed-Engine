#include "CommandList.h"

namespace Eclipse
{
    void CommandList::Init()
    {
        myData = new uint8_t[MAXCOMMANDALLOCATION];

        myRoot = reinterpret_cast<RenderCommandBase*>(myData);
        myLink = &myRoot;
    }

    void CommandList::Enqueue(const std::function<void()>& aLambda)
    {
        Enqueue<LambdaCommand>(aLambda);
    }

    void CommandList::Execute()
    {
        if (!hasCommands) return;

        RenderCommandBase* nextCMD = myRoot;

        while (nextCMD)
        {
            nextCMD->Execute();
            nextCMD = nextCMD->next;
        }
    }

    void CommandList::Reset()
    {
        if (hasCommands)
        {
            while (myRoot)
            {
                myRoot->~RenderCommandBase();
                myRoot = myRoot->next;
            }
        }

        hasCommands = false;
        std::memset(myData, 0, commandCursor);
        commandCursor = 0;

        myRoot = reinterpret_cast<RenderCommandBase*>(myData);
        myLink = &myRoot;
    }
}