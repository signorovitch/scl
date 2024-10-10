#include "../src/include/lexer.h"
#include "unity/unity.h"
#include "registry.h"

void test_lexer_init() {
    char* src = malloc(sizeof("a1b2"));
    src = "a1b2";
    Lexer* lexer = lexer_init(src);

    TEST_ASSERT_EQUAL_STRING(src, lexer->src);
    TEST_ASSERT_EQUAL_INT(4, lexer->srcl);
    TEST_ASSERT_EQUAL_CHAR(src[0], *lexer->cchar);
    TEST_ASSERT_EQUAL_INT(LEXER_STATE_CONFUSED, lexer->state);
    // Hope that token arr. is right size :).
    TEST_ASSERT_EQUAL_INT(0, lexer->ntokens);
}

void test_lexer_lex_callnum() {
    char* src = malloc(sizeof("a1b2"));
    src = "a1b2";
    Lexer* lexer = lexer_init(src);

    Token* tokens[4] = {
        token_init(TOKEN_TYPE_CALL, "a"),
        token_init(TOKEN_TYPE_NUMBER, "1"),
        token_init(TOKEN_TYPE_CALL, "b"),
        token_init(TOKEN_TYPE_NUMBER, "2"),
    };

    lexer_lex(lexer);

    TEST_ASSERT_EQUAL_INT(4, lexer->ntokens);

    for (int i = 0; i < 4; i++) {
        printf("h");
        fflush(stdout);
        TEST_ASSERT_EQUAL_INT(tokens[i]->type, lexer->tokens[i]->type);
        TEST_ASSERT_EQUAL_STRING(tokens[i]->val, lexer->tokens[i]->val);
    }
}

int test_lexer() {
    UNITY_BEGIN();
    RUN_TEST(test_lexer_init);
    RUN_TEST(test_lexer_lex_callnum);
    UNITY_END();
    return 0;
}

__attribute__((constructor)) void register_lexer() {
    register_test(test_lexer);
}
