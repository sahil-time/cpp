- Currently using c++17 until the XCODE Clang is updated to Clang17 that has implemented the c++23 Standard
- Turning off all Optimizations i.e. O0

`clang++ -std=c++17 -O0 -Wall -Wextra -Wpedantic <input_file> -o <output_file>`

### CS QUOTES

#### Performance is the currency of computing - Charles Leiserson
> *Very many software properties are introduced by sacrificing performance. Like you want your program to be more secure, so let us sacrifice some performance. You want the program to be very extensible/maintainable/debuggable so let us sacrifice some performance. Performance is therefore a currency you trade for features. Therefore you NEED to care about performance, because you will ALWAYS need currency.*
>
> *Performance might not have an intrinsic value like the dollar, BUT you can use it to buy things that do have an intrinsic value.
    Back when the Moore’s law was in full throttle till 2004, if you wanted faster software, you just had to wait a few years for doubly-fast hardware. That would automatically speed up the software and performance therefore was NOT a major concern for developers at the time. 20 years later, Performance bottlenecks are real and developers NEED to understand it well.*

#### Distributed System is a System where I cannot get my work done because some computer somewhere has failed! - Leslie Lamport
> *Martin Kleppmann defines a Distributed System as follows:*
>
> 1. *Running on several nodes that are connected by a network*
> 2. *Characterized by Partial Failure*
>
> *Failure is endemic to the Distributed System*
>
> *Peter Alvaro says "Distributed Systems = Partial Failure + Unbounded Latency"*
