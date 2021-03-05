#include <cstdint>
#include <cstdlib>
#include <type_traits>

namespace Lux
{
    using u32 = std::uint32_t;

    namespace Concepts
    {
        template<class T> concept IsEnum = __is_enum(T);

        template<class T, class U> class IsSameClass
        {
        public:
            static constexpr bool Value = false;
        };

        template<class T> class IsSameClass<T, T>
        {
        public:
            static constexpr bool Value = true;
        };

        template<class T, class U> concept IsSame = IsSameClass<T, U>::Value;
    }

    namespace Exception
    {
        constinit thread_local u32 GE = 0u;
        constinit thread_local u32 LGE = 0u;
        constinit thread_local bool HGE = false;

        inline constexpr void Try(bool& DHGE = HGE) noexcept
        {
            DHGE = true;
        }

        template<class T> inline constexpr const bool Throw(T et) noexcept requires Concepts::IsEnum<T> && Concepts::IsSame<u32, std::underlying_type_t<T>>
        {
            if(HGE)
            {
                HGE = false;
                GE = static_cast<u32>(et);
            }
            else
                std::abort();
            return true;
        }

        [[nodiscard]] inline constexpr const bool CatchAny(u32& DGE = GE) noexcept
        {
            return DGE != 0u;
        }

        template<class T, class... U> [[nodiscard]] inline constexpr const bool Catch(T et, U&&... ets) noexcept requires Concepts::IsEnum<T> && Concepts::IsSame<u32, std::underlying_type_t<T>>
        {
            if(static_cast<u32>(et) == GE)
            {
                LGE = GE;
                GE = 0u;
                return true;
            }
            else if constexpr(sizeof...(ets) > 0u)
                return Catch(ets...);
            else if(GE != 0u)
                std::abort();
            return false;
        }

        template<class T> [[nodiscard]] inline constexpr T GetLastEx() noexcept requires Concepts::IsEnum<T> && Concepts::IsSame<u32, std::underlying_type_t<T>>
        {
            return static_cast<T>(LGE);
        }

        #define try Lux::Exception::Try();{
        #define throw(x, y) return Lux::Exception::Throw(x) ? y : y
        #define nrethrow(x) Lux::Exception::Throw(x); return;
        #define catch(x, ...) }if(Lux::Exception::Catch(x, ##__VA_ARGS__))
        #define any }if(Lux::Exception::CatchAny())
        #define exception(x) enum class x : Lux::u32
        #define request(x) Lux::Exception::GetLastEx<x>()
    }
}