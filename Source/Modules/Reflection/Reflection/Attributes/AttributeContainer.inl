namespace Eclipse::Reflection
{
    template<typename Attribute, typename... Args>
    inline void AttributeContainer::Add(Args&&... args)
    {
        attributes.insert_or_assign(
            typeid(Attribute),
            Attribute{ std::forward<Args>(args)... }
        );
    }

    template<typename Attribute>
    inline const Attribute* AttributeContainer::Get()const
    {
        return std::any_cast<Attribute>(
            &attributes.at(typeid(Attribute))
        );
    }

    template<typename Attribute>
    inline const Attribute* AttributeContainer::TryGet() const
    {
        auto it = attributes.find(typeid(Attribute));

        if (it == attributes.end())
            return nullptr;

        return std::any_cast<Attribute>(&it->second);
    }
}