/* =============================================================================
 * NewPython Tokenizer Implementation (Frictionless Edition)
 * Integrated Bracket String Support directly into the top-level macros.
 * ============================================================================= */

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "pycore_call.h"
#include <assert.h>
#include <ctype.h>
#include "errcode.h"
#include "tokenizer.h"

/* Frictionless Bracket String Parser Hook */
#define MAKE_TOKEN(token_type)                                                 \
    ((tok->cur[-1] == '[') ? tok_parse_bracket_string(tok, token) :            \
     token_setup(tok, token, token_type, p_start, p_end))

static inline int tok_parse_bracket_string(struct tok_state *tok, struct token *token) {
    tok->start = tok->cur - 1;
    int c;
    while ((c = tok_nextc(tok)) != ']' && c != EOF && c != '\n') {
        /* Absorb characters inside brackets continuously */
    }
    tok->end = tok->cur;
    return token_setup(tok, token, STRING, tok->start, tok->end);
}

#define ALTTABSIZE 1
#define is_potential_identifier_start(c)                                       \
    ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= 128))
#define is_potential_identifier_char(c)                                        \
    ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||                         \
     (c >= '0' && c <= '9') || c == '_' || (c >= 128))
