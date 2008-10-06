#ifndef CIRCA_TOKEN_STREAM_INCLUDED
#define CIRCA_TOKEN_STREAM_INCLUDED

#include <set>

#include "syntax_errors.h"
#include "tokenizer.h"

namespace circa {
namespace token_stream {

struct TokenStream
{
    tokenizer::TokenList tokens;
    unsigned int currentIndex;
    std::set<const char*> skipSet;

    TokenStream(tokenizer::TokenList const& _tokens)
      : tokens(_tokens), currentIndex(0)
    {
    }

    TokenStream(std::string const& input)
      : tokens(tokenizer::tokenize(input)), currentIndex(0)
    {
    }

    void reset(std::string const& input)
    {
        tokens = tokenizer::tokenize(input);
        currentIndex = 0;
    }

    void stripWhitespace()
    {
        int deleteCount = 0;

        for (int i=0; i < (int) tokens.size(); i++) {

            if (tokens[i].match == tokenizer::WHITESPACE) {
                deleteCount++;
            } else if (deleteCount > 0) {
                tokens[i - deleteCount] = tokens[i];
            }
        }

        tokens.resize(tokens.size() - deleteCount);
    }

    tokenizer::TokenInstance const& next(int lookahead=0) const
    {
        unsigned int i = this->currentIndex + lookahead;

        if (i >= tokens.size())
            throw syntax_errors::SyntaxError("Unexpected EOF");

        return tokens[i];
    }

    bool nextIs(int match, int lookahead=0) const
    {
        if ((this->currentIndex + lookahead) >= tokens.size())
            return false;
            
        return next(lookahead).match == match;
    }

    std::string consume(int match = -1)
    {
        if (finished())
            throw syntax_errors::SyntaxError(
                    std::string("Unexpected EOF while looking for ") + tokenizer::getMatchText(match));

        if ((match != -1) && next().match != match)
            throw syntax_errors::UnexpectedToken(match, next().match, next().text.c_str());

        return tokens[currentIndex++].text;
    }

    bool nextNonWhitespaceIs(int match, int lookahead=0) const
    {
        int index = this->currentIndex;

        while (true) {

            if (index >= (int) tokens.size())
                return false;

            if (tokens[index].match == tokenizer::WHITESPACE) {
                index++;
                continue;
            }

            if (lookahead == 0)
                return tokens[index].match == match;

            lookahead--;
            index++;
        }
    }

    bool finished() const
    {
        return (currentIndex >= tokens.size());
    }
};

} // namespace token_stream
} // namespace circa

#endif
