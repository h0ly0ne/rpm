#if defined(__BORLANDC__) || defined(_MSC_VER)
    #define PACKED
    #pragma pack(push, 1)
#elif defined(__GNUC__)
    #define PACKED  __attribute__((__packed__))
#else
    #error PACKED macro is not defined for this compiler
#endif