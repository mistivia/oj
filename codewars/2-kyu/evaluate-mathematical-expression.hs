-- https://www.codewars.com/kata/52a78825cdfc2cfc87000005
module EvaluateMathematicalExpression (calc) where

import Data.Char (isDigit, isSpace)

newtype Parser a = Parser { runParser :: String -> Maybe (a, String) }

instance Functor Parser where
  fmap f p = Parser $ \s -> case runParser p s of
    Nothing -> Nothing
    Just (r, xs) -> Just (f r, xs)

instance Applicative Parser where
  pure x = Parser $ \s -> Just (x, s)
  mf <*> ma = do
    f <- mf
    f <$> ma

instance Monad Parser where
  p >>= f = Parser $ \s -> case runParser p s of
    Nothing -> Nothing
    Just (x, rs) -> (runParser $ f x) rs

instance Semigroup (Parser a) where
  p1 <> p2 = Parser $
    \s -> case runParser p1 s of 
            Just a -> Just a
            Nothing -> runParser p2 s

instance Monoid (Parser a) where
  mempty = Parser $ const Nothing

satisfy :: (Char -> Bool) -> Parser Char
satisfy cond = Parser fn where
  fn [] = Nothing
  fn (x:xs) = if cond x then Just (x, xs) else Nothing

item :: Char -> Parser Char
item c = satisfy (==c)

one :: Parser Char -> Parser String
one pc = do
 ret <- pc
 return [ret]

optional p = p <> pure []

many p = optional $ some p

some p = (:) <$> p <*> many p

digit = satisfy isDigit

skipSpace = do
  let isSpace x = x == ' ' || x == '\t' || x == '\n'
  many (satisfy isSpace)
  return ()

token p = do
  skipSpace
  r <- p
  skipSpace
  pure r

number :: Parser Double
number = token impl where
  impl =
    let 
      intPart = (++) <$> optional (one $ item '-') <*> some digit
      dotPart = (++) <$> one (item '.') <*> some digit
    in do 
      numStr <- (++) <$> intPart <*> optional dotPart
      pure $ read numStr

charToOp '+' = (+)
charToOp '-' = (-)
charToOp '*' = (*)
charToOp '/' = (/)

op1 = token impl where
  impl = do
   c <- item '+' <> item '-'
   pure $ charToOp c

op2 = token impl where
  impl = do
   c <- item '/' <> item '*'
   pure $ charToOp c

leftBracket = token $ one $ item '('
rightBracket = token $ one $ item ')'

expr = do
  lhs <- term
  exprs <- many $ do
    op <- op1
    t <- term
    pure (`op` t)
  pure $ foldl (flip ($)) lhs exprs

negSign = do
  x <- many $ item '-'
  return $ if even $ length x then id else negate

term = do
  lhs <- signedFactor
  terms <- many $ do
    op <- op2
    f <- signedFactor
    pure (`op` f)
  pure $ foldl (flip ($)) lhs terms

signedFactor = token $ do
  s <- negSign
  s <$> factor

factor = number <> bracketExpr where
  bracketExpr = do
    leftBracket
    e <- expr
    rightBracket
    pure e

calc :: String -> Double
calc s =
  case runParser expr s of
    Just (x, _) -> x
    Nothing -> 0
         
