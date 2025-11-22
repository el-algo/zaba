#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>

#include <filesystem>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>

enum TokenType
{
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COLON,
    TOKEN_DOT,
    TOKEN_DOTDOT,
    TOKEN_DOTDOTDOT,
    TOKEN_COMMA,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_HASH,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_LTLT,
    TOKEN_GTGT,
    TOKEN_PIPE,
    TOKEN_PIPEPIPE,
    TOKEN_CARET,
    TOKEN_AMP,
    TOKEN_AMPAMP,
    TOKEN_BANG,
    TOKEN_TILDE,
    TOKEN_QUESTION,
    TOKEN_EQ,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LTEQ,
    TOKEN_GTEQ,
    TOKEN_EQEQ,
    TOKEN_BANGEQ,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_CLASS,
    TOKEN_CONSTRUCT,
    TOKEN_ELSE,
    TOKEN_FALSE,
    TOKEN_FOR,
    TOKEN_FOREIGN,
    TOKEN_IF,
    TOKEN_IMPORT,
    TOKEN_AS,
    TOKEN_IN,
    TOKEN_IS,
    TOKEN_NULL,
    TOKEN_RETURN,
    TOKEN_STATIC,
    TOKEN_STRUCT,
    TOKEN_SUPER,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VAR,
    TOKEN_WHILE,
    TOKEN_FIELD,
    TOKEN_STATIC_FIELD,
    TOKEN_NAME,
    TOKEN_FUNC,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_INTERPOLATION,
    TOKEN_LINE,
    TOKEN_ERROR,
    TOKEN_EOF
};

struct Token
{
    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;

    Token() = default;

    Token(TokenType type, std::string lexeme, std::string literal, int line)
        : type{type}, lexeme{lexeme}, literal{literal}, line{line}
    {
    }

    std::string typePrint(TokenType type)
    {
        static std::string names[]{
            "TOKEN_LEFT_PAREN",
            "TOKEN_RIGHT_PAREN",
            "TOKEN_LEFT_BRACKET",
            "TOKEN_RIGHT_BRACKET",
            "TOKEN_LEFT_BRACE",
            "TOKEN_RIGHT_BRACE",
            "TOKEN_COLON",
            "TOKEN_DOT",
            "TOKEN_DOTDOT",
            "TOKEN_DOTDOTDOT",
            "TOKEN_COMMA",
            "TOKEN_STAR",
            "TOKEN_SLASH",
            "TOKEN_PERCENT",
            "TOKEN_HASH",
            "TOKEN_PLUS",
            "TOKEN_MINUS",
            "TOKEN_LTLT",
            "TOKEN_GTGT",
            "TOKEN_PIPE",
            "TOKEN_PIPEPIPE",
            "TOKEN_CARET",
            "TOKEN_AMP",
            "TOKEN_AMPAMP",
            "TOKEN_BANG",
            "TOKEN_TILDE",
            "TOKEN_QUESTION",
            "TOKEN_EQ",
            "TOKEN_LT",
            "TOKEN_GT",
            "TOKEN_LTEQ",
            "TOKEN_GTEQ",
            "TOKEN_EQEQ",
            "TOKEN_BANGEQ",
            "TOKEN_BREAK",
            "TOKEN_CONTINUE",
            "TOKEN_CLASS",
            "TOKEN_CONSTRUCT",
            "TOKEN_ELSE",
            "TOKEN_FALSE",
            "TOKEN_FOR",
            "TOKEN_FOREIGN",
            "TOKEN_IF",
            "TOKEN_IMPORT",
            "TOKEN_AS",
            "TOKEN_IN",
            "TOKEN_IS",
            "TOKEN_NULL",
            "TOKEN_RETURN",
            "TOKEN_STATIC",
            "TOKEN_STRUCT",
            "TOKEN_SUPER",
            "TOKEN_THIS",
            "TOKEN_TRUE",
            "TOKEN_VAR",
            "TOKEN_WHILE",
            "TOKEN_FIELD",
            "TOKEN_STATIC_FIELD",
            "TOKEN_NAME",
            "TOKEN_FUNC",
            "TOKEN_NUMBER",
            "TOKEN_STRING",
            "TOKEN_INTERPOLATION",
            "TOKEN_LINE",
            "TOKEN_ERROR",
            "TOKEN_EOF"};

        int t = static_cast<int>(type);
        if (t < 0 || t >= (int)(sizeof(names) / sizeof(names[0])))
            return "TOKEN_UNKNOWN";

        return names[t];
    }

