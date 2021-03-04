# LuxExceptions
A sub-repository about exceptions of Lux, the upcoming game engine SDK


I made an constexpr exception system(kinda), which doesn't use standard exceptions(requires C++20).

It's dynamic(but static), so users can define their own "exceptions" too, without runtime overhead.

<https://godbolt.org/z/fMq7P7>

that's one example, playing with 'any'. which "catches" any exception
then request simply retries the specific thrown exception

as you can see, 'Ex1' is defined later, so it works with "Exceptions" defined by the user, making it a dynamic(but static) system
also, it's thread_local, so every exception thrown in one threads is local to such thread
and, if you didn't catch the specific thrown exception, it will abort <https://godbolt.org/z/EM1P1r>

now, about limitations... there's only one that i consider significant, you can't have multiple catch's consecutively to each other
BUT, you can catch multiple exceptions at once, and call request to get the specific thrown exception
like <https://godbolt.org/z/641of9>
