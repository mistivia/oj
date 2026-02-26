-- https://www.codewars.com/kata/55eec0ee00ae4a8fa0000075
module PermutationalPrimes (permutationalPrimes) where

import Control.Monad
import qualified Data.List as L
import qualified Data.Set as Set

deduplicateList :: Ord a => [a] -> [a]
deduplicateList = Set.toList . Set.fromList

permutations :: Int -> Int -> [Int]
permutations n upperLimit =
  filter (<= upperLimit) $
    map read $ filter (\lst -> head lst /= '0') $
      lstPermutations $ show n

lstPermutations = L.permutations

primes = filterPrime [2..] where
  filterPrime (p:xs) =
    p : filterPrime [x | x <- xs, x `mod` p /= 0]


permuatationsPrimeCnt primeSet n upperLimit =
  if foldl1 min permPrimes == n then (length permPrimes - 1)
  else 0  
  where
    permPrimes = deduplicateList $ filter isPrime $ permutations n upperLimit
        where isPrime x = Set.member x primeSet

permutationalPrimes :: Int -> Int -> Maybe (Int, Int, Int)
permutationalPrimes nMax kPerms =
    if ret /= [] then
        Just (length ret, head ret, foldl1 max ret)
    else 
        Nothing
    where
        ret = seq primeSet $ filter (\n -> kPerms == permuatationsPrimeCnt primeSet n nMax) $
                Set.toList primeSet
              where primeSet = Set.fromList $ takeWhile (<=nMax) primes
