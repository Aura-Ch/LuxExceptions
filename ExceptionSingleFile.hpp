#include <cstdint>
#include <cstdlib>

namespace Lux
{
    using letter = char8_t;
    using u8 = std::uint8_t;
    using s8 = std::int8_t;
    using u16 = std::uint16_t;
    using s16 = std::int16_t;
    using u32 = std::uint32_t;
    using s32 = std::int32_t;
    using f32 = float;
    using u64 = std::uint64_t;
    using s64 = std::int64_t;
    using f64 = double;

    namespace Concepts
    {
        template<class T> class IsUnsignedClass
        {
        public:
            static constexpr bool Value = false;
        };

        template<> class IsUnsignedClass<bool>
        {
        public:
            static constexpr bool Value = true;
        };

        template<> class IsUnsignedClass<letter>
        {
        public:
            static constexpr bool Value = true;
        };

        template<> class IsUnsignedClass<u8>
        {
        public:
            static constexpr bool Value = true;
        };

        template<> class IsUnsignedClass<u16>
        {
        public:
            static constexpr bool Value = true;
        };

        template<> class IsUnsignedClass<u32>
        {
        public:
            static constexpr bool Value = true;
        };

        template<> class IsUnsignedClass<u64>
        {
        public:
            static constexpr bool Value = true;
        };

        template<class T> concept IsUnsigned = IsUnsignedClass<T>::Value;

        template<class T, u64 Size> concept IsSize = Size == sizeof(T);

        template<class T> concept IsEnum = __is_enum(T);
    }

    namespace Exception
    {
        thread_local u32 GE = 0u;
        thread_local u32 LGE = 0u;
        thread_local bool HGE = false;

        inline constexpr void Try(bool& DHGE = HGE) noexcept
        {
            DHGE = true;
        }

        template<class T, class U> inline constexpr U Throw(T et) noexcept requires Concepts::IsEnum<T> && Concepts::IsSize<T, 4u> && Concepts::IsUnsigned<decltype(static_cast<u32>(T()))>
        {
            if(HGE)
                GE = static_cast<u32>(et);
            else
                std::abort();
            return U();
        }

        [[nodiscard]] inline constexpr const bool CatchAny(u32& DGE = GE, bool& DHGE = HGE) noexcept
        {
            DHGE = false;
            if(DGE != 0u)
            {
                LGE = GE;
                GE = 0u;
                return true;
            }
            return false;
        }

        template<class T, class... U> [[nodiscard]] inline constexpr const bool Catch(T et, U&&... ets) noexcept requires Concepts::IsEnum<T> && Concepts::IsSize<T, 4u> && Concepts::IsUnsigned<decltype(static_cast<u32>(T()))>
        {
            if(static_cast<u32>(et) == GE)
            {
                GE = 0u;
                return true;
            }
            else if constexpr(sizeof...(ets) > 0u)
                return Catch(ets...);
            else if(GE != 0u)
                std::abort();
            return false;
        }

        template<class T> [[nodiscard]] inline constexpr T GetLastEx() noexcept requires Concepts::IsEnum<T> && Concepts::IsSize<T, 4u>
        {
            return static_cast<T>(LGE);
        }

        #define try Lux::Exception::Try();{
        #define throw(x, y) return Lux::Exception::Throw<decltype(x), y>(x);
        #define catch(x, ...) }if(Lux::Exception::Catch(x, ##__VA_ARGS__))
        #define any }if(Lux::Exception::CatchAny())
        #define exception(x) enum class x : Lux::u32
        #define request(x) Lux::Exception::GetLastEx<x>() 
    }
}