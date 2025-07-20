## Description


---
## Brainstorming - JSON Parser Logic 

### JSON Rules

1. A key always appears inside of an object (`{}`)
2. A key is followed by a colon `:` and then a value
3. The value can be:
   1. A string -- e.g. `"hello"`
   2. A number -- e.g. `21`
   3. A boolean -- e.g. `true`
   4. `null`
   5. Another object (which has its own keys/values)
   6. An array, which contains multiple values (possibly nested too)
4. Every time you parse a value, check if it's a nested container
   1. If it starts with `{` -> it's an object -> recurse to parse keys/values
   2. If it starts with `[` -> it's an array -> recurse to parse list items
   3. Else -> it's a literal (number, string, etc...)

### Reminder

* A key always belongs to an object (`{key: value}`)
* A value may be a terminal value (like a string/number) or a nested container (array/object)
* Your parser should keep going deeper (recursing) only when the value itself is a container (`{}` or `[]`)

### Note-1

If we're parsing an object and we find a key, then we stop treating it as a container and expect a value next — that value may itself be an object, in which case we recurse again.

### Note-2

If we find a key we would add it to the Jason object as a key, otherwise we would add it as a value to the previously key found.

### Note-3

If we already found a key and the associated value, then the next key found would be a sibling of the previous key.


