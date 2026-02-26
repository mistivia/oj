-- https://www.codewars.com/kata/521c2db8ddc89b9b7a0000c1

module Snail where

snail :: [[Int]] -> [Int]
snail [] = []
snail arr = foldl1 (++) $ map (roundAt n arr) [0..(n `div` 2)]
    where n = length arr

rowRange col from to arr =
    drop from $ take (to+1) $ arr !! col

colRange row from to arr =
    map (!!row) $ drop from $ take (to+1) arr

roundAt n arr x =
    (rowRange x x (n-x-1) arr) ++
        (colRange (n-x-1) (x+1) (n-x-1) arr) ++
            (reverse $ rowRange (n-x-1) x (n-x-2) arr) ++
                (reverse $ colRange x (x+1) (n-x-2) arr)
