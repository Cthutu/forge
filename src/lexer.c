#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static const char* token_type_strings[] = {
    [TT_INVALID] = "INVALID",       [TT_SPACE] = "SPACE",
    [TT_NUMBER] = "NUMBER",         [TT_INTEGER] = "INTEGER",
    [TT_FLOAT] = "FLOAT",           [TT_IDENT] = "IDENT",
    [TT_STRING] = "STRING",         [TT_CHAR] = "CHAR",
    [TT_PUNCT] = "PUNCT",           [TT_COLON] = "COLON",
    [TT_OPEN_PAREN] = "OPEN_PAREN", [TT_CLOSE_PAREN] = "CLOSE_PAREN",
    [TT_ARROW] = "ARROW",
};

Lexer lex_init(const u8* source, usize length)
{
    return ( Lexer ){
        .source = source, .length = length, .offset = 0, .line = 0, .column = 0
    };
}

void dump_token(Token token)
{
    printf("Token { type: %-12s, line: %3zu, col: %3zu, offset: %3zu, length: "
           "%2zu",
           token_type_strings[ token.type ],
           token.line + 1,   // Convert to 1-based for display
           token.column + 1, // Convert to 1-based for display
           token.offset,
           token.length);

    switch ( token.type ) {
        case TT_INTEGER:
            printf(", value: %lld", token.value.int_value);
            break;
        case TT_FLOAT:
            printf(", value: %f", token.value.float_value);
            break;
        case TT_IDENT:
        case TT_STRING:
        case TT_CHAR:
            printf(", value: '");
            for ( usize i = 0; i < token.value.string.len; i++ ) {
                u8 c = token.value.string.ptr[ i ];
                if ( c >= 32 && c <= 126 ) { // Printable ASCII
                    putchar(( char )c);
                } else {
                    printf("\\x%02x", c);
                }
            }
            printf("'");
            break;
        default:
            break;
    }

    printf(" }\n");
}

Token lex_token(Lexer* lexer)
{
    // Skip any whitespace, updating line/column info
    while ( lexer->offset < lexer->length ) {
        u8 c = lexer->source[ lexer->offset ];
        if ( c >= 128 || char_to_token[ c ] != TT_SPACE ) {
            break;
        }

        // Update position info
        if ( c == '\n' ) {
            lexer->line++;
            lexer->column = 0;
        } else {
            lexer->column++;
        }
        lexer->offset++;
    }

    // Check for end of input
    if ( lexer->offset >= lexer->length ) {
        return ( Token ){ .type   = TT_EOF,
                          .offset = lexer->offset,
                          .length = 0,
                          .line   = lexer->line,
                          .column = lexer->column };
    }

    // Store starting position for token
    usize     start_line   = lexer->line;
    usize     start_column = lexer->column;

    u8        c            = lexer->source[ lexer->offset ];
    TokenType type         = (c < 128) ? char_to_token[ c ] : TT_INVALID;

    Token     token;
    switch ( type ) {
        case TT_NUMBER:
            token = parse_number(lexer->source, lexer->length, lexer->offset);
            break;
        case TT_IDENT:
            token = parse_ident(lexer->source, lexer->length, lexer->offset);
            break;
        case TT_STRING:
            token = parse_string(lexer->source, lexer->length, lexer->offset);
            break;
        case TT_CHAR:
            token = parse_char(lexer->source, lexer->length, lexer->offset);
            break;
        case TT_PUNCT:
            // Handle multi-character operators first
            if ( c == '-' && lexer->offset + 1 < lexer->length &&
                 lexer->source[ lexer->offset + 1 ] == '>' )
            {
                token = ( Token ){ .type   = TT_ARROW,
                                   .offset = lexer->offset,
                                   .length = 2 };
                break;
            }

            // Handle single character operators
            switch ( c ) {
                case '(':
                    token = ( Token ){ .type   = TT_OPEN_PAREN,
                                       .offset = lexer->offset,
                                       .length = 1 };
                    break;
                case ')':
                    token = ( Token ){ .type   = TT_CLOSE_PAREN,
                                       .offset = lexer->offset,
                                       .length = 1 };
                    break;
                case ':':
                    token = ( Token ){ .type   = TT_COLON,
                                       .offset = lexer->offset,
                                       .length = 1 };
                    break;
                default:
                    token = ( Token ){ .type   = TT_PUNCT,
                                       .offset = lexer->offset,
                                       .length = 1 };
                    break;
            }
            break;
        default:
            token = ( Token ){ .type   = TT_INVALID,
                               .offset = lexer->offset,
                               .length = 1 };
            break;
    }

    // Update token with position info
    token.line     = start_line;
    token.column   = start_column;

    lexer->offset += token.length;
    // Update lexer position (column only, since newlines are handled in
    // whitespace)
    lexer->column += token.length;
    return token;
}

