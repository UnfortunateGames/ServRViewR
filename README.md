# ServRViewR

> Check out the original [ServRLookR](https://github.com/UnfortunateGames/ServRLookR)

## About ServRViewR

It is a faithful replica of ServeRLookR. Same functionality, with C++ boundaries

> This is also the last in the C++ quarter

## My Goals

[x] - Learn the differences between Rust and C++
[x] - Use more STL
[x] - ~~Forget basic C++ file manipulation~~
[x] - *Suffer*
[ ] - Impress papa and friends

## Review

This was a hell of a ride to make.

No seriously, I have procrastinated a lot on this, to the point I question how I finished
this early.

The expected end for this was on late-March, maybe a bit March 27-30. It was also a long
ride to debug it, like no shit, I made this blindly and the only part that was not working
was something I overlooked and didn't fully finalize.

Either way, many things were on point on my thought of what C++ is, and thought to be.

The first problem when I finished designing it was the switch case not accepting
uninitialized variables and such. To add on this, I also found a problem, which I learned
something important to not do. Optional actually doesn't accept addresses, I may have
known about this if I read some documentation, but it's just a small roadblock I had to
learn once. I fixed it via simple pointer use, `nullptr`, or `*Server` could be returned,
and be the same pointer.

Now, the last bug I had to handle was the command splitting. The design was really janky.
When I made it, I didn't think much of it. But it really was a bigger mess than I thought,
I tried fixing it mulitple times, but what I didn't know was that the design was really
flawed. I didn't think it through, and edge cases were making it a mess.

And then I just put the logic in another file in pseudocode... I saw the issues immediently,
and refactored it that way. I thought to myself, this happened 2 times! One in the switch
cases where it was really dense in code that I get demoralized and don't really wanna fix it
anymore. Now it's in this small part of the code, where I get overwhelmed with syntax.

Maybe this is special to C++, or not? It's just that Rust never made me feel that way, at
least this much, though this isn't by any means "C++ is bad, Rust better", it's just
the fair share of trades the languages have is uncomfortably different.
