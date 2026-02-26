-- https://www.codewars.com/kata/534e01fbbb17187c7e0000c6
module Spiral where

setRow :: Int -> (Int, Int) -> [[Int]] -> [[Int]]
setRow r (from, to) mat = take r mat ++ [set oldRow] ++ drop (r+1) mat
  where
    oldRow = head $ drop r mat
    set row = take from row ++ take (to - from + 1) (repeat 1) ++ drop (to+1) row

setCol :: Int -> (Int, Int) -> [[Int]] -> [[Int]]
setCol c (from, to) mat =
  take from mat
  ++ map (setNth c) (drop from $ take (to+1) mat)
  ++ drop (to+1) mat
    where
      setNth n row = take n row ++ [1] ++ drop (n+1) row

emptySquare :: Int -> [[Int]]
emptySquare n = take n $ repeat $ take n $ repeat 0

data Direction = DRight | DLeft | DDown | DUp

spiralize :: Int -> [[Int]]
spiralize n = go (emptySquare n) (0,0) [n-1,n-1,0] n DRight
  where
    go mat (x,y) limit prevDelta DRight =
      if x >= head limit || prevDelta < 2 then mat
      else go (setRow y (x,head limit) mat)
              (head limit, y)
              (tail limit ++ [y+2])
              (head limit - x)
              DDown
    go mat (x,y) limit prevDelta DDown =
      if y >= head limit || prevDelta < 2 then mat
      else go (setCol x (y, head limit) mat)
              (x, head limit)
              (tail limit ++ [x-2])
              (head limit - y)
              DLeft
    go mat (x,y) limit prevDelta DLeft =
      if x <= head limit || prevDelta < 2 then mat
      else go (setRow y (head limit, x) mat)
              (head limit, y)
              (tail limit ++ [y-2])
              (x - head limit)
              DUp
    go mat (x,y) limit prevDelta DUp =
      if y <= head limit || prevDelta < 2 then mat
      else go (setCol x (head limit, y) mat)
              (x, head limit)
              (tail limit ++ [x+2])
              (y - head limit)
              DRight
