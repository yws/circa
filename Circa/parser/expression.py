import pdb

from Circa import (
  builtins,
  ca_function,
  code,
  debug,
  token,
)

from Circa.token.definitions import *
from Circa.token import token_stream

import parse_errors

import builder as builder_module

VERBOSE_DEBUGGING = False

# Expression parsing
def parseExpression(tokens):
   # Coerce 'tokens' into a token stream
   tokens = token_stream.asTokenStream(tokens)

   # Mark the start location for backtracking
   start_loc = tokens.markLocation()

   try:
      return infix_expression(tokens, 0)
   except MatchFailed, e:
      # Backtrack if we didn't match
      tokens.restoreMark(start_loc)
      return None

# Alias for parseExpression
parse = parseExpression

class TermToCreate(object):
   def __init__(self, functionTerm, inputs):
      self.functionTerm = functionTerm
      self.inputs = inputs

   def create(self, builder):
      return builder.createTerm(functionTerm, inputs)

# AST Classes
class ASTNode(object):
   def eval(self, builder):
      raise Exception("Need to implement this")

   """
   def handleImplant(self, builder, target):
      raise Exception("Need to implement this")
   """

   def getFirstToken(self):
      raise Exception("Need to implement this")

class Infix(ASTNode):
   def __init__(self, function_token, left, right):
      assert isinstance(function_token, token.Token)
      assert isinstance(left, ASTNode)
      assert isinstance(right, ASTNode)

      self.token = function_token
      self.left = left
      self.right = right

   def getTermToCreate(self, builder):
      normalFunction = getOperatorFunction(self.token.match)
      if normalFunction is not None:
         return TermToCreate(normalFunction, 
                              [self.left.eval(builder), self.right.eval(builder)] )

   def eval(self, builder):

      # evaluate as an assignment?
      if self.token.match == EQUALS:
         right_term = self.right.eval(builder)
         if not isinstance(right_term, code.Term):
            raise parse_errors.ExpressionDidNotEvaluateToATerm(self.right.getFirstToken())
         return builder.bindName(self.left.getName(), right_term)

      # Evaluate as a feedback?
      """
      if self.token.match is COLON_EQUALS:
         subject = self.left.eval(builder)
         desired = self.right.eval(builder)

         code.putFeedbackOnTerm(builder.codeUnit, subject, desired)

         return None
      """

      # normal function?
      # try to find a defined operator
      normalFunction = getOperatorFunction(self.token.match)
      if normalFunction is not None:

         assert normalFunction.pythonValue is not None

         return builder.createTerm(normalFunction,
            inputs=[self.left.eval(builder), self.right.eval(builder)] )

      # evaluate as a function + assign?
      # try to find an assign operator
      assignFunction = getAssignOperatorFunction(self.token.match)
      if assignFunction is not None:
         # create a term that's the result of the operation
         result_term = builder.createTerm(assignFunction,
            inputs=[self.left.eval(builder), self.right.eval(builder)])

         # bind the name to this result
         return builder.bindName(self.left.getName(), result_term)

      raise Exception("Unable to evaluate token: " + self.token.text)

   """
   def handleImplant(self, builder, target):
      return builder.handleImplant(functionTerm, inputs, target)
   """

   def getFirstToken(self):
      return self.left.getFirstToken()

   def __str__(self):
      return self.function.text + "(" + str(self.left) + "," + str(self.right) + ")"

# Infix precedence
HIGHEST_INFIX_PRECEDENCE = 6
infixPrecedence = {
    DOT: 6,
    STAR: 5, SLASH: 5,
    PLUS: 4, MINUS: 4,
    LTHAN: 3, LTHANEQ: 3, GTHAN: 3, GTHANEQ: 3, DOUBLE_EQUALS: 3, NOT_EQUALS: 3,
    EQUALS: 1, PLUS_EQUALS: 1, MINUS_EQUALS: 1, STAR_EQUALS: 1, SLASH_EQUALS: 1,
      COLON_EQUALS: 1
}

def getInfixPrecedence(token):
   if token and token.match in infixPrecedence:
      return infixPrecedence[token.match]
   else: return -1

class Literal(ASTNode):
   def __init__(self, token, hasQuestionMark=False):
      self.token = token
      self.hasQuestionMark = hasQuestionMark

      if token.match == FLOAT:
         self.value = float(token.text)
      elif token.match == INTEGER:
         self.value = int(token.text)
      elif token.match == STRING:
         self.value = parseStringLiteral(token.text)
      else:
         raise parse_errors.InternalError("Couldn't recognize token: " + str(token))

   def eval(self, builder):
      if self.hasQuestionMark:
         return builder.createVariable(self.value, sourceToken=self.token)
      else:
         return builder.createConstant(self.value, sourceToken=self.token)

   """
   def handleImplant(self, builder, target):
      raise parse_errors.CantUseImplantOperatorOnLiteral(self.token)
   """

   def getFirstToken(self):
      return self.token

   def __str__(self):
      return str(self.value)

class Ident(ASTNode):
   def __init__(self, token):
      self.token = token

   def eval(self, builder):
      term = builder.getNamed(self.token.text)

      if not term:
         raise parse_errors.IdentifierNotFound(self.token)

      return builder.getNamed(self.token.text)

   def getTermToCreate(self, builder):
      return None

   def getFirstToken(self):
      return self.token

   """
   def handleImplant(self, builder, target):
      print "need to implement: handleImplant in Ident"
   """

   def getName(self):
      return self.token.text

   def __str__(self):
      return self.token.text