    std::string toString()
    {
        return typePrint(type) + " " + lexeme + "\n";
    }
};

struct Scanner
{
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    Scanner() = default;
    Scanner(std::string source)
        : source{source} {}

    std::unordered_map<std::string, TokenType> keywords = {
        {"and", TOKEN_AMPAMP}, // Extension of &&
        {"break", TOKEN_BREAK},
        {"continue", TOKEN_CONTINUE},
        {"class", TOKEN_CLASS},
        {"construct", TOKEN_CONSTRUCT},
        {"else", TOKEN_ELSE},
        {"false", TOKEN_FALSE},
        {"for", TOKEN_FOR},
        {"foreign", TOKEN_FOREIGN},
        {"if", TOKEN_IF},
        {"import", TOKEN_IMPORT},
        {"as", TOKEN_AS},
        {"in", TOKEN_IN},
        {"is", TOKEN_IS},
        {"null", TOKEN_NULL},
        {"or", TOKEN_PIPEPIPE}, // Extension of ||
        {"return", TOKEN_RETURN},
        {"static", TOKEN_STATIC},
        {"struct", TOKEN_STRUCT},
        {"super", TOKEN_SUPER},
        {"this", TOKEN_THIS},
        {"true", TOKEN_TRUE},
        {"var", TOKEN_VAR},
        {"while", TOKEN_WHILE},
        {"func", TOKEN_FUNC}, // Extension of Fn.new
        {"\0", TOKEN_EOF}};

    void printTokens()
    {
        for (auto &t : tokens)
        {
            std::cout << std::setw(20) << t.typePrint(t.type)
                      << " " << std::setw(4) << t.line << " |"
                      << t.lexeme << "|\n";
        }
    }

    bool isAtEnd() const
    {
        return current >= source.length();
    }

    char advance()
    {
        return source.at(current++);
    }

    void addToken(TokenType type)
    {
        addToken(type, "");
    }

    void addToken(TokenType type, std::string literal)
    {
        std::string text = source.substr(start, current - start);
        if (text == "\n")
            text = "\\n";
        tokens.emplace_back(type, text, literal, line);
    }

    bool match(char expected)
    {
        if (isAtEnd())
            return false;
        if (source.at(current) != expected)
            return false;

        current++;
        return true;
    }

    char peek()
    {
        if (isAtEnd())
            return '\0';
        return source.at(current);
    }

    void string()
    {
        while (peek() != '"' && !isAtEnd())
        {
            if (peek() == '\n')
                line++;
            advance();
        }

        if (isAtEnd())
        {
            std::printf("Unterminated string.");
            return;
        }

        // The closing ".
        advance();

        // Trim the surrounding quotes.
        std::string value = source.substr(start + 1, current - start - 2);
        addToken(TOKEN_STRING, value);
    }

    bool isDigit(char c) const
    {
        return c >= '0' && c <= '9';
    }

    char peekNext()
    {
        if (current + 1 >= source.length())
            return '\0';
        return source.at(current + 1);
    }

    void number()
    {
        while (isDigit(peek()))
            advance();

        // Look for a fractional part.
        if (peek() == '.' && isDigit(peekNext()))
        {
            // Consume the "."
            advance();

            while (isDigit(peek()))
                advance();
        }

        addToken(TOKEN_NUMBER, source.substr(start, current - start));
    }

    bool isAlpha(char c) const
    {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               c == '_';
    }

    bool isAlphaNumeric(char c) const
    {
        return isAlpha(c) || isDigit(c);
    }

    void identifier()
    {
        while (isAlphaNumeric(peek()))
            advance();

        std::string text = source.substr(start, current - start);
        auto it = keywords.find(text);

        TokenType type;
        if (it != keywords.end())
        {
            type = it->second;
        }
        else if (text.at(0) == '_')
        {
            if (text.at(1) == '_')
                type = TOKEN_STATIC_FIELD;
            else
                type = TOKEN_FIELD;
        }
        else
        {
            type = TOKEN_NAME;
        }

        addToken(type);
    }

