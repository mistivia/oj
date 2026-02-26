-- https://www.codewars.com/kata/59db393bc1596bd2b700007f
{-# LANGUAGE GADTs         #-}
{-# LANGUAGE TypeFamilies  #-}
{-# LANGUAGE TypeOperators #-}

module Kata.AdditionCommutes
  ( plusCommutes ) where

import Kata.AdditionCommutes.Definitions
  ( Z, S
  , Natural(..), Equal(..)
  , (:+:))

reflexive :: Natural n -> Equal n n
reflexive NumZ  = EqlZ
reflexive (NumS n) = EqlS $ reflexive n

symmetric :: Equal a b -> Equal b a
symmetric EqlZ = EqlZ
symmetric (EqlS e) = EqlS $ symmetric e

transitive :: Equal a b -> Equal b c -> Equal a c
transitive EqlZ EqlZ = EqlZ
transitive (EqlS e1) (EqlS e2) = EqlS $ transitive e1 e2 

-- lemma1 (a + s(b)) = s(a+b)
lemma1 :: Natural a -> Natural b -> Equal (a :+: S b) (S (a :+: b))
lemma1 NumZ NumZ = EqlS EqlZ
lemma1 NumZ (NumS b) = EqlS $ lemma1 NumZ b
lemma1 (NumS a) b = EqlS $ lemma1 a b

-- lemma2 (s(b) + a) = s(b+a)
lemma2 :: Natural a -> Natural b -> Equal (S b :+: a) (S (b :+: a))
lemma2 NumZ NumZ = EqlS EqlZ
lemma2 (NumS a) NumZ = EqlS $ lemma2 a NumZ
lemma2 a (NumS b) = EqlS $ lemma2 a b

plusCommutes :: Natural a -> Natural b -> Equal (a :+: b) (b :+: a)
plusCommutes NumZ NumZ = EqlZ
plusCommutes NumZ (NumS b) = EqlS $ plusCommutes NumZ b
plusCommutes (NumS a) NumZ = EqlS $ plusCommutes a NumZ
plusCommutes a (NumS b) = transitive (transitive eq1 known) eq2 where
    known = EqlS $ plusCommutes a b
    eq1 = lemma1 a b
    eq2 = symmetric $ lemma2 a b

{-

data Z
data S n

data Natural :: Type -> Type where
    NumZ :: Natural Z
    NumS :: Natural n -> Natural (S n)

data Equal :: Type -> Type -> Type where
    EqlZ :: Equal Z Z
    EqlS :: Equal n m -> Equal (S n) (S m)

type family (:+:) (n :: Type) (m :: Type) :: Type
type instance Z :+: m = m
type instance S n :+: m = S (n :+: m)

-}
