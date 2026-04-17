#include "Transform2D.h"

#include <sstream>

#include "CoreEngine/Math/Vector/Vector3.h"

namespace Eclipse
{
    void Transform2D::position_OnRep()
    {
    }

    void Transform2D::rotation_OnRep()
    {
    }

    void Transform2D::scale_OnRep()
    {
    }

    void Transform2D::OnComponentAddedNoCreations()
    {
        gameObject->transform = this;

        UpdateTransforms();
    }

    void Transform2D::AddParentTransform(GameObject* aParent, Math::Mat3x3f& aTransform) const
    {
        Transform2D* parentTransform = aParent->GetComponent<Transform2D>();
        if (!parentTransform)
            return;

        Math::Mat3x3f parentTransformMatrix = Math::Mat3x3f::CreateTranslation(parentTransform->GetPosition());
        parentTransformMatrix *= Math::Mat3x3f::CreateRotation(-parentTransform->GetRotation());
        //parentTransformMatrix *= Math::Mat3x3f::CreateScale(parentTransform->GetLocalScale());

        aTransform *= parentTransformMatrix;

        // GameObject* parent = aParent->GetParent();
        // if (parent && parent->transform)
        //     AddParentTransform(parent, aTransform);
    }

    Math::Matrix3x3f Transform2D::GetTransformMatrix()
    {
        return GlobalTransformationMatrix;
    }
    
    Math::Matrix3x3f Transform2D::GetTransform() const
    {
        Math::Mat3x3f mat = GetLocalTransformMatrix();

        GameObject* parent = gameObject->GetParent();
        if (parent && parent->transform)
            AddParentTransform(parent, mat);

        return mat;
    }

    Math::Matrix3x3f Transform2D::GetLocalTransformMatrix() const
    {
        Math::Mat3x3f mat = Math::Mat3x3f::CreateTranslation(GetLocalPosition());
        mat *= Math::Mat3x3f::CreateRotation(-GetLocalRotation());

        return mat;
    }

    void Transform2D::AddParentRotation(GameObject* aParent, float& totalRotation) const
    {
        Transform2D* parentTransform = aParent->GetComponent<Transform2D>();
        if (!parentTransform)
            return;

        totalRotation += parentTransform->GetLocalRotation();

        GameObject* parent = aParent->GetParent();
        if (parent && parent->transform)
            AddParentRotation(parent, totalRotation);
    }

    
    Math::Vector2f Transform2D::GetPosition()
    {
        return GlobalPosition;
    }
    float Transform2D::GetRotation()
    {
        return GlobalRotation * Math::deg2Rad;
    }
    Math::Vector2f Transform2D::GetScale()
    {
        return GlobalScale;
    }

    
    void Transform2D::AddParentScale(GameObject* aParent, Math::Vector2f& totalScale) const
    {
        Transform2D* parentTransform = aParent->GetComponent<Transform2D>();
        if (!parentTransform)
            return;

        totalScale *= parentTransform->GetLocalScale();

        GameObject* parent = aParent->GetParent();
        if (parent && parent->transform)
            AddParentScale(parent, totalScale);
    }

    const Math::Vector2f& Transform2D::GetLocalPosition() const
    {
        return position;
    }

    const float Transform2D::GetLocalRotation() const
    {
        return rotation;
    }

    const Math::Vector2f& Transform2D::GetLocalScale() const
    {
        return scale;
    }

    Math::Vector2f* Transform2D::GetPositionPtr()
    {
        return &position.Get();
    }

    float* Transform2D::GetRotationPtr()
    {
        return &rotation.Get();
    }

    Math::Vector2f* Transform2D::GetScalePtr()
    {
        return &scale.Get();
    }

    void Transform2D::SetPosition(const Math::Vector2f& aPosition)
    {
        SetPosition(aPosition.x, aPosition.y);
    }

    void Transform2D::SetPosition(float aX, float aY)
    {
        position->x = aX;
        position->y = aY;

        lastPosition = position;

        myIsDirty = true;
    }

    void Transform2D::SetRotation(float aRotation)
    {
        rotation = aRotation;

        lastRotation = rotation;

        myIsDirty = true;
    }

    void Transform2D::SetScale(const Math::Vector2i& aScale)
    {
        SetScale(static_cast<float>(aScale.x), static_cast<float>(aScale.y));
    }

    void Transform2D::SetScale(const Math::Vector2f& aScale)
    {
        SetScale(aScale.x, aScale.y);
    }

    void Transform2D::SetScale(float aX, float aY)
    {
        scale->x = aX;
        scale->y = aY;

        lastScale = scale;

        myIsDirty = true;
    }

    void Transform2D::AddFunctionToRunOnDirtyUpdate(Component* aComponent, const std::function<void()>& aFunction)
    {
        myFunctionsToRunOnDirtyUpdate.push_back({aComponent, aFunction});
    }

    void Transform2D::ChildingObject(GameObject* aChild, GameObject* aParent)
    {
        
    }

    void Transform2D::UpdateTransforms()
    {
        Math::Vector3f PositionV3(position->x, position->y, 1.f);
        GlobalRotation = rotation;
        GlobalScale = scale;
        
        GameObject* parent = gameObject->GetParent();
        if (parent && parent->transform)
        {
            GlobalTransformationMatrix = Math::Matrix3x3f();
            AddParentTransform(parent, GlobalTransformationMatrix);
            
            PositionV3 = PositionV3 * GlobalTransformationMatrix;
            GlobalPosition.x = PositionV3.x;
            GlobalPosition.y = PositionV3.y;

            AddParentRotation(parent, GlobalRotation);
            AddParentScale(parent, GlobalScale);
        }
        else
        {
            GlobalTransformationMatrix = GetTransform();            
            GlobalPosition = position;
        }
    }


    void Transform2D::EditorUpdate()
    {
        if (position->x != lastPosition.x || position->y != lastPosition.y)
        {
            lastPosition = position;
            myIsDirty = true;
        }
        if (scale->x != lastScale.x || scale->y != lastScale.y)
        {
            lastScale = scale;
            myIsDirty = true;
        }
        if (lastRotation != rotation)
        {
            lastRotation = rotation;
            myIsDirty = true;
        }


        if (myIsDirty)
        {
            DirtyUpdate();
            myIsDirty = false;
        }
    }

    void Transform2D::DirtyUpdate()
    {
        UpdateTransforms();

        std::vector<unsigned> indeciesToDelete;
        unsigned currentIndex = 0;
        for (auto& func : myFunctionsToRunOnDirtyUpdate)
        {
            if (func.component->IsDeleted)
            {
                indeciesToDelete.emplace_back(currentIndex);
                continue;
            }
            func.function();

            currentIndex++;
        }

        for (auto index : indeciesToDelete)
        {
            for (int i = index; i < indeciesToDelete.size() - 1; ++i)
                myFunctionsToRunOnDirtyUpdate[i] = myFunctionsToRunOnDirtyUpdate[i++];
            
            myFunctionsToRunOnDirtyUpdate.pop_back();
        }


        for (const auto& child : gameObject->GetChildren())
        {
            if (child->transform)
                child->transform->DirtyUpdate();
        }
    }
}
