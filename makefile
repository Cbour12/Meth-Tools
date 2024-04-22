parse: lex.yy.o driver.o parser.o parse_tree_nodes.o
	$(CXX) $(CXXFLAGS) -o parse lex.yy.o driver.o parser.o parse_tree_nodes.o

#     -o flag specifies the output file
#
#     The above rule could be written with macros as
#        $(CXX) $(CXXFLAGS) -o $@ $^

driver.o: driver.cpp parser.h lexer.h parse_tree_nodes.h
	$(CXX) $(CXXFLAGS) -o driver.o -c driver.cpp

#      -c flag specifies stop after compiling, do not link

parser.o: parser.cpp parser.h lexer.h parse_tree_nodes.h
	$(CXX) $(CXXFLAGS) -o parser.o -c parser.cpp

parse_tree_nodes.o: parse_tree_nodes.cpp lexer.h 
	$(CXX) $(CXXFLAGS) -o parse_tree_nodes.o -c parse_tree_nodes.cpp

lex.yy.o: lex.yy.c lexer.h
	$(CC) $(CCFLAGS) -o lex.yy.o -c lex.yy.c

lex.yy.c: rules.l lexer.h
	$(LEX) -o lex.yy.c rules.l

clean: 
	$(RM) *.o lex.yy.c tips_parse
#   delete all generated files	