class Unary(ASTNode):
   def __init__(self, function_token, right):
      self.function_token = function_token
      self.right = right

   def eval(self, builder):
      return self.getTermToCreate(builder).create(builder)

   def getTermToCreate(self, builder):
      return TermToCreate(builtins.MULT,
                                inputs = [builder.createConstant(-1),
                                self.right.eval(builder)])

   def getFirstToken(self):
      return self.function_token;

   def __str__(self):
      return self.function_token.text + "(" + str(self.right) + ")"

class Function(ASTNode):
   def __init__(self, function_name, args):
      self.function_name = function_name
      self.args = args

   def eval(self, builder):
      return self.getTermToCreate(builder).create(builder)

   def getTermToCreate(self, builder):
      arg_terms = [t.eval(builder) for t in self.args]
      func = builder.getNamed(self.function_name.text)
 
      if func is None or func.pythonValue is None:
         raise parse_errors.InternalError(self.function_name,
            "Function " + self.function_name.text + " not found.")

      return TermToCreate(func, arg_terms)

   def eval(self, builder):
      arg_terms = [t.eval(builder) for t in self.args]
      func = builder.getNamed(self.function_name.text)

      if func is None:
        raise parse_errors.InternalError(self.function_name,
            "Function " + self.function_name.text + " not found.")

      # Check for Function
      if func.getType() is builtins.FUNC_TYPE:
        return builder.createTerm(func, inputs=arg_terms)

      # Check for Subroutine
      elif func.getType() is builtins.SUBROUTINE_TYPE:

         # Todo: special behavior for invoking subroutines
         return builder.createTerm(builtins.INVOKE_SUB_FUNC)

      # Temp: Use a Python dynamic type check to see if this is a function
      elif isinstance(func.pythonValue, ca_function._Function):
        return builder.createTerm(func, inputs=arg_terms)

      else:
         raise parse_errors.InternalError(self.function_name,
            "Term " + self.function_name.text + " is not a function.")

   def getFirstToken(self):
      return self.function_name;

class MatchFailed(Exception):
   pass


def infix_expression(tokens, precedence):
   if (precedence > HIGHEST_INFIX_PRECEDENCE):
      return unary_expression(tokens)

   expr = infix_expression(tokens, precedence + 1)
   if not expr: return None

   while getInfixPrecedence(tokens.next()) == precedence:
      operator = tokens.consume()

      first_righthand_token = tokens.next()
      right_expr = infix_expression(tokens, precedence + 1)

      if not right_expr:
         raise parse_errors.InternalError(first_righthand_token)

      expr = Infix(operator, expr, right_expr)

   return expr

def unary_expression(tokens):
   if tokens.nextIs(MINUS):
      minus = tokens.consume(MINUS)
      return Unary(minus, atom(tokens))
   else:
      return atom(tokens)

def atom(tokens):

   if VERBOSE_DEBUGGING:
      print "atom, next = " + tokens.next().name()

   # function call
   if tokens.nextIs(IDENT) and tokens.nextIs(LPAREN, lookahead=1):
      return function_call(tokens)

   # Literal value
   if tokens.nextIn((FLOAT, INTEGER, STRING)):
      token = tokens.consume()
      questionMark = False
      if tokens.nextIs(QUESTION):
         tokens.consume(QUESTION)
         questionMark = True
      return Literal(token, hasQuestionMark=questionMark)

   # identifier
   if tokens.nextIs(IDENT):
      token = tokens.consume()
      return Ident(token)

   # parenthesized expression
   if tokens.nextIs(LPAREN):
      tokens.consume(LPAREN)
      expr = infix_expression(tokens, 0)
      tokens.consume(RPAREN)
      return expr
 
   # failed to match
   if VERBOSE_DEBUGGING:
      print "atom failed to match"

   raise MatchFailed()
 
def function_call(tokens):
   function_name = tokens.consume(IDENT)
   tokens.consume(LPAREN)

   args = []

   if not tokens.nextIs(RPAREN):
      args.append( infix_expression(tokens, 0) )

      while tokens.nextIs(COMMA):
         tokens.consume(COMMA)
         args.append( infix_expression(tokens, 0) )

   tokens.consume(RPAREN)

   return Function(function_name, args)
 
def parseStringLiteral(text):
   # the literal should have ' or " marks on either side, strip these
   return text.strip("'\"")

def getOperatorFunction(token):
   # Special case: := operator
   if token == COLON_EQUALS:
      return builtins.FEEDBACK_FUNC

   circaObj = pythonTokenToBuiltin(token)

   if circaObj is None:
       print "Notice: couldn't find an operator func for " + token.raw_string
       return None

   result = builtins.BUILTINS.getTerm(builtins.OPERATOR_FUNC,
         inputs=[pythonTokenToBuiltin(token)])

   if result.pythonValue is None:
      return None

   return result

def getAssignOperatorFunction(token):
   circaObj = pythonTokenToBuiltin(token)
   if circaObj is None:
       print "Notice: couldn't find an assign operator func for " + token.raw_string
       return None
   result = builtins.BUILTINS.getTerm(builtins.ASSIGN_OPERATOR_FUNC,
         inputs=[pythonTokenToBuiltin(token)])

   if result.pythonValue is None:
      return None

   return result

def pythonTokenToBuiltin(token):
   token_string = builtins.BUILTINS.createConstant(token.raw_string)
   return builtins.BUILTINS.getTerm(builtins.TOKEN_FUNC, inputs=[token_string])
