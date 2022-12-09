from nltk.tree import *

# assign your output (generalied list of the syntax tree) to varaible text
text = "[expr[term[term[term[factor[operand[var[c]]]]][+][factor[factor[operand[var[a]]]][*][operand[(][expr[term[term[factor[operand[var[b]]]]][+][factor[operand[var[c]]]]]][)]]]][-][factor[operand[var[a]]]]]]"

text = text.replace("(", "ob")    #in the syntax tree, 'ob' will display in place of '('
text = text.replace(")", "cb")    #in the syntax tree, 'cb' will display in place of ')'
text = text.replace("[", "(")
text = text.replace("]", ")")


tree = Tree.fromstring(text)
tree.pretty_print(unicodelines=True, nodedist=10)   

