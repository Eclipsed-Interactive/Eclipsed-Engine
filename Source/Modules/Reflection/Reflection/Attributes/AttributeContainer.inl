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
    inline Attribute* AttributeContainer::Get()
    {
        return std::any_cast<Attribute>(
            &attributes.at(typeid(Attribute))
        );
    }

    template<typename Attribute>
    inline Attribute* AttributeContainer::TryGet()
    {
        auto it = attributes.find(typeid(Attribute));

        if (it == attributes.end())
            return nullptr;

        return std::any_cast<Attribute>(&it->second);
    }
}