# lf
 
compatible with Lua 5.4.6

Differences between lf and Lua:
- [ ] [class](#class)
- [ ] [table improvements](#tables)
- [x] [string improvements](#strings)

## Class

## Tables



## Strings

### String Indexing
Strings can be indexed like arrays, supporting both positive and negative indices. Out of range indexing will not cause errors, and instead return `nil`.
```lua
> str = "Hello World"
> str[5]
o
> str[-3]
r
> str[0]
nil
```
