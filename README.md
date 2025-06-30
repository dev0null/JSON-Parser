# Description
Simple JSON parser


## Usage

### class instance (Entry point)
```cpp
JsonParser parser(json);                // load JSON
JsonValue value = parser.parseValue();  // parse JSON
```
### results:
```cpp
printValue(value);          // provided by "utils.hpp"
```