    void lineComment()
    {
        while (peek() != '\n' && !isAtEnd())
            advance();
    }

    void blockComment()
    {
        int nesting = 1;
        while (nesting > 0)
        {
            if (peek() == '\0')
            {
                std::cerr << "Unterminated block comment.";
                return;
            }

            if (peek() == '/' && peekNext() == '*')
            {
                advance();
                advance();
                nesting++;
                continue;
            }

            if (peek() == '*' && peekNext() == '/')
            {
                advance();
                advance();
                nesting--;
                continue;
            }

            advance();
        }
    }

    void scanToken()
    {
        char c = advance();
        switch (c)
        {
        case '(':
            addToken(TOKEN_LEFT_PAREN);
            break;
        case ')':
            addToken(TOKEN_RIGHT_PAREN);
            break;
        case '[':
            addToken(TOKEN_LEFT_BRACKET);
            break;
        case ']':
            addToken(TOKEN_RIGHT_BRACKET);
            break;
        case '{':
            addToken(TOKEN_LEFT_BRACE);
            break;
        case '}':
            addToken(TOKEN_RIGHT_BRACE);
            break;
        case ':':
            addToken(TOKEN_COLON);
            break;
        case ',':
            addToken(TOKEN_COMMA);
            break;
        case '*':
            addToken(TOKEN_STAR);
            break;
        case '%':
            addToken(TOKEN_PERCENT);
            break;
        case '#':
            addToken(TOKEN_HASH);
            break;
        case '^':
            addToken(TOKEN_CARET);
            break;
        case '+':
            addToken(TOKEN_PLUS);
            break;
        case '-':
            addToken(TOKEN_MINUS);
            break;
        case '~':
            addToken(TOKEN_TILDE);
            break;
        case '?':
            addToken(TOKEN_QUESTION);
            break;

        case '|':
            addToken(match('|') ? TOKEN_PIPEPIPE : TOKEN_PIPE);
            break;
        case '&':
            addToken(match('&') ? TOKEN_AMPAMP : TOKEN_AMP);
            break;
        case '=':
            addToken(match('=') ? TOKEN_EQEQ : TOKEN_EQ);
            break;
        case '!':
            addToken(match('=') ? TOKEN_BANGEQ : TOKEN_BANG);
            break;

        case '.':
            if (match('.'))
            {
                addToken(match('.') ? TOKEN_DOTDOTDOT : TOKEN_DOTDOT);
            }
            else
            {
                addToken(TOKEN_DOT);
            }
            break;

        case '<':
            if (match('<'))
            {
                addToken(TOKEN_LTLT);
            }
            else
            {
                addToken(match('=') ? TOKEN_LTEQ : TOKEN_LT);
            }
            break;
        case '>':
            if (match('>'))
            {
                addToken(TOKEN_GTGT);
            }
            else
            {
                addToken(match('=') ? TOKEN_GTEQ : TOKEN_GT);
            }
            break;

        case '/':
            if (match('/'))
            {
                lineComment();
                break;
            }

            if (match('*'))
            {
                blockComment();
                break;
            }

            addToken(TOKEN_SLASH);
            break;
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        case '\n':
            addToken(TOKEN_LINE);
            line++;
            break;
        case '"':
            string();
            break;

        default:
            if (isDigit(c))
            {
                number();
            }
            else if (isAlpha(c))
            {
                identifier();
            }
            else
            {
                std::printf("Unexpected character\n");
            }
            break;
        }
    }

    std::vector<Token> scanTokens()
    {
        tokens.clear();
        current = 0;
        start = 0;
        while (!isAtEnd())
        {
            start = current;
            scanToken();
        }

        tokens.emplace_back(TOKEN_EOF, "<EOF>", "", line);
        return tokens;
    }
};

struct SymbolTable
{
    std::unordered_map<std::string, std::vector<std::string>> table;
    SymbolTable() = default;

    void printSymbolTable()
    {
        std::cout << "\n{\n";
        for (const auto &[key, values] : table)
        {
            std::cout << "  \"" << key << "\": [";

            for (std::size_t i = 0; i < values.size(); ++i)
            {
                std::cout << "\"" << values[i] << "\"";
                if (i + 1 < values.size())
                    std::cout << ", ";
            }

            std::cout << "]\n";
        }
        std::cout << "}\n";
    }
};

