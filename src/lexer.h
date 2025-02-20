#pragma once

#include "forge.h"

// Token type enumeration
typedef enum
{
    TT_EOF, // End of file marker

    // These enums are used to initially categorise an incoming character
    TT_INVALID, // Invalid or unrecognized character
    TT_NUMBER,  // Numeric literals (0-9)
    TT_IDENT,   // Identifiers (a-z, A-Z, _)
    TT_PUNCT,   // Punctuation and operators
    TT_SPACE,   // Whitespace characters
    TT_CHAR,    // Character literals ('x')
    TT_STRING,  // String literals ("x")

    // These enums are used to categorise a token after lexing
    TT_INTEGER, // Integer literals
    TT_FLOAT,   // Floating-point literals

    // Operators
    TT_COLON,       // Colon (:)
    TT_OPEN_PAREN,  // Open parenthesis
    TT_CLOSE_PAREN, // Close parenthesis
    TT_ARROW,       // Arrow (->)
} TokenType;

// Token value union to store different types of token data
typedef union
{
    i64 int_value;
    f64 float_value;
    struct
    {
        const u8* ptr;
        usize     len;
    } string;
} TokenValue;

// Token structure containing type, value, and position information
typedef struct
{
    TokenType  type;
    TokenValue value;
    usize      offset; // Start position in source
    usize      length; // Length of token in source
    usize      line;   // 0-based line number
    usize      column; // 0-based column number
} Token;

typedef struct
{
    const u8* source; // Input buffer
    usize     length; // Total length of source
    usize     offset; // Current position in source
    usize     line;   // Current line (0-based)
    usize     column; // Current column (0-based)
} Lexer;

