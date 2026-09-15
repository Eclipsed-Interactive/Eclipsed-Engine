#ifndef SERIALIZED_ENUM_HPP
#define SERIALIZED_ENUM_HPP

#define MAKE_ENUM(name, value) name = value,
#define MAKE_STRING(name, value) { e::name, #name },
#define MAKE_LIST(value, name) value,

class SerializedEnum {};

#define SERIALIZABLE_ENUM(name, ENUM_ITEMS)                                     \
class name : SerializedEnum {                                                   \
public:                                                                         \
    enum e : int { ENUM_ITEMS(MAKE_ENUM) };                                     \
    e value = {};                                                               \
                                                                                \
    static inline const std::unordered_map<e, std::string> ToString = {         \
        ENUM_ITEMS(MAKE_STRING)                                                 \
    };                                                                          \
                                                                                \
    static inline std::vector<e> List = { ENUM_ITEMS(MAKE_LIST) };              \
                                                                                \
    constexpr operator e() const { return value; }                              \
    constexpr name() = default;                                                 \
    constexpr name(e val) : value(val){}                                        \
    constexpr name(int val) : value(static_cast<e>(val)){}                      \
    constexpr name(unsigned val) : value(static_cast<e>(val)){}                 \
                                                                                \
    friend inline unsigned operator|(e a, e b) {                                \
        return static_cast<unsigned>(a) | static_cast<unsigned>(b);             \
    }                                                                           \
                                                                                \
    static inline std::string AsString(e v) {                                   \
        auto it = ToString.find(v);                                             \
        return (it != ToString.end()) ? it->second : "Unknown";                 \
    }                                                                           \
    static inline std::string AsString(unsigned v) {                            \
        return AsString(static_cast<e>(v));                                     \
    }                                                                           \
};
#endif