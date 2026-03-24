# Simple-bash-utils

Reimplemented standard Unix utilities (cat, grep) in C with integrated automated testing scripts in Bash.

## 1. s21_cat

A full-featured implementation of the cat utility.

Functionality: Supports displaying, combining, and creating text files.
Implemented Flags:
    -b, --number-nonblank: Numbers only non-empty lines.
    -e, -E: Displays end-of-line characters as $.
    -n, --number: Numbers all output lines.
    -s, --squeeze-blank: Squeezes multiple adjacent blank lines.
    -t, -T: Displays tabs as ^I.
## 2. s21_grep

A robust version of the grep utility for pattern searching using regular expressions.

Library: Utilizes regex for pattern matching.
Implemented Flags:
Full support for basic flags: -e, -i, -v, -c, -l, -n.
Bonus features: Extended support for -h (suppress filenames), -s (suppress errors), -f (regex from file), and -o (matched parts only).

### Technical Specifications

Code Quality: Developed according to the Google C++ Style Guide.
Portability: Compliant with the POSIX.1-2017 standard; avoids legacy or obsolete functions.
Build System: Managed via Makefile with dedicated targets for each utility (s21_cat, s21_grep).
Architecture: Designed using structured programming principles with a focus on code reuse.
### Test Automation

The reliability of the utilities is ensured by a custom Bash automation suite:

Integration Testing: Automated scripts compare the output of s21_cat and s21_grep against the original Bash utilities.
Coverage: Tests cover all implemented flag variants, including complex combinations and various input values.