// Initialize the character-to-token type mapping table
static const TokenType char_to_token[ 128 ] = {
    // Control characters (0-31), excluding whitespace
    [0]    = TT_INVALID, // NUL
    [1]    = TT_INVALID, // SOH
    [2]    = TT_INVALID, // STX
    [3]    = TT_INVALID, // ETX
    [4]    = TT_INVALID, // EOT
    [5]    = TT_INVALID, // ENQ
    [6]    = TT_INVALID, // ACK
    [7]    = TT_INVALID, // BEL
    [8]    = TT_INVALID, // BS
    [11]   = TT_INVALID, // VT
    [12]   = TT_INVALID, // FF
    [14]   = TT_INVALID, // SO
    [15]   = TT_INVALID, // SI
    [16]   = TT_INVALID, // DLE
    [17]   = TT_INVALID, // DC1
    [18]   = TT_INVALID, // DC2
    [19]   = TT_INVALID, // DC3
    [20]   = TT_INVALID, // DC4
    [21]   = TT_INVALID, // NAK
    [22]   = TT_INVALID, // SYN
    [23]   = TT_INVALID, // ETB
    [24]   = TT_INVALID, // CAN
    [25]   = TT_INVALID, // EM
    [26]   = TT_INVALID, // SUB
    [27]   = TT_INVALID, // ESC
    [28]   = TT_INVALID, // FS
    [29]   = TT_INVALID, // GS
    [30]   = TT_INVALID, // RS
    [31]   = TT_INVALID, // US

    [' ']  = TT_SPACE, // Space (32)
    ['\t'] = TT_SPACE, // Tab (9)
    ['\n'] = TT_SPACE, // Newline (10)
    ['\r'] = TT_SPACE, // Carriage return (13)

    // Numbers
    ['0']  = TT_NUMBER,
    ['1']  = TT_NUMBER,
    ['2']  = TT_NUMBER,
    ['3']  = TT_NUMBER,
    ['4']  = TT_NUMBER,
    ['5']  = TT_NUMBER,
    ['6']  = TT_NUMBER,
    ['7']  = TT_NUMBER,
    ['8']  = TT_NUMBER,
    ['9']  = TT_NUMBER,

    // Lowercase letters
    ['a']  = TT_IDENT,
    ['b']  = TT_IDENT,
    ['c']  = TT_IDENT,
    ['d']  = TT_IDENT,
    ['e']  = TT_IDENT,
    ['f']  = TT_IDENT,
    ['g']  = TT_IDENT,
    ['h']  = TT_IDENT,
    ['i']  = TT_IDENT,
    ['j']  = TT_IDENT,
    ['k']  = TT_IDENT,
    ['l']  = TT_IDENT,
    ['m']  = TT_IDENT,
    ['n']  = TT_IDENT,
    ['o']  = TT_IDENT,
    ['p']  = TT_IDENT,
    ['q']  = TT_IDENT,
    ['r']  = TT_IDENT,
    ['s']  = TT_IDENT,
    ['t']  = TT_IDENT,
    ['u']  = TT_IDENT,
    ['v']  = TT_IDENT,
    ['w']  = TT_IDENT,
    ['x']  = TT_IDENT,
    ['y']  = TT_IDENT,
    ['z']  = TT_IDENT,

    // Uppercase letters
    ['A']  = TT_IDENT,
    ['B']  = TT_IDENT,
    ['C']  = TT_IDENT,
    ['D']  = TT_IDENT,
    ['E']  = TT_IDENT,
    ['F']  = TT_IDENT,
    ['G']  = TT_IDENT,
    ['H']  = TT_IDENT,
    ['I']  = TT_IDENT,
    ['J']  = TT_IDENT,
    ['K']  = TT_IDENT,
    ['L']  = TT_IDENT,
    ['M']  = TT_IDENT,
    ['N']  = TT_IDENT,
    ['O']  = TT_IDENT,
    ['P']  = TT_IDENT,
    ['Q']  = TT_IDENT,
    ['R']  = TT_IDENT,
    ['S']  = TT_IDENT,
    ['T']  = TT_IDENT,
    ['U']  = TT_IDENT,
    ['V']  = TT_IDENT,
    ['W']  = TT_IDENT,
    ['X']  = TT_IDENT,
    ['Y']  = TT_IDENT,
    ['Z']  = TT_IDENT,

    // Underscore (valid in identifiers)
    ['_']  = TT_IDENT,

    // Quote characters
    ['"']  = TT_STRING,
    ['\''] = TT_CHAR,

    // Punctuation and operators
    ['!']  = TT_PUNCT,
    ['@']  = TT_PUNCT,
    ['#']  = TT_PUNCT,
    ['$']  = TT_PUNCT,
    ['%']  = TT_PUNCT,
    ['^']  = TT_PUNCT,
    ['&']  = TT_PUNCT,
    ['*']  = TT_PUNCT,
    ['(']  = TT_PUNCT,
    [')']  = TT_PUNCT,
    ['-']  = TT_PUNCT,
    ['+']  = TT_PUNCT,
    ['=']  = TT_PUNCT,
    ['{']  = TT_PUNCT,
    ['}']  = TT_PUNCT,
    ['[']  = TT_PUNCT,
    [']']  = TT_PUNCT,
    ['|']  = TT_PUNCT,
    [';']  = TT_PUNCT,
    [':']  = TT_PUNCT,
    ['<']  = TT_PUNCT,
    ['>']  = TT_PUNCT,
    [',']  = TT_PUNCT,
    ['.']  = TT_PUNCT,
    ['?']  = TT_PUNCT,
    ['/']  = TT_PUNCT,
    ['\\'] = TT_PUNCT,
    ['~']  = TT_PUNCT,

    // DEL character
    [127]  = TT_INVALID
};

// Main lexer function declaration
Lexer        lex_init(const u8* source, usize length);
Token        lex_token(Lexer* lexer);

// Forward declarations for parsing routines
static Token parse_number(const u8* source, usize length, usize offset);
static Token parse_ident(const u8* source, usize length, usize offset);
static Token parse_string(const u8* source, usize length, usize offset);
static Token parse_char(const u8* source, usize length, usize offset);

// Add to header file
void         dump_token(Token token);