struct Parser
{
    std::vector<Token> tokens;
    std::string out;
    Token currentToken;
    int tokenIndex = 0;

    Parser() = default;

    TokenType peekType(int offset = 0) const
    {
        if (tokenIndex + offset >= (int)tokens.size())
            return TOKEN_EOF;
        return tokens[tokenIndex + offset].type;
    }

    Token &current()
    {
        return tokens[tokenIndex];
    }

    Token peek() const
    {
        return tokens[tokenIndex + 1];
    }

    void emit(const std::string &text)
    {
        out += text;
    }

    void emitToken(const Token &tok, std::string separator = " ")
    {
        if (tok.type == TOKEN_LINE)
            out += "\n";
        else
            out += tok.lexeme + separator;
    }

    bool matchesPattern(std::vector<TokenType> toks)
    {
        for (int i = 0; i < (int)toks.size(); i++)
        {
            if (peekType(i) != toks[i])
                return false;
        }
        return true;
    }

    void emitSettersAndGetters(SymbolTable &symTable, std::string key)
    {
        if (symTable.table.empty())
            return;

        std::vector<std::string> fields = symTable.table[key];

        for (auto &f : fields)
        {
            std::string fieldName = f.substr(1);
            std::string getter = fieldName + "{" + f + "}";
            emit(getter);
            emit("\n");
            std::string setter = fieldName + "=(value){" + f + "=value}";
            emit(setter);
            emit("\n");
        }
    }

    std::string utils()
    {
        std::string utils =
            "class Slice {\n"
            "    construct new() {\n"
            "        _slice = []\n"
            "    }\n"
            "\n"
            "    construct new(list) {\n"
            "        _slice = list\n"
            "    }\n"
            "\n"
            "    // Values at particular position\n"
            "    [index] = (value) {\n"
            "        _slice[index] = value\n"
            "    }\n"
            "    \n"
            "    \n"
            "    [index] {\n"
            "        return _slice[index]\n"
            "    }\n"
            "\n"
            "    at(index) {\n"
            "        return _slice[index]\n"
            "    }\n"
            "\n"
            "    back {\n"
            "        return _slice[-1]\n"
            "    }\n"
            "\n"
            "    // Add values\n"
            "    insert(position, value) {\n"
            "        _slice[position] = value\n"
            "    }\n"
            "\n"
            "    append(value) {\n"
            "        _slice.insert(-1, value)\n"
            "    }\n"
            "\n"
            "    // Checks\n"
            "    empty {\n"
            "        return _slice.count == 0\n"
            "    }\n"
            "\n"
            "    size {\n"
            "        return _slice.count\n"
            "    }\n"
            "\n"
            "    // Remove elements\n"
            "    erase(index) {\n"
            "        _slice.removeAt(index)\n"
            "    }\n"
            "\n"
            "    clear {\n"
            "        _slice.clear()\n"
            "    }\n"
            "\n"
            "    toString {\n"
            "        return _slice\n"
            "    }\n"
            "}\n"
            "class Dict {\n"
            "    construct new() {\n"
            "        _dict = {}\n"
            "    }\n"
            "\n"
            "    construct new(map) {\n"
            "        _dict = map\n"
            "    }\n"
            "\n"
            "    [key] = (value) {\n"
            "        _dict[key] = value\n"
            "    }\n"
            "    \n"
            "    [key] {\n"
            "        return _dict[key]\n"
            "    }\n"
            "\n"
            "    at(key) {\n"
            "        return _dict[key]\n"
            "    }\n"
            "\n"
            "    insert(key, value) {\n"
            "        _dict[key] = value\n"
            "    }\n"
            "    \n"
            "    delete(key) {\n"
            "        _dict.remove(key)\n"
            "    }\n"
            "\n"
            "    size {\n"
            "        return _dict.count\n"
            "    }\n"
            "\n"
            "    empty {\n"
            "        return _dict.count == 0\n"
            "    }\n"
            "\n"
            "    find(key) {\n"
            "        return _dict.containsKey(key)\n"
            "    }\n"
            "\n"
            "    keys {\n"
            "        return _dict.keys\n"
            "    }\n"
            "\n"
            "    clear {\n"
            "        return _dict.clear()\n"
            "    }\n"
            "\n"
            "    toString {\n"
            "        return _dict\n"
            "    }\n"
            "}\n";

        return utils;
    }

