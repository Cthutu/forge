#include <stdio.h>
#include <stdlib.h>

#include "file_ops.h"
#include "lexer.h"

static const char* token_type_to_string(TokenType type)
{
    switch ( type ) {
        case TT_INVALID:
            return "TT_INVALID";
        case TT_NUMBER:
            return "TT_NUMBER";
        case TT_IDENT:
            return "TT_IDENT";
        case TT_PUNCT:
            return "TT_PUNCT";
        case TT_SPACE:
            return "TT_SPACE";
        case TT_CHAR:
            return "TT_CHAR";
        case TT_STRING:
            return "TT_STRING";
        default:
            return "UNKNOWN";
    }
}

static void print_buffer(const char* buffer$, size_t size)
{
    printf("File contents:\n%.*s\n", ( int )size, buffer$);
}

int main(int argc, char** argv)
{
    if ( argc != 2 ) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[ 0 ]);
        return 1;
    }

    size_t size;
    char*  content = read_file(argv[ 1 ], &size);
    if ( !content ) {
        return 1;
    }

    // Print file contents
    print_buffer(content, size);

    // Print separator bar
    printf("\n---------------------------------------------------------"
           "\nTokens:\n");

    // Process all tokens
    Lexer lexer = lex_init(( const u8* )content, size);
    for ( ;; ) {
        Token token = lex_token(&lexer);
        if ( token.type == TT_EOF ) {
            break;
        }
        dump_token(token);
    }

    close_file(content);
    return 0;
}
