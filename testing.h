const char* tokens[] = {
	"TYPE_EOF", 
	"TYPE_EOL", 
	"TYPE_IDENTIFIER", 
	"TYPE_KEYWORD",
	"TYPE_STRING_LITERAL", 

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

};