    void handleSlices()
    {
        int bracketDepth = 1;
        while (tokenIndex < (int)tokens.size())
        {
            Token &inner = current();
            emitToken(inner, "");

            if (inner.type == TOKEN_LEFT_BRACKET)
                bracketDepth++;
            if (inner.type == TOKEN_RIGHT_BRACKET)
            {
                bracketDepth--;

                if (bracketDepth == 0)
                {
                    emit(")");
                    tokenIndex++;
                    break;
                }
            }

            tokenIndex++;
        }
    }

    void rewrite(Scanner &scanner)
    {
        tokens = scanner.scanTokens();
        tokenIndex = 0;
        out.clear();
        out += utils();

        // Partial pattern. Variadic <name>, <...>
        std::vector<TokenType> import = {TOKEN_IMPORT, TOKEN_STRING,
                                         TOKEN_FOR};

        std::vector<TokenType> func = {TOKEN_NAME, TOKEN_EQ,
                                       TOKEN_FUNC, TOKEN_LEFT_BRACE};

        // Partial pattern. Variadic [ ... ]
        std::vector<TokenType> slices = {TOKEN_NAME, TOKEN_EQ,
                                         TOKEN_LEFT_BRACKET};

        // Partial pattern. Variadic { ... }
        std::vector<TokenType> dict = {TOKEN_NAME, TOKEN_EQ,
                                       TOKEN_LEFT_BRACE};

        std::vector<TokenType> structClass = {TOKEN_STRUCT, TOKEN_NAME,
                                              TOKEN_LEFT_BRACE};

        while (tokenIndex < (int)tokens.size())
        {
            Token &t = current();

            if (t.type == TOKEN_EOF)
                break;

            // import "string" for <name>, <...>
            if (matchesPattern(import))
            {
                Token &importTok = tokens[tokenIndex];     // import
                Token &stringTok = tokens[tokenIndex + 1]; // "string"
                Token &forTok = tokens[tokenIndex + 2];    // for

                emitToken(importTok);
                emitToken(stringTok);
                emitToken(forTok);

                tokenIndex += import.size();

                while (tokenIndex < (int)tokens.size())
                {
                    Token &names = current();
                    emitToken(names, "");

                    if (names.type == TOKEN_LINE)
                    {
                        tokenIndex++;
                        break;
                    }

                    tokenIndex++;
                }
                continue;
            }

            // var <name> = Fn.new {
            if (matchesPattern(func))
            {
                Token &nameTok = tokens[tokenIndex];       // name
                Token &eqTok = tokens[tokenIndex + 1];     // =
                Token &funcTok = tokens[tokenIndex + 2];   // func
                Token &lbraceTok = tokens[tokenIndex + 3]; // {

                emitToken(nameTok);
                emitToken(eqTok);
                emit("Fn.new ");
                emitToken(lbraceTok);

                tokenIndex += func.size();
                continue;
            }

            // <name> = []
            if (matchesPattern(slices))
            {
                Token &nameTok = tokens[tokenIndex];      // name
                Token &eqTok = tokens[tokenIndex + 1];    // =
                Token &lbracket = tokens[tokenIndex + 2]; // [

                emitToken(nameTok);
                emitToken(eqTok);
                emit("Slice.new(");
                emitToken(lbracket, "");

                // Copy everything until the matching right bracket is found
                tokenIndex += slices.size();
                handleSlices();
                continue;
            }

            // <name> = {}
            if (matchesPattern(dict))
            {
                Token &nameTok = tokens[tokenIndex];    // name
                Token &eqTok = tokens[tokenIndex + 1];  // =
                Token &lbrace = tokens[tokenIndex + 2]; // {

                emitToken(nameTok);
                emitToken(eqTok);
                emit("Dict.new(");
                emitToken(lbrace, "");

                // Copy everything until the matching right brace is found
                tokenIndex += dict.size();
                int braceDepth = 1;
                while (tokenIndex < (int)tokens.size())
                {
                    Token &inner = current();

                    if (inner.type == TOKEN_LEFT_BRACE)
                        braceDepth++;
                    if (inner.type == TOKEN_RIGHT_BRACE)
                        braceDepth--;
                    if (inner.type == TOKEN_RIGHT_BRACE && braceDepth == 0)
                    {
                        emit("})");
                        tokenIndex++;
                        break;
                    }

                    if (inner.type == TOKEN_LEFT_BRACKET)
                    {
                        emit("Slice.new([");
                        tokenIndex++;
                        handleSlices();
                        continue;
                    }

                    emitToken(inner, "");
                    tokenIndex++;
                }
                continue;
            }

            // struct <name> {}
            // class <name> {}
            if (matchesPattern(structClass) || t.type == TOKEN_CLASS)
            {
                Token &nameTok = tokens[tokenIndex + 1]; // name
                Token &lbrace = tokens[tokenIndex + 2];  // {

                emit("class ");
                emitToken(nameTok);
                emitToken(lbrace, "");

                tokenIndex += structClass.size();

                int braceDepth = 1;
                SymbolTable symbolTable = SymbolTable();
                
                bool isClass = false;
                if (t.type == TOKEN_CLASS)
                    isClass = true;

                while (tokenIndex < (int)tokens.size() - 1)
                {
                    Token &tk = current();

                    if (tk.type == TOKEN_FIELD)
                        symbolTable.table[nameTok.lexeme].emplace_back(tk.lexeme);
                    if (tk.type == TOKEN_LEFT_BRACE)
                        braceDepth++;
                    if (tk.type == TOKEN_RIGHT_BRACE)
                        braceDepth--;
                    if (tk.type == TOKEN_RIGHT_BRACE && braceDepth == 0)
                    {
                        if (!isClass)
                            emitSettersAndGetters(symbolTable, nameTok.lexeme);
                        emit("}");
                        tokenIndex++;
                        break;
                    }

                    emitToken(tk);
                    tokenIndex++;
                }
                continue;
            }

            if (t.type == TOKEN_IN)
            {
                emit(" ");
                emitToken(t);
                tokenIndex++;
                continue;
            }

            if (t.type == TOKEN_AMPAMP)
            {
                emit("&& ");
                tokenIndex++;
                continue;
            }

            if (t.type == TOKEN_PIPEPIPE)
            {
                emit("|| ");
                tokenIndex++;
                continue;
            }

            if (t.type == TOKEN_VAR)
            {
                emitToken(t);
                tokenIndex++;
                continue;
            }

            // No need to translate
            emitToken(t, "");
            tokenIndex++;
        }
    }
};

