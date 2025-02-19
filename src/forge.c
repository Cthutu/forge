#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "file_ops.h"

static const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TT_INVALID: return "TT_INVALID";
        case TT_NUMBER:  return "TT_NUMBER";
        case TT_IDENT:   return "TT_IDENT";
        case TT_PUNCT:   return "TT_PUNCT";
        case TT_SPACE:   return "TT_SPACE";
        case TT_CHAR:    return "TT_CHAR";
        case TT_STRING:  return "TT_STRING";
        default:         return "UNKNOWN";
    }
}

static void print_buffer(const char* buffer, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        unsigned char c = buffer[i];
        TokenType type = c < 128 ? char_to_token[c] : TT_INVALID;
        const char* display_char;
        switch (c) {
            case '\n': display_char = "\\n"; break;
            case '\r': display_char = "\\r"; break;
            case '\t': display_char = "\\t"; break;
            case ' ':  display_char = "\\s"; break;
            default:   display_char = NULL;
        }
        if (display_char) {
            printf("%s: %s\n", display_char, token_type_to_string(type));
        } else {
            printf("%c: %s\n", c, token_type_to_string(type));
        }
    }
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    size_t size;
    char* content = read_file(argv[1], &size);
    if (!content) {
        return 1;
    }

    print_buffer(content, size);
    close_file(content);
    return 0;
}