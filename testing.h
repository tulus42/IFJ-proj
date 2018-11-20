#ifndef TESTING
#define TESTING

const char* tokens[] = {
	"TYPE_EOF", 
	"TYPE_EOL", 
	"TYPE_IDENTIFIER", 
	"TYPE_KEYWORD",

	"TYPE_ASSIGN", // =
	"TYPE_NEQ", // !=
	"TYPE_LEQ", // <=
	"TYPE_LTN", // <
	"TYPE_MEQ", // >=
	"TYPE_MTN", // >
	"TYPE_EQ", // ==
	
	"TYPE_PLUS", // +
	"TYPE_MINUS", //  -
	"TYPE_MUL", // *
	"TYPE_DIV", // /
	"TYPE_QUESTION_MARK", // ?
	"TYPE_COLON", // :

	"TYPE_LEFT_BRACKET", // (
	"TYPE_RIGHT_BRACKET", // )
	"TYPE_COMMA", // ,

	"TYPE_COMMENT", // #
	"TYPE_COMMENT_START", // =begin 
	"TYPE_COMMENT_END", // =end 

	"TYPE_INT", 
	"TYPE_FLOAT", 
	"TYPE_STRING",
};

const char* keyword[] = {
	"KEYWORD_DEF",
	"KEYWORD_DO",
	"KEYWORD_ELSE",
	"KEYWORD_END",
	"KEYWORD_IF",
	"KEYWORD_NOT",
	"KEYWORD_NIL",
	"KEYWORD_THEN",
	"KEYWORD_WHILE",
	"KEYWORD_PRINT",
	"KEYWORD_INPUTS",
	"KEYWORD_INPUTI",
	"KEYWORD_INPUTF",
	"KEYWORD_LENGTH",
	"KEYWORD_SUBSTR",
	"KEYWORD_ORD",
	"KEYWORD_CHR",
};

const char* symbols[] = {
	"PLUS",   // +
    "MINUS",  // -
    "MUL",    // *
    "DIV",    // /
    "LEFT_B", // (
    "RIGHT_B", // )
    "ID",     // i
    "EQL",    // ==
    "NEQ",   // !=
    "LEQ",    // >=
    "LTN",    // >
    "MEQ",    // <=
    "MTN",    // <
    "DOLLAR",  // $
	"E",
    "START",
    "IDIV"
};

const char* rules[] = {
	"S",  // shift <
    "R",  // reduce >
    "E",  // equal =
    "U" 
};



#endif