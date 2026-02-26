-- https://www.codewars.com/kata/59c132fb70a3b7efd3000024
{-# LANGUAGE ScopedTypeVariables, Rank2Types #-}
module ScottEncoding where

import Prelude hiding (null, length, map, foldl, foldr, take, fst, snd, curry, uncurry, concat, zip, (++))

newtype SPair a b = SPair { runPair :: forall c. (a -> b -> c) -> c }

toPair :: SPair a b -> (a,b)
toPair p = (fst p, snd p)

fromPair :: (a,b) -> SPair a b
fromPair (a, b) = SPair $ \f -> f a b

fst :: SPair a b -> a
fst p = runPair p (\a b -> a)

snd :: SPair a b -> b
snd p = runPair p (\a b -> b)

swap :: SPair a b -> SPair b a
swap p = fromPair (b, a) where
  a = fst p
  b = snd p

curry :: (SPair a b -> c) -> (a -> b -> c)
curry f = \a b -> f $ fromPair (a, b)

uncurry :: (a -> b -> c) -> (SPair a b -> c)
uncurry f = \p -> f (fst p) (snd p)

newtype SMaybe a = SMaybe { runMaybe :: forall b. b -> (a -> b) -> b }

toMaybe :: SMaybe a -> Maybe a
toMaybe sm = runMaybe sm Nothing Just

fromMaybe :: Maybe a -> SMaybe a
fromMaybe m = SMaybe $ case m of
    Just x -> \b f -> f x
    Nothing -> \b f -> b

isJust :: SMaybe a -> Bool
isJust sm = runMaybe sm False $ const True

isNothing :: SMaybe a -> Bool
isNothing sm = runMaybe sm True $ const False

catMaybes :: SList (SMaybe a) -> SList a
catMaybes slm = go slm $ fromList [] where
  go l r =
    if null l then sreverse r
    else
      let x = car l
      in
        case toMaybe x of
          Just x -> go (cdr l) (cons x r)
          Nothing -> go (cdr l) r 

newtype SEither a b = SEither { runEither :: forall c. (a -> c) -> (b -> c) -> c }

toEither :: SEither a b -> Either a b
toEither se = runEither se Left Right

fromEither :: Either a b -> SEither a b
fromEither e = SEither $ case e of
    Left x -> \l r -> l x
    Right x -> \l r -> r x

isLeft :: SEither a b -> Bool
isLeft se = runEither se (const True) (const False)

isRight :: SEither a b -> Bool
isRight se = runEither se (const False) (const True)

partition :: SList (SEither a b) -> SPair (SList a) (SList b)
partition sle = go sle (fromList []) (fromList []) where
  go sle sll slr =
    if null sle then fromPair (sreverse sll, sreverse slr)
    else
      case toEither (car sle) of
        Left x -> go (cdr sle) (cons x sll) slr
        Right x -> go (cdr sle) sll (cons x slr)

newtype SList a = SList { runList :: forall b. b -> (a -> SList a -> b) -> b }

toList :: SList a -> [a]
toList sl = runList sl [] (\x xs -> x:toList xs)

fromList :: [a] -> SList a
fromList [] = SList $ \nil _ -> nil
fromList (x:xs) = SList $ \_ f -> f x $ fromList xs

cons :: a -> SList a -> SList a
cons  x xs = SList $ \_ f -> f x xs

null :: SList a -> Bool
null sl = runList sl True $ \x xs -> False

car :: SList a -> a
car sl = runList sl (error "cannot car on null list") $ \x xs -> x

cdr :: SList a -> SList a
cdr sl = runList sl (error "cannot car on null list") $ \x xs -> xs

sreverse :: SList a -> SList a
sreverse sl = go sl $ fromList [] where
  go l r =
    if null l then r
    else go (cdr l) (cons (car l) r)

concat :: SList a -> SList a -> SList a
concat sla slb = go (sreverse sla) slb where
  go la lb =
    if null la then lb
    else
      go (cdr la) (cons (car la) lb)

length :: SList a -> Int
length sl = go sl 0 where
  go sl l =
    if null sl then l
    else
      go (cdr sl) (l+1)

map :: (a -> b) -> SList a -> SList b
map f sl = go sl $ fromList [] where
  go sl r =
    if null sl then sreverse r
    else
      go (cdr sl) (cons (f (car sl)) r)

zip :: SList a -> SList b -> SList (SPair a b)
zip sla slb = go sla slb (fromList []) where
  go l1 l2 r = 
    if null l1 || null l2 then sreverse r
    else
      go (cdr l1) (cdr l2) (cons (fromPair (car l1, car l2)) r)

foldl :: (b -> a -> b) -> b -> SList a -> b
foldl f init sl =
  if null sl then init
  else
    foldl f (f init (car sl)) (cdr sl)

foldr :: (a -> b -> b) -> b -> SList a -> b
foldr f init sl = foldl (flip f) init (sreverse sl)

take :: Int -> SList a -> SList a
take i sl = go i sl $ fromList [] where
  go i sl r
    | i <= 0 = sreverse r
    | null sl = sreverse r
    | otherwise = go (i-1) (cdr sl) (cons (car sl) r)
