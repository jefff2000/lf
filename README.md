# lf
 
lf is a mod / extension of Lua 5.4.6

Differences between lf and Lua:
- [ ] [class](#class)
- [x] [table improvements](#tables)
- [x] [string improvements](#strings)

## Class

## Tables

### Undefined Table Members
Whenever a table member is followed by a semicolon, it will be declared as a key instead of a value. If the value is not defined, it is assumed to be `true`.
```lua
> t = { x = 1, y = 2, z = 3 } -- same behaviour as Lua
> for k, v in pairs(t) do
>> print(k, v)
>> end
y       2
z       3
x       1
```
```lua
> t = { x = 1; y = 2; z = 3; } -- same behaviour as Lua
> for k, v in pairs(t) do
>> print(k, v)
>> end
y       2
z       3
x       1
```
```lua
> t = { x = 1; y; z = 3; } -- y is declared as a key with a value of true
> for k, v in pairs(t) do
>> print(k, v)
>> end
y       true
z       3
x       1
```
```lua
> t = { 1, 2; 3; x = 4; y = 5, z; } -- numbers are unaffected and are not treated as keys
> for k, v in pairs(t) do
>> print(k, v)
>> end
1       1
2       2
3       3
y       5
z       true
x       4
```
```lua
> u = "u_key"
> t = { [u]; v = 2, x; ["y"] = 3; ["z"]; } -- square bracket keys are also assigned with true
> for k, v in pairs(t) do
>> print(k, v)
>> end
v       2
u_key   true
y       3
z       true
x       true
```
```lua
> a = function() return {} end
> b = {}
> t = { [a]; [b]; a; b; } -- the same also applies to non-string keys
> for k, v in pairs(t) do
>> print(k, v)
>> end
function: 000001B6F83AEAC0      true
b       true
a       true
table: 000001B6F83AFDB0 true
```

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
