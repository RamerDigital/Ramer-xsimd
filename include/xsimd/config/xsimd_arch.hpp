/***************************************************************************
 * Copyright (c) Johan Mabille, Sylvain Corlay, Wolf Vollprecht and         *
 * Martin Renou                                                             *
 * Copyright (c) QuantStack                                                 *
 * Copyright (c) Serge Guelton                                              *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef XSIMD_ARCH_HPP
#define XSIMD_ARCH_HPP

#include "../types/xsimd_all_registers.hpp"
#include "./xsimd_config.hpp"
#include "./xsimd_cpuid.hpp"

#include <initializer_list>
#include <type_traits>
#include <utility>

namespace xsimd
{

    /**
     * @ingroup architectures
     *
     * Dummy architectures that only appears in a list of architecture when no
     * other architecture has been detected.
     */
    struct unavailable
    {
        static constexpr bool supported() noexcept { return false; }
        static constexpr bool available() noexcept { return false; }
        static constexpr std::size_t alignment() noexcept { return 0; }
        static constexpr bool requires_alignment() noexcept { return false; }
        static constexpr char const* name() noexcept { return "<none>"; }
    };

    namespace detail
    {
        // Checks whether T appears in Tys.
        template <class T, class... Tys>
        struct contains;

        template <class T>
        struct contains<T> : std::false_type
        {
        };

        template <class T, class Ty, class... Tys>
        struct contains<T, Ty, Tys...>
            : std::conditional_t<std::is_same<Ty, T>::value, std::true_type,
                                 contains<T, Tys...>>
        {
        };

        template <typename T>
        XSIMD_INLINE constexpr T max_of(T value) noexcept
        {
            return value;
        }

        template <typename T, typename... Ts>
        XSIMD_INLINE constexpr T max_of(T head0, T head1, Ts... tail) noexcept
        {
            return max_of((head0 > head1 ? head0 : head1), tail...);
        }

        template <typename... Ts>
        struct head;

        template <typename T, typename... Ts>
        struct head<T, Ts...>
        {
            using type = T;
        };

        template <>
        struct head<>
        {
            using type = unavailable;
        };

    } // namespace detail

    // An arch_list is a list of architectures.
    template <class... Archs>
    struct arch_list
    {
        using best = typename detail::head<Archs...>::type;

        template <class Arch>
        using add = arch_list<Archs..., Arch>;

        template <class... OtherArchs>
        using extend = arch_list<Archs..., OtherArchs...>;

        template <class Arch>
        static constexpr bool contains() noexcept
        {
            return detail::contains<Arch, Archs...>::value;
        }

        template <class F>
        static XSIMD_INLINE void for_each(F&& f) noexcept
        {
            (void)std::initializer_list<bool> { (f(Archs {}), true)... };
        }

        static constexpr std::size_t alignment() noexcept
        {
            // all alignments are a power of two
            return detail::max_of(Archs::alignment()..., static_cast<size_t>(0));
        }
    };

    namespace detail
    {

        // Filter archlists Archs, picking only supported archs and adding
        // them to L.
        template <class L, class... Archs>
        struct supported_helper;

        template <class L>
        struct supported_helper<L, arch_list<>>
        {
            using type = L;
        };

        template <class L, class Arch, class... Archs>
        struct supported_helper<L, arch_list<Arch, Archs...>>
            : supported_helper<
                  std::conditional_t<Arch::supported(),
                                     typename L::template add<Arch>, L>,
                  arch_list<Archs...>>
        {
        };

        template <class... Archs>
        struct supported : supported_helper<arch_list<>, Archs...>
        {
        };

        // Joins all arch_list Archs in a single arch_list.
        template <class... Archs>
        struct join;

        template <class Arch>
        struct join<Arch>
        {
            using type = Arch;
        };

        template <class Arch, class... Archs, class... Args>
        struct join<Arch, arch_list<Archs...>, Args...>
            : join<typename Arch::template extend<Archs...>, Args...>
        {
        };
    } // namespace detail

    using all_x86_architectures = arch_list<>;
    using all_sve_architectures = arch_list<>;
    using all_rvv_architectures = arch_list<>;
    using all_arm_architectures = arch_list<i8mm<neon64>, neon64, neon>;
    using all_power_architectures = arch_list<>;
    using all_riscv_architectures = arch_list<>;
    using all_wasm_architectures = arch_list<>;
    using all_s390x_architectures = arch_list<>;
    using all_architectures = all_arm_architectures;

    using supported_architectures = typename detail::supported<all_architectures>::type;

    using x86_arch = unavailable;
    using arm_arch = typename detail::supported<all_arm_architectures>::type::best;
    using power_arch = unavailable;
    using riscv_arch = unavailable;
    using s390x_arch = unavailable;
    using best_arch = typename supported_architectures::best;

#ifdef XSIMD_DEFAULT_ARCH
    using default_arch = XSIMD_DEFAULT_ARCH;
#else
    using default_arch = best_arch;
#endif

    namespace detail
    {
        template <class F, class ArchList>
        class dispatcher
        {

            const decltype(available_architectures()) availables_archs;
            F functor;

            template <class Arch, class... Tys>
            XSIMD_INLINE auto walk_archs(arch_list<Arch>, Tys&&... args) noexcept
            {
                assert(Arch::available() && "At least one arch must be supported during dispatch");
                return functor(Arch {}, std::forward<Tys>(args)...);
            }

            template <class Arch, class ArchNext, class... Archs, class... Tys>
            XSIMD_INLINE auto walk_archs(arch_list<Arch, ArchNext, Archs...>, Tys&&... args) noexcept
            {
                if (availables_archs.has(Arch {}))
                    return functor(Arch {}, std::forward<Tys>(args)...);
                else
                    return walk_archs(arch_list<ArchNext, Archs...> {}, std::forward<Tys>(args)...);
            }

        public:
            XSIMD_INLINE dispatcher(F f) noexcept
                : availables_archs(available_architectures())
                , functor(f)
            {
            }

            template <class... Tys>
            XSIMD_INLINE auto operator()(Tys&&... args) noexcept
            {
                return walk_archs(ArchList {}, std::forward<Tys>(args)...);
            }
        };
    }

    // Generic function dispatch, à la ifunc
    template <class ArchList = supported_architectures, class F>
    XSIMD_INLINE detail::dispatcher<F, ArchList> dispatch(F&& f) noexcept
    {
        return { std::forward<F>(f) };
    }

} // namespace xsimd

#endif
