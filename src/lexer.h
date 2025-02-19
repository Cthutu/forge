#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>

// Token type enumeration
typedef enum {
    TT_INVALID,  // Invalid or unrecognized character
    TT_NUMBER,   // Numeric literals (0-9)
    TT_IDENT,    // Identifiers (a-z, A-Z, _)
    TT_PUNCT,    // Punctuation and operators
    TT_SPACE,    // Whitespace characters
    TT_CHAR,     // Character literals ('x')
    TT_STRING,   // String literals ("x")
} TokenType;

// Initialize the character-to-token type mapping table
static const TokenType char_to_token[128] = {
    // Control characters (0-31), excluding whitespace
    [0 ... 8] = TT_INVALID,   // NUL through BS
    [11 ... 12] = TT_INVALID, // VT through FF
    [14 ... 31] = TT_INVALID, // SO through US

    // ASCII printable characters (32-126)
    [' ']  = TT_SPACE,
    ['\t'] = TT_SPACE,
    ['\n'] = TT_SPACE,
    ['\r'] = TT_SPACE,

    // Numbers
    ['0'] = TT_NUMBER, ['1'] = TT_NUMBER, ['2'] = TT_NUMBER, ['3'] = TT_NUMBER,
    ['4'] = TT_NUMBER, ['5'] = TT_NUMBER, ['6'] = TT_NUMBER, ['7'] = TT_NUMBER,
    ['8'] = TT_NUMBER, ['9'] = TT_NUMBER,

    // Lowercase letters
    ['a'] = TT_IDENT, ['b'] = TT_IDENT, ['c'] = TT_IDENT, ['d'] = TT_IDENT,
    ['e'] = TT_IDENT, ['f'] = TT_IDENT, ['g'] = TT_IDENT, ['h'] = TT_IDENT,
    ['i'] = TT_IDENT, ['j'] = TT_IDENT, ['k'] = TT_IDENT, ['l'] = TT_IDENT,
    ['m'] = TT_IDENT, ['n'] = TT_IDENT, ['o'] = TT_IDENT, ['p'] = TT_IDENT,
    ['q'] = TT_IDENT, ['r'] = TT_IDENT, ['s'] = TT_IDENT, ['t'] = TT_IDENT,
    ['u'] = TT_IDENT, ['v'] = TT_IDENT, ['w'] = TT_IDENT, ['x'] = TT_IDENT,
    ['y'] = TT_IDENT, ['z'] = TT_IDENT,

    // Uppercase letters
    ['A'] = TT_IDENT, ['B'] = TT_IDENT, ['C'] = TT_IDENT, ['D'] = TT_IDENT,
    ['E'] = TT_IDENT, ['F'] = TT_IDENT, ['G'] = TT_IDENT, ['H'] = TT_IDENT,
    ['I'] = TT_IDENT, ['J'] = TT_IDENT, ['K'] = TT_IDENT, ['L'] = TT_IDENT,
    ['M'] = TT_IDENT, ['N'] = TT_IDENT, ['O'] = TT_IDENT, ['P'] = TT_IDENT,
    ['Q'] = TT_IDENT, ['R'] = TT_IDENT, ['S'] = TT_IDENT, ['T'] = TT_IDENT,
    ['U'] = TT_IDENT, ['V'] = TT_IDENT, ['W'] = TT_IDENT, ['X'] = TT_IDENT,
    ['Y'] = TT_IDENT, ['Z'] = TT_IDENT,

    // Underscore (valid in identifiers)
    ['_'] = TT_IDENT,

    // Quote characters
    ['"'] = TT_STRING,
    ['\'']=  TT_CHAR,

    // Punctuation and operators
    ['!'] = TT_PUNCT, ['@'] = TT_PUNCT, ['#'] = TT_PUNCT, ['$'] = TT_PUNCT,
    ['%'] = TT_PUNCT, ['^'] = TT_PUNCT, ['&'] = TT_PUNCT, ['*'] = TT_PUNCT,
    ['('] = TT_PUNCT, [')'] = TT_PUNCT, ['-'] = TT_PUNCT, ['+'] = TT_PUNCT,
    ['='] = TT_PUNCT, ['{'] = TT_PUNCT, ['}'] = TT_PUNCT, ['['] = TT_PUNCT,
    [']'] = TT_PUNCT, ['|'] = TT_PUNCT, [';'] = TT_PUNCT, [':'] = TT_PUNCT,
    ['<'] = TT_PUNCT, ['>'] = TT_PUNCT, [','] = TT_PUNCT, ['.'] = TT_PUNCT,
    ['?'] = TT_PUNCT, ['/'] = TT_PUNCT, ['\\'] = TT_PUNCT,

    // DEL character
    [127] = TT_INVALID
};

#endif // LEXER_H