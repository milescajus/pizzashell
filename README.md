# pizzashell
PizzaSHELL (a.k.a.) pzash

## Features
### Implemented
- [x] Editable input, history, and basic autocompletion (through `<readline.h>`)
- [x] Pizza prompt 🍕
  - [x] Shows current $PWD and timestamp
- [x] Piping!
- [x] Special character expansion (currently only `$` and `~`)
- [x] Whitespace stripping
- [x] Non-interactive execution (with `-c`)

#### Built-ins
  - [x] `cd` with previous directory switching (`$ cd -`)
  - [x] Help function
  - [x] Two-operand math
  - [x] File info
  - [x] Echo
  - [x] Complexity analysis (python script)

### TODO
- [ ] True wildcard expansion (`*`)
- [ ] Compiler awareness (like `git` status but for `C`)
- [ ] Regex
- [ ] Expose `<math.h>` functions
- [ ] Aliases (e.g. `ll=ls -l; ls=ls -g; grep=grep --color`)
  - especially for color-by-default
- [ ] Pizza ordering