static Token parse_ident(const u8* source, usize length, usize offset)
{
    usize start = offset;
    offset++; // Move past first character which we know is valid

    // Continue while we see valid identifier characters
    while ( offset < length ) {
        u8 c = source[ offset ];
        if ( c >= 128 ) {
            break; // Early exit for non-ASCII
        }

        TokenType type = char_to_token[ c ];
        if ( type != TT_IDENT && type != TT_NUMBER ) {
            break;
        }

        offset++;
    }

    return ( Token ){ .type         = TT_IDENT,
                      .value.string = { .ptr = &source[ start ],
                                        .len = offset - start },
                      .offset       = start,
                      .length       = offset - start };
}

static Token parse_number(const u8* source, usize length, usize offset)
{
    usize start    = offset;
    bool  is_float = false;

    // Parse integer part
    while ( offset < length ) {
        u8 c = source[ offset ];
        if ( c < '0' || c > '9' ) {
            // Check for decimal point
            if ( c == '.' && !is_float && offset + 1 < length &&
                 source[ offset + 1 ] >= '0' && source[ offset + 1 ] <= '9' )
            {
                is_float = true;
                offset++;
                continue;
            }
            break;
        }
        offset++;
    }

    // Convert the string to a number
    TokenValue value = { 0 };
    usize      len   = offset - start;

    if ( is_float ) {
        char temp[ 32 ] = { 0 }; // Temporary buffer for string conversion
        if ( len < sizeof(temp) ) {
            for ( usize i = 0; i < len; i++ ) {
                temp[ i ] = ( char )source[ start + i ];
            }
            value.float_value = atof(temp);
        }
        return ( Token ){
            .type = TT_FLOAT, .value = value, .offset = start, .length = len
        };
    } else {
        char temp[ 32 ] = { 0 };
        if ( len < sizeof(temp) ) {
            for ( usize i = 0; i < len; i++ ) {
                temp[ i ] = ( char )source[ start + i ];
            }
            value.int_value = atoll(temp);
        }
        return ( Token ){
            .type = TT_INTEGER, .value = value, .offset = start, .length = len
        };
    }
}

static Token parse_string(const u8* source, usize length, usize offset)
{
    usize start = offset;
    offset++; // Skip opening quote

    // Scan until closing quote
    while ( offset < length ) {
        u8 c = source[ offset ];
        if ( c == '"' ) {
            offset++; // Include closing quote
            return (Token){
                .type = TT_STRING,
                .value.string = {
                    .ptr = &source[start + 1], // Skip opening quote
                    .len = offset - start - 2  // Exclude both quotes
                },
                .offset = start,
                .length = offset - start
            };
        }

        // Handle escape sequences
        if ( c == '\\' && offset + 1 < length ) {
            offset += 2; // Skip escape sequence
            continue;
        }

        // Error on newline in string
        if ( c == '\n' ) {
            return ( Token ){ .type   = TT_INVALID,
                              .offset = start,
                              .length = offset - start };
        }

        offset++;
    }

    // Unterminated string
    return ( Token ){ .type   = TT_INVALID,
                      .offset = start,
                      .length = offset - start };
}

static Token parse_char(const u8* source, usize length, usize offset)
{
    usize start = offset;
    offset++; // Skip opening quote

    // Empty character literal
    if ( offset >= length || source[ offset ] == '\'' ) {
        return ( Token ){ .type   = TT_INVALID,
                          .offset = start,
                          .length = offset - start + 1 };
    }

    // Handle escape sequence
    if ( source[ offset ] == '\\' ) {
        if ( offset + 2 >= length || source[ offset + 2 ] != '\'' ) {
            return ( Token ){ .type   = TT_INVALID,
                              .offset = start,
                              .length = (offset + 2 < length)
                                            ? offset - start + 3
                                            : length - start };
        }
        offset += 3; // Skip escape sequence and closing quote
    } else {
        // Regular character
        if ( offset + 1 >= length || source[ offset + 1 ] != '\'' ) {
            return ( Token ){ .type   = TT_INVALID,
                              .offset = start,
                              .length = (offset + 1 < length)
                                            ? offset - start + 2
                                            : length - start };
        }
        offset += 2; // Skip character and closing quote
    }

    return (Token){
        .type = TT_CHAR,
        .value.string = {
            .ptr = &source[start + 1], // Skip opening quote
            .len = offset - start - 2  // Exclude both quotes
        },
        .offset = start,
        .length = offset - start
    };
}
