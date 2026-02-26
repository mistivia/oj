-- https://www.codewars.com/kata/55aa075506463dac6600010d
module Codewars.G964.Sumdivsq where

divisors x = impl 1 []
  where
    impl cur result =
      if cur > x then
        result
      else
        if x `mod` cur == 0 then
          impl (cur+1) (cur:result)
        else
          impl (cur+1) result
        
isSquare :: Int -> Bool
isSquare n = root * root == n
    where root = round (sqrt $ fromIntegral n)

listSquared :: Int -> Int -> [(Int, Int)]
listSquared m n = filter (\(n, s) -> isSquare s) pairs
  where  
    pairs = map
      (\x -> (x, sum $ map (^2) $ divisors x))
      [m..n]