struct Drop
{
    std::string code = "";
    std::string outName = "";
    Scanner scanner;
    Parser parser;

    Drop()
    {
    }

    bool loadFileIntoString(const std::string &filePath)
    {
        std::ifstream file(filePath);

        if (!file.is_open())
        {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        code = buffer.str();
        return true;
    }

    void createOutFile(std::filesystem::path &src)
    {
        outName = src.filename().string();
        int j = 0;
        for (int i = outName.size() - 1; i >= 0; i--)
        {
            if (outName.at(i) == '.')
                break;
            j++;
        }

        outName.erase(outName.length() - j);
        outName += "wren";

        std::ofstream outFile(outName);

        if (!outFile.is_open())
        {
            std::cerr << "Error opening file for writing!" << std::endl;
            return;
        }

        outFile << parser.out;
        outFile.close();
    }

    void run()
    {
        scanner = Scanner(code);
        parser.rewrite(scanner);
    }
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: drop <filename>.zaba" << std::endl;
        return 1;
    }

    int srcArg = 1;
    if (strcmp(argv[srcArg], "-l") == 0 || strcmp(argv[srcArg], "-c") == 0)
        srcArg++;

    std::filesystem::path src(argv[srcArg]);
    if (src.extension() != ".zaba")
    {
        std::cerr << "Expected a .zaba file" << std::endl;
        return 2;
    }

    Drop drop = Drop();
    if (!drop.loadFileIntoString(argv[srcArg]))
    {
        std::cerr << "Error opening file" << std::endl;
        return 3;
    }

    drop.run();

    if (strcmp(argv[1], "-l") == 0)
    {
        drop.scanner.printTokens();
        return 0;
    }

    if (strcmp(argv[1], "-c") == 0)
    {
        drop.createOutFile(src);
        return 0;
    }

    drop.createOutFile(src);
    std::string executeScript = ".\\wren_cli.exe .\\" + drop.outName;
    system(executeScript.c_str());
}