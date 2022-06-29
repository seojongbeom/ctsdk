#ifndef UIX_H
#define UIX_H

namespace uix {
namespace variant {
enum State {
        InValidate = 0,
        Validate = 1,
        Partially = 2
    };

 enum Type {
        Bool,
        Char,
        Byte,
        Short,
        UShort,
        Int,
        UInt,
        LongLong,
        ULongLong,
        Float,
        Double,
        ByteArray,
        String,
        VariantList,
        VariantMap,
        Unknown
    };

} // namespace property

namespace theme {
    enum PropertiesId {
        accent = 0,
        background = 1,
        elevation = 2,
        foreground = 3,
        primary = 4
    };

    enum Id {
        dark = 0,
        light = 1,
    };	
} // namespace theme



    enum ControllerEventState {
        None,
        Hover,
        Active
    };

} // namespace uix


#endif //UIX_H
