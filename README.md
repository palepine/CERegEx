# CERegEx

A Cheat Engine plugin for scanning process memory with regular expressions.

## Features

- Scan selected memory regions using Cheat-Engine-like protection flags `RWXC`
- Multiple regex engines supported
- Return match addresses and matched text back to Lua
- Configuration:
  - encoding
  - regex engine
  - case sensitivity
  - minimum / maximum extracted string length
  - find-one mode

## Build Requirements

- Windows
- VS2026 with C++ desktop development tools
- `vcpkg`

## Dependency Layout

- `lua 5.3` (provided)
- `CE SDK v5.0.0` (provided)

### vcpkg dependencies

- `re2`
- `pcre2`

## Building

### 1. Install and integrate vcpkg

If you do not already have `vcpkg` integrated with Visual Studio:
`vcpkg integrate install`

### 2. Restore dependencies

From the solution directory, run:
`vcpkg install`

### 4. Build the project

- `Release | x64`

## Setup

1. Copy the compiled DLL to Cheat Engine\plugins
2. Open Cheat Engine
3. Go to `Edit -> Settings -> Plugins`
4. `Add new` the plugin DLL
5. Check the added plugin and hit `OK`

## Lua API

The plugin exposes a Lua entrypoint for regex-based memory scanning.

### Function

```lua
lregexScan( { pattern, protectionFlags, encoding, regexEngine, findOne, caseSensitive, minLength, maxLength } )
```

### Parameters

A dictionary table with the following fields (all optional except for pattern):
- `pattern`
  - Regex pattern string
- `protection`
  - Cheat Engine-style protection filter string
- `encoding`
  - String extraction encoding, such as `"ASCII"` or `"UTF16"`
- `engine`
  - Regex engine name, such as `"STD"`, `"RE2"`, or `"PCRE2"`
- `findOne`
  - Boolean flag that stops after the first match if enabled
- `caseSensitive`
  - Boolean flag for case-sensitive matching
- `minLength`
  - Minimum extracted candidate string length
- `maxLength`
  - Maximum extracted candidate string length

On success, it returns a table with dictionaries of two fields:
- `address`
  - Integer pointing to the start of the matched string in the target
- `text`
  - String containing a matched string

Returns `nil` on any failure or empty table if no matches

### Example

```lua
return lregexScan({
    pattern = [[Godot\sEngine\s]],
    protection = "WR-E-C",
    encoding = "ASCII",
    engine = "RE2",
    findOne = true,
    caseSensitive = true,
    minLength = 8,
    maxLength = 20
}) or {}
```

## Supported Encodings

Currently supported:
- `ASCII`
- `UTF-16LE BMP`

## Supported Regex Engines

- `STD` uses [std::regex](https://en.cppreference.com/w/cpp/regex.html)
- `RE2`
  - Faster and safer
  - Does not support the full PCRE-style feature set
  - Good for portable and simpler patterns
- `PCRE2`
  - Supports more advanced syntax
  - Suitable for patterns using features such as lookaheads

Engine-specific syntax is allowed when that engine is selected

## License
This project is released under the MIT License. See LICENSE for details.
