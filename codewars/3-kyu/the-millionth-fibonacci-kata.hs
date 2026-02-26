-- https://www.codewars.com/kata/53d40c1e2f13e331fc000c26
module Fibonacci (fib) where

fib :: Integer -> Integer
fib n = if n >= 0 then posFib n else negFib (-n)

negFib :: Integer -> Integer
negFib n = if n `mod` 2 == 0 then (- (posFib n)) else posFib n


matMul :: [[Integer]] -> [[Integer]] -> [[Integer]]
matMul [[a,b],[c,d]] [[x,y],[z,w]] = [[a*x+b*z, a*y+b*w], [c*x+d*z, c*y+d*w]]
matMul _ _ = error "Error in matMul: ill formatted matrix"

matExp :: [[Integer]] -> Integer -> [[Integer]]
matExp m 1 = m
matExp m n
  | n < 1 = error "Error in matExp: n must > 0"
  | n `mod` 2 == 0 =
      let half = matExp m (n `div` 2) in
        seq half $ matMul half half
  | otherwise = matMul m (matExp m (n-1))

posFib :: Integer -> Integer
posFib 0 = 0
posFib 1 = 1
posFib n = (matExp [[1,1], [1,0]] (n-1)) !! 0 !! 0
