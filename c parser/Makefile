inputs := test-case/test1.c test-case/test2.c test-case/test3.c test-case/test4.c test-case/test5.c test-case/test6.c test-case/test7.c
all:clean y.tab.c y.tab.h lex.yy.c parser
y.tab.c y.tab.h : parser.y
	-yacc -d parser.y
lex.yy.c : parser.l
	-lex parser.l
parser : lex.yy.c y.tab.c y.tab.h cfg.c cfg.h point_analysis.c point_analysis.h clone_build_graph.c clone_build_graph.h first_analysis.c first_analysis.h second_analysis.c second_analysis.h final_pass.c final_pass.h
	-g++ -std=c++11 -w lex.yy.c -ll y.tab.c cfg.c point_analysis.c clone_build_graph.c first_analysis.c second_analysis.c final_pass.c -o parser -g
test : parser $(inputs)
	$(foreach x,$(inputs),$(shell ./parser $(x) $(subst .c,-out.c,$(x))>$(subst .c,-extra.txt,$(x))))
clean :
	-rm y.tab.c y.tab.h lex.yy.c parser
	-rm test-case/*.txt test-case/*out.c