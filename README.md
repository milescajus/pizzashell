# pizzashell
PizzaSHELL (a.k.a.) pzash

## Features
### Implemented
- [x] Editable input, history, and basic autocompletion (through `<readline.h>`)
- [x] Pizza prompt 🍕 (shows current working directory and timestamp)
- [x] Piping!
- [x] Special character expansion (currently only `$` and `~`)
- [x] Whitespace stripping
- [x] Non-interactive execution (with `-c`)
- [x] True wildcard expansion/globbing (`*`)
- [x] Quote handling (especially for non-interactive mode)
    - both of the above implemented with `<wordexp.h>`

#### Built-ins
  - [x] `cd` with previous directory switching (`$ cd -`)
  - [x] Help function
  - [x] Two-operand math
  - [x] File info (simplified `lstat`)
  - [x] Echo

### TODO
- [ ] Expose `<math.h>` functions?
- [ ] Aliases (e.g. `ll=ls -l; ls=ls -g; grep=grep --color`)
  - especially for color-by-default
- [ ] Piping builtins
- [ ] Pizza ordering
