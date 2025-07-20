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

### Notes

#### Note-1

If we're parsing an object and we find a key, then we stop treating it as a container and expect a value next — that value may itself be an object, in which case we recurse again.

#### Note-2

If we find a key we would add it to the Jason object as a key, otherwise we would add it as a value to the previously key found.

#### Note-3

If we already found a key and the associated value, then the next key found would be a sibling of the previous key.

### Psuedocode

1. Parse until we find a key
2. Once a key is found, look for its value (which is after the colon)
3. Add the value found to the JSON parse object as the value of the key if it's a primitive value (i.e. integer/number, string, boolean, or null)
4. Otherwise, treat the value as an object or array (after checking for { or [), and then find the values that aren't also keys
5. Add the object/array as a value to the previously found key
6. The next key find will be a sibling of the prior key
7. Repeat steps #1-6

### Error Handling 

If any of the below is found while parsing, exit immediately and return an error:
1. Missing `:` after key
2. Unclosed `{` or `[`
3. Unquoted keys
4. Incorrect strings or numbers
5. Trailing commas (extra comma at the end of a list or object)


